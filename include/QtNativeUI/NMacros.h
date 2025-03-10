#ifndef QTNATIVEUI_NMACROS_H
#define QTNATIVEUI_NMACROS_H

#include <QtCore/qglobal.h>

// 避免重复定义 QTNATIVEUI_EXPORT
#ifndef QTNATIVEUI_EXPORT
#ifdef QTNATIVEUI_LIBRARY
#define QTNATIVEUI_EXPORT Q_DECL_EXPORT
#else
#define QTNATIVEUI_EXPORT Q_DECL_IMPORT
#endif
#endif

// 简化属性创建
#define N_PROPERTY(TYPE, NAME) \
    Q_PROPERTY(TYPE NAME READ get##NAME WRITE set##NAME NOTIFY NAME##Changed) \
public: \
    Q_SIGNAL void NAME##Changed(); \
    void set##NAME(TYPE value) { \
        if (_##NAME != value) { \
            _##NAME = value; \
            Q_EMIT NAME##Changed(); \
        } \
    } \
    TYPE get##NAME() const { return _##NAME; } \
private: \
    TYPE _##NAME;

// 简化d指针使用的属性创建
#define N_PROPERTY_D(TYPE, NAME) \
    Q_PROPERTY(TYPE NAME READ get##NAME WRITE set##NAME NOTIFY NAME##Changed) \
public: \
    Q_SIGNAL void NAME##Changed(); \
    void set##NAME(TYPE value); \
    TYPE get##NAME() const;

// d指针属性实现
#define N_PROPERTY_D_IMPL(CLASS, TYPE, NAME) \
    void CLASS::set##NAME(TYPE value) { \
        Q_D(CLASS); \
        if (d->_##NAME != value) { \
            d->_##NAME = value; \
            Q_EMIT NAME##Changed(); \
        } \
    } \
    TYPE CLASS::get##NAME() const { \
        Q_D(const CLASS); \
        return d->_##NAME; \
    }

// 简化d指针类的创建
#define N_DECLARE_PRIVATE(CLASS) \
protected: \
    explicit CLASS(CLASS##Private& dd, QObject* parent = nullptr); \
    QScopedPointer<CLASS##Private> d_ptr; \
private: \
    Q_DISABLE_COPY(CLASS) \
    Q_DECLARE_PRIVATE(CLASS)

// d指针实现类的声明
#define N_DECLARE_PUBLIC(CLASS) \
protected: \
    CLASS* q_ptr; \
private: \
    Q_DECLARE_PUBLIC(CLASS)

// 颜色快速访问宏 (更新为使用新的NTheme类)
#define N_COLOR(ROLE) QtNativeUI::NTheme::instance()->color(QtNativeUI::NColorRole::ROLE)

#endif // QTNATIVEUI_NMACROS_H 