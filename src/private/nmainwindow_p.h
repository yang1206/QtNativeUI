#ifndef QTNATIVEUI_NMAINWINDOW_P_H
#define QTNATIVEUI_NMAINWINDOW_P_H

#include "QtNativeUI/NMainWindow.h"

class NMainWindowPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NMainWindowPrivate(QObject* parent = nullptr);
    ~NMainWindowPrivate() override;

    NMainWindow* q_ptr;
    NMainWindow::BackdropType backdropEffect = NMainWindow::None;
    bool isDarkMode = false;
    bool isWindowActive = true;
    QColor backgroundColor;

    void setBackdropEffect(NMainWindow::BackdropType type);

    void updateBackgroundColor();
};

#endif // QTNATIVEUI_NMAINWINDOW_P_H