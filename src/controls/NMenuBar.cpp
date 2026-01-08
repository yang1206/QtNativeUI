#include "QtNativeUI/NMenuBar.h"
#include "QtNativeUI/NIcon.h"

NMenuBar::NMenuBar(QWidget* parent) : QMenuBar(parent) {}

NMenuBar::~NMenuBar() = default;

NMenu* NMenuBar::addMenu(const QString& title) {
    auto menu = new NMenu(title, this);
    QMenuBar::addMenu(menu);
    return menu;
}

NMenu* NMenuBar::addMenu(NRegularIconType::Icon icon, const QString& title) {
    auto menu = new NMenu(title, this);
    menu->setIcon(nIcon->fromRegular(icon));
    QMenuBar::addMenu(menu);
    return menu;
}

NMenu* NMenuBar::addMenu(NFilledIconType::Icon icon, const QString& title) {
    auto menu = new NMenu(title, this);
    menu->setIcon(nIcon->fromFilled(icon));
    QMenuBar::addMenu(menu);
    return menu;
}

NMenu* NMenuBar::addMenu(const QIcon& icon, const QString& title) {
    auto menu = new NMenu(title, this);
    menu->setIcon(icon);
    QMenuBar::addMenu(menu);
    return menu;
}

QAction* NMenuBar::addMenu(QMenu* menu) {
    return QMenuBar::addMenu(menu);
}
