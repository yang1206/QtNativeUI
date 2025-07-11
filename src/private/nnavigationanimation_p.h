#ifndef NNAVIGATIONANIMATION_P_H
#define NNAVIGATIONANIMATION_P_H

#include <QGraphicsEffect>
#include <QWidget>
#include "QtNativeUI/NEnums.h"

class NNavigationAnimationManager : public QObject {
    Q_OBJECT
  public:
    explicit NNavigationAnimationManager(QObject* parent = nullptr);
    ~NNavigationAnimationManager();

    // 执行页面过渡动画
    void executeTransition(QWidget* targetWidget, NNavigationType::PageTransitionType type, int duration = 250);

  private:
    // 淡入淡出动画
    void executeFadeTransition(QWidget* targetWidget, int duration);

    // 水平滑动动画
    void executeSlideHorizontalTransition(QWidget* targetWidget, int duration);

    // 垂直滑动动画
    void executeSlideVerticalTransition(QWidget* targetWidget, int duration);

    // 缩放过渡动画
    void executeZoomTransition(QWidget* targetWidget, int duration);

    // 翻转过渡动画
    void executeFlipTransition(QWidget* targetWidget, int duration);

    // 推入过渡动画
    void executePushTransition(QWidget* targetWidget, int duration);

    // 揭示过渡动画
    void executeRevealTransition(QWidget* targetWidget, int duration);

    // 弹性过渡动画
    void executeElasticTransition(QWidget* targetWidget, int duration);

    // 清理动画效果
    void cleanupEffects(QWidget* widget, QGraphicsEffect* effect);
};

// 自定义旋转效果类
class NRotationEffect : public QGraphicsEffect {
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle)
    Q_PROPERTY(Qt::Axis axis READ axis WRITE setAxis)

  public:
    explicit NRotationEffect(QObject* parent = nullptr);
    ~NRotationEffect();

    qreal angle() const { return m_angle; }
    void  setAngle(qreal angle);

    Qt::Axis axis() const { return m_axis; }
    void     setAxis(Qt::Axis axis);

  protected:
    void   draw(QPainter* painter) override;
    QRectF boundingRectFor(const QRectF& rect) const override;

  private:
    qreal    m_angle;
    Qt::Axis m_axis;
};

#endif // NNAVIGATIONANIMATION_P_H