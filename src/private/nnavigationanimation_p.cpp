#include "nnavigationanimation_p.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTransform>

NNavigationAnimationManager::NNavigationAnimationManager(QObject* parent) : QObject(parent) {}

NNavigationAnimationManager::~NNavigationAnimationManager() {}

void NNavigationAnimationManager::executeTransition(QWidget*                            targetWidget,
                                                    NNavigationType::PageTransitionType type,
                                                    int                                 duration) {
    if (!targetWidget)
        return;

    switch (type) {
        case NNavigationType::FadeTransition:
            executeFadeTransition(targetWidget, duration);
            break;
        case NNavigationType::SlideHorizontal:
            executeSlideHorizontalTransition(targetWidget, duration);
            break;
        case NNavigationType::SlideVertical:
            executeSlideVerticalTransition(targetWidget, duration);
            break;
        case NNavigationType::ZoomTransition:
            executeZoomTransition(targetWidget, duration);
            break;
        case NNavigationType::FlipTransition:
            executeFlipTransition(targetWidget, duration);
            break;
        case NNavigationType::PushTransition:
            executePushTransition(targetWidget, duration);
            break;
        case NNavigationType::RevealTransition:
            executeRevealTransition(targetWidget, duration);
            break;
        case NNavigationType::ElasticTransition:
            executeElasticTransition(targetWidget, duration);
            break;
        default:
            break;
    }
}

