//
// Created by Yang1206 on 2025/12/30.
//

#include "ntreeviewstyle_p.h"

#include <QPainter>
#include <QStyleOption>

NTreeViewStyle::NTreeViewStyle(QStyle* style) : QProxyStyle(style) {}

NTreeViewStyle::~NTreeViewStyle() = default;

void NTreeViewStyle::drawPrimitive(PrimitiveElement    element,
                                   const QStyleOption* option,
                                   QPainter*           painter,
                                   const QWidget*      widget) const {
    switch (element) {
        case PE_PanelItemViewItem:
        case PE_PanelItemViewRow:
        case PE_FrameFocusRect:
            return;
        default:
            break;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NTreeViewStyle::drawControl(ControlElement      element,
                                 const QStyleOption* option,
                                 QPainter*           painter,
                                 const QWidget*      widget) const {
    if (element == CE_ShapedFrame) {
        if (!m_borderVisible && !m_backgroundVisible)
            return;
        QRect frameRect = option->rect;
        frameRect.adjust(1, 1, -1, -1);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(m_borderVisible ? QPen(_borderColor, 1) : Qt::NoPen);
        painter->setBrush(m_backgroundVisible ? _backgroundColor : Qt::NoBrush);
        painter->drawRoundedRect(frameRect, _borderRadius, _borderRadius);
        painter->restore();
        return;
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize NTreeViewStyle::sizeFromContents(ContentsType        type,
                                       const QStyleOption* option,
                                       const QSize&        size,
                                       const QWidget*      widget) const {
    if (type == CT_ItemViewItem) {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(_itemHeight);
        return itemSize;
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void NTreeViewStyle::setBackgroundColor(const QColor& color) { _backgroundColor = color; }

void NTreeViewStyle::setBorderColor(const QColor& color) { _borderColor = color; }

void NTreeViewStyle::setBorderRadius(int radius) { _borderRadius = radius; }

void NTreeViewStyle::setItemHeight(int height) { _itemHeight = height; }

void NTreeViewStyle::setBorderVisible(bool visible) { m_borderVisible = visible; }

void NTreeViewStyle::setBackgroundVisible(bool visible) { m_backgroundVisible = visible; }
