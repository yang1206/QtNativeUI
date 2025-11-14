#include <QEnterEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QtNativeUI/NRangeSlider.h>
#include "../private/nrangeslider_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, LightTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, DarkTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, LightProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, DarkProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, LightDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, DarkDisabledTrackColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, LightDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, DarkDisabledProgressColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, LightThumbOuterColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, QColor, DarkThumbOuterColor)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, TrackHeight)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, ThumbDiameter)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, ThumbInnerDiameter)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, TrackCornerRadius)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, Minimum)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, Maximum)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, LowerValue)
Q_PROPERTY_CREATE_Q_CPP(NRangeSlider, int, UpperValue)

NRangeSlider::NRangeSlider(QWidget* parent) : QWidget(parent), d_ptr(new NRangeSliderPrivate()) {
    Q_D(NRangeSlider);
    d->q_ptr = this;
    init();
}

NRangeSlider::NRangeSlider(Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent), d_ptr(new NRangeSliderPrivate()) {
    Q_D(NRangeSlider);
    d->q_ptr        = this;
    d->_orientation = orientation;
    init();
}

NRangeSlider::~NRangeSlider() {}

void NRangeSlider::init() {
    Q_D(NRangeSlider);

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pTrackHeight        = 5;
    d->_pThumbDiameter      = 20;
    d->_pThumbInnerDiameter = 10;
    d->_pTrackCornerRadius  = 2;

    d->_pMinimum    = 0;
    d->_pMaximum    = 100;
    d->_pLowerValue = 25;
    d->_pUpperValue = 75;

    d->_pLightTrackColor         = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    d->_pDarkTrackColor          = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);
    d->_pLightDisabledTrackColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledTrackColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDisabled, NThemeType::Dark);
    d->_pLightThumbOuterColor    = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Light);
    d->_pDarkThumbOuterColor     = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Dark);

    updateAccentColors();

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);

    if (d->_orientation == Qt::Horizontal) {
        setFixedHeight(d->_pThumbDiameter + 8);
        setMinimumWidth(d->_pThumbDiameter * 2);
    } else {
        setFixedWidth(d->_pThumbDiameter + 8);
        setMinimumHeight(d->_pThumbDiameter * 2);
    }

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NRangeSlider);
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

