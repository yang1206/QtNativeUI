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
    // 这里可以释放加载的库资源
}

bool NWindowEffectWin::initialize() {
    if (_isInitialized) {
        return true;
    }

    // 获取Windows版本信息
    HMODULE ntdllModule = LoadLibraryW(L"ntdll.dll");
    if (ntdllModule) {
        auto rtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(GetProcAddress(ntdllModule, "RtlGetVersion"));
        if (rtlGetVersion) {
            _windowsVersionInfo.dwOSVersionInfoSize = sizeof(_windowsVersionInfo);
            rtlGetVersion(&_windowsVersionInfo);
        }
        FreeLibrary(ntdllModule);
    }

    // 加载dwmapi.dll函数
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

    // 加载user32.dll函数
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
    // 使用特殊值-1来扩展整个窗口区域
    const MARGINS margins = {-1, -1, -1, -1};
    _dwmExtendFrameIntoClientArea(hwnd, &margins);
}

void NWindowEffectWin::extendFrameIntoClientArea(QWidget* window) {
    if (!window || !_isInitialized) {
        return;
    }

    // 设置窗口背景透明，但不影响子控件
    window->setAttribute(Qt::WA_TranslucentBackground);
    QPalette pal = window->palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    window->setPalette(pal);

    // 扩展窗口框架到客户区
    const HWND hwnd = reinterpret_cast<HWND>(window->winId());

    // 启用DPI感知
    EnableNonClientDpiScaling(hwnd);

    // 扩展窗口边框
    _externWindowMargins(hwnd);
}

bool NWindowEffectWin::setDarkMode(QWidget* window, bool isDark) {
    if (!window || !_isInitialized) {
        return false;
    }

    const HWND hwnd = reinterpret_cast<HWND>(window->winId());

    // Windows 10 1809以上版本支持暗色模式
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

    // 处理旧效果
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

    // 应用新效果
    switch (newType) {
        case None: {
            // 禁用特殊效果，设置普通背景色
            window->setAttribute(Qt::WA_TranslucentBackground, false);

            QColor backgroundColor =
                nTheme->isDarkMode()
                    ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
                    : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);

            QPalette pal = window->palette();
            pal.setColor(QPalette::Window, backgroundColor);
            window->setPalette(pal);
            window->setAutoFillBackground(true);

            // 恢复普通窗口边距
            const MARGINS margins = {0, 0, 0, 0};
            _dwmExtendFrameIntoClientArea(hwnd, &margins);
            return true;
        }

        case Mica: {
            // 设置透明背景
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
            // 如果不支持Mica，则回退到Acrylic
            newType = Acrylic;
            break;
        }

        case MicaAlt: {
            // 设置透明背景
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
            // 如果不支持MicaAlt，则回退到Mica
            newType = Mica;
            return setWindowEffect(window, Mica, oldType);
        }

        case Acrylic: {
            // 设置透明背景
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

            // Windows 10回退方案
            if (_setWindowCompositionAttribute) {
                // 获取主题色并调整透明度
                QColor themeColor =
                    nTheme->isDarkMode()
                        ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
                        : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);

                themeColor.setAlpha(204); // 80%透明度

                // ARGB格式
                DWORD color = (themeColor.alpha() << 24) | (themeColor.red() << 16) | (themeColor.green() << 8) |
                              themeColor.blue();

                ACCENTPOLICY accent = {ACCENT_ENABLE_ACRYLICBLURBEHIND, 0, color, 0};

                WINCOMPATTRDATA data = {WCA_ACCENT_POLICY, &accent, sizeof(accent)};

                if (_setWindowCompositionAttribute(hwnd, &data)) {
                    // 设置正确的窗口边距
                    const MARGINS margins = {0, 0, 1, 0}; // 只扩展顶部
                    _dwmExtendFrameIntoClientArea(hwnd, &margins);
                    return true;
                }
            }

            // 如果亚克力效果不可用，回退到DWM模糊
            newType = DWMBlur;
            break;
        }

        case DWMBlur: {
            // 设置透明背景
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