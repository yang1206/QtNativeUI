#ifndef QTNATIVEUI_NANIMATION_H
#define QTNATIVEUI_NANIMATION_H

#include <QEasingCurve>
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QObject>
#include <QPointF>
#include <QPropertyAnimation>
#include <QWidget>

#include "NIconEnums.h"
#include "stdafx.h"

namespace QtNativeUI {

// 动画速度枚举
Q_BEGIN_ENUM_CREATE(NAnimationSpeed)
enum Speed { Fast, Medium, Slow };
Q_ENUM_CREATE(Speed)
Q_END_ENUM_CREATE(NAnimationSpeed)

// 动画类型枚举
Q_BEGIN_ENUM_CREATE(NAnimationType)
enum Type { FastInvoke, StrongInvoke, FastDismiss, SoftDismiss, PointToPoint, FadeInOut };
Q_ENUM_CREATE(Type)
Q_END_ENUM_CREATE(NAnimationType)

// 动画属性枚举
Q_BEGIN_ENUM_CREATE(NAnimationProperty)
enum Property { Position, Scale, Angle, Opacity };
Q_ENUM_CREATE(Property)
Q_END_ENUM_CREATE(NAnimationProperty)

// 动画基类
class QTNATIVEUI_EXPORT NAnimationBase : public QObject {
    Q_OBJECT
  public:
    explicit NAnimationBase(QWidget* parent);
    ~NAnimationBase();

  protected:
    virtual void onHover(QEnterEvent* e);
    virtual void onLeave(QEvent* e);
    virtual void onPress(QMouseEvent* e);
    virtual void onRelease(QMouseEvent* e);

    bool eventFilter(QObject* obj, QEvent* e) override;

  private:
    QWidget* m_parent;
};

// Y轴平移动画
class QTNATIVEUI_EXPORT NTranslateYAnimation : public NAnimationBase {
    Q_OBJECT
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
  public:
    explicit NTranslateYAnimation(QWidget* parent, qreal offset = 2.0);
    ~NTranslateYAnimation();

    qreal y() const;
    void  setY(qreal y);

  signals:
    void yChanged(qreal value);

  private:
    qreal               m_y;
    qreal               m_maxOffset;
    QPropertyAnimation* m_animation;
};

// 阴影动画
class QTNATIVEUI_EXPORT NShadowAnimation : public QPropertyAnimation {
    Q_OBJECT
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor)
  public:
    explicit NShadowAnimation(QWidget* parent);
    ~NShadowAnimation();

    QColor shadowColor() const;
    void   setShadowColor(const QColor& color);

    void setNormalColor(const QColor& color);
    void setHoverColor(const QColor& color);
    void setPressColor(const QColor& color);

    bool eventFilter(QObject* obj, QEvent* e) override;

  protected:
    void onHover(QEnterEvent* e);
    void onLeave(QEvent* e);
    void onPress(QMouseEvent* e);
    void onRelease(QMouseEvent* e);

  private slots:
    void onAniFinished();

  private:
    QWidget*                   m_parent;
    QColor                     m_normalColor;
    QColor                     m_hoverColor;
    QColor                     m_pressColor;
    QColor                     m_shadowColor;
    bool                       m_isHover;
    QGraphicsDropShadowEffect* m_shadowEffect;
};

// 动画属性对象
class QTNATIVEUI_EXPORT NAnimationPropertyObject : public QObject {
    Q_OBJECT
  public:
    explicit NAnimationPropertyObject(QObject* parent = nullptr);
    virtual ~NAnimationPropertyObject();

    virtual QVariant getValue() const                = 0;
    virtual void     setValue(const QVariant& value) = 0;
};

// 位置动画属性对象
class QTNATIVEUI_EXPORT NPositionObject : public NAnimationPropertyObject {
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE setPosition)
  public:
    explicit NPositionObject(QObject* parent = nullptr);

    QPointF position() const;
    void    setPosition(const QPointF& pos);

    QVariant getValue() const override;
    void     setValue(const QVariant& value) override;

  private:
    QPointF m_position;
};

// 缩放动画属性对象
class QTNATIVEUI_EXPORT NScaleObject : public NAnimationPropertyObject {
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
  public:
    explicit NScaleObject(QObject* parent = nullptr);

