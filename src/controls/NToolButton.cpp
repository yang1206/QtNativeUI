//
// Created by Yang1206 on 2025/4/1.
//

#include <QtNativeUI/NToolButton.h>
#include "../private/ntoolbutton_p.h"

NToolButton::NToolButton(QWidget* parent) : QToolButton(parent), d_ptr(new NToolButtonPrivate()) {
    Q_D(NToolButton);
    d->q_ptr = this;
}

NToolButton::~NToolButton() {}

void NToolButton::paintEvent(QPaintEvent* event) { QToolButton::paintEvent(event); }