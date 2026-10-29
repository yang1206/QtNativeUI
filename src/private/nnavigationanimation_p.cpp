#include "nnavigationanimation_p.h"

#include <QEvent>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <cmath>

NNavigationAnimationManager::NNavigationAnimationManager(QStackedWidget* stackedWidget, QObject* parent)
    : QObject(parent), m_stackedWidget(stackedWidget) {
    m_animation = new QPropertyAnimation(this, "progress", this);
    m_animation->setEasingCurve(QEasingCurve::Linear);
    connect(m_animation, &QPropertyAnimation::finished, this, &NNavigationAnimationManager::cancelTransition);
    connect(m_stackedWidget, &QStackedWidget::widgetRemoved, this, [this] { cancelTransition(); });
    connect(m_stackedWidget, &QStackedWidget::currentChanged, this, [this] {
        if (m_hiddenPage && m_hiddenPage != m_stackedWidget->currentWidget()) cancelTransition();
    });
    m_stackedWidget->installEventFilter(this);
}

NNavigationAnimationManager::~NNavigationAnimationManager() = default;

void NNavigationAnimationManager::cancelTransition() {
    ++m_requestId;
    m_animation->stop();
    m_targetStackPix = {};
    m_currentStackPix = {};
    if (m_blurEffect) m_stackedWidget->setGraphicsEffect(nullptr);
    const auto page = m_hiddenPage;
    m_hiddenPage.clear();
    m_stackedWidget->update();
    if (page && page == m_stackedWidget->currentWidget()) page->show();
}

bool NNavigationAnimationManager::eventFilter(QObject* watched, QEvent* event) {
    if (watched == m_stackedWidget && (event->type() == QEvent::Resize || event->type() == QEvent::Hide)
        && m_hiddenPage) {
        // 页面布局变化后旧快照已经失效，立即交还实时绘制。
        cancelTransition();
    }
    return QObject::eventFilter(watched, event);
}

void NNavigationAnimationManager::setProgress(qreal value) {
    m_progress = value;
    if (m_blurEffect)
        m_blurEffect->setBlurRadius(20 * (1 - QEasingCurve(QEasingCurve::OutCubic).valueForProgress(value)));
    m_stackedWidget->update();
}

void NNavigationAnimationManager::executeTransition(NNavigationType::PageTransitionType type,
                                                    int targetIndex, bool back, int duration) {
    QPointer<QWidget> target = m_stackedWidget->widget(targetIndex);
    if (!target) return;
    QPointer<NNavigationAnimationManager> guard(this);
    // 连续切换从当前可见帧接续，避免突然显示尚未进入完毕的页面。
    QPixmap interruptedFrame;
    if (!m_targetStackPix.isNull() && m_stackedWidget->isVisible()) interruptedFrame = m_stackedWidget->grab();
    if (!guard) return;
    cancelTransition();
    if (!guard || !target || m_stackedWidget->indexOf(target) < 0) return;
    const auto request = m_requestId;
    if (target == m_stackedWidget->currentWidget()) return;
    if (duration <= 0 || type == NNavigationType::NoTransition || !m_stackedWidget->isVisible()) {
        m_stackedWidget->setCurrentWidget(target);
        return;
    }

    m_transitionType = type;
    m_isRouteBack = back;
    QPixmap current = interruptedFrame;
    if (current.isNull() && type != NNavigationType::PopupTransition && type != NNavigationType::BlurTransition) {
        if (auto* page = m_stackedWidget->currentWidget(); page && page->isVisible()) current = page->grab();
    }
    if (!guard || request != m_requestId || !target) return;
    m_stackedWidget->setCurrentWidget(target);
    if (!guard || request != m_requestId || !target) return;
    const QPixmap next = target->grab();
    if (!guard || request != m_requestId || !target || target != m_stackedWidget->currentWidget() || next.isNull()) return;
    m_currentStackPix = current;
    m_targetStackPix = next;
    m_hiddenPage = target;
    target->hide();
    if (!guard || request != m_requestId) return;
    // 模糊只作用于固定快照，其他动画不安装图形效果，也不逐帧重绘整棵页面树。
    if (type == NNavigationType::BlurTransition && !m_stackedWidget->graphicsEffect()) {
        m_blurEffect = new QGraphicsBlurEffect;
        m_blurEffect->setBlurHints(QGraphicsBlurEffect::AnimationHint);
        m_stackedWidget->setGraphicsEffect(m_blurEffect);
    }
    m_animation->setDuration(duration);
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(1.0);
    m_animation->start();
}

