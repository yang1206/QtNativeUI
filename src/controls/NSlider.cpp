//
// Created by Yang1206 on 2025/4/10.
//

#include <QEnterEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QtNativeUI/NSlider.h>
#include "../private/nslider_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, LightTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, DarkTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, LightProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, DarkProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, LightDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, DarkDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, LightDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, DarkDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, LightThumbOuterColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, DarkThumbOuterColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, int, TrackHeight)
Q_PROPERTY_CREATE_Q_CPP(NSlider, int, ThumbDiameter)
Q_PROPERTY_CREATE_Q_CPP(NSlider, int, ThumbInnerDiameter)
Q_PROPERTY_CREATE_Q_CPP(NSlider, int, TrackCornerRadius)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, LightTickColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, DarkTickColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, LightDisabledTickColor)
Q_PROPERTY_CREATE_Q_CPP(NSlider, QColor, DarkDisabledTickColor)
// TODO 重构 tick刻度，保持和 QSlider 一致
Q_PROPERTY_CREATE_Q_CPP(NSlider, int, TickLength)
Q_PROPERTY_CREATE_Q_CPP(NSlider, int, TickThickness)

NSlider::NSlider(QWidget* parent) : QSlider(parent), d_ptr(new NSliderPrivate()) {
    Q_D(NSlider);
    d->q_ptr = this;
    init();
}

NSlider::NSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent), d_ptr(new NSliderPrivate()) {
    Q_D(NSlider);
    d->q_ptr = this;
    init();
}

NSlider::~NSlider() {}

void NSlider::init() {
    Q_D(NSlider);

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pTrackHeight        = 5;
    d->_pThumbDiameter      = 20;
    d->_pThumbInnerDiameter = 10;
    d->_pTrackCornerRadius  = 2;

    d->_pLightTrackColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    d->_pDarkTrackColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);

    d->_pLightDisabledTrackColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledTrackColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Dark);

    d->_pLightThumbOuterColor = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Light);
    d->_pDarkThumbOuterColor  = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Dark);

    d->_pTickLength    = 5; // 刻度线长度
    d->_pTickThickness = 1; // 刻度线厚度

    d->_pLightTickColor         = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    d->_pDarkTickColor          = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);
    d->_pLightDisabledTickColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledTickColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Dark);

    updateAccentColors();
    d->_sliderStyle = new NSliderPrivate::Style(d, style());
    setStyle(d->_sliderStyle);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);

    d->_showTooltip = true;

    connect(this, &QSlider::sliderPressed, this, [this]() {
        Q_D(NSlider);
        if (d->_showTooltip) {
            d->updateTooltip();
        }
    });

    connect(this, &QSlider::sliderReleased, this, [this]() {
        Q_D(NSlider);
        if (d->_showTooltip) {
            d->hideTooltip();
        }
    });

    connect(this, &QSlider::valueChanged, this, [this](int) {
        Q_D(NSlider);
        if (d->_isDragging && d->_showTooltip) {
            d->updateTooltip();
        }
    });

    if (orientation() == Qt::Horizontal) {
        setFixedHeight(d->_pThumbDiameter);
    } else {
        setFixedWidth(d->_pThumbDiameter);
    }

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NSlider);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        updateAccentColors();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        updateAccentColors();
        update();
    });
}

void NSlider::updateAccentColors() {
    Q_D(NSlider);
    NAccentColor accentColor = nTheme->accentColor();

    d->_accentColor         = accentColor.normal();
    d->_accentHoverColor    = accentColor.light();
    d->_accentPressedColor  = accentColor.dark();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_pLightProgressColor         = d->_accentColor;
    d->_pDarkProgressColor          = d->_accentColor;
    d->_pLightDisabledProgressColor = d->_accentDisabledColor;
    d->_pDarkDisabledProgressColor  = d->_accentDisabledColor;
}

void NSlider::setAccentColor(const QColor& color) { setAccentColor(NColorUtils::toAccentColor(color)); }

void NSlider::setAccentColor(const NAccentColor& color) {
    Q_D(NSlider);

    d->_accentColor         = color.normal();
    d->_accentHoverColor    = color.light();
    d->_accentPressedColor  = color.dark();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_pLightProgressColor         = d->_accentColor;
    d->_pDarkProgressColor          = d->_accentColor;
    d->_pLightDisabledProgressColor = d->_accentDisabledColor;
    d->_pDarkDisabledProgressColor  = d->_accentDisabledColor;

    update();
}

void NSlider::resetAccentColor() {
    updateAccentColors();
    update();
}

void NSlider::enterEvent(QEnterEvent* event) {
    Q_D(NSlider);

    if (!isEnabled()) {
        QSlider::enterEvent(event);
        return;
    }

    d->_isHovered = true;
    d->startThumbAnimation(d->_thumbScale, 1.3);
    update();
    QSlider::enterEvent(event);
}

void NSlider::leaveEvent(QEvent* event) {
    Q_D(NSlider);

    if (!isEnabled()) {
        QSlider::leaveEvent(event);
        return;
    }

    d->_isHovered = false;
    d->startThumbAnimation(d->_thumbScale, 1.0);
    update();
    QSlider::leaveEvent(event);
}

void NSlider::mousePressEvent(QMouseEvent* event) {
    Q_D(NSlider);

    if (isEnabled()) {
        d->_isPressed  = true;
        d->_isDragging = true;
        d->startThumbAnimation(d->_thumbScale, 0.8);

        if (d->_showTooltip) {
            d->updateTooltip();
        }

        update();
    }

    QSlider::mousePressEvent(event);
}

void NSlider::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NSlider);

    if (isEnabled()) {
        d->_isPressed  = false;
        d->_isDragging = false;

        if (d->_isHovered) {
            d->startThumbAnimation(d->_thumbScale, 1.3);
        } else {
            d->startThumbAnimation(d->_thumbScale, 1.0);
        }

        if (d->_showTooltip) {
            d->hideTooltip();
        }

        update();
    }

    QSlider::mouseReleaseEvent(event);
}

bool NSlider::showTooltip() const {
    Q_D(const NSlider);
    return d->_showTooltip;
}

void NSlider::setShowTooltip(bool show) {
    Q_D(NSlider);
    if (d->_showTooltip == show)
        return;

    d->_showTooltip = show;

    if (!show) {
        d->hideTooltip();
    }
}

void NSlider::setTooltipFormatter(std::function<QString(int)> formatter) {
    Q_D(NSlider);
    d->_tooltipFormatter = formatter;
}

void NSlider::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NSlider);

    if (isEnabled() && d->_isDragging && d->_showTooltip) {
        QTimer::singleShot(0, this, [this]() {
            Q_D(NSlider);
            if (d->_isDragging && d->_showTooltip) {
                d->updateTooltip();
            }
        });
    }

    if (isEnabled() && !d->_isDragging) {
        d->_isHovered = rect().contains(event->pos());
        update();
    }

    QSlider::mouseMoveEvent(event);
}
