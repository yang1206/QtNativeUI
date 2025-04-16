#include "ntabbarstyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionTab>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTabBar.h"
#include "QtNativeUI/NTheme.h"

NTabBarStyle::NTabBarStyle(QStyle* style) : QProxyStyle(style) {}

NTabBarStyle::~NTabBarStyle() {}

void NTabBarStyle::drawPrimitive(PrimitiveElement    element,
                                 const QStyleOption* option,
                                 QPainter*           painter,
                                 const QWidget*      widget) const {
    const NTabBar* tabBar = qobject_cast<const NTabBar*>(widget);
    if (!tabBar) {
        return QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

    switch (element) {
        case PE_FrameTabBarBase: {
            painter->save();

            bool   isDark      = nTheme->isDarkMode();
            QColor borderColor = isDark ? tabBar->getDarkItemSeparator() : tabBar->getLightItemSeparator();

            painter->setPen(borderColor);

            QRect selectedTabRect;
            int   currentIndex = tabBar->currentIndex();
            if (currentIndex >= 0) {
                selectedTabRect = tabBar->tabRect(currentIndex);
            }

            if (tabBar->shape() == QTabBar::RoundedNorth || tabBar->shape() == QTabBar::TriangularNorth) {
                int lineY = option->rect.bottom();
                if (selectedTabRect.isValid()) {
                    if (selectedTabRect.left() > option->rect.left()) {
                        painter->drawLine(option->rect.left(), lineY, selectedTabRect.left(), lineY);
                    }
                    if (selectedTabRect.right() < option->rect.right()) {
                        painter->drawLine(selectedTabRect.right(), lineY, option->rect.right(), lineY);
                    }
                } else {
                    painter->drawLine(option->rect.left(), lineY, option->rect.right(), lineY);
                }
            } else if (tabBar->shape() == QTabBar::RoundedSouth || tabBar->shape() == QTabBar::TriangularSouth) {
                int lineY = option->rect.top();
                if (selectedTabRect.isValid()) {
                    if (selectedTabRect.left() > option->rect.left()) {
                        painter->drawLine(option->rect.left(), lineY, selectedTabRect.left(), lineY);
                    }
                    if (selectedTabRect.right() < option->rect.right()) {
                        painter->drawLine(selectedTabRect.right(), lineY, option->rect.right(), lineY);
                    }
                } else {
                    painter->drawLine(option->rect.left(), lineY, option->rect.right(), lineY);
                }
            } else if (tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::TriangularWest) {
                int lineX = selectedTabRect.right();

                if (selectedTabRect.isValid()) {
                    if (selectedTabRect.top() > option->rect.top()) {
                        painter->drawLine(lineX, option->rect.top(), lineX, selectedTabRect.top());
                    }

                    if (selectedTabRect.bottom() < option->rect.bottom()) {
                        painter->drawLine(lineX, selectedTabRect.bottom(), lineX, option->rect.bottom());
                    }
                } else {
                    painter->drawLine(lineX, option->rect.top(), lineX, option->rect.bottom());
                }
            } else if (tabBar->shape() == QTabBar::RoundedEast || tabBar->shape() == QTabBar::TriangularEast) {
                int lineX = selectedTabRect.left();

                if (selectedTabRect.isValid()) {
                    if (selectedTabRect.top() > option->rect.top()) {
                        painter->drawLine(lineX, option->rect.top(), lineX, selectedTabRect.top());
                    }

                    if (selectedTabRect.bottom() < option->rect.bottom()) {
                        painter->drawLine(lineX, selectedTabRect.bottom(), lineX, option->rect.bottom());
                    }
                } else {
                    painter->drawLine(lineX, option->rect.top(), lineX, option->rect.bottom());
                }
            }

            painter->restore();
            return;
        }

        case PE_IndicatorTabClose: {
            if (const QStyleOption* closeOption = option) {
                bool enabled   = closeOption->state & State_Enabled;
                bool selected  = closeOption->state & State_Selected;
                bool mouseOver = closeOption->state & State_MouseOver;
                bool pressed   = closeOption->state & State_Sunken;

                bool   isDark = nTheme->isDarkMode();
                QColor foregroundColor;

                if (!enabled) {
                    foregroundColor = isDark ? tabBar->getDarkItemHeaderForegroundDisabled()
                                             : tabBar->getLightItemHeaderForegroundDisabled();
                } else if (pressed) {
                    foregroundColor = isDark ? tabBar->getDarkItemHeaderForegroundPressed()
                                             : tabBar->getLightItemHeaderForegroundPressed();
                } else if (mouseOver) {
                    foregroundColor = isDark ? tabBar->getDarkItemHeaderForegroundPointerOver()
                                             : tabBar->getLightItemHeaderForegroundPointerOver();
                } else if (selected) {
                    foregroundColor = isDark ? tabBar->getDarkItemHeaderForegroundSelected()
                                             : tabBar->getLightItemHeaderForegroundSelected();
                } else {
                    foregroundColor =
                        isDark ? tabBar->getDarkItemHeaderForeground() : tabBar->getLightItemHeaderForeground();
                }

                QRect iconRect = closeOption->rect;

                int iconSize = qMin(iconRect.width(), iconRect.height());

                QIcon closeIcon = nIcon->fromRegular(NRegularIconType::Dismiss24Regular, iconSize, foregroundColor);

                QSize actualSize = QSize(iconSize, iconSize);
                QRect drawRect   = QRect(iconRect.center().x() - actualSize.width() / 2,
                                       iconRect.center().y() - actualSize.height() / 2,
                                       actualSize.width(),
                                       actualSize.height());

                closeIcon.paint(painter, drawRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
                return;
            }
            break;
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
    const NTabBar* tabBar = qobject_cast<const NTabBar*>(widget);
    if (!tabBar) {
        return QProxyStyle::drawControl(element, option, painter, widget);
    }

    switch (element) {
        case CE_TabBarTabShape: {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing, true);

                bool selected   = tab->state & State_Selected;
                bool enabled    = tab->state & State_Enabled;
                bool mouseOver  = tab->state & State_MouseOver;
                bool pressed    = tab->state & State_Sunken;
                bool isDark     = nTheme->isDarkMode();
                bool isVertical = tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::RoundedEast ||
                                  tabBar->shape() == QTabBar::TriangularWest ||
                                  tabBar->shape() == QTabBar::TriangularEast;

                int   borderRadius = tabBar->getBorderRadius();
                QRect rect         = tab->rect;

                QColor backgroundColor;
                if (!enabled) {
                    backgroundColor = isDark ? tabBar->getDarkItemHeaderBackgroundDisabled()
                                             : tabBar->getLightItemHeaderBackgroundDisabled();
                } else if (selected) {
                    backgroundColor = isDark ? tabBar->getDarkItemHeaderBackgroundSelected()
                                             : tabBar->getLightItemHeaderBackgroundSelected();
                } else if (pressed) {
                    backgroundColor = isDark ? tabBar->getDarkItemHeaderBackgroundPressed()
                                             : tabBar->getLightItemHeaderBackgroundPressed();
                } else if (mouseOver) {
                    backgroundColor = isDark ? tabBar->getDarkItemHeaderBackgroundPointerOver()
                                             : tabBar->getLightItemHeaderBackgroundPointerOver();
                } else {
                    backgroundColor =
                        isDark ? tabBar->getDarkItemHeaderBackground() : tabBar->getLightItemHeaderBackground();
                }
                QPainterPath path;

                if (isVertical) {
                    if (tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::TriangularWest) {
                        path.moveTo(rect.right(), rect.top());
                        path.lineTo(rect.right(), rect.bottom());
                        path.lineTo(rect.left() + borderRadius, rect.bottom());
                        path.arcTo(rect.left(),
                                   rect.bottom() - borderRadius * 2,
                                   borderRadius * 2,
                                   borderRadius * 2,
                                   270,
                                   -90);
                        path.lineTo(rect.left(), rect.top() + borderRadius);
                        path.arcTo(rect.left(), rect.top(), borderRadius * 2, borderRadius * 2, 180, -90);
                        path.closeSubpath();
                    } else {
                        path.moveTo(rect.left(), rect.top());
                        path.lineTo(rect.left(), rect.bottom());
                        path.lineTo(rect.right() - borderRadius, rect.bottom());
                        path.arcTo(rect.right() - borderRadius * 2,
                                   rect.bottom() - borderRadius * 2,
                                   borderRadius * 2,
                                   borderRadius * 2,
                                   270,
                                   90);
                        path.lineTo(rect.right(), rect.top() + borderRadius);
                        path.arcTo(
                            rect.right() - borderRadius * 2, rect.top(), borderRadius * 2, borderRadius * 2, 0, 90);
                        path.closeSubpath();
                    }
                } else {
                    if (tabBar->shape() == QTabBar::RoundedSouth || tabBar->shape() == QTabBar::TriangularSouth) {
                        path.moveTo(rect.topLeft());
                        path.lineTo(rect.topRight());
                        path.lineTo(rect.right(), rect.bottom() - borderRadius);
                        path.arcTo(QRectF(rect.right() - 2.0 * borderRadius,
                                          rect.bottom() - 2.0 * borderRadius,
                                          2.0 * borderRadius,
                                          2.0 * borderRadius),
                                   0,
                                   -90);
                        path.arcTo(QRectF(rect.left(),
                                          rect.bottom() - 2.0 * borderRadius,
                                          2.0 * borderRadius,
                                          2.0 * borderRadius),
                                   270,
                                   -90);
                        path.closeSubpath();
                    } else {
                        path.moveTo(rect.left(), rect.bottom());
                        path.lineTo(rect.left(), rect.top() + borderRadius);
                        path.arcTo(rect.left(), rect.top(), borderRadius * 2, borderRadius * 2, 180, -90);
                        path.lineTo(rect.right() - borderRadius, rect.top());
                        path.arcTo(
                            rect.right() - borderRadius * 2, rect.top(), borderRadius * 2, borderRadius * 2, 90, -90);
                        path.lineTo(rect.right(), rect.bottom());
                        path.closeSubpath();
                    }
                }

                painter->fillPath(path, backgroundColor);

                if (selected) {
                    QColor borderColor = isDark ? tabBar->getDarkItemHeaderBorderColorSelected()
                                                : tabBar->getLightItemHeaderBorderColorSelected();
                    painter->setPen(QPen(borderColor, 1.0));

                    QPainterPath borderPath;
                    qreal        topY    = rect.top();
                    qreal        bottomY = rect.bottom();
                    qreal        leftX   = rect.left();
                    qreal        rightX  = rect.right();
                    qreal        radius  = borderRadius;

                    if (isVertical) {
                        if (tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::TriangularWest) {
                            borderPath.moveTo(rightX, topY);
                            borderPath.lineTo(leftX + radius, topY);
                            borderPath.arcTo(leftX, topY, radius * 2, radius * 2, 90, 90);
                            borderPath.lineTo(leftX, bottomY - radius);
                            borderPath.arcTo(leftX, bottomY - radius * 2, radius * 2, radius * 2, 180, 90);
                            borderPath.lineTo(rightX, bottomY);
                        } else {
                            borderPath.moveTo(leftX, topY);
                            borderPath.lineTo(rightX - radius, topY);
                            borderPath.arcTo(rightX - radius * 2, topY, radius * 2, radius * 2, 90, -90);
                            borderPath.lineTo(rightX, bottomY - radius);
                            borderPath.arcTo(rightX - radius * 2, bottomY - radius * 2, radius * 2, radius * 2, 0, -90);
                            borderPath.lineTo(leftX, bottomY);
                        }
                    } else {
                        if (tabBar->shape() == QTabBar::RoundedNorth || tabBar->shape() == QTabBar::TriangularNorth) {
                            borderPath.moveTo(leftX, bottomY);
                            borderPath.lineTo(leftX, topY + radius);
                            borderPath.arcTo(leftX, topY, radius * 2, radius * 2, 180, -90);
                            borderPath.lineTo(rightX - radius, topY);
                            borderPath.arcTo(rightX - radius * 2, topY, radius * 2, radius * 2, 90, -90);
                            borderPath.lineTo(rightX, bottomY);
                        } else {
                            borderPath.moveTo(leftX, topY);
                            borderPath.lineTo(leftX, bottomY - radius);
                            borderPath.arcTo(leftX, bottomY - radius * 2, radius * 2, radius * 2, 180, 90);
                            borderPath.lineTo(rightX - radius, bottomY);
                            borderPath.arcTo(
                                rightX - radius * 2, bottomY - radius * 2, radius * 2, radius * 2, 270, 90);
                            borderPath.lineTo(rightX, topY);
                        }
                    }

                    painter->drawPath(borderPath);
                }

                int tabIndex = -1;
                for (int i = 0; i < tabBar->count(); ++i) {
                    if (tabBar->tabRect(i).contains(tab->rect.center())) {
                        tabIndex = i;
                        break;
                    }
                }

                if (tabIndex == -1) {
                } else {
                    int tabCount     = tabBar->count();
                    int currentIndex = tabBar->currentIndex();

                    bool isSelected = (tabIndex == currentIndex);
                    bool isLastTab  = (tabIndex == tabCount - 1);

                    bool drawSeparator = false;
                    if (!isLastTab) {
                        if (!isSelected) {
                            bool nextTabIsSelected = (tabIndex + 1 == currentIndex);
                            if (!nextTabIsSelected) {
                                drawSeparator = true;
                            }
                        }
                    }

                    if (drawSeparator) {
                        QColor separatorColor =
                            isDark ? tabBar->getDarkItemSeparator() : tabBar->getLightItemSeparator();
                        painter->setPen(separatorColor);

                        if (isVertical) {
                            int lineY;
                            lineY         = rect.bottom();
                            int lineWidth = rect.width() / 2;
                            int startX    = rect.center().x() - lineWidth / 2;
                            int endX      = startX + lineWidth;
                            painter->drawLine(startX, lineY, endX, lineY);

                        } else {
                            int lineX      = rect.right();
                            int lineHeight = rect.height() / 3;
                            int startY     = rect.center().y() - lineHeight / 2;
                            painter->drawLine(lineX, startY, lineX, startY + lineHeight);
                        }
                    }
                } // end if (tabIndex != -1)

                painter->restore();
                return;
            }
        } break;

        case CE_TabBarTabLabel: {
            if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing, true);

                bool selected   = tab->state & State_Selected;
                bool enabled    = tab->state & State_Enabled;
                bool mouseOver  = tab->state & State_MouseOver;
                bool pressed    = tab->state & State_Sunken;
                bool isDark     = nTheme->isDarkMode();
                bool isVertical = tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::RoundedEast ||
                                  tabBar->shape() == QTabBar::TriangularWest ||
                                  tabBar->shape() == QTabBar::TriangularEast;

                QColor textColor;
                if (!enabled) {
                    textColor = isDark ? tabBar->getDarkItemHeaderForegroundDisabled()
                                       : tabBar->getLightItemHeaderForegroundDisabled();
                } else if (selected) {
                    textColor = isDark ? tabBar->getDarkItemHeaderForegroundSelected()
                                       : tabBar->getLightItemHeaderForegroundSelected();
                } else if (pressed) {
                    textColor = isDark ? tabBar->getDarkItemHeaderForegroundPressed()
                                       : tabBar->getLightItemHeaderForegroundPressed();
                } else if (mouseOver) {
                    textColor = isDark ? tabBar->getDarkItemHeaderForegroundPointerOver()
                                       : tabBar->getLightItemHeaderForegroundPointerOver();
                } else {
                    textColor = isDark ? tabBar->getDarkItemHeaderForeground() : tabBar->getLightItemHeaderForeground();
                }

                QColor iconColor;
                if (!enabled) {
                    iconColor = isDark ? tabBar->getDarkItemIconForegroundDisabled()
                                       : tabBar->getLightItemIconForegroundDisabled();
                } else if (selected) {
                    iconColor = isDark ? tabBar->getDarkItemIconForegroundSelected()
                                       : tabBar->getLightItemIconForegroundSelected();
                } else if (pressed) {
                    iconColor = isDark ? tabBar->getDarkItemIconForegroundPressed()
                                       : tabBar->getLightItemIconForegroundPressed();
                } else if (mouseOver) {
                    iconColor = isDark ? tabBar->getDarkItemIconForegroundPointerOver()
                                       : tabBar->getLightItemIconForegroundPointerOver();
                } else {
                    iconColor = isDark ? tabBar->getDarkItemIconForeground() : tabBar->getLightItemIconForeground();
                }

                QRect textRect = tab->rect;

                painter->setFont(widget->font());
                QFontMetrics metrics(widget->font());

                int textHeight   = metrics.height();
                int textBaseline = textRect.center().y() + (metrics.ascent() - textHeight / 2);

                if (!tab->icon.isNull()) {
                    QSize iconSize(16, 16);
                    QRect iconRect;

                    if (isVertical) {
                        iconRect.setWidth(iconSize.width());
                        iconRect.setHeight(iconSize.height());
                        iconRect.moveLeft(textRect.left() + 8);
                        iconRect.moveTop(textRect.center().y() - iconSize.height() / 2);

                        textRect.setLeft(iconRect.right() + 8);
                    } else {
                        iconRect.setWidth(iconSize.width());
                        iconRect.setHeight(iconSize.height());

                        int iconY = textRect.top() + (textRect.height() - iconSize.height()) / 2;

                        iconY = iconY - 1;

                        iconRect.moveLeft(textRect.left() + 6);
                        iconRect.moveTop(iconY);

                        textRect.setLeft(iconRect.right() + 6);
                    }

                    QIcon   icon   = tab->icon;
                    QPixmap pixmap = icon.pixmap(iconSize);
                    painter->drawPixmap(iconRect, pixmap);
                }

                if (tab->rightButtonSize.width() > 0) {
                    textRect.setRight(textRect.right() - tab->rightButtonSize.width() - 4);
                }

                textRect.adjust(4, 0, -4, 0);
                QString text = tab->text;

                painter->setPen(textColor);

                if (isVertical) {
                    QString displayText = text;

                    painter->save();

                    if (tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::TriangularWest) {
                        painter->translate(textRect.center().x(), textRect.center().y());
                        painter->rotate(-90);
                    } else {
                        painter->translate(textRect.center().x(), textRect.center().y());
                        painter->rotate(90);
                    }

                    int textWidth  = metrics.horizontalAdvance(displayText);
                    int textHeight = metrics.height();

                    if (textWidth > textRect.height()) {
                        displayText = metrics.elidedText(text, Qt::ElideRight, textRect.height());
                        textWidth   = metrics.horizontalAdvance(displayText);
                    }

                    QRect rotatedTextRect(-textWidth / 2, -textHeight / 2, textWidth, textHeight);

                    painter->drawText(rotatedTextRect, Qt::AlignCenter, displayText);

                    painter->restore();
                } else {
                    QString elidedText = metrics.elidedText(text, Qt::ElideRight, textRect.width());

                    int textY = textBaseline;

                    painter->drawText(textRect.left(), textY, elidedText);
                }

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
    if (type == CT_TabBarTab) {
        if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
            QSize          newSize = size;
            const QTabBar* tabBar  = qobject_cast<const QTabBar*>(widget);
            bool           isVertical =
                tabBar && (tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::RoundedEast ||
                           tabBar->shape() == QTabBar::TriangularWest || tabBar->shape() == QTabBar::TriangularEast);

            if (isVertical) {
                QSize origSize = QProxyStyle::sizeFromContents(type, option, size, widget);

                newSize.setWidth(qMax(origSize.width(), 32));
                newSize.setHeight(qMax(origSize.height(), 100));

                newSize.setHeight(qMin(newSize.height(), 240));
                return newSize;
            } else {
                newSize.setHeight(qMax(newSize.height(), 32));

                if (!tab->text.isEmpty()) {
                    newSize.setWidth(qMax(newSize.width(), 100));
                }

                newSize.setWidth(qMin(newSize.width(), 240));
            }

            return newSize;
        }
    }

    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

QRect NTabBarStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const {
    const QTabBar* tabBar = qobject_cast<const QTabBar*>(widget);

    bool isVertical =
        tabBar && (tabBar->shape() == QTabBar::RoundedWest || tabBar->shape() == QTabBar::RoundedEast ||
                   tabBar->shape() == QTabBar::TriangularWest || tabBar->shape() == QTabBar::TriangularEast);

    if (isVertical && (element == SE_TabBarTabText || element == SE_TabBarTabLeftButton)) {
        return QProxyStyle::subElementRect(element, option, widget);
    }

    if (element == SE_TabBarTabText) {
        if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
            QRect textRect = tab->rect;

            if (!tab->icon.isNull()) {
                textRect.setLeft(textRect.left() + 24);
            }

            if (tab->rightButtonSize.width() > 0) {
                textRect.setRight(textRect.right() - tab->rightButtonSize.width() - 4);
            }

            textRect.adjust(8, 0, -8, 0);

            return textRect;
        }
    } else if (element == SE_TabBarTabRightButton) {
        if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
            QRect closeRect;
            if (tab->rightButtonSize.isValid()) {
                const int size = 20;
                closeRect.setSize(QSize(size, size));

                if (isVertical) {
                    closeRect.moveCenter(QPoint(tab->rect.center().x(), tab->rect.top() + size / 2 + 8));
                } else {
                    closeRect.moveRight(tab->rect.right() - 8);
                    closeRect.moveTop(tab->rect.center().y() - size / 2);
                }
            }
            return closeRect;
        }
    }

    return QProxyStyle::subElementRect(element, option, widget);
}
