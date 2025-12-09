#include "nmenu_p.h"

#include <QPainterPath>
#include <qstyleoption.h>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NMenuPrivate::Style::Style(NMenuPrivate* parent, QStyle* style) : QProxyStyle(style), d(parent) {}

void NMenuPrivate::Style::drawControl(ControlElement      element,
                                      const QStyleOption* option,
                                      QPainter*           painter,
                                      const QWidget*      widget) const {
    bool isNMenuOrChild = false;
    if (widget) {
        const NMenu* menuWidget = qobject_cast<const NMenu*>(widget);
        if (menuWidget) {
            isNMenuOrChild = true;
        } else {
            QWidget* parent = widget->parentWidget();
            while (parent && !isNMenuOrChild) {
                if (qobject_cast<const NMenu*>(parent)) {
                    isNMenuOrChild = true;
                }
                parent = parent->parentWidget();
            }
        }
    }

    if (!isNMenuOrChild) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }

    if (element == CE_MenuItem && qobject_cast<const NMenu*>(widget)) {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option)) {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                                    QPainter::SmoothPixmapTransform);

            QRect menuRect = mopt->rect;
            menuRect.adjust(0, -2, 0, -2);

            int hPadding = d->_itemPadding;
            int iconSize = d->_itemIconSize;

            int menuBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

            if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
                QColor sepColor = d->_isDark ? d->_pDarkSeparatorColor : d->_pLightSeparatorColor;

                painter->setPen(Qt::NoPen);
                painter->setBrush(sepColor);
                painter->drawRoundedRect(menuRect.x(), menuRect.center().y(), menuRect.width(), 1, 0, 0);

                painter->restore();
                return;
            }

            if (mopt->state.testFlag(State_Enabled) &&
                (mopt->state.testFlag(State_Selected) || mopt->state.testFlag(State_MouseOver) || mopt->checked)) {
                QColor hoverColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, d->_themeMode);

                painter->setPen(Qt::NoPen);
                painter->setBrush(hoverColor);
                painter->drawRoundedRect(menuRect, menuBorderRadius, menuBorderRadius);
            }

            QColor textColor =
                NThemeColor(mopt->state.testFlag(QStyle::State_Enabled) ? NFluentColorKey::TextFillColorPrimary
                                                                        : NFluentColorKey::TextFillColorDisabled,
                            d->_themeMode);
            painter->setPen(textColor);

            if (!mopt->icon.isNull()) {
                int   iconY = menuRect.y() + (menuRect.height() - iconSize) / 2;
                QRect iconRect(menuRect.x() + hPadding, iconY, iconSize, iconSize);

                QIcon::Mode iconMode = mopt->state.testFlag(QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled;
                mopt->icon.paint(painter, iconRect, Qt::AlignCenter, iconMode);
            }

            else if (mopt->checked) {
                int   checkSize = iconSize;
                QRect checkRect(
                    menuRect.x() + hPadding, menuRect.y() + (menuRect.height() - checkSize) / 2, checkSize, checkSize);

                QIcon checkIcon = nIcon->fromRegular(NRegularIconType::Checkmark20Regular, checkSize, textColor);
                checkIcon.paint(painter, checkRect, Qt::AlignCenter);
            }

            if (!mopt->text.isEmpty()) {
                QString itemText = mopt->text;
                QString shortcutText;

                int tabIndex = itemText.indexOf('\t');
                if (tabIndex != -1) {
                    shortcutText = itemText.mid(tabIndex + 1);
                    itemText     = itemText.left(tabIndex);
                }

                int textX = menuRect.x() + hPadding;

                if (!mopt->icon.isNull() || mopt->checked) {
                    textX += d->_checkmarkWidth;
                }

                int shortcutWidth = 0;
                if (!shortcutText.isEmpty()) {
                    shortcutWidth = painter->fontMetrics().horizontalAdvance(shortcutText) + 2 * d->_itemPadding;
                }

                QRect textRect(
                    textX, menuRect.y(), menuRect.width() - textX - hPadding - shortcutWidth, menuRect.height());

                painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextSingleLine, itemText);

                if (!shortcutText.isEmpty()) {
                    QRect shortcutRect(
                        menuRect.right() - shortcutWidth - hPadding, menuRect.y(), shortcutWidth, menuRect.height());

                    QColor shortcutColor = NThemeColor(mopt->state.testFlag(QStyle::State_Enabled)
                                                           ? NFluentColorKey::TextFillColorSecondary
                                                           : NFluentColorKey::TextFillColorDisabled,
                                                       d->_themeMode);
                    painter->setPen(shortcutColor);

                    painter->drawText(
                        shortcutRect, Qt::AlignVCenter | Qt::AlignRight | Qt::TextSingleLine, shortcutText);
                }
            }

            if (mopt->menuItemType == QStyleOptionMenuItem::SubMenu) {
                QRect arrowRect(menuRect.right() - hPadding - iconSize,
                                menuRect.y() + (menuRect.height() - iconSize) / 2,
                                iconSize,
                                iconSize);

                QIcon arrowIcon = nIcon->fromFilled(NFilledIconType::ChevronRight16Filled, iconSize, textColor);
                arrowIcon.paint(painter, arrowRect, Qt::AlignCenter);
            }

            painter->restore();
            return;
        }
    } else if (element == CE_MenuBarItem) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuScroller) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuTearoff) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuEmptyArea) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
}

