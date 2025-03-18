#include <QEvent>
#include <QPainter>
#include <QtNativeUI/NDropDownButton.h>
#include "../private/ndropdownbutton_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NDropDownButton::NDropDownButton(QWidget* parent) : NPushButton(parent), d_ptr(new NDropDownButtonPrivate()) { init(); }

NDropDownButton::NDropDownButton(const QString& text, QWidget* parent) : NDropDownButton(parent) { setText(text); }

NDropDownButton::~NDropDownButton() {}

void NDropDownButton::init() {
    Q_D(NDropDownButton);
    d->q_ptr = this;
    // 基本初始化代码将在实现具体功能时添加
}

void NDropDownButton::setMenu(QMenu* menu) {
    Q_D(NDropDownButton);
    d->_menu = menu;
}

QMenu* NDropDownButton::menu() const {
    Q_D(const NDropDownButton);
    return d->_menu;
}

void NDropDownButton::paintEvent(QPaintEvent* event) { Q_UNUSED(event); }

void NDropDownButton::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange ||
        event->type() == QEvent::LanguageChange) {
        update();
    }
    QPushButton::changeEvent(event);
}
