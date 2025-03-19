#include "nmenu_p.h"

#include <qstyleoption.h>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NMenuPrivate::Style::Style(NMenuPrivate* parent, QStyle* style) : QProxyStyle(style), d(parent) {}

void NMenuPrivate::Style::drawControl(ControlElement      element,
                                      const QStyleOption* option,
                                      QPainter*           painter,
                                      const QWidget*      widget) const {
    if (element == CE_MenuItem) {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option)) {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                                    QPainter::SmoothPixmapTransform);

            QRect menuRect = mopt->rect;

            // 使用设计令牌获取间距
            int hPadding         = NDesignToken(NDesignTokenKey::SpacingM).toInt();
            int iconSize         = d->_itemIconSize;
            int iconTextSpacing  = NDesignToken(NDesignTokenKey::SpacingS).toInt();
            int menuBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusSmall).toInt();

            // 分隔线
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
                // 使用 Fluent 分隔线颜色
                QColor sepColor = d->_isDark ? d->_pDarkSeparatorColor : d->_pLightSeparatorColor;

                // 分隔线边距
                int margin = hPadding;

                painter->setPen(Qt::NoPen);
                painter->setBrush(sepColor);
                painter->drawRoundedRect(
                    menuRect.x() + margin, menuRect.center().y(), menuRect.width() - 2 * margin, 1, 0.5, 0.5);

                painter->restore();
                return;
            }

            // 悬停/选中状态背景
            if (mopt->state.testFlag(QStyle::State_Enabled) &&
                (mopt->state.testFlag(QStyle::State_Selected) || mopt->state.testFlag(QStyle::State_MouseOver))) {
                // 使用 Fluent 悬停颜色
                QColor hoverColor = NThemeColor(mopt->state.testFlag(QStyle::State_Selected)
                                                    ? NFluentColorKey::ControlFillColorInputActive
                                                    : NFluentColorKey::ControlFillColorSecondary,
                                                d->_themeMode);

                painter->setPen(Qt::NoPen);
                painter->setBrush(hoverColor);
                painter->drawRoundedRect(menuRect, menuBorderRadius, menuBorderRadius);
            }

            // 文本颜色
            QColor textColor =
                NThemeColor(mopt->state.testFlag(QStyle::State_Enabled) ? NFluentColorKey::TextFillColorPrimary
                                                                        : NFluentColorKey::TextFillColorDisabled,
                            d->_themeMode);
            painter->setPen(textColor);

            // 检查是否是可勾选项
            if (mopt->checked) {
                // 绘制勾选图标
                int   checkSize = iconSize;
                QRect checkRect(
                    menuRect.x() + hPadding, menuRect.y() + (menuRect.height() - checkSize) / 2, checkSize, checkSize);

                // 使用 Fluent 图标
                QIcon checkIcon = nIcon->fromFilled(NFilledIconType::Checkmark16Filled, checkSize, textColor);
                checkIcon.paint(painter, checkRect, Qt::AlignCenter);
            }
            // 绘制普通图标
            else if (!mopt->icon.isNull()) {
                int   iconY = menuRect.y() + (menuRect.height() - iconSize) / 2;
                QRect iconRect(menuRect.x() + hPadding, iconY, iconSize, iconSize);

                QIcon::Mode iconMode = mopt->state.testFlag(QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled;
                mopt->icon.paint(painter, iconRect, Qt::AlignCenter, iconMode);
            }

            // 文本绘制
            if (!mopt->text.isEmpty()) {
                // 处理包含快捷键的文本
                QString itemText = mopt->text;
                QString shortcutText;

                // 检查是否有制表符分隔的快捷键文本
                int tabIndex = itemText.indexOf('\t');
                if (tabIndex != -1) {
                    shortcutText = itemText.mid(tabIndex + 1);
                    itemText     = itemText.left(tabIndex);
                }

                // 计算文本位置
                int textX = menuRect.x() + hPadding;

                // 如果有图标或是可选择的项，增加偏移
                if (!mopt->icon.isNull() || mopt->checked) {
                    textX += iconSize + iconTextSpacing;
                }

                // 文本矩形
                QRect textRect(textX,
                               menuRect.y(),
                               menuRect.width() - textX - hPadding - (shortcutText.isEmpty() ? 0 : 20),
                               menuRect.height());

                // 绘制主文本
                painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextSingleLine, itemText);

                // 绘制快捷键文本
                if (!shortcutText.isEmpty()) {
                    QRect shortcutRect(menuRect.right() - 20 - hPadding, menuRect.y(), 20, menuRect.height());

                    // 使用次要文本颜色
                    QColor shortcutColor = NThemeColor(mopt->state.testFlag(QStyle::State_Enabled)
                                                           ? NFluentColorKey::TextFillColorSecondary
                                                           : NFluentColorKey::TextFillColorDisabled,
                                                       d->_themeMode);
                    painter->setPen(shortcutColor);

                    painter->drawText(
                        shortcutRect, Qt::AlignVCenter | Qt::AlignRight | Qt::TextSingleLine, shortcutText);
                }
            }

            // 子菜单指示器
            if (mopt->menuItemType == QStyleOptionMenuItem::SubMenu) {
                QRect arrowRect(menuRect.right() - hPadding - iconSize,
                                menuRect.y() + (menuRect.height() - iconSize) / 2,
                                iconSize,
                                iconSize);

                // 使用 Fluent 图标
                QIcon arrowIcon = nIcon->fromFilled(NFilledIconType::ChevronRight16Filled, iconSize, textColor);
                arrowIcon.paint(painter, arrowRect, Qt::AlignCenter);
            }

            painter->restore();
            return;
        }
    } else if (element == CE_MenuBarItem) {
        // 自定义菜单栏项的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuScroller) {
        // 自定义菜单滚动条的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuTearoff) {
        // 自定义菜单可拖动部分的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    } else if (element == CE_MenuEmptyArea) {
        // 自定义菜单空白区域的绘制
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
}

