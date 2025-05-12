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
    // 首先判断是否为NMenu组件或其子组件
    bool isNMenuOrChild = false;
    if (widget) {
        const NMenu* menuWidget = qobject_cast<const NMenu*>(widget);
        if (menuWidget) {
            isNMenuOrChild = true;
        } else {
            // 如果不是NMenu，检查它是否是NMenu的子控件
            QWidget* parent = widget->parentWidget();
            while (parent && !isNMenuOrChild) {
                if (qobject_cast<const NMenu*>(parent)) {
                    isNMenuOrChild = true;
                }
                parent = parent->parentWidget();
            }
        }
    }
    
    // 如果不是NMenu或其子组件，使用基础样式
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

            // 使用设计令牌获取间距
            int hPadding = d->_itemPadding;
            int iconSize = d->_itemIconSize;
            // int iconTextSpacing  = d->_iconTextSpacing;
            int menuBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

            // 分隔线
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator) {
                // 使用 Fluent 分隔线颜色
                QColor sepColor = d->_isDark ? d->_pDarkSeparatorColor : d->_pLightSeparatorColor;

                painter->setPen(Qt::NoPen);
                painter->setBrush(sepColor);
                painter->drawRoundedRect(menuRect.x(), menuRect.center().y(), menuRect.width(), 1, 0, 0);

                painter->restore();
                return;
            }

            // 悬停/选中状态背景
            if (mopt->state.testFlag(State_Enabled) &&
                (mopt->state.testFlag(State_Selected) || mopt->state.testFlag(State_MouseOver) || mopt->checked)) {
                // 使用 Fluent 悬停颜色
                QColor hoverColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, d->_themeMode);

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
            // 绘制图标或勾选标记
            if (!mopt->icon.isNull()) {
                // 如果有图标，始终绘制图标
                int   iconY = menuRect.y() + (menuRect.height() - iconSize) / 2;
                QRect iconRect(menuRect.x() + hPadding, iconY, iconSize, iconSize);

                QIcon::Mode iconMode = mopt->state.testFlag(QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled;
                mopt->icon.paint(painter, iconRect, Qt::AlignCenter, iconMode);
            }
            // 检查是否是可勾选项
            else if (mopt->checked) {
                // 绘制勾选图标
                int   checkSize = iconSize;
                QRect checkRect(
                    menuRect.x() + hPadding, menuRect.y() + (menuRect.height() - checkSize) / 2, checkSize, checkSize);

                // 使用 Fluent 图标
                QIcon checkIcon = nIcon->fromRegular(NRegularIconType::Checkmark20Regular, checkSize, textColor);
                checkIcon.paint(painter, checkRect, Qt::AlignCenter);
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
                    textX += d->_checkmarkWidth;
                }

                // 在绘制文本之前，先计算快捷键文本需要的宽度
                int shortcutWidth = 0;
                if (!shortcutText.isEmpty()) {
                    shortcutWidth = painter->fontMetrics().horizontalAdvance(shortcutText) + 2 * d->_itemPadding;
                }

                // 文本矩形
                QRect textRect(
                    textX, menuRect.y(), menuRect.width() - textX - hPadding - shortcutWidth, menuRect.height());

                // 绘制主文本
                painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextSingleLine, itemText);

                // 绘制快捷键文本
                if (!shortcutText.isEmpty()) {
                    QRect shortcutRect(
                        menuRect.right() - shortcutWidth - hPadding, menuRect.y(), shortcutWidth, menuRect.height());

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
    if (widget) {
        const NMenu* menuWidget = qobject_cast<const NMenu*>(widget);
        if (!menuWidget) {
            // 如果不是NMenu，也检查它是否是NMenu的子控件
            QWidget* parent = widget->parentWidget();
            while (parent && !menuWidget) {
                menuWidget = qobject_cast<const NMenu*>(parent);
                parent     = parent->parentWidget();
            }
            if (!menuWidget) {
                // 如果不是NMenu或其子控件，使用基础样式
                return QProxyStyle::drawPrimitive(element, option, painter, widget);
            }
        }
    }
    if (element == PE_PanelMenu) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QColor bgColor     = d->_isDark ? d->_pDarkBackgroundColor : d->_pLightBackgroundColor;
        QColor borderColor = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;

        QPainterPath path;
        path.addRoundedRect(option->rect, d->_pBorderRadius, d->_pBorderRadius);

        painter->setPen(QPen(borderColor, 1));
        painter->setBrush(bgColor);
        painter->drawPath(path);

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
    // 判断是否为NMenu组件
    const NMenu* menu = qobject_cast<const NMenu*>(widget);
    bool isNMenuOrChild = false;
    
    if (menu) {
        isNMenuOrChild = true;
    } else if (widget) {
        // 如果不是NMenu，检查它是否是NMenu的子控件
        QWidget* parent = widget->parentWidget();
        while (parent && !isNMenuOrChild) {
            if (qobject_cast<const NMenu*>(parent)) {
                isNMenuOrChild = true;
            }
            parent = parent->parentWidget();
        }
    }
    
    // 只有当组件是NMenu或其子组件时才应用自定义度量
    if (isNMenuOrChild) {
        const NMenuPrivate* d = menu ? menu->d_func() : this->d;
        
        switch (metric) {
            case PM_MenuPanelWidth:
                return 0;
            case QStyle::PM_MenuHMargin:
                return d->_itemPadding;
            case QStyle::PM_MenuVMargin:
                return 4;
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

QSize NMenuPrivate::Style::sizeFromContents(ContentsType type,
                                          const QStyleOption* option,
                                          const QSize& size,
                                          const QWidget* widget) const {
    // 判断是否为NMenu组件
    bool isNMenuOrChild = false;
    if (widget) {
        const NMenu* menuWidget = qobject_cast<const NMenu*>(widget);
        if (menuWidget) {
            isNMenuOrChild = true;
        } else {
            // 如果不是NMenu，检查它是否是NMenu的子控件
            QWidget* parent = widget->parentWidget();
            while (parent && !isNMenuOrChild) {
                if (qobject_cast<const NMenu*>(parent)) {
                    isNMenuOrChild = true;
                }
                parent = parent->parentWidget();
            }
        }
    }
    
    // 只有当组件是NMenu或其子组件时才应用自定义大小
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
