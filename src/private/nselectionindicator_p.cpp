#include "nselectionindicator_p.h"

#include <QtNativeUI/NAnimation.h>

namespace {

qreal lerp(qreal from, qreal to, qreal t) { return from + (to - from) * t; }

} // namespace

NSelectionIndicatorMotion::NSelectionIndicatorMotion(QObject* parent) : QObject(parent) {
    m_animation = new QtNativeUI::NPointToPointAnimation(this);
    m_animation->setTargetObject(this);
    m_animation->setPropertyName("progress");
    m_animation->setSpeed(QtNativeUI::NAnimationSpeed::Medium);
    connect(m_animation, &QtNativeUI::NFluentAnimation::valueChanged, this, [this](const QVariant&) {
        emit tick();
    });
}

NSelectionIndicatorMotion::~NSelectionIndicatorMotion() = default;

qreal NSelectionIndicatorMotion::progress() const { return m_progress; }

void NSelectionIndicatorMotion::setProgress(qreal value) {
    const qreal clamped = qBound(0.0, value, 1.0);
    if (qFuzzyCompare(m_progress + 1.0, clamped + 1.0))
        return;
    m_progress = clamped;
    emit progressChanged(m_progress);
    emit tick();
}

NSelectionIndicatorMotion::Direction NSelectionIndicatorMotion::direction() const { return m_direction; }

void NSelectionIndicatorMotion::setDirection(Direction direction) { m_direction = direction; }

void NSelectionIndicatorMotion::start(bool animated) {
    if (!m_animation)
        return;
    m_animation->stop();
    if (!animated) {
        setProgress(1.0);
        return;
    }
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(1.0);
    setProgress(0.0);
    m_animation->start();
}

void NSelectionIndicatorMotion::finishImmediately() {
    if (m_animation)
        m_animation->stop();
    setProgress(1.0);
}

void NSelectionIndicatorMotion::reset() {
    if (m_animation)
        m_animation->stop();
    m_progress  = 1.0;
    m_direction = Direction::None;
}

namespace NSelectionIndicator {

qreal verticalInset(const QRectF& itemRect) {
    if (!itemRect.isValid() || itemRect.height() <= 0.0)
        return 10.0;
    return itemRect.height() / 4.0;
}

QRectF markRectForItem(const QRectF& itemRect) {
    if (!itemRect.isValid())
        return {};
    const qreal inset = verticalInset(itemRect);
    const qreal height = itemRect.height() - 2.0 * inset;
    if (height <= 0.5)
        return {};
    return QRectF(itemRect.x() + kMarkLeftOffset, itemRect.y() + inset, kMarkWidth, height);
}

qreal leadingProgress(qreal progress) { return qBound(0.0, progress * 1.35, 1.0); }

qreal trailingProgress(qreal progress) { return qBound(0.0, (progress - 0.18) / 0.82, 1.0); }

QRectF interpolatedMarkRect(const QRectF& previous, const QRectF& target, qreal progress,
                            NSelectionIndicatorMotion::Direction direction) {
    if (previous.isEmpty())
        return target;
    if (target.isEmpty())
        return previous;
    const qreal clamped = qBound(0.0, progress, 1.0);
    if (qFuzzyCompare(clamped + 1.0, 2.0))
        return target;
    if (qFuzzyCompare(clamped + 1.0, 1.0))
        return previous;
    const qreal leading  = leadingProgress(clamped);
    const qreal trailing = trailingProgress(clamped);
    qreal       top      = lerp(previous.top(), target.top(), clamped);
    qreal       bottom   = lerp(previous.bottom(), target.bottom(), clamped);
    qreal       left     = lerp(previous.left(), target.left(), clamped);
    qreal       right    = lerp(previous.right(), target.right(), clamped);
    switch (direction) {
        case NSelectionIndicatorMotion::Direction::Down:
            top    = lerp(previous.top(), target.top(), trailing);
            bottom = lerp(previous.bottom(), target.bottom(), leading);
            left   = target.left();
            right  = target.right();
            break;
        case NSelectionIndicatorMotion::Direction::Up:
            top    = lerp(previous.top(), target.top(), leading);
            bottom = lerp(previous.bottom(), target.bottom(), trailing);
            left   = target.left();
            right  = target.right();
            break;
        case NSelectionIndicatorMotion::Direction::None:
            return target;
    }
    const qreal normalizedLeft   = qMin(left, right);
    const qreal normalizedTop    = qMin(top, bottom);
    const qreal normalizedRight  = qMax(left, right);
    const qreal normalizedBottom = qMax(top, bottom);
    return QRectF(QPointF(normalizedLeft, normalizedTop), QPointF(normalizedRight, normalizedBottom));
}

void paintMark(QPainter* painter, const QRectF& markRect, const QColor& accentColor) {
    if (!painter || markRect.isEmpty())
        return;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(accentColor);
    painter->drawRoundedRect(markRect, kCornerRadius, kCornerRadius);
    painter->restore();
}

} // namespace NSelectionIndicator

NSelectionIndicatorSession::NSelectionIndicatorSession(std::function<void()> repaint, QObject* parent)
    : QObject(parent), m_repaint(std::move(repaint)) {
    connect(&m_motion, &NSelectionIndicatorMotion::tick, this, [this]() {
        if (m_repaint)
            m_repaint();
    });
}

void NSelectionIndicatorSession::clear() {
    m_motion.reset();
    m_previousRect         = nullptr;
    m_currentRect          = nullptr;
    m_animateFromPrevious  = false;
}

void NSelectionIndicatorSession::showAt(RectFn currentRect) {
    m_previousRect        = nullptr;
    m_currentRect         = std::move(currentRect);
    m_animateFromPrevious = false;
    m_motion.setDirection(NSelectionIndicatorMotion::Direction::None);
    m_motion.start(false);
    if (m_repaint)
        m_repaint();
}

void NSelectionIndicatorSession::transition(RectFn previousRect, RectFn currentRect,
                                            NSelectionIndicatorMotion::Direction direction) {
    m_previousRect        = std::move(previousRect);
    m_currentRect         = std::move(currentRect);
    m_animateFromPrevious = direction != NSelectionIndicatorMotion::Direction::None;
    m_motion.setDirection(direction);
    m_motion.start(m_animateFromPrevious);
    if (m_repaint)
        m_repaint();
}

QRectF NSelectionIndicatorSession::currentRect() const {
    if (!m_currentRect)
        return {};
    const QRectF target = m_currentRect();
    if (target.isEmpty())
        return {};
    if (!m_animateFromPrevious || m_motion.direction() == NSelectionIndicatorMotion::Direction::None ||
        !m_previousRect) {
        return target;
    }
    const QRectF previous = m_previousRect();
    if (previous.isEmpty())
        return target;
    return NSelectionIndicator::interpolatedMarkRect(previous, target, m_motion.progress(), m_motion.direction());
}
