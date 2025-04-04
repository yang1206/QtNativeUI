//
// Created by Yang1206 on 2025/4/1.
//

#include "nradiobutton_p.h"
#include "QtNativeUI/NRadioButton.h"
#include "QtNativeUI/NTheme.h"

NRadioButtonPrivate::NRadioButtonPrivate(QObject* parent) : QObject(parent) {
    _innerCircleAnimation = new QPropertyAnimation(this, "innerCircleScale");
    _innerCircleAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _innerCircleAnimation->setDuration(nTheme->getToken(NDesignTokenKey::AnimationNormal).toInt());
}

NRadioButtonPrivate::~NRadioButtonPrivate() { delete _innerCircleAnimation; }

void NRadioButtonPrivate::startInnerCircleAnimation(qreal startScale, qreal endScale) {
    _innerCircleAnimation->stop();
    _innerCircleAnimation->setStartValue(startScale);
    _innerCircleAnimation->setEndValue(endScale);

    QObject::disconnect(_innerCircleAnimation, nullptr, q_ptr, nullptr);

    QObject::connect(_innerCircleAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
        _innerCircleScale = value.toReal();
        q_ptr->update();
    });

    QObject::connect(_innerCircleAnimation, &QPropertyAnimation::finished, q_ptr, [this]() {
        QObject::disconnect(_innerCircleAnimation, nullptr, q_ptr, nullptr);
    });

    _innerCircleAnimation->start();
}
