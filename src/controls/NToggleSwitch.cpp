#include <QEnterEvent>
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NToggleSwitch.h>
#include "../private/ntoggleswitch_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, int, TrackBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, int, TrackBorderWidth)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightTrackDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkTrackDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightTrackBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkTrackBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightThumbDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkThumbDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightThumbCheckedColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkThumbCheckedColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkTextColor)

NToggleSwitch::NToggleSwitch(QWidget* parent) : QAbstractButton(parent), d_ptr(new NToggleSwitchPrivate()) { init(); }

NToggleSwitch::NToggleSwitch(const QString& text, QWidget* parent) : NToggleSwitch(parent) { setText(text); }

NToggleSwitch::~NToggleSwitch() {}

void NToggleSwitch::init() {
    Q_D(NToggleSwitch);
    d->q_ptr = this;

    d->_pTrackBorderRadius      = d->_trackHeight / 2;
    d->_pTrackBorderWidth       = 1;
    d->_themeMode               = nTheme->themeMode();
    d->_isDark                  = nTheme->isDarkMode();
    d->_pLightTrackDefaultColor = NThemeColor(NFluentColorKey::ControlAltFillColorTransparent, NThemeType::Light);
    d->_pDarkTrackDefaultColor  = NThemeColor(NFluentColorKey::ControlAltFillColorTransparent, NThemeType::Dark);
    d->_pLightTrackBorderColor  = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Light);
    d->_pDarkTrackBorderColor   = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Dark);

    d->_pLightThumbDefaultColor = QColor("#5A5A5A");
    d->_pDarkThumbDefaultColor  = QColor("#CECECE");

    d->_pLightThumbCheckedColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Light);
    d->_pDarkThumbCheckedColor  = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Dark);

    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_thumbCenterX = d->_trackHeight / 2;
    d->_thumbRadius  = d->_thumbSize / 2;

    updateAccentColors();

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setObjectName("NToggleSwitch");
    setCheckable(true);
    setMinimumSize(d->_trackWidth, d->_trackHeight);
    setFixedHeight(d->_trackHeight);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NToggleSwitch);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        updateAccentColors();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        updateAccentColors();
        update();
    });

    connect(this, &QAbstractButton::toggled, this, [this](bool checked) {
        Q_D(NToggleSwitch);
        if (checked) {
            if (d->_thumbPosAnimation->state() != QPropertyAnimation::Running) {
                d->_thumbCenterX = d->_trackWidth - d->_trackHeight / 2;
            }
        } else {
            if (d->_thumbPosAnimation->state() != QPropertyAnimation::Running) {
                d->_thumbCenterX = d->_trackHeight / 2;
            }
        }
        update();
    });
}

void NToggleSwitch::setChecked(bool checked) {
    QAbstractButton::setChecked(checked);
    Q_D(NToggleSwitch);

    if (checked == isChecked()) {
        return;
    }

    int startX = d->_thumbCenterX;
    int endX   = checked ? d->_trackWidth - d->_trackHeight / 2 : d->_trackHeight / 2;
    d->startThumbPosAnimation(startX, endX, checked);
}

bool NToggleSwitch::isChecked() const { return QAbstractButton::isChecked(); }

bool NToggleSwitch::event(QEvent* event) {
    Q_D(NToggleSwitch);

    switch (event->type()) {
        case QEvent::Enter:
            if (isEnabled()) {
                d->_isHovered = true;
                d->startThumbRadiusAnimation(d->_thumbRadius, d->_trackHeight * 0.35);
            }
            break;

        case QEvent::Leave:
            if (isEnabled()) {
                d->_isHovered = false;
                d->startThumbRadiusAnimation(d->_thumbRadius, d->_thumbSize / 2);
            }
            break;

        default:
            break;
    }

    return QAbstractButton::event(event);
}

void NToggleSwitch::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    drawTrack(&painter);

    drawThumb(&painter);

    drawText(&painter);
}

void NToggleSwitch::drawTrack(QPainter* painter) {
    Q_D(NToggleSwitch);
    painter->save();

    QPainterPath path;
    int          margin = d->_trackMargin;

    qreal radius = (d->_trackHeight - 2 * margin) / 2;

    path.moveTo(d->_trackWidth - radius - margin, d->_trackHeight - margin);
    path.arcTo(QRectF(d->_trackWidth - 2 * radius - margin, margin, 2 * radius, 2 * radius), -90, 180);

    path.lineTo(radius + margin, margin);
    path.arcTo(QRectF(margin, margin, 2 * radius, 2 * radius), 90, 180);

    path.closeSubpath();

    QColor trackColor;
    QColor borderColor = d->_isDark ? d->_pDarkTrackBorderColor : d->_pLightTrackBorderColor;

    if (isChecked()) {
        if (!isEnabled()) {
            trackColor  = d->_accentDisabledColor;
            borderColor = Qt::transparent;
        } else {
            trackColor  = d->_accentDefaultColor;
            borderColor = Qt::transparent;
        }
        if (d->_isHovered) {
            trackColor = d->_accentHoverColor;
        } else if (d->_isPressed) {
            trackColor = d->_accentPressedColor;
        }
    } else if (d->_isHovered) {
        trackColor = NThemeColor(NFluentColorKey::ControlAltFillColorTertiary, d->_themeMode);
    } else if (d->_isPressed) {
        trackColor = NThemeColor(NFluentColorKey::ControlAltFillColorQuarternary, d->_themeMode);
    } else {
        if (!isEnabled()) {
            trackColor  = NThemeColor(NFluentColorKey::ControlAltFillColorDisabled, d->_themeMode);
            borderColor = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDisabled, d->_themeMode);
        } else {
            trackColor  = d->_isDark ? d->_pDarkTrackDefaultColor : d->_pLightTrackDefaultColor;
            borderColor = d->_isDark ? d->_pDarkTrackBorderColor : d->_pLightTrackBorderColor;
        }
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(trackColor);
    painter->drawPath(path);

    if (borderColor != Qt::transparent) {
        painter->setPen(QPen(borderColor, d->_pTrackBorderWidth));
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path);
    }

    painter->restore();
}

