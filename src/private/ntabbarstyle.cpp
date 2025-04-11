#include "ntabbarstyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NTabBarStyle::NTabBarStyle(QStyle* style) : QProxyStyle(style) {
    _themeMode = nTheme->themeMode();
    _isDark    = nTheme->isDarkMode();

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        _isDark    = nTheme->isDarkMode();
    });
}

NTabBarStyle::~NTabBarStyle() {}

QPainterPath NTabBarStyle::getTabPath(const QRect& tabRect) const {
    QPainterPath path;
    path.moveTo(tabRect.x(), tabRect.bottom() + 1);
    path.arcTo(QRectF(tabRect.x() - _margin, tabRect.bottom() - _margin * 2 + 1, _margin * 2, _margin * 2), -90, 90);
    path.lineTo(tabRect.x() + _margin, tabRect.y() + _topRadius);
    path.arcTo(QRectF(tabRect.x() + _margin, tabRect.y(), _topRadius * 2, _topRadius * 2), 180, -90);
    path.lineTo(tabRect.right() - _margin - _topRadius, tabRect.y());
    path.arcTo(
        QRectF(tabRect.right() - _margin - 2 * _topRadius, tabRect.y(), _topRadius * 2, _topRadius * 2), 90, -90);
    path.lineTo(tabRect.right() - _margin, tabRect.bottom() - _margin);
    path.arcTo(
        QRectF(tabRect.right() - _margin, tabRect.bottom() - 2 * _margin + 1, _margin * 2, _margin * 2), -180, 90);
    path.lineTo(tabRect.right(), tabRect.bottom() + 10);
    path.lineTo(tabRect.x(), tabRect.bottom() + 10);
    path.closeSubpath();

    return path;
}

