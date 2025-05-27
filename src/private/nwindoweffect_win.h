#ifndef QTNATIVEUI_NWINDOWEFFECT_WIN_H
#define QTNATIVEUI_NWINDOWEFFECT_WIN_H

#include <QWidget>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <dwmapi.h>
#include <uxtheme.h>

class NWindowEffectWin : public QObject {
    Q_OBJECT

  public:
    static NWindowEffectWin* getInstance();
    ~NWindowEffectWin();
    friend class NMainWindow;

    // 窗口效果类型
    enum WindowBackdropType {
        Auto    = 0, // 系统自动选择
        None    = 1, // 无特殊效果
        Mica    = 2, // Mica效果 (Windows 11)
        Acrylic = 3, // 亚克力效果 (Windows 10/11)
        MicaAlt = 4, // MicaAlt效果 (Windows 11 22H2+)
        DWMBlur = 5  // DWM模糊效果 (Windows Vista+)
    };

    // API功能
    bool initialize();
    bool setWindowEffect(QWidget* window, WindowBackdropType newType, WindowBackdropType oldType = Auto);
    bool setDarkMode(QWidget* window, bool isDark);
    void extendFrameIntoClientArea(QWidget* window);

  private:
    NWindowEffectWin(QObject* parent = nullptr);
    static NWindowEffectWin* instance;

    // 私有实现方法
    bool isWindowsVersionOrGreater(int major, int minor, int build) const;
    bool isWindows10OrGreater() const;
    bool isWindows11OrGreater() const;
    bool isWindows11_22H2OrGreater() const;

    // 内部DLL函数指针和状态管理
    typedef NTSTATUS(WINAPI* RtlGetVersionFunc)(PRTL_OSVERSIONINFOW);
    typedef HRESULT(WINAPI* DwmExtendFrameIntoClientAreaFunc)(HWND, const MARGINS*);
    typedef HRESULT(WINAPI* DwmSetWindowAttributeFunc)(HWND, DWORD, LPCVOID, DWORD);
    typedef HRESULT(WINAPI* DwmIsCompositionEnabledFunc)(BOOL*);
    typedef HRESULT(WINAPI* DwmEnableBlurBehindWindowFunc)(HWND, const DWM_BLURBEHIND*);
    typedef BOOL(WINAPI* SetWindowCompositionAttributeFunc)(HWND, void*);

    // 函数指针
    DwmExtendFrameIntoClientAreaFunc  _dwmExtendFrameIntoClientArea  = nullptr;
    DwmSetWindowAttributeFunc         _dwmSetWindowAttribute         = nullptr;
    DwmIsCompositionEnabledFunc       _dwmIsCompositionEnabled       = nullptr;
    DwmEnableBlurBehindWindowFunc     _dwmEnableBlurBehindWindow     = nullptr;
    SetWindowCompositionAttributeFunc _setWindowCompositionAttribute = nullptr;

    // 系统版本信息
    RTL_OSVERSIONINFOW _windowsVersionInfo = {0};
    bool               _isInitialized      = false;

    // 常量定义
    enum {
        DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 = 19,
        DWMWA_USE_IMMERSIVE_DARK_MODE             = 20,
        DWMWA_WINDOW_CORNER_PREFERENCE            = 33,
        DWMWA_SYSTEMBACKDROP_TYPE                 = 38,
        DWMWA_MICA_EFFECT                         = 1029
    };

    enum {
        DWMSBT_AUTO            = 0,
        DWMSBT_NONE            = 1,
        DWMSBT_MAINWINDOW      = 2, // Mica
        DWMSBT_TRANSIENTWINDOW = 3, // Acrylic
        DWMSBT_TABBEDWINDOW    = 4  // MicaAlt
    };

    // Windows 10亚克力效果结构体
    struct ACCENTPOLICY {
        int nAccentState;
        int nFlags;
        int nColor;
        int nAnimationId;
    };

    struct WINCOMPATTRDATA {
        int   nAttribute;
        PVOID pData;
        ULONG ulDataSize;
    };

    enum {
        ACCENT_DISABLED                   = 0,
        ACCENT_ENABLE_GRADIENT            = 1,
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        ACCENT_ENABLE_BLURBEHIND          = 3,
        ACCENT_ENABLE_ACRYLICBLURBEHIND   = 4,
        ACCENT_ENABLE_HOSTBACKDROP        = 5
    };

    enum { WCA_ACCENT_POLICY = 19 };

    void _externWindowMargins(HWND hwnd);
};

#endif // Q_OS_WIN
#endif // QTNATIVEUI_NWINDOWEFFECT_WIN_H