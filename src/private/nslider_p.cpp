//
// Created by Yang1206 on 2025/4/10.
//

#include "nslider_p.h"

#include <QPainterPath>
#include <QStyleOptionSlider>
#include <QtNativeUI/NToolTip.h>
#include "QtNativeUI/NSlider.h"
#include "QtNativeUI/NTheme.h"

NSliderPrivate::Style::Style(NSliderPrivate* parent, QStyle* style) : QProxyStyle(style), d(parent) {}

void NSliderPrivate::Style::drawComplexControl(ComplexControl             control,
                                               const QStyleOptionComplex* option,
                                               QPainter*                  painter,
                                               const QWidget*             widget) const {
    if (control == CC_Slider) {
        if (const QStyleOptionSlider* sliderOption = qstyleoption_cast<const QStyleOptionSlider*>(option)) {
            drawTrack(sliderOption, painter, widget);
            drawProgress(sliderOption, painter, widget);

            if (sliderOption->subControls & SC_SliderTickmarks) {
                drawTicks(sliderOption, painter, widget);
            }

            drawHandle(sliderOption, painter, widget);
            return;
        }
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

void NSliderPrivate::Style::drawControl(ControlElement      element,
                                        const QStyleOption* option,
                                        QPainter*           painter,
                                        const QWidget*      widget) const {
    QProxyStyle::drawControl(element, option, painter, widget);
}

QStyle::SubControl NSliderPrivate::Style::hitTestComplexControl(ComplexControl             control,
                                                                const QStyleOptionComplex* option,
                                                                const QPoint&              pos,
                                                                const QWidget*             widget) const {
    if (control == CC_Slider) {
        if (const QStyleOptionSlider* sliderOption = qstyleoption_cast<const QStyleOptionSlider*>(option)) {
            QRect handleRect;

            int sliderPos = sliderPositionFromValue(sliderOption->minimum,
                                                    sliderOption->maximum,
                                                    sliderOption->sliderPosition,
                                                    sliderOption->orientation == Qt::Horizontal
                                                        ? sliderOption->rect.width() - d->_pThumbDiameter
                                                        : sliderOption->rect.height() - d->_pThumbDiameter,
                                                    sliderOption->upsideDown);

            if (sliderOption->orientation == Qt::Horizontal) {
                handleRect = QRect(sliderPos,
                                   (sliderOption->rect.height() - d->_pThumbDiameter) / 2,
                                   d->_pThumbDiameter,
                                   d->_pThumbDiameter);
            } else {
                handleRect = QRect((sliderOption->rect.width() - d->_pThumbDiameter) / 2,
                                   sliderPos,
                                   d->_pThumbDiameter,
                                   d->_pThumbDiameter);
            }
            int   extraMargin        = 4;
            QRect expandedHandleRect = handleRect.adjusted(-extraMargin, -extraMargin, extraMargin, extraMargin);

            if (expandedHandleRect.contains(pos)) {
                return SC_SliderHandle;
            } else if (sliderOption->rect.contains(pos)) {
                return SC_SliderGroove;
            }
        }
    }

    return QProxyStyle::hitTestComplexControl(control, option, pos, widget);
}

void NSliderPrivate::Style::drawTrack(const QStyleOptionSlider*       option,
                                      QPainter*                       painter,
                                      [[maybe_unused]] const QWidget* widget) const {
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor trackColor;
    if (!(option->state & QStyle::State_Enabled)) {
        trackColor = d->_isDark ? d->_pDarkDisabledTrackColor : d->_pLightDisabledTrackColor;
    } else {
        trackColor = d->_isDark ? d->_pDarkTrackColor : d->_pLightTrackColor;
    }

    int padding = d->_pThumbDiameter / 2;

    if (option->orientation == Qt::Horizontal) {
        int   trackY = (option->rect.height() - d->_pTrackHeight) / 2;
        QRect trackRect(padding, trackY, option->rect.width() - 2 * padding, d->_pTrackHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(trackColor);
        painter->drawRoundedRect(trackRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    } else {
        int   trackX = (option->rect.width() - d->_pTrackHeight) / 2;
        QRect trackRect(trackX, padding, d->_pTrackHeight, option->rect.height() - 2 * padding);

        painter->setPen(Qt::NoPen);
        painter->setBrush(trackColor);
        painter->drawRoundedRect(trackRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    }

    painter->restore();
}

void NSliderPrivate::Style::drawProgress(const QStyleOptionSlider*       option,
                                         QPainter*                       painter,
                                         [[maybe_unused]] const QWidget* widget) const {
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor progressColor;
    if (!(option->state & QStyle::State_Enabled)) {
        progressColor = d->_isDark ? d->_pDarkDisabledProgressColor : d->_pLightDisabledProgressColor;
    } else if (d->_isPressed) {
        progressColor = d->_accentPressedColor;
    } else if (d->_isHovered) {
        progressColor = d->_accentHoverColor;
    } else {
        progressColor = d->_isDark ? d->_pDarkProgressColor : d->_pLightProgressColor;
    }

    int    padding = d->_pThumbDiameter / 2;
    double ratio = (option->sliderPosition - option->minimum) / static_cast<double>(option->maximum - option->minimum);

    if (option->orientation == Qt::Horizontal) {
        int trackY        = (option->rect.height() - d->_pTrackHeight) / 2;
        int progressWidth = static_cast<int>((option->rect.width() - 2 * padding) * ratio);

        QRect progressRect;
        if (option->upsideDown) {
            int progressX = option->rect.width() - padding - progressWidth;
            progressRect  = QRect(progressX, trackY, progressWidth, d->_pTrackHeight);
        } else {
            progressRect = QRect(padding, trackY, progressWidth, d->_pTrackHeight);
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(progressColor);
        painter->drawRoundedRect(progressRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    } else {
        int trackX         = (option->rect.width() - d->_pTrackHeight) / 2;
        int progressHeight = static_cast<int>((option->rect.height() - 2 * padding) * ratio);

        QRect progressRect;
        if (option->upsideDown) {
            int progressY = option->rect.height() - padding - progressHeight;
            progressRect  = QRect(trackX, progressY, d->_pTrackHeight, progressHeight);
        } else {
            progressRect = QRect(trackX, padding, d->_pTrackHeight, progressHeight);
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(progressColor);
        painter->drawRoundedRect(progressRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    }

    painter->restore();
}

void NSliderPrivate::Style::drawHandle(const QStyleOptionSlider*       option,
                                       QPainter*                       painter,
                                       [[maybe_unused]] const QWidget* widget) const {
    painter->save();

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    int sliderPos =
        sliderPositionFromValue(option->minimum,
                                option->maximum,
                                option->sliderPosition,
                                option->orientation == Qt::Horizontal ? option->rect.width() - d->_pThumbDiameter
                                                                      : option->rect.height() - d->_pThumbDiameter,
                                option->upsideDown);

    QPointF thumbCenter;
    if (option->orientation == Qt::Horizontal) {
        int thumbX  = sliderPos + d->_pThumbDiameter / 2;
        int thumbY  = option->rect.height() / 2;
        thumbCenter = QPointF(thumbX, thumbY);
    } else {
        int thumbX  = option->rect.width() / 2;
        int thumbY  = sliderPos + d->_pThumbDiameter / 2;
        thumbCenter = QPointF(thumbX, thumbY);
    }

    QColor outerColor = d->_isDark ? d->_pDarkThumbOuterColor : d->_pLightThumbOuterColor;
    QColor borderColor =
        NThemeColor(NFluentColorKey::ControlStrokeColorDefault, d->_isDark ? NThemeType::Dark : NThemeType::Light);
    qreal outerRadius = (d->_pThumbDiameter / 2.0) - 1;

    painter->setPen(QPen(borderColor, 1));
    painter->setBrush(outerColor);
    painter->drawEllipse(thumbCenter, outerRadius, outerRadius);

    QColor innerColor;
    if (!(option->state & QStyle::State_Enabled)) {
        innerColor = d->_accentDisabledColor;
    } else if (d->_isPressed) {
        innerColor = d->_accentPressedColor;
    } else if (d->_isHovered) {
        innerColor = d->_accentHoverColor;
    } else {
        innerColor = d->_accentColor;
    }

    qreal innerRadius = (d->_pThumbInnerDiameter / 2.0) * d->_thumbScale;

    painter->setPen(Qt::NoPen);
    painter->setBrush(innerColor);
    painter->drawEllipse(thumbCenter, innerRadius, innerRadius);

    painter->restore();
}

void NSliderPrivate::Style::drawTicks(const QStyleOptionSlider*       option,
                                      QPainter*                       painter,
                                      [[maybe_unused]] const QWidget* widget) const {
    painter->save();

    QColor tickColor;
    if (!(option->state & QStyle::State_Enabled)) {
        tickColor = d->_isDark ? d->_pDarkDisabledTickColor : d->_pLightDisabledTickColor;
    } else {
        tickColor = d->_isDark ? d->_pDarkTickColor : d->_pLightTickColor;
    }

    painter->setPen(QPen(tickColor, d->_pTickThickness));

    int padding    = d->_pThumbDiameter / 2;
    int valueRange = option->maximum - option->minimum;

    int interval = option->tickInterval;
    if (interval <= 0) {
        interval = qMax(1, valueRange / 10);
    }

    if (option->orientation == Qt::Horizontal) {
        int trackY     = (option->rect.height() - d->_pTrackHeight) / 2;
        int tickTop    = trackY - d->_pTickLength - 1;
        int tickBottom = trackY + d->_pTrackHeight + 1;

        for (int value = option->minimum; value <= option->maximum; value += interval) {
            double ratio = (value - option->minimum) / static_cast<double>(valueRange);
            int    tickX = padding + static_cast<int>((option->rect.width() - 2 * padding) * ratio);

            if (option->tickPosition & QSlider::TicksAbove) {
                painter->drawLine(tickX, tickTop, tickX, tickTop + d->_pTickLength);
            }
            if (option->tickPosition & QSlider::TicksBelow) {
                painter->drawLine(tickX, tickBottom, tickX, tickBottom + d->_pTickLength);
            }
        }
    } else {
        int trackX    = (option->rect.width() - d->_pTrackHeight) / 2;
        int tickLeft  = trackX - d->_pTickLength - 1;
        int tickRight = trackX + d->_pTrackHeight + 1;

        for (int value = option->minimum; value <= option->maximum; value += interval) {
            double ratio = (value - option->minimum) / static_cast<double>(valueRange);
            int    tickY =
                option->rect.height() - padding - static_cast<int>((option->rect.height() - 2 * padding) * ratio);

            if (option->tickPosition & QSlider::TicksLeft) {
                painter->drawLine(tickLeft, tickY, tickLeft + d->_pTickLength, tickY);
            }
            if (option->tickPosition & QSlider::TicksRight) {
                painter->drawLine(tickRight, tickY, tickRight + d->_pTickLength, tickY);
            }
        }
    }

    painter->restore();
}

NSliderPrivate::NSliderPrivate(QObject* parent) : QObject(parent) {
    _thumbAnimation = new QPropertyAnimation(this, "thumbScale");
    _thumbAnimation->setEasingCurve(QEasingCurve::OutQuad);
    _thumbAnimation->setDuration(150);
}

NSliderPrivate::~NSliderPrivate() {
    if (_tooltip) {
        _tooltip->deleteLater();
        _tooltip = nullptr;
    }
}

void NSliderPrivate::startThumbAnimation(qreal startScale, qreal endScale) {
    _thumbAnimation->stop();
    _thumbAnimation->setStartValue(startScale);
    _thumbAnimation->setEndValue(endScale);

    QObject::disconnect(_thumbAnimation, nullptr, q_ptr, nullptr);

    QObject::connect(_thumbAnimation, &QPropertyAnimation::valueChanged, q_ptr, [this](const QVariant& value) {
        _thumbScale = value.toReal();
        q_ptr->update();
    });

    QObject::connect(_thumbAnimation, &QPropertyAnimation::finished, q_ptr, [this]() {
        QObject::disconnect(_thumbAnimation, nullptr, q_ptr, nullptr);
    });

    _thumbAnimation->start();
}

void NSliderPrivate::updateTooltip() {
    if (!_showTooltip || !q_ptr)
        return;

    if (!_tooltip) {
        _tooltip = new NToolTip("", q_ptr->window());
        _tooltip->setDuration(-1);
    }

    int value = q_ptr->value();
    QString text = _tooltipFormatter ? _tooltipFormatter(value) : QString::number(value);
    _tooltip->setText(text);

    // 计算滑块位置 - 使用简化的方法
    int padding = _pThumbDiameter / 2;
    double ratio = (q_ptr->value() - q_ptr->minimum()) / static_cast<double>(q_ptr->maximum() - q_ptr->minimum());
    
    QPoint thumbCenter;
    if (q_ptr->orientation() == Qt::Horizontal) {
        int availableWidth = q_ptr->width() - 2 * padding;
        int thumbX = padding + static_cast<int>(availableWidth * ratio);
        int thumbY = q_ptr->height() / 2;
        thumbCenter = q_ptr->mapToGlobal(QPoint(thumbX, thumbY));
    } else {
        int availableHeight = q_ptr->height() - 2 * padding;
        int thumbX = q_ptr->width() / 2;
        int thumbY = q_ptr->height() - padding - static_cast<int>(availableHeight * ratio);
        thumbCenter = q_ptr->mapToGlobal(QPoint(thumbX, thumbY));
    }
    
    _tooltip->adjustSize();
    
    QPoint tooltipPos;
    if (q_ptr->orientation() == Qt::Horizontal) {
        tooltipPos = QPoint(thumbCenter.x() - _tooltip->width() / 2,
                           thumbCenter.y() - _pThumbDiameter / 2 - _tooltip->height() - 5);
    } else {
        tooltipPos = QPoint(thumbCenter.x() + _pThumbDiameter / 2 + 5,
                           thumbCenter.y() - _tooltip->height() / 2);
    }
    
    _tooltip->move(tooltipPos);
    _tooltip->show();
}

void NSliderPrivate::hideTooltip() {
    if (_tooltip && _tooltip->isVisible()) {
        _tooltip->hide();
    }
}

QRect NSliderPrivate::getThumbRect() const {
    if (!q_ptr || !_sliderStyle)
        return QRect();
    
    return _sliderStyle->calculateThumbRect(q_ptr);
}

QRect NSliderPrivate::Style::calculateThumbRect(const QWidget* widget) const {
    const QSlider* slider = qobject_cast<const QSlider*>(widget);
    if (!slider)
        return QRect();

    QStyleOptionSlider option;
    option.initFrom(slider);
    option.orientation = slider->orientation();
    option.minimum = slider->minimum();
    option.maximum = slider->maximum();
    option.sliderPosition = slider->sliderPosition();
    option.sliderValue = slider->value();
    option.singleStep = slider->singleStep();
    option.pageStep = slider->pageStep();
    option.upsideDown = slider->invertedAppearance();
    option.direction = slider->layoutDirection();
    option.rect = slider->rect();

    int sliderPos = sliderPositionFromValue(option.minimum,
                                          option.maximum,
                                          option.sliderPosition,
                                          option.orientation == Qt::Horizontal
                                              ? option.rect.width() - d->_pThumbDiameter
                                              : option.rect.height() - d->_pThumbDiameter,
                                          option.upsideDown);

    if (option.orientation == Qt::Horizontal) {
        return QRect(sliderPos + d->_pThumbDiameter / 2,
                    (option.rect.height() - d->_pThumbDiameter) / 2,
                    d->_pThumbDiameter,
                    d->_pThumbDiameter);
    } else {
        return QRect((option.rect.width() - d->_pThumbDiameter) / 2,
                    sliderPos + d->_pThumbDiameter / 2,
                    d->_pThumbDiameter,
                    d->_pThumbDiameter);
    }
}