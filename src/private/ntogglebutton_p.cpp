#include "ntogglebutton_p.h"

#include <QPropertyAnimation>

#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToggleButton.h"

NToggleButtonPrivate::NToggleButtonPrivate(QObject* parent) : QObject(parent) {
    _pCheckAlpha = 0;

    _alphaAnimation = new QPropertyAnimation(this, "pCheckAlpha");
    _alphaAnimation->setDuration(NDesignToken(NDesignTokenKey::AnimationSlow).toInt());
    _alphaAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(_alphaAnimation, &QPropertyAnimation::valueChanged, this, [this]() {
        if (q_ptr) {
            q_ptr->update();
        }
    });

    connect(_alphaAnimation, &QPropertyAnimation::finished, this, [this]() { _isAnimationFinished = true; });
}

NToggleButtonPrivate::~NToggleButtonPrivate() { delete _alphaAnimation; }

void NToggleButtonPrivate::startAlphaAnimation(bool checked) {
    _alphaAnimation->stop();

    _isAnimationFinished = false;

    _alphaAnimation->setStartValue(_pCheckAlpha);
    _alphaAnimation->setEndValue(checked ? 255 : 0);

    _alphaAnimation->start();
}