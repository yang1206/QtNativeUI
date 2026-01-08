#ifndef QTNATIVEUI_NWINDOWBAR_P_H
#define QTNATIVEUI_NWINDOWBAR_P_H

#include "QtNativeUI/NWindowBar.h"
#include <QHBoxLayout>
#include <QLabel>

class NWindowButton;

class NWindowBarPrivate {
    Q_DECLARE_PUBLIC(NWindowBar)

public:
    NWindowBarPrivate();
    ~NWindowBarPrivate();

    NWindowBar* q_ptr = nullptr;
    QWidget* hostWidget = nullptr;

    QHBoxLayout* mainLayout = nullptr;
    QHBoxLayout* leftLayout = nullptr;
    QHBoxLayout* centerLayout = nullptr;
    QHBoxLayout* rightLayout = nullptr;

    QLabel* iconLabel = nullptr;
    QLabel* titleLabel = nullptr;
    QMenuBar* menuBar = nullptr;

    NWindowButton* minButton = nullptr;
    NWindowButton* maxButton = nullptr;
    NWindowButton* closeButton = nullptr;
    NWindowButton* pinButton = nullptr;
    NWindowButton* themeButton = nullptr;

    bool titleVisible = true;
    bool iconVisible = true;
};

#endif // QTNATIVEUI_NWINDOWBAR_P_H
