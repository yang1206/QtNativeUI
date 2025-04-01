//
// Created by Yang1206 on 2025/4/1.
//

#include <QtNativeUI/NRadioButton.h>
#include "../private/nradiobutton_p.h"

NRadioButton::NRadioButton(QWidget* parent) : QRadioButton(parent), d_ptr(new NRadioButtonPrivate()) {
    Q_D(NRadioButton);
    d->q_ptr = this;
}

NRadioButton::NRadioButton(const QString& text, QWidget* parent) : NRadioButton(parent) { setText(text); }

NRadioButton::~NRadioButton() {}

void NRadioButton::paintEvent(QPaintEvent* event) { QRadioButton::paintEvent(event); }