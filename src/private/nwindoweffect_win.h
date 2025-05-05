#ifndef QTNATIVEUI_NWINDOWEFFECT_WIN_H
#define QTNATIVEUI_NWINDOWEFFECT_WIN_H

#include <QWidget>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <dwmapi.h>
#include <uxtheme.h>

// Windows 10/11背景效果类
class NWindowEffectWin {
public:
    // 背景类型常量定义
    enum BackdropType {
        Default = 0,    // 自动选择
        None = 1,       // 无特殊效果
        Mica = 2,       // Mica效果 (Windows 11)
        Acrylic = 3,    // 亚克力效果 (Windows 10/11)
        Tabbed = 4      // 标签式效果 (Windows 11)
    };

    static void initWindowEffect(QWidget* window);
    static bool setWindowBackdropEffect(QWidget* window, int type);
    static bool enableSnapLayout(QWidget* window, bool enable);

private:
    static bool applyAcrylicEffect(QWidget* window);
    static bool applyMicaEffect(QWidget* window);
    static bool disableWindowEffects(QWidget* window);
    static bool isWindows11OrGreater();

    // Windows 10亚克力效果所需结构体
    struct ACCENTPOLICY {
        int nAccentState;
        int nFlags;
        int nColor;
        int nAnimationId;
    };

    struct WINCOMPATTRDATA {
        int nAttribute;
        PVOID pData;
        ULONG ulDataSize;
    };

    // Windows 10亚克力效果相关常量
    enum ACCENT_STATE {
        ACCENT_DISABLED = 0,
        ACCENT_ENABLE_GRADIENT = 1,
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        ACCENT_ENABLE_BLURBEHIND = 3,
        ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
        ACCENT_ENABLE_HOSTBACKDROP = 5,
        ACCENT_INVALID_STATE = 6
    };

    typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);
};
#endif // Q_OS_WIN

#endif // QTNATIVEUI_NWINDOWEFFECT_WIN_H
