#include "ncheckboxindicator_p.h"

#include <QPainter>
#include <QPixmap>

#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

namespace NCheckBoxIndicator {

void paint(QPainter* painter, const QRect& rect, Qt::CheckState state, bool enabled, bool hovered, bool isDark,
           int borderRadius) {
    if (!painter || rect.isEmpty())
        return;
    const NThemeType::ThemeMode themeMode = isDark ? NThemeType::ThemeMode::Dark : NThemeType::ThemeMode::Light;
    const bool                  checked   = state == Qt::Checked || state == Qt::PartiallyChecked;
    QRect                       box       = rect;
    if (box.width() != box.height()) {
        const int side = qMin(box.width(), box.height());
        box = QRect(box.center().x() - side / 2, box.center().y() - side / 2, side, side);
    }
    const QRect inner = box.adjusted(1, 1, -1, -1);
    QColor      bg;
    QColor      border;
    if (checked && enabled) {
        const NAccentColor accent = nTheme->accentColor();
        bg                        = hovered ? accent.light() : accent.normal();
        border                    = Qt::transparent;
    } else if (checked && !enabled) {
        bg     = NThemeColor(NFluentColorKey::AccentFillColorDisabled, themeMode);
        border = Qt::transparent;
    } else if (!enabled) {
        bg     = NThemeColor(NFluentColorKey::ControlFillColorDisabled, themeMode);
        border = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDisabled, themeMode);
    } else if (hovered) {
        bg     = NThemeColor(NFluentColorKey::ControlFillColorSecondary, themeMode);
        border = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, themeMode);
    } else {
        bg     = NThemeColor(NFluentColorKey::ControlFillColorDefault, themeMode);
        border = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, themeMode);
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter->setPen(Qt::NoPen);
    painter->setBrush(bg);
    painter->drawRoundedRect(inner, borderRadius, borderRadius);
    if (!checked && border.alpha() > 0) {
        QPen pen(border);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(inner, borderRadius, borderRadius);
    }
    if (state == Qt::Checked || state == Qt::PartiallyChecked) {
        const QColor iconColor =
            enabled ? NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, themeMode)
                    : NThemeColor(NFluentColorKey::TextOnAccentFillColorDisabled, themeMode);
        if (state == Qt::PartiallyChecked) {
            painter->setPen(QPen(iconColor, 2));
            const int lineWidth = inner.width() * 0.6;
            const int y         = inner.center().y();
            painter->drawLine(inner.center().x() - lineWidth / 2, y, inner.center().x() + lineWidth / 2, y);
        } else {
            const int iconSize = qRound(box.width() * 0.75);
            const QIcon icon =
                nIcon->fromFilled(NFilledIconType::Checkmark24Filled, iconSize, iconColor);
            const QRect iconRect(inner.x() + (inner.width() - iconSize) / 2,
                                 inner.y() + (inner.height() - iconSize) / 2,
                                 iconSize,
                                 iconSize);
            const qreal dpr = painter->device() ? painter->device()->devicePixelRatioF() : 1.0;
            QPixmap     pixmap = icon.pixmap(QSize(iconSize, iconSize) * dpr);
            pixmap.setDevicePixelRatio(dpr);
            painter->drawPixmap(iconRect, pixmap);
        }
    }
    painter->restore();
}

} // namespace NCheckBoxIndicator
