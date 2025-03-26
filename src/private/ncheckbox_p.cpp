#include "ncheckbox_p.h"
#include "QtNativeUI/NCheckBox.h"
#include "QtNativeUI/NTheme.h"

NCheckBoxPrivate::NCheckBoxPrivate(QObject* parent) : QObject(parent) {
    _pCheckAlpha = 0;

    _alphaAnimation = new QPropertyAnimation(this, "pCheckAlpha");
    _alphaAnimation->setDuration(NDesignToken(NDesignTokenKey::AnimationNormal).toInt());
    _alphaAnimation->setEasingCurve(QEasingCurve::OutCubic);

    connect(_alphaAnimation, &QPropertyAnimation::valueChanged, this, [this]() {
        if (q_ptr) {
            q_ptr->update();
        }
    });

    connect(_alphaAnimation, &QPropertyAnimation::finished, this, [this]() { _isAnimationFinished = true; });
}

NCheckBoxPrivate::~NCheckBoxPrivate() { delete _alphaAnimation; }

void NCheckBoxPrivate::startAlphaAnimation(bool checked) {
    _alphaAnimation->stop();

    _isAnimationFinished = false;

    _alphaAnimation->setStartValue(_pCheckAlpha);
    _alphaAnimation->setEndValue(checked ? 255 : 0);

    _alphaAnimation->start();
}