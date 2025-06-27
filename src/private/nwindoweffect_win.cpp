#include "nwindoweffect_win.h"
#include <QDebug>
#include <QPalette>
#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NTheme.h"

#ifdef Q_OS_WIN

NWindowEffectWin* NWindowEffectWin::instance = nullptr;

NWindowEffectWin* NWindowEffectWin::getInstance() {
    if (!instance) {
        instance = new NWindowEffectWin();
    }
    return instance;
}

NWindowEffectWin::NWindowEffectWin(QObject* parent) : QObject(parent) { initialize(); }

NWindowEffectWin::~NWindowEffectWin() {

}

bool NWindowEffectWin::initialize() {
    if (_isInitialized) {
        return true;
    }

    HMODULE ntdllModule = LoadLibraryW(L"ntdll.dll");
    if (ntdllModule) {
        auto rtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(GetProcAddress(ntdllModule, "RtlGetVersion"));
        if (rtlGetVersion) {
            _windowsVersionInfo.dwOSVersionInfoSize = sizeof(_windowsVersionInfo);
            rtlGetVersion(&_windowsVersionInfo);
        }
        FreeLibrary(ntdllModule);
    }

    HMODULE dwmModule = LoadLibraryW(L"dwmapi.dll");
    if (dwmModule) {
        _dwmExtendFrameIntoClientArea = reinterpret_cast<DwmExtendFrameIntoClientAreaFunc>(
            GetProcAddress(dwmModule, "DwmExtendFrameIntoClientArea"));
        _dwmSetWindowAttribute =
            reinterpret_cast<DwmSetWindowAttributeFunc>(GetProcAddress(dwmModule, "DwmSetWindowAttribute"));
        _dwmIsCompositionEnabled =
            reinterpret_cast<DwmIsCompositionEnabledFunc>(GetProcAddress(dwmModule, "DwmIsCompositionEnabled"));
        _dwmEnableBlurBehindWindow =
            reinterpret_cast<DwmEnableBlurBehindWindowFunc>(GetProcAddress(dwmModule, "DwmEnableBlurBehindWindow"));
    } else {
        qWarning() << "加载dwmapi.dll失败";
        return false;
    }


    HMODULE user32Module = LoadLibraryW(L"user32.dll");
    if (user32Module) {
        _setWindowCompositionAttribute = reinterpret_cast<SetWindowCompositionAttributeFunc>(
            GetProcAddress(user32Module, "SetWindowCompositionAttribute"));
    } else {
        qWarning() << "加载user32.dll失败";
        return false;
    }

    _isInitialized = true;
    return true;
}

bool NWindowEffectWin::isWindowsVersionOrGreater(int major, int minor, int build) const {
    return (_windowsVersionInfo.dwMajorVersion > major) ||
           (_windowsVersionInfo.dwMajorVersion == major &&
            ((_windowsVersionInfo.dwMinorVersion > minor) ||
             (_windowsVersionInfo.dwMinorVersion == minor && _windowsVersionInfo.dwBuildNumber >= build)));
}

bool NWindowEffectWin::isWindows10OrGreater() const { return _windowsVersionInfo.dwMajorVersion >= 10; }

bool NWindowEffectWin::isWindows11OrGreater() const {
    return _windowsVersionInfo.dwMajorVersion >= 10 && _windowsVersionInfo.dwBuildNumber >= 22000;
}

bool NWindowEffectWin::isWindows11_22H2OrGreater() const {
    return _windowsVersionInfo.dwMajorVersion >= 10 && _windowsVersionInfo.dwBuildNumber >= 22621;
}

void NWindowEffectWin::_externWindowMargins(HWND hwnd) {
    const MARGINS margins = {-1, -1, -1, -1};
    _dwmExtendFrameIntoClientArea(hwnd, &margins);
}

void NWindowEffectWin::extendFrameIntoClientArea(QWidget* window) {
    if (!window || !_isInitialized) {
        return;
    }

    window->setAttribute(Qt::WA_TranslucentBackground);
    QPalette pal = window->palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    window->setPalette(pal);


    const HWND hwnd = reinterpret_cast<HWND>(window->winId());


    EnableNonClientDpiScaling(hwnd);


    _externWindowMargins(hwnd);
}

bool NWindowEffectWin::setDarkMode(QWidget* window, bool isDark) {
    if (!window || !_isInitialized) {
        return false;
    }

    const HWND hwnd = reinterpret_cast<HWND>(window->winId());


    if (isWindowsVersionOrGreater(10, 0, 17763)) {
        BOOL  darkMode  = isDark ? TRUE : FALSE;
        DWORD attribute = isWindowsVersionOrGreater(10, 0, 19041) ? DWMWA_USE_IMMERSIVE_DARK_MODE
                                                                  : DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1;

        HRESULT hr = _dwmSetWindowAttribute(hwnd, attribute, &darkMode, sizeof(darkMode));
        return SUCCEEDED(hr);
    }

    return false;
}

