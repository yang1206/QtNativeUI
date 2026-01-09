//
// Created by Yang1206 on 2025/12/30.
//

#include "ntreeviewstyle_p.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QTreeView>
#include <QtNativeUI/NIcon.h>

NTreeViewStyle::NTreeViewStyle(QStyle* style) : QProxyStyle(style) {}

NTreeViewStyle::~NTreeViewStyle() {}

void NTreeViewStyle::drawPrimitive(PrimitiveElement    element,
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
            itemRect.adjust(0, 2, 0, -2);

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
        case PE_IndicatorBranch: {
            const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option);
            if (!vopt)
                break;

            if (!(vopt->state & QStyle::State_Children))
                return;

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);

            QRect indicatorRect = option->rect;
            indicatorRect.adjust(0, 0, -2, 0);

            QIcon icon;
            if (vopt->state & QStyle::State_Open) {
                icon = nIcon->fromRegular(NRegularIconType::ChevronDown16Regular, 14, _textColor);
            } else {
                icon = nIcon->fromRegular(NRegularIconType::ChevronRight16Regular, 14, _textColor);
            }

            int   iconSize = 14;
            QRect iconRect(indicatorRect.right() - iconSize, indicatorRect.center().y() - iconSize / 2, iconSize, iconSize);
            icon.paint(painter, iconRect);

            painter->restore();
            return;
        }
        case PE_PanelItemViewRow:
        case PE_Widget:
        case PE_Frame:
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
        case CE_HeaderLabel: {
            const QStyleOptionHeader* hopt = qstyleoption_cast<const QStyleOptionHeader*>(option);
            if (!hopt)
                break;

            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            if (!hopt->text.isEmpty()) {
                painter->setPen(_textColor);
                painter->drawText(hopt->rect, hopt->textAlignment, hopt->text);
            }
            painter->restore();
            return;
        }
        case CE_HeaderSection: {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(_itemSelectedColor);
            painter->drawRect(option->rect);
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

            QRect itemRect = vopt->rect;
            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);

            if (!vopt->icon.isNull()) {
                QIcon::Mode  mode  = QIcon::Normal;
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }

            if (!vopt->text.isEmpty()) {
                painter->setPen(_textColor);
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }

            int heightOffset = itemRect.height() / 4;
            painter->setPen(Qt::NoPen);
            painter->setBrush(_accentColor);
            if (vopt->state.testFlag(QStyle::State_Selected) &&
                (vopt->viewItemPosition == QStyleOptionViewItem::Beginning ||
                 vopt->viewItemPosition == QStyleOptionViewItem::OnlyOne)) {
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

int NTreeViewStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    if (metric == PM_HeaderMargin) {
        return _headerMargin;
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

QRect NTreeViewStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const {
    switch (element) {
        case SE_ItemViewItemDecoration: {
            const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option);
            if (vopt && vopt->viewItemPosition != QStyleOptionViewItem::Middle &&
                vopt->viewItemPosition != QStyleOptionViewItem::End) {
                QRect iconRect = QProxyStyle::subElementRect(element, option, widget);
                iconRect.adjust(_leftPadding, 0, _leftPadding, 0);
                return iconRect;
            }
            break;
        }
        case SE_ItemViewItemText: {
            const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option);
            if (vopt && vopt->viewItemPosition != QStyleOptionViewItem::Middle &&
                vopt->viewItemPosition != QStyleOptionViewItem::End) {
                QRect textRect = QProxyStyle::subElementRect(element, option, widget);
                textRect.adjust(_leftPadding, 0, 0, 0);
                return textRect;
            }
            break;
        }
        default:
            break;
    }
    return QProxyStyle::subElementRect(element, option, widget);
}

void NTreeViewStyle::setBackgroundColor(const QColor& color) { _backgroundColor = color; }

void NTreeViewStyle::setBorderColor(const QColor& color) { _borderColor = color; }

void NTreeViewStyle::setBorderRadius(int radius) { _borderRadius = radius; }

void NTreeViewStyle::setItemHoverColor(const QColor& color) { _itemHoverColor = color; }

void NTreeViewStyle::setItemSelectedColor(const QColor& color) { _itemSelectedColor = color; }

void NTreeViewStyle::setItemPressedColor(const QColor& color) { _itemPressedColor = color; }

void NTreeViewStyle::setTextColor(const QColor& color) { _textColor = color; }

void NTreeViewStyle::setAccentColor(const QColor& color) { _accentColor = color; }

void NTreeViewStyle::setItemHeight(int height) { _itemHeight = height; }

void NTreeViewStyle::setItemBorderRadius(int radius) { _itemBorderRadius = radius; }

void NTreeViewStyle::setIndentation(int indentation) { _indentation = indentation; }

void NTreeViewStyle::setLeftPadding(int padding) { _leftPadding = padding; }

void NTreeViewStyle::setHeaderMargin(int margin) { _headerMargin = margin; }
