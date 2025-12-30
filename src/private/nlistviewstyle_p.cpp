//
// Created by Yang1206 on 2025/12/29.
//

#include "nlistviewstyle_p.h"
#include <QListView>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

NListViewStyle::NListViewStyle(QStyle* style) : QProxyStyle(style) {}

NListViewStyle::~NListViewStyle() {}

void NListViewStyle::drawPrimitive(PrimitiveElement    element,
                                   const QStyleOption* option,
                                   QPainter*           painter,
                                   const QWidget*      widget) const {
    switch (element) {
        case PE_PanelItemViewItem: {
            const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option);
            if (!vopt)
                break;

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);

            QRect itemRect = vopt->rect;
            itemRect.adjust(2, 2, -2, -2);

            QPainterPath path;
            path.addRoundedRect(itemRect, _itemBorderRadius, _itemBorderRadius);

            if (vopt->state & QStyle::State_Selected) {
                if (vopt->state & QStyle::State_MouseOver) {
                    painter->fillPath(path, _itemHoverColor);
                } else {
                    painter->fillPath(path, _itemSelectedColor);
                }
            } else if (vopt->state & QStyle::State_MouseOver) {
                painter->fillPath(path, _itemHoverColor);
            }

            painter->restore();
            return;
        }
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
            QRect frameRect = option->rect;
            frameRect.adjust(1, 1, -1, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(QPen(_borderColor, 1));
            painter->setBrush(_backgroundColor);
            painter->drawRoundedRect(frameRect, _borderRadius, _borderRadius);
            painter->restore();
            return;
        }
        case CE_ItemViewItem: {
            const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option);
            if (!vopt) {
                QProxyStyle::drawControl(element, option, painter, widget);
                return;
            }

            drawPrimitive(PE_PanelItemViewItem, option, painter, widget);

            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                    QPainter::TextAntialiasing);

            QRect itemRect = option->rect;

            const QListView*    listView = qobject_cast<const QListView*>(widget);
            QListView::ViewMode viewMode = listView ? listView->viewMode() : QListView::ListMode;

            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);

            iconRect.adjust(_leftPadding, 0, 0, 0);
            textRect.adjust(_leftPadding, 0, 0, 0);

            if (!vopt->icon.isNull()) {
                QIcon::Mode mode = QIcon::Normal;
                if (!(vopt->state.testFlag(QStyle::State_Enabled))) {
                    mode = QIcon::Disabled;
                } else if (vopt->state.testFlag(QStyle::State_Selected)) {
                    mode = QIcon::Selected;
                }
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
            if (!vopt->text.isEmpty()) {
                painter->setPen(_textColor);
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }

            if (vopt->state.testFlag(QStyle::State_Selected) && viewMode == QListView::ListMode) {
                int heightOffset = itemRect.height() / 4;
                painter->setPen(Qt::NoPen);
                painter->setBrush(_accentColor);
                painter->drawRoundedRect(
                    QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset),
                    2,
                    2);
            }

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
        if (viewMode == QListView::ListMode) {
            itemSize.setWidth(itemSize.width() + _leftPadding);
        }
        itemSize.setHeight(_itemHeight);
        return itemSize;
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void NListViewStyle::setBackgroundColor(const QColor& color) { _backgroundColor = color; }

void NListViewStyle::setBorderColor(const QColor& color) { _borderColor = color; }

void NListViewStyle::setBorderRadius(int radius) { _borderRadius = radius; }

void NListViewStyle::setItemHoverColor(const QColor& color) { _itemHoverColor = color; }

void NListViewStyle::setItemSelectedColor(const QColor& color) { _itemSelectedColor = color; }

void NListViewStyle::setItemPressedColor(const QColor& color) { _itemPressedColor = color; }

void NListViewStyle::setTextColor(const QColor& color) { _textColor = color; }

void NListViewStyle::setAccentColor(const QColor& color) { _accentColor = color; }

void NListViewStyle::setItemHeight(int height) { _itemHeight = height; }

void NListViewStyle::setItemBorderRadius(int radius) { _itemBorderRadius = radius; }

void NListViewStyle::setLeftPadding(int padding) { _leftPadding = padding; }