    qreal scale() const;
    void  setScale(qreal scale);

    QVariant getValue() const override;
    void     setValue(const QVariant& value) override;

  private:
    qreal m_scale;
};

// 角度动画属性对象
class QTNATIVEUI_EXPORT NAngleObject : public NAnimationPropertyObject {
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle)
  public:
    explicit NAngleObject(QObject* parent = nullptr);

    qreal angle() const;
    void  setAngle(qreal angle);

    QVariant getValue() const override;
    void     setValue(const QVariant& value) override;

  private:
    qreal m_angle;
};

// 透明度动画属性对象
class QTNATIVEUI_EXPORT NOpacityObject : public NAnimationPropertyObject {
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
  public:
    explicit NOpacityObject(QObject* parent = nullptr);

    qreal opacity() const;
    void  setOpacity(qreal opacity);

    QVariant getValue() const override;
    void     setValue(const QVariant& value) override;

  private:
    qreal m_opacity;
};

// 流畅动画基类
class QTNATIVEUI_EXPORT NFluentAnimation : public QPropertyAnimation {
    Q_OBJECT
  public:
    explicit NFluentAnimation(QObject* parent = nullptr);
    ~NFluentAnimation();

    static QEasingCurve createBezierCurve(qreal x1, qreal y1, qreal x2, qreal y2);

    void        setSpeed(NAnimationSpeed::Speed speed);
    virtual int speedToDuration(NAnimationSpeed::Speed speed) const;

    void startAnimation(const QVariant& endValue, const QVariant& startValue = QVariant());

    virtual QEasingCurve defaultCurve() const;
    QVariant             value() const;

    static NFluentAnimation* create(NAnimationType::Type         type,
                                    NAnimationProperty::Property property,
                                    NAnimationSpeed::Speed       speed  = NAnimationSpeed::Fast,
                                    QWidget*                     parent = nullptr);
};

// 快速调用动画
class QTNATIVEUI_EXPORT NFastInvokeAnimation : public NFluentAnimation {
    Q_OBJECT
  public:
    explicit NFastInvokeAnimation(QObject* parent = nullptr);
    ~NFastInvokeAnimation();

  protected:
    QEasingCurve defaultCurve() const override;
    int          speedToDuration(NAnimationSpeed::Speed speed) const override;
};

// 强调调用动画
class QTNATIVEUI_EXPORT NStrongInvokeAnimation : public NFluentAnimation {
    Q_OBJECT
  public:
    explicit NStrongInvokeAnimation(QObject* parent = nullptr);
    ~NStrongInvokeAnimation();

  protected:
    QEasingCurve defaultCurve() const override;
    int          speedToDuration(NAnimationSpeed::Speed speed) const override;
};

// 快速消失动画
class QTNATIVEUI_EXPORT NFastDismissAnimation : public NFastInvokeAnimation {
    Q_OBJECT
  public:
    explicit NFastDismissAnimation(QObject* parent = nullptr);
    ~NFastDismissAnimation();
};

// 软消失动画
class QTNATIVEUI_EXPORT NSoftDismissAnimation : public NFluentAnimation {
    Q_OBJECT
  public:
    explicit NSoftDismissAnimation(QObject* parent = nullptr);
    ~NSoftDismissAnimation();

  protected:
    QEasingCurve defaultCurve() const override;
    int          speedToDuration(NAnimationSpeed::Speed speed) const override;
};

// 点到点动画
class QTNATIVEUI_EXPORT NPointToPointAnimation : public NFastDismissAnimation {
    Q_OBJECT
  public:
    explicit NPointToPointAnimation(QObject* parent = nullptr);
    ~NPointToPointAnimation();

  protected:
    QEasingCurve defaultCurve() const override;
};

// 淡入淡出动画
class QTNATIVEUI_EXPORT NFadeInOutAnimation : public NFluentAnimation {
    Q_OBJECT
  public:
    explicit NFadeInOutAnimation(QObject* parent = nullptr);
    ~NFadeInOutAnimation();

  protected:
    int speedToDuration(NAnimationSpeed::Speed speed) const override;
};

} // namespace QtNativeUI

#endif // QTNATIVEUI_NANIMATION_H
