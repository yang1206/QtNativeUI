#ifndef QTNATIVEUI_NMENU_P_H
#define QTNATIVEUI_NMENU_P_H

#include <QtNativeUI/NMenu.h>

class NMenuPrivate : public QObject {
    Q_OBJECT

public:
    explicit NMenuPrivate(QObject* parent = nullptr);
    ~NMenuPrivate();
    Q_D_CREATE(NMenu)
};

#endif // QTNATIVEUI_NMENU_P_H 