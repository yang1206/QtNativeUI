#ifndef NNAVIGATIONANIMATION_P_H
#define NNAVIGATIONANIMATION_P_H

#include <QGraphicsBlurEffect>
#include <QObject>
#include <QPixmap>
#include <QWidget>
#include "QtNativeUI/NEnums.h"

class QStackedWidget;

    class NNavigationAnimationManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int popupAnimationYOffset READ popupAnimationYOffset WRITE setPopupAnimationYOffset)
    Q_PROPERTY(qreal scaleAnimationRatio READ scaleAnimationRatio WRITE setScaleAnimationRatio)
    Q_PROPERTY(qreal scaleAnimationPixOpacity READ scaleAnimationPixOpacity WRITE setScaleAnimationPixOpacity)
    Q_PROPERTY(qreal flipAnimationRatio READ flipAnimationRatio WRITE setFlipAnimationRatio)
    Q_PROPERTY(int blurAnimationRadius READ blurAnimationRadius WRITE setBlurAnimationRadius)
    Q_PROPERTY(qreal cubeAnimationAngle READ cubeAnimationAngle WRITE setCubeAnimationAngle)
    Q_PROPERTY(qreal rippleAnimationRadius READ rippleAnimationRadius WRITE setRippleAnimationRadius)

  public:
    explicit NNavigationAnimationManager(QStackedWidget* stackedWidget, QObject* parent = nullptr);
    ~NNavigationAnimationManager();

    void executeTransition(NNavigationType::PageTransitionType type, int targetIndex, bool isRouteBack, int duration = 300);
    void paintTransition(QPainter* painter, const QRect& rect, int borderRadius);

    int popupAnimationYOffset() const { return m_popupAnimationYOffset; }
    void setPopupAnimationYOffset(int offset) { m_popupAnimationYOffset = offset; }

    qreal scaleAnimationRatio() const { return m_scaleAnimationRatio; }
    void setScaleAnimationRatio(qreal ratio) { m_scaleAnimationRatio = ratio; }

    qreal scaleAnimationPixOpacity() const { return m_scaleAnimationPixOpacity; }
    void setScaleAnimationPixOpacity(qreal opacity) { m_scaleAnimationPixOpacity = opacity; }

    qreal flipAnimationRatio() const { return m_flipAnimationRatio; }
    void setFlipAnimationRatio(qreal ratio) { m_flipAnimationRatio = ratio; }

    int blurAnimationRadius() const { return m_blurAnimationRadius; }
    void setBlurAnimationRadius(int radius) { m_blurAnimationRadius = radius; }

    qreal cubeAnimationAngle() const { return m_cubeAnimationAngle; }
    void setCubeAnimationAngle(qreal angle) { m_cubeAnimationAngle = angle; }

    qreal rippleAnimationRadius() const { return m_rippleAnimationRadius; }
    void setRippleAnimationRadius(qreal radius) { m_rippleAnimationRadius = radius; }


  private:
    void getCurrentStackPix();
    void getTargetStackPix();

    QStackedWidget*                     m_stackedWidget{nullptr};
    NNavigationType::PageTransitionType m_transitionType{NNavigationType::NoTransition};
    QPixmap                             m_targetStackPix;
    QPixmap                             m_currentStackPix;
    QGraphicsBlurEffect*                m_blurEffect{nullptr};
    bool                                m_isDrawNewPix{false};

    int   m_popupAnimationYOffset{0};
    qreal m_scaleAnimationRatio{1.0};
    qreal m_scaleAnimationPixOpacity{1.0};
    qreal m_flipAnimationRatio{0.0};
    int   m_blurAnimationRadius{0};
    qreal m_cubeAnimationAngle{0.0};
    qreal m_rippleAnimationRadius{0.0};
};

#endif // NNAVIGATIONANIMATION_P_H
