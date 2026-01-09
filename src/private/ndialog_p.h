#ifndef QTNATIVEUI_NDIALOG_P_H
#define QTNATIVEUI_NDIALOG_P_H

#include "QtNativeUI/NDialog.h"

#include <QVBoxLayout>

class NFramelessHelper;
class NWindowBar;

class NDialogPrivate {
    Q_DECLARE_PUBLIC(NDialog)

public:
    NDialogPrivate();
    ~NDialogPrivate();

    void setupFrameless();
    void setupDefaultWindowBar();
    void setupLayout();
    void registerSystemButtons();

    NDialog*          q_ptr         = nullptr;
    NFramelessHelper* frameless     = nullptr;
    NWindowBar*       windowBar     = nullptr;
    QWidget*          titleBar      = nullptr;
    QWidget*          contentWidget = nullptr;
    QVBoxLayout*      mainLayout    = nullptr;
};

#endif // QTNATIVEUI_NDIALOG_P_H
