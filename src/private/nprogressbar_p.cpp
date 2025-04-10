#include "nprogressbar_p.h"
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "QtNativeUI/NProgressBar.h"
#include "QtNativeUI/NTheme.h"

NProgressBarPrivate::NProgressBarPrivate(QObject* parent) : QObject(parent) {
    // 初始化动画
    _progressAnimation = new QPropertyAnimation(this, "progressPosition");
    _progressAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _progressAnimation->setDuration(200);

    // 短段动画
    _shortPosAnimation = new QPropertyAnimation(this, "shortPos");
    _shortPosAnimation->setDuration(833);
    _shortPosAnimation->setStartValue(0.0);
    _shortPosAnimation->setEndValue(1.45);
    _shortPosAnimation->setEasingCurve(QEasingCurve::Linear);

    // 长段动画
    _longPosAnimation = new QPropertyAnimation(this, "longPos");
    _longPosAnimation->setDuration(1167);
    _longPosAnimation->setStartValue(0.0);
    _longPosAnimation->setEndValue(1.75);
    _longPosAnimation->setEasingCurve(QEasingCurve::OutQuad);

    // 创建动画组
    _longAnimationGroup = new QSequentialAnimationGroup(this);
    _longAnimationGroup->addPause(785);
    _longAnimationGroup->addAnimation(_longPosAnimation);

    _indeterminateAnimationGroup = new QParallelAnimationGroup(this);
    _indeterminateAnimationGroup->addAnimation(_shortPosAnimation);
    _indeterminateAnimationGroup->addAnimation(_longAnimationGroup);
    _indeterminateAnimationGroup->setLoopCount(-1); // 无限循环
}

NProgressBarPrivate::~NProgressBarPrivate() {
    delete _progressAnimation;
    delete _indeterminateAnimationGroup; // 组会自动删除内部动画
}

QColor NProgressBarPrivate::getTrackColor() const {
    QColor color;
    if (!q_ptr->isEnabled()) {
        color = _isDark ? _pDarkDisabledTrackColor : _pLightDisabledTrackColor;
    } else {
        color = _isDark ? _pDarkTrackColor : _pLightTrackColor;
    }
    
    // 添加半透明效果
    color.setAlpha(155);
    return color;
}

QColor NProgressBarPrivate::getProgressColor() const {
    if (!q_ptr->isEnabled()) {
        return _isDark ? _pDarkDisabledProgressColor : _pLightDisabledProgressColor;
    }
    return _isDark ? _pDarkProgressColor : _pLightProgressColor;
}

QColor NProgressBarPrivate::getTextColor() const {
    if (!q_ptr->isEnabled()) {
        return NThemeColor(NFluentColorKey::TextFillColorDisabled, _themeMode);
    }
    return _isDark ? _pDarkTextColor : _pLightTextColor;
}

QRect NProgressBarPrivate::getTrackRect() const {
    QRect rect = q_ptr->rect();
    
    if (q_ptr->orientation() == Qt::Horizontal) {
        // 对于水平进度条，高度设为轨道厚度，但y位置设为中心
        int y = rect.y() + (rect.height() - _pProgressThickness) / 2;
        return QRect(rect.x(), y, rect.width(), _pProgressThickness);
    } else {
        // 对于垂直进度条，宽度设为轨道厚度，但x位置设为中心
        int x = rect.x() + (rect.width() - _pProgressThickness) / 2;
        return QRect(x, rect.y(), _pProgressThickness, rect.height());
    }
}

QRect NProgressBarPrivate::getProgressRect() const {
    QRect trackRect = getTrackRect();
    
    if (q_ptr->orientation() == Qt::Horizontal) {
        int width = calculateProgressWidth();
        // 使进度条高度是轨道厚度的两倍，确保可见性
        int yOffset = (q_ptr->height() - _pProgressThickness) / 2;
        return QRect(trackRect.x(), yOffset, width, _pProgressThickness);
    } else {
        int height = calculateProgressHeight();
        int y = trackRect.bottom() - height;
        // 使进度条宽度是轨道厚度的两倍，确保可见性
        int xOffset = (q_ptr->width() - _pProgressThickness) / 2;
        return QRect(xOffset, y, _pProgressThickness, height);
    }
}

int NProgressBarPrivate::calculateProgressWidth() const {
    if (q_ptr->maximum() == q_ptr->minimum())
        return getTrackRect().width();

    qreal progress = q_ptr->minimum() == q_ptr->maximum() ? _indeterminateOffset : _progressPosition;
    return qRound(getTrackRect().width() * progress);
}

