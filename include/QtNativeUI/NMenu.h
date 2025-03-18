#ifndef QTNATIVEUI_NMENU_H
#define QTNATIVEUI_NMENU_H

#include <QMenu>
#include "stdafx.h"

class NMenuPrivate;
class QTNATIVEUI_EXPORT NMenu : public QMenu {
    Q_OBJECT
    Q_Q_CREATE(NMenu)

public:
    explicit NMenu(QWidget* parent = nullptr);
    explicit NMenu(const QString& title, QWidget* parent = nullptr);
    ~NMenu();

private:
    void init();
};

#endif // QTNATIVEUI_NMENU_H 