void NToggleSwitch::drawThumb(QPainter* painter) {
    Q_D(NToggleSwitch);
    painter->save();

    if (d->_thumbRadius <= 0) {
        d->_thumbRadius = d->_thumbSize / 2;
    }

    if (d->_thumbCenterX <= 0) {
        d->_thumbCenterX = isChecked() ? d->_trackWidth - d->_trackHeight / 2 : d->_trackHeight / 2;
    }

    QColor thumbColor;
    if (!isEnabled()) {
        thumbColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
    } else if (isChecked()) {
        thumbColor = d->_isDark ? d->_pDarkThumbCheckedColor : d->_pLightThumbCheckedColor;
    } else {
        thumbColor = d->_isDark ? d->_pDarkThumbDefaultColor : d->_pLightThumbDefaultColor;
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(thumbColor);
    painter->drawEllipse(QPointF(d->_thumbCenterX, d->_trackHeight / 2), d->_thumbRadius, d->_thumbRadius);

    painter->restore();
}

void NToggleSwitch::drawText(QPainter* painter) {
    Q_D(NToggleSwitch);
    if (text().isEmpty())
        return;

    painter->save();

    QRect textRect(d->_trackWidth + d->_spacing, 0, width() - d->_trackWidth - d->_spacing, height());

    QColor textColor;
    if (!isEnabled()) {
        textColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
    } else {
        textColor = d->_isDark ? d->_pDarkTextColor : d->_pLightTextColor;
    }

    painter->setPen(textColor);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());

    painter->restore();
}

void NToggleSwitch::mousePressEvent(QMouseEvent* event) {
    Q_D(NToggleSwitch);

    if (event->button() == Qt::LeftButton) {
        d->_isPressed  = true;
        d->_isDragging = false;
        d->_lastMouseX = event->pos().x();

        d->adjustThumbCenterX();

        d->startThumbRadiusAnimation(d->_thumbRadius, d->_trackHeight * 0.25);
    }
}

void NToggleSwitch::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NToggleSwitch);

    if (event->button() == Qt::LeftButton) {
        d->_isPressed = false;

        if (d->_isDragging) {
            d->_isDragging = false;

            if (d->_thumbCenterX > d->_trackWidth / 2) {
                d->startThumbPosAnimation(d->_thumbCenterX, d->_trackWidth - d->_trackHeight / 2, true);
            } else {
                d->startThumbPosAnimation(d->_thumbCenterX, d->_trackHeight / 2, false);
            }
        } else {
            if (isChecked()) {
                d->startThumbPosAnimation(d->_thumbCenterX, d->_trackHeight / 2, false);
            } else {
                d->startThumbPosAnimation(d->_thumbCenterX, d->_trackWidth - d->_trackHeight / 2, true);
            }
        }

        d->startThumbRadiusAnimation(d->_thumbRadius, d->_trackHeight * 0.35);
    }
}

void NToggleSwitch::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NToggleSwitch);

    if (d->_isPressed) {
        d->_isDragging = true;

        int moveX      = event->pos().x() - d->_lastMouseX;
        d->_lastMouseX = event->pos().x();

        d->_thumbCenterX += moveX;
        d->adjustThumbCenterX();
        update();
    }

    QAbstractButton::mouseMoveEvent(event);
}

QSize NToggleSwitch::sizeHint() const {
    Q_D(const NToggleSwitch);
    QFontMetrics fm(font());
    int          textWidth = text().isEmpty() ? 0 : fm.horizontalAdvance(text()) + d->_spacing;
    int          width     = d->_trackWidth + textWidth;
    int          height    = qMax(d->_trackHeight, fm.height());
    return QSize(width, height);
}

QSize NToggleSwitch::minimumSizeHint() const {
    Q_D(const NToggleSwitch);
    return QSize(d->_trackWidth, d->_trackHeight);
}

void NToggleSwitch::updateAccentColors() {
    Q_D(NToggleSwitch);
    NAccentColor accentColor = nTheme->accentColor();

    d->_accentDefaultColor  = accentColor.normal();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_accentTextColor         = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, d->_themeMode);
    d->_accentDisabledTextColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorDisabled, d->_themeMode);
    d->_accentHoverColor        = accentColor.light();
    d->_accentPressedColor      = accentColor.dark();
}