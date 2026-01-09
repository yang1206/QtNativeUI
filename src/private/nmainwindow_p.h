#ifndef QTNATIVEUI_NMAINWINDOW_P_H
#define QTNATIVEUI_NMAINWINDOW_P_H

#include "QtNativeUI/NMainWindow.h"

class NFramelessHelper;
class NWindowBar;

class NMainWindowPrivate {
    Q_DECLARE_PUBLIC(NMainWindow)

public:
    NMainWindowPrivate();
    ~NMainWindowPrivate();

    void setupFrameless();
    void registerSystemButtons(NWindowBar* bar);

    NMainWindow*      q_ptr         = nullptr;
    NFramelessHelper* frameless     = nullptr;
    NWindowBar*       windowBar     = nullptr;
};

#endif // QTNATIVEUI_NMAINWINDOW_P_H
