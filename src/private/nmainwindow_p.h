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
    
    void updateBackgroundColor();

    NMainWindow*              q_ptr        = nullptr;
    QWK::WidgetWindowAgent*   windowAgent  = nullptr;
    NWindowBar*               windowBar    = nullptr;
    NMainWindow::BackdropType backdropType = NMainWindow::None;
    QColor                    backgroundColor;
};

#endif // QTNATIVEUI_NMAINWINDOW_P_H
