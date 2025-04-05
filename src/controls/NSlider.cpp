//
// Created by Yang1206 on 2025/4/10.
//

#include <QEnterEvent>
#include <QPainter>
#include <QPainterPath>
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

    // 设置主题和颜色
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    // 设置尺寸属性
    d->_pTrackHeight        = 4;
    d->_pThumbDiameter      = 20;
    d->_pThumbInnerDiameter = 10;
    d->_pTrackCornerRadius  = 2;

    // 设置默认颜色
    d->_pLightTrackColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    d->_pDarkTrackColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);

    d->_pLightDisabledTrackColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledTrackColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Dark);

    d->_pLightThumbOuterColor = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Light);
    d->_pDarkThumbOuterColor  = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Dark);

    // 初始化刻度相关属性
    d->_pTickLength    = 5; // 刻度线长度
    d->_pTickThickness = 1; // 刻度线厚度

    d->_pLightTickColor         = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    d->_pDarkTickColor          = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);
    d->_pLightDisabledTickColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledTickColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Dark);

    updateAccentColors();

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);

    setStyleSheet("QSlider::groove:horizontal, QSlider::groove:vertical, "
                  "QSlider::handle:horizontal, QSlider::handle:vertical "
                  "{ background: transparent; border: none; }");

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

    // 连接强调色变化信号
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

void NSlider::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawTrack(&painter);
    drawProgress(&painter);

    // 如果设置了刻度位置，绘制刻度
    Q_D(NSlider);
    if (tickPosition() != QSlider::NoTicks) {
        drawTicks(&painter);
    }

    drawThumb(&painter);
}

