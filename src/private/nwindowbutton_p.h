#ifndef QTNATIVEUI_NWINDOWBUTTON_P_H
#define QTNATIVEUI_NWINDOWBUTTON_P_H

#include "QtNativeUI/NWindowButton.h"

class NWindowButtonPrivate {
    Q_DECLARE_PUBLIC(NWindowButton)

public:
    NWindowButtonPrivate();
    ~NWindowButtonPrivate();

    NWindowButton* q_ptr = nullptr;
    NWindowButton::SystemButtonType buttonType = NWindowButton::Custom;
    bool maximized = false;
    bool hovered = false;
};

#endif // QTNATIVEUI_NWINDOWBUTTON_P_H