void NNavigationAnimationManager::executeFadeTransition(QWidget* targetWidget, int duration) {
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(targetWidget);
    targetWidget->setGraphicsEffect(effect);
    effect->setOpacity(0);

    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutQuint);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::executeSlideHorizontalTransition(QWidget* targetWidget, int duration) {
    QPropertyAnimation* animation = new QPropertyAnimation(targetWidget, "pos");
    animation->setDuration(duration);

    QPoint finalPos = targetWidget->pos();
    QPoint startPos = finalPos;
    startPos.setX(finalPos.x() + targetWidget->width());

    animation->setStartValue(startPos);
    animation->setEndValue(finalPos);
    animation->setEasingCurve(QEasingCurve::OutQuint);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::executeSlideVerticalTransition(QWidget* targetWidget, int duration) {
    QPropertyAnimation* animation = new QPropertyAnimation(targetWidget, "pos");
    QPoint              finalPos  = targetWidget->pos();
    QPoint              startPos  = finalPos;
    startPos.setY(finalPos.y() + 80);

    animation->setStartValue(startPos);
    animation->setEndValue(finalPos);
    animation->setEasingCurve(QEasingCurve::OutQuint);
    animation->setDuration(duration);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::executeZoomTransition(QWidget* targetWidget, int duration) {
    QParallelAnimationGroup* group = new QParallelAnimationGroup;

    QPropertyAnimation* scaleAnimation = new QPropertyAnimation(targetWidget, "geometry");
    QRect               endRect        = targetWidget->geometry();
    QRect               startRect      = endRect;

    startRect.setWidth(endRect.width() * 0.85);
    startRect.setHeight(endRect.height() * 0.85);
    startRect.moveCenter(endRect.center());

    scaleAnimation->setDuration(duration);
    scaleAnimation->setStartValue(startRect);
    scaleAnimation->setEndValue(endRect);
    scaleAnimation->setEasingCurve(QEasingCurve::OutQuint);

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(targetWidget);
    targetWidget->setGraphicsEffect(effect);
    effect->setOpacity(0);

    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(effect, "opacity");
    opacityAnimation->setDuration(duration);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setEasingCurve(QEasingCurve::OutQuint);

    group->addAnimation(scaleAnimation);
    group->addAnimation(opacityAnimation);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::executeFlipTransition(QWidget* targetWidget, int duration) {
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(targetWidget, "windowOpacity");
    opacityAnimation->setDuration(duration);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setEasingCurve(QEasingCurve::OutQuint);
    NRotationEffect* rotationEffect = new NRotationEffect(targetWidget);
    targetWidget->setGraphicsEffect(rotationEffect);
    rotationEffect->setAxis(Qt::YAxis);
    QPropertyAnimation* rotationAnimation = new QPropertyAnimation(rotationEffect, "angle");
    rotationAnimation->setDuration(duration);
    rotationAnimation->setStartValue(90.0); // 从90度开始
    rotationAnimation->setEndValue(0.0);    // 旋转到0度
    rotationAnimation->setEasingCurve(QEasingCurve::OutQuint);
    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(opacityAnimation);
    group->addAnimation(rotationAnimation);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::executePushTransition(QWidget* targetWidget, int duration) {
    QPropertyAnimation* animation = new QPropertyAnimation(targetWidget, "pos");
    animation->setDuration(duration);

    QPoint finalPos = targetWidget->pos();
    QPoint startPos = finalPos;
    startPos.setX(finalPos.x() + targetWidget->width());

    animation->setStartValue(startPos);
    animation->setEndValue(finalPos);
    animation->setEasingCurve(QEasingCurve::OutBack); // 使用回弹效果
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(targetWidget);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(5, 5);
    targetWidget->setGraphicsEffect(shadowEffect);
    connect(animation, &QPropertyAnimation::finished, this, [this, targetWidget, shadowEffect]() {
        cleanupEffects(targetWidget, shadowEffect);
    });
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::executeRevealTransition(QWidget* targetWidget, int duration) {
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(targetWidget);
    targetWidget->setGraphicsEffect(effect);
    effect->setOpacity(0.0);
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(effect, "opacity");
    opacityAnimation->setDuration(duration);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setEasingCurve(QEasingCurve::OutQuint);
    QPropertyAnimation* slideAnimation = new QPropertyAnimation(targetWidget, "pos");
    slideAnimation->setDuration(duration);

    QPoint finalPos = targetWidget->pos();
    QPoint startPos = finalPos;
    startPos.setY(finalPos.y() + 50); // 从下方滑入

    slideAnimation->setStartValue(startPos);
    slideAnimation->setEndValue(finalPos);
    slideAnimation->setEasingCurve(QEasingCurve::OutQuint);
    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(opacityAnimation);
    group->addAnimation(slideAnimation);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::executeElasticTransition(QWidget* targetWidget, int duration) {
    QPropertyAnimation* animation = new QPropertyAnimation(targetWidget, "pos");
    animation->setDuration(duration * 1.2); // 稍微延长时间以适应弹性效果

    QPoint finalPos = targetWidget->pos();
    QPoint startPos = finalPos;
    startPos.setY(finalPos.y() + 100); // 从更远的地方开始

    animation->setStartValue(startPos);
    animation->setEndValue(finalPos);
    animation->setEasingCurve(QEasingCurve::OutElastic); // 使用弹性缓动曲线
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(targetWidget);
    targetWidget->setGraphicsEffect(effect);
    effect->setOpacity(0.0);
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(effect, "opacity");
    opacityAnimation->setDuration(duration * 0.7); // 透明度动画稍短
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setEasingCurve(QEasingCurve::OutQuint);
    QParallelAnimationGroup* group = new QParallelAnimationGroup;
    group->addAnimation(animation);
    group->addAnimation(opacityAnimation);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationAnimationManager::cleanupEffects(QWidget* widget, QGraphicsEffect* effect) {
    if (widget && effect) {
        widget->setGraphicsEffect(nullptr);
        effect->deleteLater();
    }
}

NRotationEffect::NRotationEffect(QObject* parent) : QGraphicsEffect(parent), m_angle(0), m_axis(Qt::YAxis) {}

NRotationEffect::~NRotationEffect() {}

void NRotationEffect::setAngle(qreal angle) {
    if (m_angle != angle) {
        m_angle = angle;
        update();
    }
}

void NRotationEffect::setAxis(Qt::Axis axis) {
    if (m_axis != axis) {
        m_axis = axis;
        update();
    }
}

void NRotationEffect::draw(QPainter* painter) {
    if (!painter)
        return;
    painter->save();
    QPoint  offset;
    QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);

    if (pixmap.isNull()) {
        painter->restore();
        return;
    }
    QPointF    center = QPointF(pixmap.width() / 2.0, pixmap.height() / 2.0);
    QTransform transform;
    transform.translate(offset.x() + center.x(), offset.y() + center.y());

    switch (m_axis) {
        case Qt::XAxis:
            transform.rotate(m_angle, Qt::XAxis);
            break;
        case Qt::YAxis:
            transform.rotate(m_angle, Qt::YAxis);
            break;
        case Qt::ZAxis:
            transform.rotate(m_angle, Qt::ZAxis);
            break;
    }

    transform.translate(-center.x(), -center.y());

    painter->setTransform(transform, true);
    painter->drawPixmap(0, 0, pixmap);

    painter->restore();
}

QRectF NRotationEffect::boundingRectFor(const QRectF& rect) const { return rect.adjusted(-10, -10, 10, 10); }
