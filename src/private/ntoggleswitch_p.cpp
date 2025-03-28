#include "ntoggleswitch_p.h"
#include "QtNativeUI/NToggleSwitch.h"

NToggleSwitchPrivate::NToggleSwitchPrivate(QObject* parent) : QObject(parent) {
    _thumbPosAnimation = new QPropertyAnimation(this, "thumbCenterX");
    _thumbPosAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _thumbPosAnimation->setDuration(150);

    _thumbRadiusAnimation = new QPropertyAnimation(this, "thumbRadius");
    _thumbRadiusAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _thumbRadiusAnimation->setDuration(150);

    _thumbStretchAnimation = new QPropertyAnimation(this, "thumbStretchFactor");
    _thumbStretchAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _thumbStretchAnimation->setDuration(150);
}

NToggleSwitchPrivate::~NToggleSwitchPrivate() {
    delete _thumbPosAnimation;
    delete _thumbRadiusAnimation;
    delete _thumbStretchAnimation;
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
    qreal stretchOffset = 0;

    if (_isPressed && _thumbStretchFactor > 1.0) {
        stretchOffset = _thumbRadius * (_thumbStretchFactor - 1.0);
    }

    qreal minX = _trackHeight / 2.0;
    qreal maxX = _trackWidth - _trackHeight / 2.0;

    if (q_ptr->isChecked()) {
        minX += stretchOffset;
    } else {
        maxX -= stretchOffset;
    }

    _thumbCenterX = qBound(minX, _thumbCenterX, maxX);
}

void NToggleSwitchPrivate::startThumbStretchAnimation(bool pressed) {
    _thumbStretchAnimation->stop();
    _thumbStretchAnimation->setStartValue(_thumbStretchFactor);

    // 根据开关状态设置延展因子
    // 按下时延展，释放时恢复圆形
    qreal targetFactor = pressed ? 1.8 : 1.0;

    _thumbStretchAnimation->setEndValue(targetFactor);

    QObject::connect(_thumbStretchAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
        _thumbStretchFactor = value.toReal();
        q_ptr->update();
    });

    QObject::connect(_thumbStretchAnimation, &QPropertyAnimation::finished, q_ptr, [this]() {
        QObject::disconnect(_thumbStretchAnimation, nullptr, q_ptr, nullptr);
    });

    _thumbStretchAnimation->start();
}