void NSlider::drawTrack(QPainter* painter) {
    Q_D(NSlider);

    painter->save();

    QColor trackColor;
    if (!isEnabled()) {
        trackColor = d->_isDark ? d->_pDarkDisabledTrackColor : d->_pLightDisabledTrackColor;
    } else {
        trackColor = d->_isDark ? d->_pDarkTrackColor : d->_pLightTrackColor;
    }

    int padding = d->_pThumbDiameter / 2;

    if (orientation() == Qt::Horizontal) {
        int   trackY = (height() - d->_pTrackHeight) / 2;
        QRect trackRect(padding, trackY, width() - 2 * padding, d->_pTrackHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(trackColor);
        painter->drawRoundedRect(trackRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    } else {
        int   trackX = (width() - d->_pTrackHeight) / 2;
        QRect trackRect(trackX, padding, d->_pTrackHeight, height() - 2 * padding);

        painter->setPen(Qt::NoPen);
        painter->setBrush(trackColor);
        painter->drawRoundedRect(trackRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    }

    painter->restore();
}

void NSlider::drawProgress(QPainter* painter) {
    Q_D(NSlider);

    painter->save();

    QColor progressColor;
    if (!isEnabled()) {
        progressColor = d->_isDark ? d->_pDarkDisabledProgressColor : d->_pLightDisabledProgressColor;
    } else if (d->_isPressed) {
        progressColor = d->_accentPressedColor;
    } else if (d->_isHovered) {
        progressColor = d->_accentHoverColor;
    } else {
        progressColor = d->_isDark ? d->_pDarkProgressColor : d->_pLightProgressColor;
    }

    int    padding = d->_pThumbDiameter / 2;
    double ratio   = (value() - minimum()) / static_cast<double>(maximum() - minimum());

    if (orientation() == Qt::Horizontal) {
        int   trackY        = (height() - d->_pTrackHeight) / 2;
        int   progressWidth = static_cast<int>((width() - 2 * padding) * ratio);
        QRect progressRect(padding, trackY, progressWidth, d->_pTrackHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(progressColor);
        painter->drawRoundedRect(progressRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    } else {
        int   trackX         = (width() - d->_pTrackHeight) / 2;
        int   progressHeight = static_cast<int>((height() - 2 * padding) * ratio);
        int   progressY      = height() - padding - progressHeight;
        QRect progressRect(trackX, progressY, d->_pTrackHeight, progressHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(progressColor);
        painter->drawRoundedRect(progressRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    }

    painter->restore();
}

void NSlider::drawThumb(QPainter* painter) {
    Q_D(NSlider);

    painter->save();

    double ratio = (value() - minimum()) / static_cast<double>(maximum() - minimum());

    QPointF thumbCenter;

    if (orientation() == Qt::Horizontal) {
        int thumbX  = d->_pThumbDiameter / 2 + static_cast<int>((width() - d->_pThumbDiameter) * ratio);
        int thumbY  = height() / 2;
        thumbCenter = QPointF(thumbX, thumbY);
    } else {
        int thumbX  = width() / 2;
        int thumbY  = height() - d->_pThumbDiameter / 2 - static_cast<int>((height() - d->_pThumbDiameter) * ratio);
        thumbCenter = QPointF(thumbX, thumbY);
    }

    QColor outerColor = d->_isDark ? d->_pDarkThumbOuterColor : d->_pLightThumbOuterColor;
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerColor);

    painter->drawEllipse(thumbCenter, d->_pThumbDiameter / 2.0, d->_pThumbDiameter / 2.0);

    QColor innerColor;
    if (!isEnabled()) {
        innerColor = d->_accentDisabledColor;
    } else if (d->_isPressed) {
        innerColor = d->_accentPressedColor;
    } else if (d->_isHovered) {
        innerColor = d->_accentHoverColor;
    } else {
        innerColor = d->_accentColor;
    }

    qreal innerRadius = (d->_pThumbInnerDiameter / 2.0) * d->_thumbScale;

    painter->setBrush(innerColor);
    painter->drawEllipse(thumbCenter, innerRadius, innerRadius);

    painter->restore();
}

void NSlider::drawTicks(QPainter* painter) {
    Q_D(NSlider);

    painter->save();

    QColor tickColor;
    if (!isEnabled()) {
        tickColor = d->_isDark ? d->_pDarkDisabledTickColor : d->_pLightDisabledTickColor;
    } else {
        tickColor = d->_isDark ? d->_pDarkTickColor : d->_pLightTickColor;
    }

    painter->setPen(QPen(tickColor, d->_pTickThickness));

    int padding    = d->_pThumbDiameter / 2;
    int valueRange = maximum() - minimum();

    int interval = tickInterval();
    if (interval <= 0) {
        interval = qMax(1, valueRange / 10);
    }

    bool drawAbove = (tickPosition() & QSlider::TicksAbove);
    bool drawBelow = (tickPosition() & QSlider::TicksBelow);

    if (orientation() == Qt::Horizontal) {
        int trackY     = (height() - d->_pTrackHeight) / 2;
        int tickTop    = trackY - d->_pTickLength - 1;
        int tickBottom = trackY + d->_pTrackHeight + 1;

        for (int value = minimum(); value <= maximum(); value += interval) {
            double ratio = (value - minimum()) / static_cast<double>(valueRange);
            int    tickX = padding + static_cast<int>((width() - 2 * padding) * ratio);

            if (drawAbove) {
                painter->drawLine(tickX, tickTop, tickX, tickTop + d->_pTickLength);
            }
            if (drawBelow) {
                painter->drawLine(tickX, tickBottom, tickX, tickBottom + d->_pTickLength);
            }
        }
    } else {
        int trackX    = (width() - d->_pTrackHeight) / 2;
        int tickLeft  = trackX - d->_pTickLength - 1;
        int tickRight = trackX + d->_pTrackHeight + 1;

        for (int value = minimum(); value <= maximum(); value += interval) {
            double ratio = (value - minimum()) / static_cast<double>(valueRange);
            int    tickY = height() - padding - static_cast<int>((height() - 2 * padding) * ratio);

            if (drawAbove) {
                painter->drawLine(tickLeft, tickY, tickLeft + d->_pTickLength, tickY);
            }
            if (drawBelow) {
                painter->drawLine(tickRight, tickY, tickRight + d->_pTickLength, tickY);
            }
        }
    }

    painter->restore();
}

bool NSlider::event(QEvent* event) {
    if (!isEnabled()) {
        return QSlider::event(event);
    }

    return QSlider::event(event);
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

    if (!isEnabled()) {
        QSlider::mousePressEvent(event);
        return;
    }

    d->_isPressed  = true;
    d->_isDragging = true;
    d->startThumbAnimation(d->_thumbScale, 0.8);
    update();
    QSlider::mousePressEvent(event);
}

void NSlider::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NSlider);

    if (!isEnabled()) {
        QSlider::mouseReleaseEvent(event);
        return;
    }

    d->_isPressed  = false;
    d->_isDragging = false;

    if (d->_isHovered) {
        d->startThumbAnimation(d->_thumbScale, 1.3);
    } else {
        d->startThumbAnimation(d->_thumbScale, 1.0);
    }

    update();
    QSlider::mouseReleaseEvent(event);
}

void NSlider::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NSlider);

    if (!isEnabled()) {
        QSlider::mouseMoveEvent(event);
        return;
    }

    if (!d->_isDragging) {
        d->_isHovered = rect().contains(event->pos());
    }

    update();
    QSlider::mouseMoveEvent(event);
}