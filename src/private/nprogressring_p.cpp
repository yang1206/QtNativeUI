#include "nprogressring_p.h"
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "QtNativeUI/NProgressRing.h"
#include "QtNativeUI/NTheme.h"

NProgressRingPrivate::NProgressRingPrivate(QObject* parent) : QObject(parent) {
    _progressAnimation = new QPropertyAnimation(this, "progressPosition");
    _progressAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _progressAnimation->setDuration(200);

    _startAngleAni1 = new QPropertyAnimation(this, "startAngle");
    _startAngleAni1->setDuration(1000);
    _startAngleAni1->setStartValue(0.0);
    _startAngleAni1->setEndValue(450.0);
    _startAngleAni1->setEasingCurve(QEasingCurve::Linear);

    _startAngleAni2 = new QPropertyAnimation(this, "startAngle");
    _startAngleAni2->setDuration(1000);
    _startAngleAni2->setStartValue(450.0);
    _startAngleAni2->setEndValue(1080.0);
    _startAngleAni2->setEasingCurve(QEasingCurve::Linear);

    _startAngleAniGroup = new QSequentialAnimationGroup(this);
    _startAngleAniGroup->addAnimation(_startAngleAni1);
    _startAngleAniGroup->addAnimation(_startAngleAni2);

    _spanAngleAni1 = new QPropertyAnimation(this, "spanAngle");
    _spanAngleAni1->setDuration(1000);
    _spanAngleAni1->setStartValue(0.0);
    _spanAngleAni1->setEndValue(180.0);
    _spanAngleAni1->setEasingCurve(QEasingCurve::InOutQuad);

    _spanAngleAni2 = new QPropertyAnimation(this, "spanAngle");
    _spanAngleAni2->setDuration(1000);
    _spanAngleAni2->setStartValue(180.0);
    _spanAngleAni2->setEndValue(0.0);
    _spanAngleAni2->setEasingCurve(QEasingCurve::InOutQuad);

    _spanAngleAniGroup = new QSequentialAnimationGroup(this);
    _spanAngleAniGroup->addAnimation(_spanAngleAni1);
    _spanAngleAniGroup->addAnimation(_spanAngleAni2);

    _indeterminateAnimationGroup = new QParallelAnimationGroup(this);
    _indeterminateAnimationGroup->addAnimation(_startAngleAniGroup);
    _indeterminateAnimationGroup->addAnimation(_spanAngleAniGroup);
    _indeterminateAnimationGroup->setLoopCount(-1);
}

NProgressRingPrivate::~NProgressRingPrivate() {
    delete _progressAnimation;
    delete _indeterminateAnimationGroup;
}

QColor NProgressRingPrivate::getTrackColor() const {
    QColor color;
    if (!q_ptr->isEnabled()) {
        color = _isDark ? _pDarkDisabledTrackColor : _pLightDisabledTrackColor;
    } else {
        color = _isDark ? _pDarkTrackColor : _pLightTrackColor;
    }

    return color;
}

QColor NProgressRingPrivate::getProgressColor() const {
    if (!q_ptr->isEnabled()) {
        return _isDark ? _pDarkDisabledProgressColor : _pLightDisabledProgressColor;
    }
    return _isDark ? _pDarkProgressColor : _pLightProgressColor;
}

QColor NProgressRingPrivate::getTextColor() const {
    if (!q_ptr->isEnabled()) {
        return NThemeColor(NFluentColorKey::TextFillColorDisabled, _themeMode);
    }
    return _isDark ? _pDarkTextColor : _pLightTextColor;
}

int NProgressRingPrivate::calculateAngle() const {
    if (_maximum == _minimum)
        return 0;

    qreal progress = _maximum == _minimum ? 0 : _progressPosition;
    return qRound(360 * progress);
}

void NProgressRingPrivate::setProgressPosition(qreal position) {
    _progressPosition = qBound(0.0, position, 1.0);
    if (q_ptr)
        q_ptr->update();
}

void NProgressRingPrivate::startProgressAnimation(qreal endValue) {
    if (!_pAnimationEnabled || !q_ptr)
        return;

    _progressAnimation->stop();
    _progressAnimation->setDuration(_pAnimationDuration);
    _progressAnimation->setStartValue(_progressPosition);
    _progressAnimation->setEndValue(endValue);
    _progressAnimation->start();
}

void NProgressRingPrivate::startIndeterminateAnimations() {
    if (!_pAnimationEnabled || !q_ptr)
        return;
    _startAngle = 0.0;
    _spanAngle  = 0.0;

    if (_indeterminateAnimationGroup->state() != QAbstractAnimation::Running) {
        _indeterminateAnimationGroup->start();
    }

    _isAnimationRunning = true;
}

void NProgressRingPrivate::stopIndeterminateAnimation() {
    _indeterminateAnimationGroup->stop();
    _startAngle         = 0.0;
    _spanAngle          = 0.0;
    _isAnimationRunning = false;
}

void NProgressRingPrivate::updateAnimations() {
    if (!q_ptr)
        return;

    if (_minimum == _maximum) {
        stopIndeterminateAnimation();
        startIndeterminateAnimations();
    } else {
        stopIndeterminateAnimation();

        if (_maximum > _minimum) {
            qreal endValue = static_cast<qreal>(_value - _minimum) / static_cast<qreal>(_maximum - _minimum);
            startProgressAnimation(endValue);
        }
    }
}

QColor NProgressRingPrivate::getBarColorForState() const {
    if (_isError) {
        return _isDark ? _pDarkErrorColor : _pLightErrorColor;
    }

    if (_isPaused) {
        return _isDark ? _pDarkPausedColor : _pLightPausedColor;
    }

    return getProgressColor();
}

void NProgressRingPrivate::setPaused(bool paused) {
    if (_isPaused == paused)
        return;

    _isPaused = paused;

    if (paused) {
        if (_progressAnimation->state() == QPropertyAnimation::Running)
            _progressAnimation->pause();

        if (_indeterminateAnimationGroup->state() == QAbstractAnimation::Running)
            _indeterminateAnimationGroup->pause();
        _pausedProgressPosition = _progressPosition;
    } else {
        if (!_isError) {
            if (_progressAnimation->state() == QPropertyAnimation::Paused)
                _progressAnimation->resume();

            if (q_ptr && _minimum == _maximum && _indeterminateAnimationGroup->state() == QAbstractAnimation::Paused)
                _indeterminateAnimationGroup->resume();
        }
    }

    if (q_ptr)
        q_ptr->update();
}

void NProgressRingPrivate::setError(bool error) {
    if (_isError == error)
        return;

    _isError = error;

    if (error) {
        _progressAnimation->stop();
        _indeterminateAnimationGroup->stop();
    } else {
        if (!_isPaused && q_ptr) {
            updateAnimations();
        }
    }

    if (q_ptr)
        q_ptr->update();
}

void NProgressRingPrivate::setStartAngle(qreal angle) {
    _startAngle = angle;
    if (q_ptr)
        q_ptr->update();
}

qreal NProgressRingPrivate::spanAngle() const { return _spanAngle; }

void NProgressRingPrivate::setSpanAngle(qreal angle) {
    _spanAngle = angle;
    if (q_ptr)
        q_ptr->update();
}