void NTabBarStyle::drawPrimitive(PrimitiveElement    element,
                                 const QStyleOption* option,
                                 QPainter*           painter,
                                 const QWidget*      widget) const {
    switch (element) {
        case PE_FrameTabBarBase:
            // 不绘制底边线
            return;

        case PE_IndicatorArrowLeft:
        case PE_IndicatorArrowRight:
            // 不绘制左右箭头
            return;

        case PE_PanelButtonTool:
            // 不绘制工具按钮面板
            return;

        case PE_IndicatorTabTear:
            // 不绘制标签撕裂指示器
            return;

        case PE_IndicatorTabClose: {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

            if (option->state.testFlag(QStyle::State_MouseOver)) {
                painter->setPen(Qt::NoPen);
                painter->setBrush(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
                painter->drawRoundedRect(option->rect, 2, 2);
            }

            QColor closeColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);

            // 绘制X形状
            painter->setPen(QPen(closeColor, 1.5));
            int margin = 4;
            painter->drawLine(option->rect.x() + margin,
                              option->rect.y() + margin,
                              option->rect.right() - margin,
                              option->rect.bottom() - margin);
            painter->drawLine(option->rect.right() - margin,
                              option->rect.y() + margin,
                              option->rect.x() + margin,
                              option->rect.bottom() - margin);

            painter->restore();
            return;
        }

        default:
            break;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NTabBarStyle::drawControl(ControlElement      element,
                               const QStyleOption* option,
                               QPainter*           painter,
                               const QWidget*      widget) const {
    switch (element) {
        case CE_TabBarTabShape: {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect tabRect = tab->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                painter->setPen(Qt::NoPen);

                if (tab->state.testFlag(QStyle::State_Selected)) {
                    // 选中标签的特殊形状
                    QRect adjustedRect = tabRect;
                    adjustedRect.setLeft(adjustedRect.left() - _margin);
                    if (tab->position != QStyleOptionTab::End) {
                        adjustedRect.setRight(adjustedRect.right() + _margin + 1);
                    }

                    // 选中的Tab使用SelectedBackgroundColor
                    painter->setBrush(NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, _themeMode));
                    if (_isDark) {
                        painter->setBrush(
                            NThemeColor(NFluentColorKey::SolidBackgroundFillColorQuarternary, _themeMode));
                    }

                    // 绘制特殊形状路径
                    painter->drawPath(getTabPath(adjustedRect));
                } else if (tab->state.testFlag(QStyle::State_MouseOver)) {
                    // 悬停状态 - 也使用特殊形状，与选中保持一致
                    QRect adjustedRect = tabRect;
                    adjustedRect.setLeft(adjustedRect.left() - _margin);
                    if (tab->position != QStyleOptionTab::End) {
                        adjustedRect.setRight(adjustedRect.right() + _margin + 1);
                    }

                    // 悬停的Tab使用HoverBackgroundColor
                    painter->setBrush(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));

                    // 绘制特殊形状路径
                    painter->drawPath(getTabPath(adjustedRect));
                }

                // 绘制分隔线
                if (!tab->state.testFlag(QStyle::State_Selected) && !tab->state.testFlag(QStyle::State_MouseOver) &&
                    tab->position != QStyleOptionTab::End && tab->selectedPosition != QStyleOptionTab::NextIsSelected) {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, _themeMode));
                    painter->drawRoundedRect(
                        QRectF(tabRect.right() - 3, tabRect.y() + 7, 3, tabRect.height() - 14), 2, 2);
                }

                painter->restore();
                return;
            }
        } break;

        case CE_TabBarTabLabel: {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect textRect = subElementRect(SE_TabBarTabText, tab, widget);
                textRect.setLeft(textRect.left() + 10);

                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                                        QPainter::SmoothPixmapTransform);

                // 绘制图标
                QIcon icon = tab->icon;
                if (!icon.isNull()) {
                    QRectF iconRect(tab->rect.x() + 15,
                                    textRect.center().y() - (qreal) tab->iconSize.height() / 2 + 1,
                                    tab->iconSize.width(),
                                    tab->iconSize.height());

                    QIcon::Mode  iconMode  = (tab->state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled;
                    QIcon::State iconState = (tab->state & QStyle::State_Selected) ? QIcon::On : QIcon::Off;

                    QPixmap iconPix = icon.pixmap(tab->iconSize, widget->devicePixelRatio(), iconMode, iconState);

                    painter->drawPixmap(iconRect.x(), iconRect.y(), iconPix);
                }

                // 绘制文字 - 使用恰当的颜色
                QColor textColor;
                if (!(tab->state & QStyle::State_Enabled)) {
                    textColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, _themeMode);
                } else if (tab->state & QStyle::State_Selected) {
                    textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);
                } else if (tab->state & QStyle::State_MouseOver) {
                    textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);
                } else {
                    textColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, _themeMode);
                }

                painter->setPen(textColor);
                QString text = painter->fontMetrics().elidedText(tab->text, Qt::ElideRight, textRect.width());
                painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextDontClip, text);

                painter->restore();
                return;
            }
        } break;

        default:
            break;
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize NTabBarStyle::sizeFromContents(ContentsType        type,
                                     const QStyleOption* option,
                                     const QSize&        size,
                                     const QWidget*      widget) const {
    switch (type) {
        case CT_TabBarTab:
            // 固定标签尺寸
            return QSize(220, 35);

        default:
            break;
    }

    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

QRect NTabBarStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const {
    switch (element) {
        case SE_TabBarScrollLeftButton:
        case SE_TabBarScrollRightButton:
            // 隐藏滚动按钮
            return QRect();

        case SE_TabBarTabText: {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect textRect = tab->rect;

                // 为图标预留空间
                if (!tab->icon.isNull()) {
                    textRect.setLeft(textRect.left() + tab->iconSize.width() + 20);
                } else {
                    textRect.setLeft(textRect.left() + 15);
                }

                // 为关闭按钮预留空间
                if (const QTabBar* tabBar = qobject_cast<const QTabBar*>(widget)) {
                    if (tabBar->tabsClosable() && tab->state & QStyle::State_Enabled) {
                        textRect.setRight(textRect.right() - 20);
                    }
                }

                return textRect;
            }
        } break;

        case SE_TabBarTabLeftButton:
            // 定位左侧按钮（通常是图标）
            return QRect();

        case SE_TabBarTabRightButton:
            // 定位右侧按钮（通常是关闭按钮）
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                if (tab->rightButtonSize.isEmpty())
                    return QRect();

                QRect r(QPoint(), tab->rightButtonSize);
                r.moveCenter(tab->rect.center());
                r.moveRight(tab->rect.right() - 8);
                return r;
            }

        default:
            break;
    }

    return QProxyStyle::subElementRect(element, option, widget);
}