#ifndef QTNATIVEUI_NMAINWINDOW_P_H
#define QTNATIVEUI_NMAINWINDOW_P_H

#include <QObject>
#include <QtNativeUI/NMainWindow.h>

class NMainWindowPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BackdropEffect)

public:
    explicit NMainWindowPrivate(QObject* parent = nullptr);
    ~NMainWindowPrivate();

    // 手动添加getter和setter方法
    void setBackdropEffect(int effect) { _pBackdropEffect = effect; }
    int getBackdropEffect() const { return _pBackdropEffect; }

    Q_D_CREATE(NMainWindow)
};

#endif // QTNATIVEUI_NMAINWINDOW_P_H