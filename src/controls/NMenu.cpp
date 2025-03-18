#include "QtNativeUI/NMenu.h"
#include "../private/nmenu_p.h"

NMenu::NMenu(QWidget* parent) : QMenu(parent), d_ptr(new NMenuPrivate()) {
    init();
}

NMenu::NMenu(const QString& title, QWidget* parent) : NMenu(parent) {
    setTitle(title);
}

NMenu::~NMenu() {}

void NMenu::init() {
    Q_D(NMenu);
    d->q_ptr = this;
}