void NNavigationAnimationManager::paintTransition(QPainter* painter, const QRect& rect, int borderRadius) {
    if (m_targetStackPix.isNull()) return;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath bounds;
    bounds.addRoundedRect(rect, borderRadius, borderRadius);
    painter->setClipPath(bounds, Qt::IntersectClip);
    const qreal eased = QEasingCurve(QEasingCurve::OutCubic).valueForProgress(m_progress);

    switch (m_transitionType) {
        case NNavigationType::NoTransition:
            break;
        case NNavigationType::PopupTransition:
            painter->drawPixmap(QRectF(rect).translated(0, (1 - eased) * 48), m_targetStackPix, QRectF(m_targetStackPix.rect()));
            break;
        case NNavigationType::ScaleTransition: {
            // 先淡出旧页，再淡入新页；透明页面也不能同时叠两份文字。
            const bool entering = m_progress >= 0.35;
            const qreal phase = entering ? (m_progress - 0.35) / 0.65 : m_progress / 0.35;
            const qreal zoom = QEasingCurve(QEasingCurve::OutCubic).valueForProgress(phase);
            const qreal start = entering ? (m_isRouteBack ? 1.15 : 0.85) : 1.0;
            const qreal end = entering ? 1.0 : (m_isRouteBack ? 0.85 : 1.15);
            const qreal ratio = start + (end - start) * zoom;
            painter->setOpacity(entering ? zoom : 1 - phase);
            const QPointF center = QRectF(rect).center();
            painter->translate(center);
            painter->scale(ratio, ratio);
            painter->translate(-center);
            painter->drawPixmap(rect, entering ? m_targetStackPix : m_currentStackPix);
            break;
        }
        case NNavigationType::FlipTransition:
        case NNavigationType::CubeTransition: {
            const qreal phase = QEasingCurve(QEasingCurve::InOutSine).valueForProgress(m_progress);
            const qreal angle = (m_isRouteBack ? -180 : 180) * phase;
            const bool entering = phase >= 0.5;
            const qreal rotation = entering ? angle + (m_isRouteBack ? 180 : -180) : angle;
            QTransform transform;
            const QPointF center = QRectF(rect).center();
            transform.translate(center.x(), center.y());
            transform.rotate(rotation, m_transitionType == NNavigationType::FlipTransition ? Qt::YAxis : Qt::XAxis);
            transform.translate(-center.x(), -center.y());
            painter->setTransform(transform, true);
            painter->drawPixmap(rect, entering ? m_targetStackPix : m_currentStackPix);
            break;
        }
        case NNavigationType::BlurTransition:
            painter->drawPixmap(rect, m_targetStackPix);
            break;
        case NNavigationType::RippleTransition: {
            const qreal radius = std::hypot(rect.width(), rect.height()) * eased / 2;
            QPainterPath ripple;
            ripple.addEllipse(QRectF(rect).center(), radius, radius);
            painter->save();
            painter->setClipPath(bounds.subtracted(ripple), Qt::IntersectClip);
            painter->drawPixmap(rect, m_currentStackPix);
            painter->restore();
            painter->setClipPath(ripple, Qt::IntersectClip);
            painter->drawPixmap(rect, m_targetStackPix);
            break;
        }
    }
    painter->restore();
}
