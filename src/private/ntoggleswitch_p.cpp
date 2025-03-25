#include "ntoggleswitch_p.h"
#include "QtNativeUI/NToggleSwitch.h"

NToggleSwitchPrivate::NToggleSwitchPrivate(QObject* parent) : QObject(parent) {
    _thumbPosAnimation = new QPropertyAnimation(this, "thumbCenterX");
    _thumbPosAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _thumbPosAnimation->setDuration(150);

    _thumbRadiusAnimation = new QPropertyAnimation(this, "thumbRadius");
    _thumbRadiusAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _thumbRadiusAnimation->setDuration(150);
}

NToggleSwitchPrivate::~NToggleSwitchPrivate() {
    delete _thumbPosAnimation;
    delete _thumbRadiusAnimation;
}

void NToggleSwitchPrivate::startThumbPosAnimation(qreal startX, qreal endX, bool isChecked) {
    _thumbPosAnimation->stop();
    _thumbPosAnimation->setStartValue(startX);
    _thumbPosAnimation->setEndValue(endX);

    QObject::connect(_thumbPosAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
        _thumbCenterX = value.toReal();
        q_ptr->update();
    });

    QObject::connect(_thumbPosAnimation, &QPropertyAnimation::finished, q_ptr, [this, isChecked]() {
        if (q_ptr->isChecked() != isChecked) {
            q_ptr->setChecked(isChecked);
        }
        QObject::disconnect(_thumbPosAnimation, nullptr, q_ptr, nullptr);
    });

    _thumbPosAnimation->start();
}

void NToggleSwitchPrivate::startThumbRadiusAnimation(qreal startRadius, qreal endRadius) {
    _thumbRadiusAnimation->stop();
    _thumbRadiusAnimation->setStartValue(startRadius);
    _thumbRadiusAnimation->setEndValue(endRadius);

    QObject::connect(_thumbRadiusAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
        _thumbRadius = value.toReal();
        q_ptr->update();
    });

    QObject::connect(_thumbRadiusAnimation, &QPropertyAnimation::finished, q_ptr, [this]() {
        QObject::disconnect(_thumbRadiusAnimation, nullptr, q_ptr, nullptr);
    });

    _thumbRadiusAnimation->start();
}

void NToggleSwitchPrivate::adjustThumbCenterX() {
    int minX = _trackHeight / 2;
    int maxX = _trackWidth - _trackHeight / 2;

    if (_thumbCenterX < minX) {
        _thumbCenterX = minX;
    } else if (_thumbCenterX > maxX) {
        _thumbCenterX = maxX;
    }
}