#include "nanimation_p.h"
#include <QDebug>

// 实现单例模式的属性工厂
NAnimationPropertyFactory& NAnimationPropertyFactory::instance() {
    static NAnimationPropertyFactory instance;
    return instance;
}

NAnimationPropertyFactory::NAnimationPropertyFactory() {
    // 注册默认的属性创建器
    registerCreator(NAnimationProperty::Position, [](QObject* parent) -> NAnimationPropertyObject* {
        return new NPositionObject(parent);
    });
    
    registerCreator(NAnimationProperty::Scale, [](QObject* parent) -> NAnimationPropertyObject* {
        return new NScaleObject(parent);
    });
    
    registerCreator(NAnimationProperty::Angle, [](QObject* parent) -> NAnimationPropertyObject* {
        return new NAngleObject(parent);
    });
    
    registerCreator(NAnimationProperty::Opacity, [](QObject* parent) -> NAnimationPropertyObject* {
        return new NOpacityObject(parent);
    });
}

NAnimationPropertyFactory::~NAnimationPropertyFactory() {
}

void NAnimationPropertyFactory::registerCreator(NAnimationProperty::Property type, CreatorFunc creator) {
    m_creators[type] = creator;
}

NAnimationPropertyObject* NAnimationPropertyFactory::create(NAnimationProperty::Property type, QObject* parent) {
    if (!m_creators.contains(type)) {
        qWarning() << "No creator registered for property type:" << type;
        return nullptr;
    }
    
    return m_creators[type](parent);
}

// 实现单例模式的动画工厂
NAnimationFactory& NAnimationFactory::instance() {
    static NAnimationFactory instance;
    return instance;
}

NAnimationFactory::NAnimationFactory() {
    // 注册默认的动画创建器
    registerCreator(NAnimationType::FastInvoke, [](QObject* parent) -> NFluentAnimation* {
        return new NFastInvokeAnimation(parent);
    });
    
    registerCreator(NAnimationType::StrongInvoke, [](QObject* parent) -> NFluentAnimation* {
        return new NStrongInvokeAnimation(parent);
    });
    
    registerCreator(NAnimationType::FastDismiss, [](QObject* parent) -> NFluentAnimation* {
        return new NFastDismissAnimation(parent);
    });
    
    registerCreator(NAnimationType::SoftDismiss, [](QObject* parent) -> NFluentAnimation* {
        return new NSoftDismissAnimation(parent);
    });
    
    registerCreator(NAnimationType::PointToPoint, [](QObject* parent) -> NFluentAnimation* {
        return new NPointToPointAnimation(parent);
    });
    
    registerCreator(NAnimationType::FadeInOut, [](QObject* parent) -> NFluentAnimation* {
        return new NFadeInOutAnimation(parent);
    });
}

NAnimationFactory::~NAnimationFactory() {
}

void NAnimationFactory::registerCreator(NAnimationType::Type type, CreatorFunc creator) {
    m_creators[type] = creator;
}

NFluentAnimation* NAnimationFactory::create(NAnimationType::Type type, QObject* parent) {
    if (!m_creators.contains(type)) {
        qWarning() << "No creator registered for animation type:" << type;
        return nullptr;
    }
    
    return m_creators[type](parent);
}
