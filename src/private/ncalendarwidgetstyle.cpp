#include "ncalendarwidgetstyle.h"
#include <QCalendarWidget>
#include <QPainter>
#include <QStyleOptionViewItem>
#include "QtNativeUI/NTheme.h"

NCalendarWidgetStyle::NCalendarWidgetStyle(const NCalendarWidgetStyleInterface* styleInterface, QStyle* style)
    : QProxyStyle(style), m_styleInterface(styleInterface) {}

void NCalendarWidgetStyle::drawPrimitive(PrimitiveElement    element,
                                         const QStyleOption* option,
                                         QPainter*           painter,
                                         const QWidget*      widget) const {
    // 在这里我们先使用默认绘制，未来再添加自定义绘制逻辑
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NCalendarWidgetStyle::drawControl(ControlElement      element,
                                       const QStyleOption* option,
                                       QPainter*           painter,
                                       const QWidget*      widget) const {
    // 在这里我们先使用默认绘制，未来再添加自定义绘制逻辑
    QProxyStyle::drawControl(element, option, painter, widget);
}

void NCalendarWidgetStyle::drawComplexControl(ComplexControl             control,
                                              const QStyleOptionComplex* option,
                                              QPainter*                  painter,
                                              const QWidget*             widget) const {
    // 在这里我们先使用默认绘制，未来再添加自定义绘制逻辑
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QSize NCalendarWidgetStyle::sizeFromContents(ContentsType        type,
                                             const QStyleOption* option,
                                             const QSize&        size,
                                             const QWidget*      widget) const {
    // 在这里我们先使用默认尺寸计算，未来再添加自定义逻辑
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}