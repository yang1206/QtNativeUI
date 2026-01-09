#ifndef QTNATIVEUI_NMAINWINDOW_P_H
#define QTNATIVEUI_NMAINWINDOW_P_H

#include "QtNativeUI/NMainWindow.h"

namespace QWK {
class WidgetWindowAgent;
}

class NWindowBar;

class NMainWindowPrivate {
    Q_DECLARE_PUBLIC(NMainWindow)

public:
    NMainWindowPrivate();
    ~NMainWindowPrivate();

    // ========== 平台能力查询 (编译期常量) ==========
    static constexpr bool useNativeSystemButtons()
    {
#ifdef Q_OS_MAC
        return true;
#else
        return false;
#endif
    }

    static constexpr bool supportsMica()
    {
#ifdef Q_OS_WIN
        return true;
#else
        return false;
#endif
    }

    static constexpr bool supportsAcrylic()
    {
#ifdef Q_OS_WIN
        return true;
#else
        return false;
#endif
    }

    // ========== 平台相关操作 ==========
    void setupWindowAgent();
    void setupThemeConnection();
    void registerSystemButtons(NWindowBar* bar);
    void applyBackdropEffect(NMainWindow::BackdropType type);

    // ========== 通用操作 ==========
    void updateBackgroundColor();

    // ========== 成员变量 ==========
    NMainWindow*              q_ptr        = nullptr;
    QWK::WidgetWindowAgent*   windowAgent  = nullptr;
    NWindowBar*               windowBar    = nullptr;
    NMainWindow::BackdropType backdropType = NMainWindow::None;
    QColor                    backgroundColor;
};

#endif // QTNATIVEUI_NMAINWINDOW_P_H