void NRangeSlider::updateAccentColors() {
    Q_D(NRangeSlider);
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

void NRangeSlider::setAccentColor(const QColor& color) { setAccentColor(NColorUtils::toAccentColor(color)); }

void NRangeSlider::setAccentColor(const NAccentColor& color) {
    Q_D(NRangeSlider);

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

void NRangeSlider::resetAccentColor() {
    updateAccentColors();
    update();
}

void NRangeSlider::setRange(int min, int max) {
    Q_D(NRangeSlider);
    if (min >= max)
        return;

    d->_pMinimum = min;
    d->_pMaximum = max;

    if (d->_pLowerValue < min)
        d->_pLowerValue = min;
    if (d->_pUpperValue > max)
        d->_pUpperValue = max;
    if (d->_pLowerValue > d->_pUpperValue)
        d->_pLowerValue = d->_pUpperValue;

    emit pMinimumChanged();
    emit pMaximumChanged();
    update();
}

void NRangeSlider::setValues(int lower, int upper) {
    Q_D(NRangeSlider);

    lower = qBound(d->_pMinimum, lower, d->_pMaximum);
    upper = qBound(d->_pMinimum, upper, d->_pMaximum);

    if (lower > upper) {
        qSwap(lower, upper);
    }

    bool lowerChanged = (d->_pLowerValue != lower);
    bool upperChanged = (d->_pUpperValue != upper);

    d->_pLowerValue = lower;
    d->_pUpperValue = upper;

    if (lowerChanged) {
        emit pLowerValueChanged();
        emit lowerValueChanged(lower);
    }
    if (upperChanged) {
        emit pUpperValueChanged();
        emit upperValueChanged(upper);
    }
    if (lowerChanged || upperChanged) {
        emit rangeChanged(lower, upper);
    }

    update();
}

QPair<int, int> NRangeSlider::values() const {
    Q_D(const NRangeSlider);
    return QPair<int, int>(d->_pLowerValue, d->_pUpperValue);
}

Qt::Orientation NRangeSlider::orientation() const {
    Q_D(const NRangeSlider);
    return d->_orientation;
}

void NRangeSlider::setOrientation(Qt::Orientation orientation) {
    Q_D(NRangeSlider);
    if (d->_orientation == orientation)
        return;

    d->_orientation = orientation;

    if (orientation == Qt::Horizontal) {
        setFixedHeight(d->_pThumbDiameter + 8);
        setMinimumWidth(d->_pThumbDiameter * 2);
        setMaximumHeight(d->_pThumbDiameter + 8);
        setMaximumWidth(QWIDGETSIZE_MAX);
    } else {
        setFixedWidth(d->_pThumbDiameter + 8);
        setMinimumHeight(d->_pThumbDiameter * 2);
        setMaximumWidth(d->_pThumbDiameter + 8);
        setMaximumHeight(QWIDGETSIZE_MAX);
    }

    updateGeometry();
    update();
}

QSize NRangeSlider::sizeHint() const {
    Q_D(const NRangeSlider);
    if (d->_orientation == Qt::Horizontal) {
        return QSize(200, d->_pThumbDiameter + 8);
    } else {
        return QSize(d->_pThumbDiameter + 8, 200);
    }
}

QSize NRangeSlider::minimumSizeHint() const {
    Q_D(const NRangeSlider);
    if (d->_orientation == Qt::Horizontal) {
        return QSize(d->_pThumbDiameter * 2, d->_pThumbDiameter + 8);
    } else {
        return QSize(d->_pThumbDiameter + 8, d->_pThumbDiameter * 2);
    }
}

int NRangeSlider::valueFromPosition(const QPoint& pos) const {
    Q_D(const NRangeSlider);

    int padding    = d->_pThumbDiameter / 2;
    int valueRange = d->_pMaximum - d->_pMinimum;

    if (d->_orientation == Qt::Horizontal) {
        int    availableWidth = width() - 2 * padding;
        int    position       = pos.x() - padding;
        double ratio          = qBound(0.0, static_cast<double>(position) / availableWidth, 1.0);
        return d->_pMinimum + static_cast<int>(ratio * valueRange);
    } else {
        int    availableHeight = height() - 2 * padding;
        int    position        = height() - pos.y() - padding;
        double ratio           = qBound(0.0, static_cast<double>(position) / availableHeight, 1.0);
        return d->_pMinimum + static_cast<int>(ratio * valueRange);
    }
}

QRect NRangeSlider::lowerHandleRect() const {
    Q_D(const NRangeSlider);

    int    padding    = d->_pThumbDiameter / 2;
    int    valueRange = d->_pMaximum - d->_pMinimum;
    double ratio      = (d->_pLowerValue - d->_pMinimum) / static_cast<double>(valueRange);

    if (d->_orientation == Qt::Horizontal) {
        int availableWidth = width() - 2 * padding;
        int handleX        = padding + static_cast<int>(availableWidth * ratio) - d->_pThumbDiameter / 2;
        int handleY        = (height() - d->_pThumbDiameter) / 2;
        return QRect(handleX, handleY, d->_pThumbDiameter, d->_pThumbDiameter);
    } else {
        int availableHeight = height() - 2 * padding;
        int handleY         = height() - padding - static_cast<int>(availableHeight * ratio) - d->_pThumbDiameter / 2;
        int handleX         = (width() - d->_pThumbDiameter) / 2;
        return QRect(handleX, handleY, d->_pThumbDiameter, d->_pThumbDiameter);
    }
}

QRect NRangeSlider::upperHandleRect() const {
    Q_D(const NRangeSlider);

    int    padding    = d->_pThumbDiameter / 2;
    int    valueRange = d->_pMaximum - d->_pMinimum;
    double ratio      = (d->_pUpperValue - d->_pMinimum) / static_cast<double>(valueRange);

    if (d->_orientation == Qt::Horizontal) {
        int availableWidth = width() - 2 * padding;
        int handleX        = padding + static_cast<int>(availableWidth * ratio) - d->_pThumbDiameter / 2;
        int handleY        = (height() - d->_pThumbDiameter) / 2;
        return QRect(handleX, handleY, d->_pThumbDiameter, d->_pThumbDiameter);
    } else {
        int availableHeight = height() - 2 * padding;
        int handleY         = height() - padding - static_cast<int>(availableHeight * ratio) - d->_pThumbDiameter / 2;
        int handleX         = (width() - d->_pThumbDiameter) / 2;
        return QRect(handleX, handleY, d->_pThumbDiameter, d->_pThumbDiameter);
    }
}

QRect NRangeSlider::trackRect() const {
    Q_D(const NRangeSlider);

    int padding = d->_pThumbDiameter / 2;

    if (d->_orientation == Qt::Horizontal) {
        int trackY = (height() - d->_pTrackHeight) / 2;
        return QRect(padding, trackY, width() - 2 * padding, d->_pTrackHeight);
    } else {
        int trackX = (width() - d->_pTrackHeight) / 2;
        return QRect(trackX, padding, d->_pTrackHeight, height() - 2 * padding);
    }
}

QRect NRangeSlider::progressRect() const {
    Q_D(const NRangeSlider);

    QRect lowerRect = lowerHandleRect();
    QRect upperRect = upperHandleRect();
    QRect track     = trackRect();

    if (d->_orientation == Qt::Horizontal) {
        int startX = lowerRect.center().x();
        int endX   = upperRect.center().x();
        return QRect(startX, track.y(), endX - startX, track.height());
    } else {
        int startY = upperRect.center().y();
        int endY   = lowerRect.center().y();
        return QRect(track.x(), startY, track.width(), endY - startY);
    }
}

void NRangeSlider::drawTrack(QPainter* painter) {
    Q_D(NRangeSlider);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor trackColor;
    if (!isEnabled()) {
        trackColor = d->_isDark ? d->_pDarkDisabledTrackColor : d->_pLightDisabledTrackColor;
    } else {
        trackColor = d->_isDark ? d->_pDarkTrackColor : d->_pLightTrackColor;
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(trackColor);
    painter->drawRoundedRect(trackRect(), d->_pTrackCornerRadius, d->_pTrackCornerRadius);

    painter->restore();
}

void NRangeSlider::drawProgress(QPainter* painter) {
    Q_D(NRangeSlider);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor progressColor;
    if (!isEnabled()) {
        progressColor = d->_isDark ? d->_pDarkDisabledProgressColor : d->_pLightDisabledProgressColor;
    } else if (d->_isPressed) {
        progressColor = d->_accentPressedColor;
    } else if (d->_hoveredHandle != NRangeSliderPrivate::None) {
        progressColor = d->_accentHoverColor;
    } else {
        progressColor = d->_isDark ? d->_pDarkProgressColor : d->_pLightProgressColor;
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(progressColor);
    painter->drawRoundedRect(progressRect(), d->_pTrackCornerRadius, d->_pTrackCornerRadius);

    painter->restore();
}

void NRangeSlider::drawHandle(QPainter* painter, const QRect& rect, bool isActive, bool isLower) {
    Q_D(NRangeSlider);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPointF center     = rect.center();
    QColor  outerColor = d->_isDark ? d->_pDarkThumbOuterColor : d->_pLightThumbOuterColor;
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerColor);

    qreal outerRadius = d->_pThumbDiameter / 2.0;
    painter->drawEllipse(center, outerRadius, outerRadius);

    QColor innerColor;
    if (!isEnabled()) {
        innerColor = d->_accentDisabledColor;
    } else if (isActive && d->_isPressed) {
        innerColor = d->_accentPressedColor;
    } else if (isActive) {
        innerColor = d->_accentHoverColor;
    } else {
        innerColor = d->_accentColor;
    }

    qreal scale       = isLower ? d->_lowerThumbScale : d->_upperThumbScale;
    qreal innerRadius = (d->_pThumbInnerDiameter / 2.0) * scale;

    painter->setBrush(innerColor);
    painter->drawEllipse(center, innerRadius, innerRadius);

    painter->restore();
}

void NRangeSlider::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    drawTrack(&painter);
    drawProgress(&painter);

    Q_D(NRangeSlider);
    bool lowerIsActive =
        (d->_dragHandle == NRangeSliderPrivate::Lower || d->_hoveredHandle == NRangeSliderPrivate::Lower);
    bool upperIsActive =
        (d->_dragHandle == NRangeSliderPrivate::Upper || d->_hoveredHandle == NRangeSliderPrivate::Upper);

    if (d->_dragHandle == NRangeSliderPrivate::Lower ||
        (d->_dragHandle == NRangeSliderPrivate::None && lowerIsActive)) {
        drawHandle(&painter, upperHandleRect(), upperIsActive, false);
        drawHandle(&painter, lowerHandleRect(), lowerIsActive, true);
    } else {
        drawHandle(&painter, lowerHandleRect(), lowerIsActive, true);
        drawHandle(&painter, upperHandleRect(), upperIsActive, false);
    }
}

void NRangeSlider::mousePressEvent(QMouseEvent* event) {
    if (!isEnabled()) {
        QWidget::mousePressEvent(event);
        return;
    }

    Q_D(NRangeSlider);

    QRect lowerRect = lowerHandleRect();
    QRect upperRect = upperHandleRect();

    int margin = 4;
    lowerRect  = lowerRect.adjusted(-margin, -margin, margin, margin);
    upperRect  = upperRect.adjusted(-margin, -margin, margin, margin);

    if (upperRect.contains(event->pos())) {
        d->_dragHandle    = NRangeSliderPrivate::Upper;
        d->_focusedHandle = NRangeSliderPrivate::Upper;
        d->_dragOffset    = (d->_orientation == Qt::Horizontal) ? event->pos().x() - upperHandleRect().center().x()
                                                                : event->pos().y() - upperHandleRect().center().y();
    } else if (lowerRect.contains(event->pos())) {
        d->_dragHandle    = NRangeSliderPrivate::Lower;
        d->_focusedHandle = NRangeSliderPrivate::Lower;
        d->_dragOffset    = (d->_orientation == Qt::Horizontal) ? event->pos().x() - lowerHandleRect().center().x()
                                                                : event->pos().y() - lowerHandleRect().center().y();
    } else if (trackRect().contains(event->pos())) {
        int clickValue = valueFromPosition(event->pos());
        int lowerDist  = qAbs(clickValue - d->_pLowerValue);
        int upperDist  = qAbs(clickValue - d->_pUpperValue);

        if (lowerDist < upperDist) {
            d->_dragHandle    = NRangeSliderPrivate::Lower;
            d->_focusedHandle = NRangeSliderPrivate::Lower;
            setValues(clickValue, d->_pUpperValue);
        } else {
            d->_dragHandle    = NRangeSliderPrivate::Upper;
            d->_focusedHandle = NRangeSliderPrivate::Upper;
            setValues(d->_pLowerValue, clickValue);
        }
        d->_dragOffset = 0;
    }

    if (d->_dragHandle != NRangeSliderPrivate::None) {
        d->_isPressed  = true;
        d->_isDragging = true;
        d->startThumbAnimation(d->_dragHandle,
                               d->_dragHandle == NRangeSliderPrivate::Lower ? d->_lowerThumbScale : d->_upperThumbScale,
                               0.8);

        if (d->_showTooltip) {
            d->updateTooltip(d->_dragHandle);
        }

        update();
    }

    QWidget::mousePressEvent(event);
}

void NRangeSlider::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NRangeSlider);

    if (!isEnabled()) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    if (d->_isDragging && d->_dragHandle != NRangeSliderPrivate::None) {
        QPoint adjustedPos = event->pos();
        if (d->_orientation == Qt::Horizontal) {
            adjustedPos.setX(adjustedPos.x() - d->_dragOffset);
        } else {
            adjustedPos.setY(adjustedPos.y() - d->_dragOffset);
        }

        int newValue = valueFromPosition(adjustedPos);

        if (d->_dragHandle == NRangeSliderPrivate::Lower) {
            newValue = qMin(newValue, d->_pUpperValue);
            if (newValue != d->_pLowerValue) {
                setValues(newValue, d->_pUpperValue);
            }
        } else {
            newValue = qMax(newValue, d->_pLowerValue);
            if (newValue != d->_pUpperValue) {
                setValues(d->_pLowerValue, newValue);
            }
        }

        if (d->_showTooltip) {
            d->updateTooltip(d->_dragHandle);
        }
    } else {
        QRect lowerRect = lowerHandleRect().adjusted(-4, -4, 4, 4);
        QRect upperRect = upperHandleRect().adjusted(-4, -4, 4, 4);

        NRangeSliderPrivate::DragHandle oldHovered = d->_hoveredHandle;

        if (upperRect.contains(event->pos())) {
            d->_hoveredHandle = NRangeSliderPrivate::Upper;
        } else if (lowerRect.contains(event->pos())) {
            d->_hoveredHandle = NRangeSliderPrivate::Lower;
        } else {
            d->_hoveredHandle = NRangeSliderPrivate::None;
        }

        if (d->_showTooltip) {
            if (d->_hoveredHandle == NRangeSliderPrivate::Lower) {
                d->updateTooltip(NRangeSliderPrivate::Lower);
                if (d->_upperTooltip)
                    d->_upperTooltip->hide();
            } else if (d->_hoveredHandle == NRangeSliderPrivate::Upper) {
                d->updateTooltip(NRangeSliderPrivate::Upper);
                if (d->_lowerTooltip)
                    d->_lowerTooltip->hide();
            } else {
                d->hideTooltips();
            }
        }

        if (oldHovered != d->_hoveredHandle) {
            update();
        }
    }

    QWidget::mouseMoveEvent(event);
}

