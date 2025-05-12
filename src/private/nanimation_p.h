#ifndef NANIMATION_P_H
#define NANIMATION_P_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <functional>
#include "QtNativeUI/NAnimation.h"

using namespace QtNativeUI;

// 动画属性工厂类
class NAnimationPropertyFactory {
public:
    using CreatorFunc = std::function<NAnimationPropertyObject*(QObject*)>;
    
    static NAnimationPropertyFactory& instance();
    
    void registerCreator(NAnimationProperty::Property type, CreatorFunc creator);
    NAnimationPropertyObject* create(NAnimationProperty::Property type, QObject* parent);
    
private:
    NAnimationPropertyFactory();
    ~NAnimationPropertyFactory();
    
    QHash<NAnimationProperty::Property, CreatorFunc> m_creators;
};

// 动画工厂类
class NAnimationFactory {
public:
    using CreatorFunc = std::function<NFluentAnimation*(QObject*)>;
    
    static NAnimationFactory& instance();
    
    void registerCreator(NAnimationType::Type type, CreatorFunc creator);
    NFluentAnimation* create(NAnimationType::Type type, QObject* parent);
    
private:
    NAnimationFactory();
    ~NAnimationFactory();
    
    QHash<NAnimationType::Type, CreatorFunc> m_creators;
};

#endif // NANIMATION_P_H
