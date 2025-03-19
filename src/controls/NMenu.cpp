#include "QtNativeUI/NMenu.h"
#include "../private/nmenu_p.h"
#include "QtNativeUI/NIcon.h"

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

QAction* NMenu::addItem(const QString& text) {
    return createAction(text);
}

QAction* NMenu::addItem(const QString& text, NRegularIconType::Icon icon) {
    return createAction(text, nIcon->fromRegular(icon));
}

QAction* NMenu::addItem(const QString& text, NFilledIconType::Icon icon) {
    return createAction(text, nIcon->fromFilled(icon));
}

NMenu* NMenu::addSubMenu(const QString& text) {
    NMenu* subMenu = new NMenu(text, this);
    QMenu::addMenu(subMenu);
    return subMenu;
}

NMenu* NMenu::addSubMenu(const QString& text, NRegularIconType::Icon icon) {
    NMenu* subMenu = addSubMenu(text);
    subMenu->setIcon(nIcon->fromRegular(icon));
    return subMenu;
}

NMenu* NMenu::addSubMenu(const QString& text, NFilledIconType::Icon icon) {
    NMenu* subMenu = addSubMenu(text);
    subMenu->setIcon(nIcon->fromFilled(icon));
    return subMenu;
}

void NMenu::addSeparator() {
    QMenu::addSeparator();
}

QAction* NMenu::createAction(const QString& text, const QIcon& icon) {
    QAction* action = new QAction(text, this);
    if (!icon.isNull()) {
        action->setIcon(icon);
    }
    addAction(action);
    return action;
}