bool NWindowEffectWin::setWindowEffect(QWidget* window, WindowBackdropType newType, WindowBackdropType oldType) {
    if (!window || !_isInitialized) {
        return false;
    }

    const HWND hwnd = reinterpret_cast<HWND>(window->winId());

    switch (oldType) {
        case Mica:
        case MicaAlt:
            if (isWindows11OrGreater()) {
                if (isWindows11_22H2OrGreater()) {
                    const int backdropType = DWMSBT_AUTO;
                    _dwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
                } else {
                    const BOOL enabled = FALSE;
                    _dwmSetWindowAttribute(hwnd, DWMWA_MICA_EFFECT, &enabled, sizeof(enabled));
                }
            }
            break;

        case Acrylic:
        case DWMBlur:
            if (isWindows11OrGreater()) {
                const int backdropType = DWMSBT_AUTO;
                _dwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
            }

            if (_setWindowCompositionAttribute) {
                ACCENTPOLICY    policy = {ACCENT_DISABLED};
                WINCOMPATTRDATA data   = {WCA_ACCENT_POLICY, &policy, sizeof(policy)};
                _setWindowCompositionAttribute(hwnd, &data);
            }

            if (_dwmEnableBlurBehindWindow) {
                DWM_BLURBEHIND bb = {0};
                bb.fEnable        = FALSE;
                bb.dwFlags        = DWM_BB_ENABLE;
                _dwmEnableBlurBehindWindow(hwnd, &bb);
            }
            break;

        default:
            break;
    }


    switch (newType) {
        case None: {
            window->setAttribute(Qt::WA_TranslucentBackground, false);

            QColor backgroundColor =
                nTheme->isDarkMode()
                    ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
                    : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);

            QPalette pal = window->palette();
            pal.setColor(QPalette::Window, backgroundColor);
            window->setPalette(pal);
            window->setAutoFillBackground(true);

            const MARGINS margins = {0, 0, 0, 0};
            _dwmExtendFrameIntoClientArea(hwnd, &margins);
            return true;
        }

        case Mica: {
            window->setAttribute(Qt::WA_TranslucentBackground);
            QPalette pal = window->palette();
            pal.setColor(QPalette::Window, Qt::transparent);
            window->setPalette(pal);

            if (isWindows11_22H2OrGreater()) {
                _externWindowMargins(hwnd);
                const int backdropType = DWMSBT_MAINWINDOW;
                HRESULT   hr =
                    _dwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
                return SUCCEEDED(hr);
            } else if (isWindows11OrGreater()) {
                _externWindowMargins(hwnd);
                const BOOL enabled = TRUE;
                HRESULT    hr      = _dwmSetWindowAttribute(hwnd, DWMWA_MICA_EFFECT, &enabled, sizeof(enabled));
                return SUCCEEDED(hr);
            }
            newType = Acrylic;
            break;
        }

        case MicaAlt: {
            window->setAttribute(Qt::WA_TranslucentBackground);
            QPalette pal = window->palette();
            pal.setColor(QPalette::Window, Qt::transparent);
            window->setPalette(pal);

            if (isWindows11_22H2OrGreater()) {
                _externWindowMargins(hwnd);
                const int backdropType = DWMSBT_TABBEDWINDOW;
                HRESULT   hr =
                    _dwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
                return SUCCEEDED(hr);
            }
            newType = Mica;
            return setWindowEffect(window, Mica, oldType);
        }

        case Acrylic: {
            window->setAttribute(Qt::WA_TranslucentBackground);
            QPalette pal = window->palette();
            pal.setColor(QPalette::Window, Qt::transparent);
            window->setPalette(pal);

            if (isWindows11OrGreater()) {
                _externWindowMargins(hwnd);
                const int backdropType = DWMSBT_TRANSIENTWINDOW;
                HRESULT   hr =
                    _dwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
                if (SUCCEEDED(hr)) {
                    return true;
                }
            }

            if (_setWindowCompositionAttribute) {
                QColor themeColor =
                    nTheme->isDarkMode()
                        ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
                        : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);

                themeColor.setAlpha(204);

                DWORD color = (themeColor.alpha() << 24) | (themeColor.red() << 16) | (themeColor.green() << 8) |
                              themeColor.blue();

                ACCENTPOLICY accent = {ACCENT_ENABLE_ACRYLICBLURBEHIND, 0, color, 0};

                WINCOMPATTRDATA data = {WCA_ACCENT_POLICY, &accent, sizeof(accent)};

                if (_setWindowCompositionAttribute(hwnd, &data)) {
                    const MARGINS margins = {0, 0, 1, 0}; // 只扩展顶部
                    _dwmExtendFrameIntoClientArea(hwnd, &margins);
                    return true;
                }
            }

            newType = DWMBlur;
            break;
        }

        case DWMBlur: {
            window->setAttribute(Qt::WA_TranslucentBackground);
            QPalette pal = window->palette();
            pal.setColor(QPalette::Window, Qt::transparent);
            window->setPalette(pal);

            const MARGINS margins = {0, 0, 1, 0}; // 只扩展顶部
            _dwmExtendFrameIntoClientArea(hwnd, &margins);

            if (_setWindowCompositionAttribute) {
                ACCENTPOLICY accent = {ACCENT_ENABLE_BLURBEHIND, 0, 0, 0};

                WINCOMPATTRDATA data = {WCA_ACCENT_POLICY, &accent, sizeof(accent)};

                return _setWindowCompositionAttribute(hwnd, &data);
            } else if (_dwmEnableBlurBehindWindow) {
                DWM_BLURBEHIND bb = {0};
                bb.fEnable        = TRUE;
                bb.dwFlags        = DWM_BB_ENABLE;

                HRESULT hr = _dwmEnableBlurBehindWindow(hwnd, &bb);
                return SUCCEEDED(hr);
            }
            return false;
        }

        case Auto:
        default:
            if (isWindows11_22H2OrGreater()) {
                return setWindowEffect(window, MicaAlt, oldType);
            } else if (isWindows11OrGreater()) {
                return setWindowEffect(window, Mica, oldType);
            } else if (isWindows10OrGreater()) {
                return setWindowEffect(window, Acrylic, oldType);
            } else {
                return setWindowEffect(window, None, oldType);
            }
    }

    return false;
}
#endif // Q_OS_WIN