void NRangeSlider::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NRangeSlider);

    if (isEnabled() && d->_dragHandle != NRangeSliderPrivate::None) {
        d->_isPressed  = false;
        d->_isDragging = false;

        qreal targetScale = (d->_hoveredHandle == d->_dragHandle) ? 1.3 : 1.0;
        d->startThumbAnimation(d->_dragHandle,
                               d->_dragHandle == NRangeSliderPrivate::Lower ? d->_lowerThumbScale : d->_upperThumbScale,
                               targetScale);

        if (d->_showTooltip) {
            d->hideTooltips();
        }

        d->_dragHandle = NRangeSliderPrivate::None;
        update();
    }

    QWidget::mouseReleaseEvent(event);
}

void NRangeSlider::enterEvent(QEnterEvent* event) {
    Q_D(NRangeSlider);

    if (isEnabled()) {
        QRect lowerRect = lowerHandleRect().adjusted(-4, -4, 4, 4);
        QRect upperRect = upperHandleRect().adjusted(-4, -4, 4, 4);

        if (upperRect.contains(event->position().toPoint())) {
            d->_hoveredHandle = NRangeSliderPrivate::Upper;
            d->startThumbAnimation(NRangeSliderPrivate::Upper, d->_upperThumbScale, 1.3);
        } else if (lowerRect.contains(event->position().toPoint())) {
            d->_hoveredHandle = NRangeSliderPrivate::Lower;
            d->startThumbAnimation(NRangeSliderPrivate::Lower, d->_lowerThumbScale, 1.3);
        }
        update();
    }

    QWidget::enterEvent(event);
}