int NProgressBarPrivate::calculateProgressHeight() const {
    if (q_ptr->maximum() == q_ptr->minimum())
        return getTrackRect().height();

    qreal progress = q_ptr->minimum() == q_ptr->maximum() ? 0 : _progressPosition;
    return qRound(getTrackRect().height() * progress);
}

void NProgressBarPrivate::setProgressPosition(qreal position) {
    _progressPosition = qBound(0.0, position, 1.0);
    if (q_ptr)
        q_ptr->update();
}

void NProgressBarPrivate::setIndeterminateOffset(qreal offset) {
    _indeterminateOffset = offset;
    if (q_ptr)
        q_ptr->update();
}

void NProgressBarPrivate::startProgressAnimation(qreal endValue) {
    if (!_pAnimationEnabled || !q_ptr)
        return;

    _progressAnimation->stop();
    _progressAnimation->setDuration(_pAnimationDuration);
    _progressAnimation->setStartValue(_progressPosition);
    _progressAnimation->setEndValue(endValue);
    _progressAnimation->start();
}

void NProgressBarPrivate::startIndeterminateAnimations() {
    if (!_pAnimationEnabled || !q_ptr)
        return;

    // 重置位置并启动动画
    _shortPos = 0;
    _longPos  = 0;

    if (_indeterminateAnimationGroup->state() != QAbstractAnimation::Running) {
        _indeterminateAnimationGroup->start();
    }

    _isAnimationRunning = true;
}

void NProgressBarPrivate::stopIndeterminateAnimation() {
    _indeterminateAnimationGroup->stop();
    _shortPos           = 0;
    _longPos            = 0;
    _isAnimationRunning = false;
}

void NProgressBarPrivate::updateAnimations() {
    if (!q_ptr)
        return;

    if (q_ptr->minimum() == q_ptr->maximum()) {
        stopIndeterminateAnimation();
        startIndeterminateAnimations();
    } else {
        stopIndeterminateAnimation();

        if (q_ptr->maximum() > q_ptr->minimum()) {
            qreal endValue = static_cast<qreal>(q_ptr->value() - q_ptr->minimum()) /
                             static_cast<qreal>(q_ptr->maximum() - q_ptr->minimum());
            startProgressAnimation(endValue);
        }
    }
}

void NProgressBarPrivate::setPaused(bool paused) {
    if (_isPaused == paused)
        return;

    _isPaused = paused;

    if (paused) {
        // 暂停所有动画
        if (_progressAnimation->state() == QPropertyAnimation::Running)
            _progressAnimation->pause();

        if (_indeterminateAnimationGroup->state() == QAbstractAnimation::Running)
            _indeterminateAnimationGroup->pause();
            
        // 在暂停时保存当前状态，以便恢复时继续
        _pausedProgressPosition = _progressPosition;
    } else {
        // 如果不在错误状态，则恢复动画
        if (!_isError) {
            if (_progressAnimation->state() == QPropertyAnimation::Paused)
                _progressAnimation->resume();

            if (q_ptr && q_ptr->minimum() == q_ptr->maximum() &&
                _indeterminateAnimationGroup->state() == QAbstractAnimation::Paused)
                _indeterminateAnimationGroup->resume();
        }
    }

    if (q_ptr)
        q_ptr->update();
}

void NProgressBarPrivate::setError(bool error) {
    if (_isError == error)
        return;

    _isError = error;

    if (error) {
        // 停止所有动画
        _progressAnimation->stop();
        _indeterminateAnimationGroup->stop();
    } else {
        // 恢复到正常状态
        if (!_isPaused && q_ptr) {
            updateAnimations();
        }
    }

    if (q_ptr)
        q_ptr->update();
}

QColor NProgressBarPrivate::getBarColorForState() const {
    if (_isError) {
        return _isDark ? _pDarkErrorColor : _pLightErrorColor;
    }
    
    if (_isPaused) {
        return _isDark ? _pDarkPausedColor : _pLightPausedColor;
    }
    
    return getProgressColor();
}

void NProgressBarPrivate::setShortPos(qreal pos) {
    _shortPos = pos;
    if (q_ptr)
        q_ptr->update();
}

void NProgressBarPrivate::setLongPos(qreal pos) {
    _longPos = pos;
    if (q_ptr)
        q_ptr->update();
}