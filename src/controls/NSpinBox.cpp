//
// Created by Yang1206 on 2025/4/10.
//

#include <QtNativeUI/NSpinBox.h>
#include "../private/nspinbox_p.h"

NSpinBox::NSpinBox(QWidget* parent) : QSpinBox(parent), d_ptr(new NSpinBoxPrivate()) {
    Q_D(NSpinBox);
    d->q_ptr = this;
}

NSpinBox::~NSpinBox() {}

void NSpinBox::paintEvent(QPaintEvent* event) {
    QSpinBox::paintEvent(event);
}