#include "QtNativeUI/NAnimation.h"
#include <QEnterEvent>
#include <QGraphicsEffect>
#include <QMouseEvent>
#include "../private/nanimation_p.h"

namespace QtNativeUI {

// ==================== NAnimationBase 实现 ====================
NAnimationBase::NAnimationBase(QWidget* parent) : QObject(parent), m_parent(parent) {
    parent->installEventFilter(this);
}

NAnimationBase::~NAnimationBase() {}

void NAnimationBase::onHover(QEnterEvent* e) { Q_UNUSED(e); }

void NAnimationBase::onLeave(QEvent* e) { Q_UNUSED(e); }

void NAnimationBase::onPress(QMouseEvent* e) { Q_UNUSED(e); }

void NAnimationBase::onRelease(QMouseEvent* e) { Q_UNUSED(e); }

bool NAnimationBase::eventFilter(QObject* obj, QEvent* e) {
    if (obj == m_parent) {
        if (e->type() == QEvent::MouseButtonPress) {
            onPress(static_cast<QMouseEvent*>(e));
        } else if (e->type() == QEvent::MouseButtonRelease) {
            onRelease(static_cast<QMouseEvent*>(e));
        } else if (e->type() == QEvent::Enter) {
            onHover(static_cast<QEnterEvent*>(e));
        } else if (e->type() == QEvent::Leave) {
            onLeave(e);
        }
    }
    return QObject::eventFilter(obj, e);
}

// ==================== NTranslateYAnimation 实现 ====================
NTranslateYAnimation::NTranslateYAnimation(QWidget* parent, qreal offset)
    : NAnimationBase(parent), m_y(0), m_maxOffset(offset) {
    m_animation = new QPropertyAnimation(this, "y", this);
    m_animation->setEasingCurve(QEasingCurve::OutQuad);
    m_animation->setDuration(150);
}

NTranslateYAnimation::~NTranslateYAnimation() { delete m_animation; }

qreal NTranslateYAnimation::y() const { return m_y; }

void NTranslateYAnimation::setY(qreal y) {
    if (m_y != y) {
        m_y = y;
        emit yChanged(m_y);
    }
}

// ==================== NShadowAnimation 实现 ====================
NShadowAnimation::NShadowAnimation(QWidget* parent)
    : QPropertyAnimation(this, "shadowColor"), m_parent(parent), m_isHover(false), m_shadowEffect(nullptr) {
    m_normalColor = QColor(0, 0, 0, 60);
    m_hoverColor  = QColor(0, 0, 0, 90);
    m_pressColor  = QColor(0, 0, 0, 30);
    m_shadowColor = m_normalColor;

    setDuration(150);
    setEasingCurve(QEasingCurve::OutQuad);

    parent->installEventFilter(this);
}

NShadowAnimation::~NShadowAnimation() {}

QColor NShadowAnimation::shadowColor() const { return m_shadowColor; }

void NShadowAnimation::setShadowColor(const QColor& color) {
    if (m_shadowColor != color) {
        m_shadowColor = color;
        if (m_shadowEffect) {
            m_shadowEffect->setColor(color);
        } else {
            m_shadowEffect = new QGraphicsDropShadowEffect(m_parent);
            m_shadowEffect->setOffset(0, 2);
            m_shadowEffect->setBlurRadius(6);
            m_shadowEffect->setColor(color);
            m_parent->setGraphicsEffect(m_shadowEffect);
        }
    }
}

void NShadowAnimation::setNormalColor(const QColor& color) { m_normalColor = color; }

void NShadowAnimation::setHoverColor(const QColor& color) { m_hoverColor = color; }

void NShadowAnimation::setPressColor(const QColor& color) { m_pressColor = color; }

void NShadowAnimation::onHover(QEnterEvent* e) {
    Q_UNUSED(e);
    m_isHover = true;
    if (m_parent->isEnabled()) {
        this->stop();
        this->setEndValue(m_hoverColor);
        this->start();
    }
}

void NShadowAnimation::onLeave(QEvent* e) {
    Q_UNUSED(e);
    m_isHover = false;
    if (m_parent->isEnabled()) {
        this->stop();
        this->setEndValue(m_normalColor);
        this->start();
    }
}

void NShadowAnimation::onPress(QMouseEvent* e) {
    Q_UNUSED(e);
    if (m_parent->isEnabled()) {
        this->stop();
        this->setEndValue(m_pressColor);
        this->start();
    }
}

void NShadowAnimation::onRelease(QMouseEvent* e) {
    Q_UNUSED(e);
    if (m_parent->isEnabled()) {
        this->stop();
        this->setEndValue(m_isHover ? m_hoverColor : m_normalColor);
        this->start();
    }
}

bool NShadowAnimation::eventFilter(QObject* obj, QEvent* e) {
    if (obj == m_parent) {
        if (e->type() == QEvent::MouseButtonPress) {
            onPress(static_cast<QMouseEvent*>(e));
        } else if (e->type() == QEvent::MouseButtonRelease) {
            onRelease(static_cast<QMouseEvent*>(e));
        } else if (e->type() == QEvent::Enter) {
            onHover(static_cast<QEnterEvent*>(e));
        } else if (e->type() == QEvent::Leave) {
            onLeave(e);
            if (m_parent->graphicsEffect()) {
                connect(this, &QPropertyAnimation::finished, this, &NShadowAnimation::onAniFinished);
                this->setEndValue(m_normalColor);
                this->start();
            }
        }
    }
    return QObject::eventFilter(obj, e);
}

void NShadowAnimation::onAniFinished() {
    disconnect(this, &QPropertyAnimation::finished, this, &NShadowAnimation::onAniFinished);
    m_shadowEffect = nullptr;
    m_parent->setGraphicsEffect(nullptr);
}

// ==================== NAnimationPropertyObject 实现 ====================
NAnimationPropertyObject::NAnimationPropertyObject(QObject* parent) : QObject(parent) {}

NAnimationPropertyObject::~NAnimationPropertyObject() {}

// ==================== NPositionObject 实现 ====================
NPositionObject::NPositionObject(QObject* parent) : NAnimationPropertyObject(parent) {}

QPointF NPositionObject::position() const { return m_position; }

void NPositionObject::setPosition(const QPointF& pos) {
    if (m_position != pos) {
        m_position = pos;
        if (QWidget* widget = qobject_cast<QWidget*>(parent())) {
            widget->update();
        }
    }
}

QVariant NPositionObject::getValue() const { return QVariant::fromValue(m_position); }

void NPositionObject::setValue(const QVariant& value) { setPosition(value.toPointF()); }

// ==================== NScaleObject 实现 ====================
NScaleObject::NScaleObject(QObject* parent) : NAnimationPropertyObject(parent), m_scale(1.0) {}

qreal NScaleObject::scale() const { return m_scale; }

void NScaleObject::setScale(qreal scale) {
    if (m_scale != scale) {
        m_scale = scale;
        if (QWidget* widget = qobject_cast<QWidget*>(parent())) {
            widget->update();
        }
    }
}

QVariant NScaleObject::getValue() const { return QVariant::fromValue(m_scale); }

void NScaleObject::setValue(const QVariant& value) { setScale(value.toReal()); }

// ==================== NAngleObject 实现 ====================
NAngleObject::NAngleObject(QObject* parent) : NAnimationPropertyObject(parent), m_angle(0.0) {}

qreal NAngleObject::angle() const { return m_angle; }

void NAngleObject::setAngle(qreal angle) {
    if (m_angle != angle) {
        m_angle = angle;
        if (QWidget* widget = qobject_cast<QWidget*>(parent())) {
            widget->update();
        }
    }
}

QVariant NAngleObject::getValue() const { return QVariant::fromValue(m_angle); }

void NAngleObject::setValue(const QVariant& value) { setAngle(value.toReal()); }

// ==================== NOpacityObject 实现 ====================
NOpacityObject::NOpacityObject(QObject* parent) : NAnimationPropertyObject(parent), m_opacity(0.0) {}

qreal NOpacityObject::opacity() const { return m_opacity; }

void NOpacityObject::setOpacity(qreal opacity) {
    if (m_opacity != opacity) {
        m_opacity = opacity;
        if (QWidget* widget = qobject_cast<QWidget*>(parent())) {
            widget->update();
        }
    }
}

QVariant NOpacityObject::getValue() const { return QVariant::fromValue(m_opacity); }

void NOpacityObject::setValue(const QVariant& value) { setOpacity(value.toReal()); }

// ==================== NFluentAnimation 实现 ====================
NFluentAnimation::NFluentAnimation(QObject* parent) : QPropertyAnimation(parent) {
    setSpeed(NAnimationSpeed::Fast);
    setEasingCurve(defaultCurve());
}

NFluentAnimation::~NFluentAnimation() {}

QEasingCurve NFluentAnimation::createBezierCurve(qreal x1, qreal y1, qreal x2, qreal y2) {
    QEasingCurve curve(QEasingCurve::BezierSpline);
    curve.addCubicBezierSegment(QPointF(x1, y1), QPointF(x2, y2), QPointF(1, 1));
    return curve;
}

QEasingCurve NFluentAnimation::defaultCurve() const { return createBezierCurve(0, 0, 1, 1); }

void NFluentAnimation::setSpeed(NAnimationSpeed::Speed speed) { setDuration(speedToDuration(speed)); }

int NFluentAnimation::speedToDuration(NAnimationSpeed::Speed speed) const {
    Q_UNUSED(speed);
    return 100;
}

void NFluentAnimation::startAnimation(const QVariant& endValue, const QVariant& startValue) {
    stop();

    if (!startValue.isNull()) {
        setStartValue(startValue);
    } else {
        // 获取当前值作为起始值
        QObject* targetObj = targetObject();
        if (NAnimationPropertyObject* propObj = qobject_cast<NAnimationPropertyObject*>(targetObj)) {
            setStartValue(propObj->getValue());
        }
    }

    setEndValue(endValue);
    start();
}

QVariant NFluentAnimation::value() const {
    QObject* targetObj = targetObject();
    if (NAnimationPropertyObject* propObj = qobject_cast<NAnimationPropertyObject*>(targetObj)) {
        return propObj->getValue();
    }
    return QVariant();
}

NFluentAnimation* NFluentAnimation::create(NAnimationType::Type         type,
                                           NAnimationProperty::Property property,
                                           NAnimationSpeed::Speed       speed,
                                           QWidget*                     parent) {
    // 创建属性对象
    NAnimationPropertyObject* propertyObj = NAnimationPropertyFactory::instance().create(property, parent);
    if (!propertyObj) {
        return nullptr;
    }

    // 创建动画
    NFluentAnimation* animation = NAnimationFactory::instance().create(type, parent);
    if (!animation) {
        delete propertyObj;
        return nullptr;
    }

    // 设置动画属性
    animation->setTargetObject(propertyObj);
    animation->setPropertyName(QByteArray());
    switch (property) {
        case NAnimationProperty::Position:
            animation->setPropertyName("position");
            break;
        case NAnimationProperty::Scale:
            animation->setPropertyName("scale");
            break;
        case NAnimationProperty::Angle:
            animation->setPropertyName("angle");
            break;
        case NAnimationProperty::Opacity:
            animation->setPropertyName("opacity");
            break;
    }

    // 设置动画速度
    animation->setSpeed(speed);

    return animation;
}

// ==================== NFastInvokeAnimation 实现 ====================
NFastInvokeAnimation::NFastInvokeAnimation(QObject* parent) : NFluentAnimation(parent) {}

NFastInvokeAnimation::~NFastInvokeAnimation() {}

QEasingCurve NFastInvokeAnimation::defaultCurve() const { return createBezierCurve(0, 0, 0, 1); }

int NFastInvokeAnimation::speedToDuration(NAnimationSpeed::Speed speed) const {
    switch (speed) {
        case NAnimationSpeed::Fast:
            return 187;
        case NAnimationSpeed::Medium:
            return 333;
        case NAnimationSpeed::Slow:
            return 500;
        default:
            return 187;
    }
}

// ==================== NStrongInvokeAnimation 实现 ====================
NStrongInvokeAnimation::NStrongInvokeAnimation(QObject* parent) : NFluentAnimation(parent) {}

NStrongInvokeAnimation::~NStrongInvokeAnimation() {}

QEasingCurve NStrongInvokeAnimation::defaultCurve() const { return createBezierCurve(0.13, 1.62, 0, 0.92); }

int NStrongInvokeAnimation::speedToDuration(NAnimationSpeed::Speed speed) const {
    Q_UNUSED(speed);
    return 667;
}

// ==================== NFastDismissAnimation 实现 ====================
NFastDismissAnimation::NFastDismissAnimation(QObject* parent) : NFastInvokeAnimation(parent) {}

NFastDismissAnimation::~NFastDismissAnimation() {}

// ==================== NSoftDismissAnimation 实现 ====================
NSoftDismissAnimation::NSoftDismissAnimation(QObject* parent) : NFluentAnimation(parent) {}

NSoftDismissAnimation::~NSoftDismissAnimation() {}

QEasingCurve NSoftDismissAnimation::defaultCurve() const { return createBezierCurve(1, 0, 1, 1); }

int NSoftDismissAnimation::speedToDuration(NAnimationSpeed::Speed speed) const {
    Q_UNUSED(speed);
    return 167;
}

// ==================== NPointToPointAnimation 实现 ====================
NPointToPointAnimation::NPointToPointAnimation(QObject* parent) : NFastDismissAnimation(parent) {}

NPointToPointAnimation::~NPointToPointAnimation() {}

QEasingCurve NPointToPointAnimation::defaultCurve() const { return createBezierCurve(0.55, 0.55, 0, 1); }

// ==================== NFadeInOutAnimation 实现 ====================
NFadeInOutAnimation::NFadeInOutAnimation(QObject* parent) : NFluentAnimation(parent) {}

NFadeInOutAnimation::~NFadeInOutAnimation() {}

int NFadeInOutAnimation::speedToDuration(NAnimationSpeed::Speed speed) const {
    Q_UNUSED(speed);
    return 83;
}

} // namespace QtNativeUI