void NMenuPrivate::Style::drawPrimitive(PrimitiveElement    element,
                                        const QStyleOption* option,
                                        QPainter*           painter,
                                        const QWidget*      widget) const {
    if (element == PE_PanelMenu) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        nTheme->drawEffectShadow(
            painter, option->rect, d->_shadowBorderWidth, d->_pBorderRadius, NDesignTokenKey::ElevationRest);
        // 背景绘制
        QRect  foregroundRect(d->_shadowBorderWidth,
                             d->_shadowBorderWidth,
                             option->rect.width() - 2 * d->_shadowBorderWidth,
                             option->rect.height() - 2 * d->_shadowBorderWidth);
        QColor bgColor = d->_isDark ? d->_pDarkBackgroundColor : d->_pLightBackgroundColor;

        painter->setPen(d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
        painter->restore();
        return;
    } else if (element == PE_IndicatorMenuCheckMark) {
        return;
    } else if (element == PE_FrameMenu) {
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int NMenuPrivate::Style::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    const NMenu* menu = qobject_cast<const NMenu*>(widget);

    if (menu) {
        const NMenuPrivate* d = menu->d_func();

        switch (metric) {
            case PM_MenuPanelWidth:
                return 0; // 菜单面板宽度
            case QStyle::PM_MenuHMargin:
                return d->_itemPadding; // 菜单水平边距
            case QStyle::PM_MenuVMargin:
                return 4; // 菜单垂直边距
            case QStyle::PM_MenuScrollerHeight:
                return 16; // 菜单滚动条高度
            case QStyle::PM_SubMenuOverlap:
                return -2; // 子菜单重叠量
            case QStyle::PM_MenuDesktopFrameWidth:
                return 0; // 桌面菜单框架宽度
            case QStyle::PM_MenuTearoffHeight:
                return 0; // 菜单可拖动部分高度
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
    switch (type) {
        case QStyle::CT_MenuItem: {
            if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option)) {
                if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
                    break;
                }
                QSize        menuItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
                const NMenu* menu         = dynamic_cast<const NMenu*>(widget);
                if (menu->isHasIcon() || mopt->menuHasCheckableItems) {
                    d->_isAnyoneItemHasIcon = true;
                }
                if (menu->isHasChildMenu()) {
                    return QSize(menuItemSize.width() + 20, d->_itemHeight);
                } else {
                    return QSize(menuItemSize.width(), d->_itemHeight);
                }
            }
        }
        default: {
            break;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

NMenuPrivate::NMenuPrivate(QObject* parent) : QObject(parent) {}

NMenuPrivate::~NMenuPrivate() {}
