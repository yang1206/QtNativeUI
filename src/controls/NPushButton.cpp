#include <QtNativeUI/NPushButton.h>
#include "../private/npushbutton_p.h"

NPushButton::NPushButton(QWidget* parent) : QPushButton(parent), d_ptr(new NPushButtonPrivate(this)) {
    Q_D(NPushButton);
}

NPushButton::NPushButton(const QString& text, QWidget* parent) : QPushButton(text, parent) { setText(text); }

NPushButton::~NPushButton() {}

void NPushButton::paintEvent(QPaintEvent* event) {
    Q_D(NPushButton);
    QPushButton::paintEvent(event);
}
