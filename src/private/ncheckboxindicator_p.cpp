#include "ncheckboxindicator_p.h"

#include <QPainter>
#include <QPainterPath>

#include "QtNativeUI/NColor.h"
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
        bg     = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, themeMode);
        border = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, themeMode);
    } else {
        bg     = NThemeColor(NFluentColorKey::ControlFillColorDefault, themeMode);
        border = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, themeMode);
    }
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
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
    if (state == Qt::Checked) {
        const QIcon icon = nIcon->fromRegular(NRegularIconType::Checkmark16Regular);
        icon.paint(painter, inner.adjusted(2, 2, -2, -2), Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
    } else if (state == Qt::PartiallyChecked) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(enabled ? nTheme->accentColor().normal()
                                  : NThemeColor(NFluentColorKey::TextFillColorDisabled, themeMode));
        const QRect mark(inner.center().x() - 4, inner.center().y() - 1, 8, 2);
        painter->drawRoundedRect(mark, 1, 1);
    }
    painter->restore();
}

} // namespace NCheckBoxIndicator