void NMenuPrivate::Style::drawPrimitive(PrimitiveElement    element,
                                        const QStyleOption* option,
                                        QPainter*           painter,
                                        const QWidget*      widget) const {
    if (widget) {
        const NMenu* menuWidget = qobject_cast<const NMenu*>(widget);
        if (!menuWidget) {
            QWidget* parent = widget->parentWidget();
            while (parent && !menuWidget) {
                menuWidget = qobject_cast<const NMenu*>(parent);
                parent     = parent->parentWidget();
            }
            if (!menuWidget) {
                return QProxyStyle::drawPrimitive(element, option, painter, widget);
            }
        }
    }
    // if (element == PE_PanelMenu) {
    //     painter->save();
    //     painter->setRenderHint(QPainter::Antialiasing);
    //
    //     QColor       bgColor     = d->_isDark ? d->_pDarkBackgroundColor : d->_pLightBackgroundColor;
    //     QColor       borderColor = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
    //     QPainterPath path;
    //     path.addRoundedRect(option->rect.adjusted(0, 0, -1, -1), d->_pBorderRadius, d->_pBorderRadius);
    //
    //     painter->setPen(QPen(borderColor, 1));
    //     painter->setBrush(bgColor);
    //     painter->drawPath(path);
    //
    //     painter->restore();
    //     return;
    // }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int NMenuPrivate::Style::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    const NMenu* menu           = qobject_cast<const NMenu*>(widget);
    bool         isNMenuOrChild = false;

    if (menu) {
        isNMenuOrChild = true;
    } else if (widget) {
        QWidget* parent = widget->parentWidget();
        while (parent && !isNMenuOrChild) {
            if (qobject_cast<const NMenu*>(parent)) {
                isNMenuOrChild = true;
            }
            parent = parent->parentWidget();
        }
    }

    if (isNMenuOrChild) {
        const NMenuPrivate* d = menu ? menu->d_func() : this->d;

        switch (metric) {
            case PM_MenuPanelWidth:
                return 0;
            case QStyle::PM_MenuHMargin:
                return d->_itemPadding;
            case QStyle::PM_MenuVMargin:
                return 8;
            case QStyle::PM_MenuScrollerHeight:
                return 16;
            case QStyle::PM_SubMenuOverlap:
                return -2;
            case QStyle::PM_MenuDesktopFrameWidth:
                return 0;
            case QStyle::PM_MenuTearoffHeight:
                return 0;
            default:
                break;
        }
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

QSize NMenuPrivate::Style::sizeFromContents(ContentsType        type,
                                            const QStyleOption* option,
                                            const QSize&        size,
                                            const QWidget*      widget) const {
    bool isNMenuOrChild = false;
    if (widget) {
        const NMenu* menuWidget = qobject_cast<const NMenu*>(widget);
        if (menuWidget) {
            isNMenuOrChild = true;
        } else {
            QWidget* parent = widget->parentWidget();
            while (parent && !isNMenuOrChild) {
                if (qobject_cast<const NMenu*>(parent)) {
                    isNMenuOrChild = true;
                }
                parent = parent->parentWidget();
            }
        }
    }

    if (isNMenuOrChild && type == QStyle::CT_MenuItem) {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option)) {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
                return QProxyStyle::sizeFromContents(type, option, size, widget);
            }
            QSize menuItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            return QSize(menuItemSize.width(), d->_itemHeight);
        }
    }

    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

NMenuPrivate::NMenuPrivate(QObject* parent) : QObject(parent) {}

NMenuPrivate::~NMenuPrivate() {}