void NRangeSlider::leaveEvent(QEvent* event) {
    Q_D(NRangeSlider);

    if (isEnabled() && !d->_isDragging) {
        if (d->_hoveredHandle == NRangeSliderPrivate::Lower) {
            d->startThumbAnimation(NRangeSliderPrivate::Lower, d->_lowerThumbScale, 1.0);
        } else if (d->_hoveredHandle == NRangeSliderPrivate::Upper) {
            d->startThumbAnimation(NRangeSliderPrivate::Upper, d->_upperThumbScale, 1.0);
        }
        d->_hoveredHandle = NRangeSliderPrivate::None;
        if (d->_showTooltip && d->_dragHandle == NRangeSliderPrivate::None) {
            d->hideTooltips();
        }

        update();
    }

    QWidget::leaveEvent(event);
}

void NRangeSlider::resizeEvent(QResizeEvent* event) {
    Q_D(NRangeSlider);
    QWidget::resizeEvent(event);
    update();
}

void NRangeSlider::keyPressEvent(QKeyEvent* event) {
    if (!isEnabled()) {
        QWidget::keyPressEvent(event);
        return;
    }

    Q_D(NRangeSlider);

    int step = (d->_pMaximum - d->_pMinimum) / 10;
    if (step == 0)
        step = 1;

    bool handled = false;

    switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Down:
            if (d->_focusedHandle == NRangeSliderPrivate::Lower) {
                setValues(qMax(d->_pMinimum, d->_pLowerValue - step), d->_pUpperValue);
            } else {
                setValues(d->_pLowerValue, qMax(d->_pLowerValue, d->_pUpperValue - step));
            }
            handled = true;
            break;

        case Qt::Key_Right:
        case Qt::Key_Up:
            if (d->_focusedHandle == NRangeSliderPrivate::Lower) {
                setValues(qMin(d->_pUpperValue, d->_pLowerValue + step), d->_pUpperValue);
            } else {
                setValues(d->_pLowerValue, qMin(d->_pMaximum, d->_pUpperValue + step));
            }
            handled = true;
            break;

        case Qt::Key_Home:
            if (d->_focusedHandle == NRangeSliderPrivate::Lower) {
                setValues(d->_pMinimum, d->_pUpperValue);
            } else {
                setValues(d->_pLowerValue, d->_pLowerValue);
            }
            handled = true;
            break;

        case Qt::Key_End:
            if (d->_focusedHandle == NRangeSliderPrivate::Lower) {
                setValues(d->_pUpperValue, d->_pUpperValue);
            } else {
                setValues(d->_pLowerValue, d->_pMaximum);
            }
            handled = true;
            break;

        case Qt::Key_Tab:
            d->_focusedHandle = (d->_focusedHandle == NRangeSliderPrivate::Lower) ? NRangeSliderPrivate::Upper
                                                                                  : NRangeSliderPrivate::Lower;
            update();
            handled = true;
            break;
    }

    if (!handled) {
        QWidget::keyPressEvent(event);
    }
}

void NRangeSlider::focusInEvent(QFocusEvent* event) {
    update();
    QWidget::focusInEvent(event);
}

void NRangeSlider::focusOutEvent(QFocusEvent* event) {
    update();
    QWidget::focusOutEvent(event);
}

void NRangeSlider::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange) {
        update();
    }
    QWidget::changeEvent(event);
}

bool NRangeSlider::showTooltip() const {
    Q_D(const NRangeSlider);
    return d->_showTooltip;
}

void NRangeSlider::setShowTooltip(bool show) {
    Q_D(NRangeSlider);
    if (d->_showTooltip == show)
        return;

    d->_showTooltip = show;

    if (!show) {
        d->hideTooltips();
    }
}

void NRangeSlider::setTooltipFormatter(std::function<QString(int)> formatter) {
    Q_D(NRangeSlider);
    d->_tooltipFormatter = formatter;
}
