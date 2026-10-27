#include "ntabbarstyle.h"

#include <QAbstractButton>
#include <QIcon>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPixmapCache>
#include <QStyleOptionTab>
#include <QStyleOptionToolButton>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTheme.h>

namespace {
const NTabBar* owningBar(const QWidget* widget) {
    for (const QWidget* current = widget; current; current = current->parentWidget()) {
        if (const auto* bar = qobject_cast<const NTabBar*>(current))
            return bar;
    }
    return nullptr;
}
bool vertical(const NTabBar* bar) {
    return bar->shape() == QTabBar::RoundedWest || bar->shape() == QTabBar::RoundedEast ||
           bar->shape() == QTabBar::TriangularWest || bar->shape() == QTabBar::TriangularEast;
}
QSizeF canonicalSize(const QStyleOptionTab& tab, const NTabBar* bar) {
    return vertical(bar) ? QSizeF(tab.rect.height(), tab.rect.width()) : QSizeF(tab.rect.size());
}
QTransform tabTransform(const QStyleOptionTab& tab, const NTabBar* bar, bool shape) {
    QTransform transform;
    if (bar->shape() == QTabBar::RoundedWest || bar->shape() == QTabBar::TriangularWest) {
        transform.translate(tab.rect.left(), tab.rect.bottom() + 1);
        transform.rotate(-90);
    } else if (bar->shape() == QTabBar::RoundedEast || bar->shape() == QTabBar::TriangularEast) {
        transform.translate(tab.rect.right() + 1, tab.rect.top());
        transform.rotate(90);
    } else if (shape && (bar->shape() == QTabBar::RoundedSouth || bar->shape() == QTabBar::TriangularSouth)) {
        transform.translate(tab.rect.left(), tab.rect.bottom() + 1);
        transform.scale(1, -1);
    } else {
        transform.translate(tab.rect.left(), tab.rect.top());
    }
    return transform;
}
bool compact(const QStyleOptionTab& tab, const NTabBar* bar) {
    return bar->getTabWidthMode() == NTabBar::TabWidthMode::Compact && !(tab.state & QStyle::State_Selected) &&
           !tab.icon.isNull();
}
bool nativeCloseButton(const QWidget* button) { return button && button->property("_nTabNativeCloseButton").toBool(); }
bool nativeCloseButton(const QStyleOptionTab& tab, const NTabBar* bar, QTabBar::ButtonPosition side) {
    return nativeCloseButton(bar->tabButton(tab.tabIndex, side));
}
qreal buttonLength(const QSize& size, const NTabBar* bar) { return vertical(bar) ? size.height() : size.width(); }
bool  hasClose(const QStyleOptionTab& tab, const NTabBar* bar) {
    const bool hasNativeButton =
        nativeCloseButton(tab, bar, QTabBar::LeftSide) || nativeCloseButton(tab, bar, QTabBar::RightSide);
    return hasNativeButton && bar->tabsClosable() && bar->getTabClosable(tab.tabIndex) && !compact(tab, bar) &&
           bar->getCloseButtonDisplayMode() != NTabBar::CloseButtonDisplayMode::Never;
}
QRectF textRect(const QStyleOptionTab& tab, const NTabBar* bar) {
    const QSizeF size    = canonicalSize(tab, bar);
    const int    padding = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
    const int    gap     = NSpacingToken(NDesignTokenKey::SpacingS).toInt();
    qreal        left    = padding;
    qreal        right   = size.width() - padding;
    if (buttonLength(tab.leftButtonSize, bar) > 0 &&
        (!nativeCloseButton(tab, bar, QTabBar::LeftSide) || hasClose(tab, bar)))
        left += buttonLength(tab.leftButtonSize, bar) + gap;
    if (!tab.icon.isNull())
        left += bar->iconSize().width() + padding + gap / 2;
    if (buttonLength(tab.rightButtonSize, bar) > 0 &&
        (!nativeCloseButton(tab, bar, QTabBar::RightSide) || hasClose(tab, bar)))
        right -= buttonLength(tab.rightButtonSize, bar) + gap;
    QRectF result(left, 0, qMax(0.0, right - left), size.height());
    if (tab.direction == Qt::RightToLeft && !vertical(bar))
        result.moveLeft(size.width() - result.right());
    return result;
}
QColor stateColor(const NTabBar* bar, QStyle::State state, bool foreground, bool icon = false) {
    const bool dark = nTheme->isDarkMode();
#define N_TAB_STATE_COLOR(Name)                                                                                        \
    (icon         ? (dark ? bar->getDarkItemIconForeground##Name() : bar->getLightItemIconForeground##Name())          \
     : foreground ? (dark ? bar->getDarkItemHeaderForeground##Name() : bar->getLightItemHeaderForeground##Name())      \
                  : (dark ? bar->getDarkItemHeaderBackground##Name() : bar->getLightItemHeaderBackground##Name()))
    if (!(state & QStyle::State_Enabled))
        return N_TAB_STATE_COLOR(Disabled);
    if (state & QStyle::State_Selected)
        return N_TAB_STATE_COLOR(Selected);
    if (state & QStyle::State_Sunken)
        return N_TAB_STATE_COLOR(Pressed);
    if (state & QStyle::State_MouseOver)
        return N_TAB_STATE_COLOR(PointerOver);
    return N_TAB_STATE_COLOR();
#undef N_TAB_STATE_COLOR
}
void paintGlyph(QPainter* painter, const QRectF& rect, NRegularIconType::Icon glyph, const QColor& color, int size) {
    QFont font(nIcon->getRegularFontFamily());
    font.setPixelSize(qMax(1, size));
    painter->setFont(font);
    painter->setPen(color);
    painter->drawText(rect, Qt::AlignCenter, nIcon->getIconText(static_cast<uint32_t>(glyph)));
}
void paintFocusVisual(QPainter* painter, const QRectF& rect, qreal radius) {
    const auto theme = nTheme->isDarkMode() ? NThemeType::Dark : NThemeType::Light;
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(NThemeColor(NFluentColorKey::FocusStrokeColorOuter, theme), 2));
    painter->drawRoundedRect(rect.adjusted(1, 1, -1, -1), qMax(0.0, radius - 1), qMax(0.0, radius - 1));
    painter->setPen(QPen(NThemeColor(NFluentColorKey::FocusStrokeColorInner, theme), 1));
    painter->drawRoundedRect(rect.adjusted(2.5, 2.5, -2.5, -2.5), qMax(0.0, radius - 2.5), qMax(0.0, radius - 2.5));
}
QPixmap tabIcon(const QIcon& icon, const QSize& size, qreal dpr, const QColor& color, bool enabled) {
    const QString key = QStringLiteral("QtNativeUI/tab/%1/%2/%3/%4/%5/%6")
                            .arg(icon.cacheKey())
                            .arg(size.width())
                            .arg(size.height())
                            .arg(dpr, 0, 'f', 3)
                            .arg(color.rgba())
                            .arg(enabled);
    QPixmap pixmap;
    if (QPixmapCache::find(key, &pixmap))
        return pixmap;
    pixmap = icon.pixmap(size, dpr, enabled ? QIcon::Normal : QIcon::Disabled);
    // 单色 Fluent 图标遵循 foreground token；多色位图保留业务原色。
    const QImage image      = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QRgb         first      = 0;
    bool         found      = false;
    bool         monochrome = true;
    for (int y = 0; y < image.height() && monochrome; ++y) {
        for (int x = 0; x < image.width(); ++x) {
            const QRgb pixel = image.pixel(x, y);
            if (qAlpha(pixel) < 128)
                continue;
            if (!found) {
                first = pixel;
                found = true;
            } else if (qAbs(qRed(first) - qRed(pixel)) > 12 || qAbs(qGreen(first) - qGreen(pixel)) > 12 ||
                       qAbs(qBlue(first) - qBlue(pixel)) > 12) {
                monochrome = false;
                break;
            }
        }
    }
    if (found && monochrome) {
        QPainter painter(&pixmap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(pixmap.rect(), color);
    }
    QPixmapCache::insert(key, pixmap);
    return pixmap;
}
// 几何依据 WinUI TabView 的选中轮廓独立实现；颜色、圆角和尺寸取本项目 token/API。
// https://github.com/microsoft/microsoft-ui-xaml/tree/main/controls/dev/TabView
QPainterPath tabContour(qreal width, qreal height, qreal topRadius, qreal footRadius) {
    const qreal  left   = 0.5;
    const qreal  right  = qMax(left, width - 0.5);
    const qreal  top    = 0.5;
    const qreal  bottom = qMax(top, height - 0.5);
    const qreal  foot   = qBound(0.0, footRadius, (bottom - top) / 2);
    const qreal  radius = qBound(0.0, topRadius, qMin((right - left) / 2, bottom - top - foot));
    QPainterPath path;
    path.moveTo(left - foot, bottom);
    if (foot > 0)
        path.arcTo(QRectF(left - 2 * foot, bottom - 2 * foot, 2 * foot, 2 * foot), 270, 90);
    path.lineTo(left, top + radius);
    if (radius > 0)
        path.arcTo(QRectF(left, top, 2 * radius, 2 * radius), 180, -90);
    path.lineTo(right - radius, top);
    if (radius > 0)
        path.arcTo(QRectF(right - 2 * radius, top, 2 * radius, 2 * radius), 90, -90);
    path.lineTo(right, bottom - foot);
    if (foot > 0)
        path.arcTo(QRectF(right, bottom - 2 * foot, 2 * foot, 2 * foot), 180, 90);
    return path;
}
} // namespace

void nPaintTabToolButton(QPainter* painter, const QStyleOptionToolButton& option, const QWidget*) {
    const bool dark    = nTheme->isDarkMode();
    const auto theme   = dark ? NThemeType::Dark : NThemeType::Light;
    const bool enabled = option.state & QStyle::State_Enabled;
    const bool pressed = option.state & QStyle::State_Sunken;
    const bool hover   = option.state & QStyle::State_MouseOver;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    const QColor background = NThemeColor(
        pressed ? NFluentColorKey::SubtleFillColorTertiary : NFluentColorKey::SubtleFillColorSecondary, theme);
    if (enabled && (hover || pressed)) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(background);
        const int radius = NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt();
        painter->drawRoundedRect(QRectF(option.rect).adjusted(1, 1, -1, -1), radius, radius);
    }
    const QColor foreground =
        NThemeColor(enabled ? NFluentColorKey::TextFillColorSecondary : NFluentColorKey::TextFillColorDisabled, theme);
    const int glyphSize = NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt();
    if (option.arrowType != Qt::NoArrow) {
        auto glyph = NRegularIconType::ChevronLeft12Regular;
        if (option.arrowType == Qt::RightArrow)
            glyph = NRegularIconType::ChevronRight12Regular;
        else if (option.arrowType == Qt::UpArrow)
            glyph = NRegularIconType::ChevronUp12Regular;
        else if (option.arrowType == Qt::DownArrow)
            glyph = NRegularIconType::ChevronDown12Regular;
        paintGlyph(painter, option.rect, glyph, foreground, glyphSize);
    } else {
        const QSize size = option.iconSize.boundedTo(option.rect.size());
        const QRect iconRect(option.rect.center() - QPoint(size.width() / 2, size.height() / 2), size);
        option.icon.paint(painter, iconRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
    }
    if (nShouldPaintFocusVisual(option.state)) {
        const int radius = qMax(0, NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt() - 1);
        paintFocusVisual(painter, option.rect, radius);
    }
    painter->restore();
}

NTabBarStyle::NTabBarStyle(QStyle* style) : QProxyStyle(style) {}
NTabBarStyle::~NTabBarStyle() = default;

void NTabBarStyle::drawPrimitive(PrimitiveElement    element,
                                 const QStyleOption* option,
                                 QPainter*           painter,
                                 const QWidget*      widget) const {
    const auto* bar = owningBar(widget);
    if (!bar) {
        QProxyStyle::drawPrimitive(element, option, painter, widget);
        return;
    }
    if (element == PE_FrameTabBarBase) {
        painter->save();
        painter->setPen(nTheme->isDarkMode() ? bar->getDarkItemHeaderBorderColorSelected()
                                             : bar->getLightItemHeaderBorderColorSelected());
        const QRect selected = bar->currentIndex() >= 0 ? bar->tabRect(bar->currentIndex()) : QRect();
        const int   radius   = qMax(0, NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt());
        if (!vertical(bar)) {
            const qreal y = (bar->shape() == QTabBar::RoundedSouth || bar->shape() == QTabBar::TriangularSouth)
                                ? 0.5
                                : bar->height() - 0.5;
            if (selected.isValid()) {
                painter->drawLine(QPointF(0, y), QPointF(qMax(0, selected.left() - radius), y));
                painter->drawLine(QPointF(selected.right() + radius + 1, y), QPointF(bar->width(), y));
            } else {
                painter->drawLine(QPointF(0, y), QPointF(bar->width(), y));
            }
        }
        painter->restore();
        return;
    }
    if (element == PE_IndicatorTabClose) {
        QStyleOptionToolButton button;
        button.rect  = option->rect;
        button.state = option->state;
        painter->save();
        button.arrowType = Qt::NoArrow;
        nPaintTabToolButton(painter, button, widget);
        const auto theme = nTheme->isDarkMode() ? NThemeType::Dark : NThemeType::Light;
        paintGlyph(painter,
                   option->rect,
                   NRegularIconType::Dismiss12Regular,
                   NThemeColor(option->state & State_Enabled ? NFluentColorKey::TextFillColorSecondary
                                                             : NFluentColorKey::TextFillColorDisabled,
                               theme),
                   NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt());
        painter->restore();
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NTabBarStyle::drawControl(ControlElement      element,
                               const QStyleOption* option,
                               QPainter*           painter,
                               const QWidget*      widget) const {
    const auto* bar = qobject_cast<const NTabBar*>(widget);
    const auto* tab = qstyleoption_cast<const QStyleOptionTab*>(option);
    if (!bar || !tab || (element != CE_TabBarTabShape && element != CE_TabBarTabLabel)) {
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
    const QSizeF size     = canonicalSize(*tab, bar);
    const bool   selected = tab->state & State_Selected;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setWorldTransform(tabTransform(*tab, bar, element == CE_TabBarTabShape), true);
    if (element == CE_TabBarTabShape) {
        const QColor fill   = stateColor(bar, tab->state, false);
        const qreal  radius = qMax(0, bar->getBorderRadius());
        if (selected) {
            const qreal  footRadius = qMax(0, NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt());
            QPainterPath border     = tabContour(size.width(), size.height(), radius, footRadius);
            QPainterPath background(border);
            // 描边保持半像素对齐，填充延伸到控件边界，避免和内容区各自抗锯齿后透出细缝。
            background.lineTo(border.currentPosition().x(), size.height());
            background.lineTo(border.elementAt(0).x, size.height());
            background.closeSubpath();
            painter->fillPath(background, fill);
            painter->setPen(QPen(nTheme->isDarkMode() ? bar->getDarkItemHeaderBorderColorSelected()
                                                      : bar->getLightItemHeaderBorderColorSelected(),
                                 1));
            painter->drawPath(border); // 选中项底部不描边，与内容表面连通。
        } else {
            // WinUI 的非选中背景只圆化顶部两角，底部与标签栏基线相接。
            QPainterPath background = tabContour(size.width(), size.height(), radius, 0);
            background.closeSubpath();
            painter->fillPath(background, fill);
        }
        const int index   = tab->tabIndex;
        const int hovered = bar->hoverIndex();
        if (!selected && index >= 0 && index + 1 < bar->count() &&
            tab->selectedPosition != QStyleOptionTab::NextIsSelected && index != hovered && index + 1 != hovered) {
            painter->setPen(nTheme->isDarkMode() ? bar->getDarkItemSeparator() : bar->getLightItemSeparator());
            const int inset = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
            painter->drawLine(QPointF(size.width() - 0.5, inset), QPointF(size.width() - 0.5, size.height() - inset));
        }
    } else {
        painter->setFont(bar->font());
        const bool iconOnly = compact(*tab, bar);
        const int  padding  = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
        const int  gap      = NSpacingToken(NDesignTokenKey::SpacingS).toInt();
        qreal      left     = padding;
        if (buttonLength(tab->leftButtonSize, bar) > 0 &&
            (!nativeCloseButton(*tab, bar, QTabBar::LeftSide) || hasClose(*tab, bar)))
            left += buttonLength(tab->leftButtonSize, bar) + gap;
        const QSize iconSize = bar->iconSize();
        QRectF      iconRect(iconOnly ? (size.width() - iconSize.width()) / 2 : left,
                        (size.height() - iconSize.height()) / 2,
                        iconSize.width(),
                        iconSize.height());
        if (tab->direction == Qt::RightToLeft && !vertical(bar))
            iconRect.moveLeft(size.width() - iconRect.right());
        if (!tab->icon.isNull()) {
            const QPixmap pixmap = tabIcon(tab->icon,
                                           iconSize,
                                           widget->devicePixelRatioF(),
                                           stateColor(bar, tab->state, true, true),
                                           tab->state & State_Enabled);
            painter->drawPixmap(iconRect.toRect(), pixmap);
        }
        if (!iconOnly) {
            QRectF       text      = textRect(*tab, bar);
            const QColor textColor = bar->tabTextColor(tab->tabIndex);
            painter->setPen(textColor.isValid() ? textColor : stateColor(bar, tab->state, true));
            const int flags = Qt::AlignVCenter | Qt::AlignLeading | Qt::TextShowMnemonic;
            painter->drawText(text, flags, tab->text);
        }
        if (nShouldPaintFocusVisual(tab->state)) {
            paintFocusVisual(painter, QRectF(0, 0, size.width(), size.height()), qMax(0, bar->getBorderRadius() - 1));
        }
    }
    painter->restore();
}

QSize NTabBarStyle::sizeFromContents(ContentsType        type,
                                     const QStyleOption* option,
                                     const QSize&        size,
                                     const QWidget*      widget) const {
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
QRect NTabBarStyle::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const {
    const auto* bar = qobject_cast<const NTabBar*>(widget);
    const auto* tab = qstyleoption_cast<const QStyleOptionTab*>(option);
    if (bar && tab) {
        if (element == SE_TabBarTabText)
            return tabTransform(*tab, bar, false).mapRect(textRect(*tab, bar)).toAlignedRect();
        if (element == SE_TabBarTabRightButton && nativeCloseButton(*tab, bar, QTabBar::RightSide)) {
            const QSizeF size    = canonicalSize(*tab, bar);
            const int    spacing = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
            QRectF       rect(size.width() - 4 * spacing - spacing / 2.0,
                        (size.height() - 3 * spacing) / 2,
                        4 * spacing,
                        3 * spacing);
            if (bar->layoutDirection() == Qt::RightToLeft)
                rect.moveLeft(spacing / 2.0);
            return tabTransform(*tab, bar, false).mapRect(rect).toAlignedRect();
        }
    }
    return QProxyStyle::subElementRect(element, option, widget);
}
int NTabBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    const int spacing = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
    if (metric == PM_TabCloseIndicatorWidth || metric == PM_TabBarScrollButtonWidth)
        return 4 * spacing;
    if (metric == PM_TabCloseIndicatorHeight)
        return 3 * spacing;
    if (metric == PM_TabBarBaseOverlap)
        return 1;
    if (metric == PM_TabBarTabOverlap)
        return 0;
    return QProxyStyle::pixelMetric(metric, option, widget);
}
int NTabBarStyle::styleHint(StyleHint           hint,
                            const QStyleOption* option,
                            const QWidget*      widget,
                            QStyleHintReturn*   returnData) const {
    if (hint == SH_TabBar_CloseButtonPosition)
        return QTabBar::RightSide;
    if (hint == SH_TabBar_Alignment)
        return Qt::AlignLeft;
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
