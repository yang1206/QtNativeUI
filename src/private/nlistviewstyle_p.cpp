//
// Created by Yang1206 on 2025/12/29.
//

#include "nlistviewstyle_p.h"
#include <QListView>
#include <QPainter>
#include <QStyleOption>

NListViewStyle::NListViewStyle(QStyle* style) : QProxyStyle(style) {}

NListViewStyle::~NListViewStyle() {}

void NListViewStyle::drawPrimitive(PrimitiveElement    element,
                                   const QStyleOption* option,
                                   QPainter*           painter,
                                   const QWidget*      widget) const {
    switch (element) {
        case PE_PanelItemViewItem:
            return;
        case PE_FrameFocusRect:
            return;
        default:
            break;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NListViewStyle::drawControl(ControlElement      element,
                                 const QStyleOption* option,
                                 QPainter*           painter,
                                 const QWidget*      widget) const {
    switch (element) {
        case CE_ShapedFrame: {
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
        default:
            break;
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize NListViewStyle::sizeFromContents(ContentsType        type,
                                       const QStyleOption* option,
                                       const QSize&        size,
                                       const QWidget*      widget) const {
    if (type == CT_ItemViewItem) {
        const QListView*    listView = qobject_cast<const QListView*>(widget);
        QListView::ViewMode viewMode = listView ? listView->viewMode() : QListView::ListMode;

        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (viewMode == QListView::IconMode) {
            const QSize grid = listView ? listView->gridSize() : QSize();
            if (grid.width() > 0 && grid.height() > 0)
                return grid;
            return itemSize;
        }
        if (viewMode == QListView::ListMode)
            itemSize.setWidth(itemSize.width() + _leftPadding);
        itemSize.setHeight(_itemHeight);
        return itemSize;
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void NListViewStyle::setBackgroundColor(const QColor& color) { _backgroundColor = color; }

void NListViewStyle::setBorderColor(const QColor& color) { _borderColor = color; }

void NListViewStyle::setBorderRadius(int radius) { _borderRadius = radius; }

void NListViewStyle::setItemHeight(int height) { _itemHeight = height; }

void NListViewStyle::setBorderVisible(bool visible) { m_borderVisible = visible; }

void NListViewStyle::setBackgroundVisible(bool visible) { m_backgroundVisible = visible; }
