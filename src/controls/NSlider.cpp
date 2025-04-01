//
// Created by Yang1206 on 2025/4/10.
//

#include <QtNativeUI/NSlider.h>
#include "../private/nslider_p.h"

NSlider::NSlider(QWidget* parent) : QSlider(parent), d_ptr(new NSliderPrivate()) {
    Q_D(NSlider);
    d->q_ptr = this;
}

NSlider::NSlider(Qt::Orientation orientation, QWidget* parent) : QSlider(orientation, parent), d_ptr(new NSliderPrivate()) {
    Q_D(NSlider);
    d->q_ptr = this;
}

NSlider::~NSlider() {}

void NSlider::paintEvent(QPaintEvent* event) {
    QSlider::paintEvent(event);
}