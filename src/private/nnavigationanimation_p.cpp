#include "nnavigationanimation_p.h"

#include <QApplication>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QTimer>
#include <cmath>

NNavigationAnimationManager::NNavigationAnimationManager(QStackedWidget* stackedWidget, QObject* parent)
    : QObject(parent), m_stackedWidget(stackedWidget) {
    m_blurEffect = new QGraphicsBlurEffect(m_stackedWidget);
    m_blurEffect->setBlurHints(QGraphicsBlurEffect::BlurHint::QualityHint);
    m_blurEffect->setBlurRadius(0);
    m_blurEffect->setEnabled(false);
    m_stackedWidget->setGraphicsEffect(m_blurEffect);
}

NNavigationAnimationManager::~NNavigationAnimationManager() {}

void NNavigationAnimationManager::executeTransition(NNavigationType::PageTransitionType type,
                                                    int                                 targetIndex,
                                                    bool                                isRouteBack,
                                                    int                                 duration) {
    m_transitionType = type;

    switch (type) {
        case NNavigationType::NoTransition: {
            m_stackedWidget->setCurrentIndex(targetIndex);
            break;
        }
        case NNavigationType::PopupTransition: {
            QTimer::singleShot(180, this, [=]() {
                QWidget* targetWidget = m_stackedWidget->widget(targetIndex);
                m_stackedWidget->setCurrentIndex(targetIndex);
                getTargetStackPix();
                targetWidget->setVisible(false);

                QPropertyAnimation* popupAnimation = new QPropertyAnimation(this, "popupAnimationYOffset");
                connect(popupAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                    m_stackedWidget->parentWidget()->update();
                });
                connect(popupAnimation, &QPropertyAnimation::finished, this, [=]() {
                    m_targetStackPix = QPixmap();
                    targetWidget->setVisible(true);
                });
                popupAnimation->setEasingCurve(QEasingCurve::OutCubic);
                popupAnimation->setDuration(duration);
                int targetWidgetY = m_stackedWidget->mapToParent(QPoint(0, 0)).y();
                popupAnimation->setEndValue(targetWidgetY);
                targetWidgetY += 80;
                popupAnimation->setStartValue(targetWidgetY);
                popupAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            });
            break;
        }
        case NNavigationType::ScaleTransition: {
            QWidget* targetWidget = m_stackedWidget->widget(targetIndex);
            getCurrentStackPix();
            m_stackedWidget->setCurrentIndex(targetIndex);
            getTargetStackPix();
            targetWidget->setVisible(false);
            m_isDrawNewPix = false;

            QPropertyAnimation* currentPixZoomAnimation = new QPropertyAnimation(this, "scaleAnimationRatio");
            connect(currentPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                m_stackedWidget->parentWidget()->update();
            });
            connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
                m_isDrawNewPix = true;
                QPropertyAnimation* targetPixZoomAnimation = new QPropertyAnimation(this, "scaleAnimationRatio");
                connect(targetPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                    m_stackedWidget->parentWidget()->update();
                });
                connect(targetPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
                    m_targetStackPix = QPixmap();
                    m_currentStackPix = QPixmap();
                    targetWidget->setVisible(true);
                });
                if (isRouteBack) {
                    targetPixZoomAnimation->setStartValue(1.5);
                    targetPixZoomAnimation->setEndValue(1);
                } else {
                    targetPixZoomAnimation->setStartValue(0.85);
                    targetPixZoomAnimation->setEndValue(1);
                }
                targetPixZoomAnimation->setDuration(duration);
                targetPixZoomAnimation->setEasingCurve(QEasingCurve::OutCubic);
                targetPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            });

            if (isRouteBack) {
                currentPixZoomAnimation->setStartValue(1);
                currentPixZoomAnimation->setEndValue(0.85);
            } else {
                currentPixZoomAnimation->setStartValue(1);
                currentPixZoomAnimation->setEndValue(1.15);
            }
            currentPixZoomAnimation->setDuration(150);
            currentPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* currentPixOpacityAnimation = new QPropertyAnimation(this, "scaleAnimationPixOpacity");
            connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
                QPropertyAnimation* targetPixOpacityAnimation = new QPropertyAnimation(this, "scaleAnimationPixOpacity");
                targetPixOpacityAnimation->setStartValue(0);
                targetPixOpacityAnimation->setEndValue(1);
                targetPixOpacityAnimation->setDuration(duration);
                targetPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            });
            currentPixOpacityAnimation->setStartValue(1);
            currentPixOpacityAnimation->setEndValue(0);
            currentPixOpacityAnimation->setDuration(150);
            currentPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case NNavigationType::FlipTransition: {
            QWidget* targetWidget = m_stackedWidget->widget(targetIndex);
            getCurrentStackPix();
            m_stackedWidget->setCurrentIndex(targetIndex);
            getTargetStackPix();
            targetWidget->setVisible(false);

            QPropertyAnimation* flipAnimation = new QPropertyAnimation(this, "flipAnimationRatio");
            connect(flipAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                m_stackedWidget->parentWidget()->update();
            });
            connect(flipAnimation, &QPropertyAnimation::finished, this, [=]() {
                m_targetStackPix = QPixmap();
                m_currentStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            flipAnimation->setEasingCurve(QEasingCurve::InOutSine);
            flipAnimation->setDuration(650);
            flipAnimation->setStartValue(0);
            flipAnimation->setEndValue(isRouteBack ? -180 : 180);
            flipAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case NNavigationType::BlurTransition: {
            m_targetStackPix = QPixmap();
            m_blurEffect->setEnabled(true);

            QPropertyAnimation* blurAnimation = new QPropertyAnimation(this, "blurAnimationRadius");
            connect(blurAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                m_blurEffect->setBlurRadius(m_blurAnimationRadius);
            });
            connect(blurAnimation, &QPropertyAnimation::finished, this, [=]() { m_blurEffect->setEnabled(false); });
            blurAnimation->setEasingCurve(QEasingCurve::InOutSine);
            blurAnimation->setDuration(350);
            blurAnimation->setStartValue(40);
            blurAnimation->setEndValue(2);
            blurAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            QApplication::processEvents();
            m_stackedWidget->setCurrentIndex(targetIndex);
            break;
        }
        case NNavigationType::CubeTransition: {
            QWidget* targetWidget = m_stackedWidget->widget(targetIndex);
            getCurrentStackPix();
            m_stackedWidget->setCurrentIndex(targetIndex);
            getTargetStackPix();
            targetWidget->setVisible(false);

            QPropertyAnimation* cubeAnimation = new QPropertyAnimation(this, "cubeAnimationAngle");
            connect(cubeAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                m_stackedWidget->parentWidget()->update();
            });
            connect(cubeAnimation, &QPropertyAnimation::finished, this, [=]() {
                m_targetStackPix = QPixmap();
                m_currentStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            cubeAnimation->setEasingCurve(QEasingCurve::InOutSine);
            cubeAnimation->setDuration(650);
            cubeAnimation->setStartValue(0);
            cubeAnimation->setEndValue(isRouteBack ? -180 : 180);
            cubeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case NNavigationType::RippleTransition: {
            QWidget* targetWidget = m_stackedWidget->widget(targetIndex);
            getCurrentStackPix();
            m_stackedWidget->setCurrentIndex(targetIndex);
            getTargetStackPix();
            targetWidget->setVisible(false);

            QPropertyAnimation* rippleAnimation = new QPropertyAnimation(this, "rippleAnimationRadius");
            connect(rippleAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                m_stackedWidget->parentWidget()->update();
            });
            connect(rippleAnimation, &QPropertyAnimation::finished, this, [=]() {
                m_targetStackPix = QPixmap();
                m_currentStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            rippleAnimation->setEasingCurve(QEasingCurve::OutCubic);
            rippleAnimation->setDuration(duration);
            rippleAnimation->setStartValue(0);
            QRect rect = m_stackedWidget->rect();
            qreal maxRadius = qSqrt(rect.width() * rect.width() + rect.height() * rect.height()) / 2.0;
            rippleAnimation->setEndValue(maxRadius);
            rippleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
    }
}

void NNavigationAnimationManager::paintTransition(QPainter* painter, const QRect& rect, int borderRadius) {
    if (m_targetStackPix.isNull())
        return;

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect centralStackRect = m_stackedWidget->rect();

    if (borderRadius > 0) {
        QPainterPath clipPath;
        clipPath.addRoundedRect(centralStackRect, borderRadius, borderRadius);
        painter->setClipPath(clipPath);
    }

    switch (m_transitionType) {
        case NNavigationType::NoTransition: {
            break;
        }
        case NNavigationType::PopupTransition: {
            painter->drawPixmap(QRect(0, m_popupAnimationYOffset, rect.width(), m_stackedWidget->height()),
                               m_targetStackPix);
            break;
        }
        case NNavigationType::ScaleTransition: {
            painter->setOpacity(m_scaleAnimationPixOpacity);
            painter->translate(m_stackedWidget->rect().center());
            painter->scale(m_scaleAnimationRatio, m_scaleAnimationRatio);
            painter->translate(-m_stackedWidget->rect().center());
            painter->drawPixmap(centralStackRect, m_isDrawNewPix ? m_targetStackPix : m_currentStackPix);
            break;
        }
        case NNavigationType::FlipTransition: {
            QTransform transform;
            transform.translate(centralStackRect.center().x(), 0);
            if (abs(m_flipAnimationRatio) >= 90) {
                transform.rotate(-180 + m_flipAnimationRatio, Qt::YAxis);
            } else {
                transform.rotate(m_flipAnimationRatio, Qt::YAxis);
            }
            transform.translate(-centralStackRect.center().x(), 0);
            painter->setTransform(transform);
            if (abs(m_flipAnimationRatio) >= 90) {
                painter->drawPixmap(centralStackRect, m_targetStackPix);
            } else {
                painter->drawPixmap(centralStackRect, m_currentStackPix);
            }
            break;
        }
        case NNavigationType::BlurTransition: {
            break;
        }
        case NNavigationType::CubeTransition: {
            QTransform transform;
            transform.translate(0, centralStackRect.center().y());
            if (abs(m_cubeAnimationAngle) >= 90) {
                transform.rotate(-180 + m_cubeAnimationAngle, Qt::XAxis);
            } else {
                transform.rotate(m_cubeAnimationAngle, Qt::XAxis);
            }
            transform.translate(0, -centralStackRect.center().y());
            painter->setTransform(transform);
            if (abs(m_cubeAnimationAngle) >= 90) {
                painter->drawPixmap(centralStackRect, m_targetStackPix);
            } else {
                painter->drawPixmap(centralStackRect, m_currentStackPix);
            }
            break;
        }
        case NNavigationType::RippleTransition: {
            QPainterPath ripplePath;
            QPointF center = centralStackRect.center();
            ripplePath.addEllipse(center, m_rippleAnimationRadius, m_rippleAnimationRadius);
            painter->setClipPath(ripplePath);
            painter->drawPixmap(centralStackRect, m_targetStackPix);
            
            if (m_rippleAnimationRadius < centralStackRect.width() / 2.0) {
                painter->setClipping(false);
                QPainterPath inversePath;
                inversePath.addRect(centralStackRect);
                inversePath = inversePath.subtracted(ripplePath);
                painter->setClipPath(inversePath);
                painter->drawPixmap(centralStackRect, m_currentStackPix);
            }
            break;
        }
    }

    painter->restore();
}

void NNavigationAnimationManager::getCurrentStackPix() {
    m_targetStackPix = QPixmap();
    m_stackedWidget->currentWidget()->setVisible(true);
    m_currentStackPix = m_stackedWidget->grab();
    m_stackedWidget->currentWidget()->setVisible(false);
}

void NNavigationAnimationManager::getTargetStackPix() {
    m_targetStackPix = QPixmap();
    m_targetStackPix = m_stackedWidget->grab();
}
