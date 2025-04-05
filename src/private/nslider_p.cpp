//
// Created by Yang1206 on 2025/4/10.
//

#include "nslider_p.h"
#include "QtNativeUI/NSlider.h"
#include "QtNativeUI/NTheme.h"

NSliderPrivate::NSliderPrivate(QObject* parent) : QObject(parent) {
    _thumbAnimation = new QPropertyAnimation(this, "thumbScale");
    _thumbAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _thumbAnimation->setDuration(150);
}

NSliderPrivate::~NSliderPrivate() {}

void NSliderPrivate::startThumbAnimation(qreal startScale, qreal endScale) {
    _thumbAnimation->stop();
    _thumbAnimation->setStartValue(startScale);
    _thumbAnimation->setEndValue(endScale);

    QObject::disconnect(_thumbAnimation, nullptr, q_ptr, nullptr);

    QObject::connect(_thumbAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
        _thumbScale = value.toReal();
        q_ptr->update();
    });

    QObject::connect(_thumbAnimation, &QPropertyAnimation::finished, q_ptr, [this]() {
        QObject::disconnect(_thumbAnimation, nullptr, q_ptr, nullptr);
    });

    _thumbAnimation->start();
}