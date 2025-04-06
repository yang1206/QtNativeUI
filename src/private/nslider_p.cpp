//
// Created by Yang1206 on 2025/4/10.
//

#include "nslider_p.h"

#include <QPainterPath>
#include <QStyleOptionSlider>
#include "QtNativeUI/NSlider.h"
#include "QtNativeUI/NTheme.h"

NSliderPrivate::Style::Style(NSliderPrivate* parent, QStyle* style) : QProxyStyle(style), d(parent) {}

void NSliderPrivate::Style::drawComplexControl(ComplexControl             control,
                                               const QStyleOptionComplex* option,
                                               QPainter*                  painter,
                                               const QWidget*             widget) const {
    if (control == CC_Slider) {
        if (const QStyleOptionSlider* sliderOption = qstyleoption_cast<const QStyleOptionSlider*>(option)) {
            // 绘制轨道、进度、滑块和刻度
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
            // 计算滑块区域
            QRect handleRect;

            // 获取滑块位置
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

            // 扩大点击区域，使其更容易点击
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

void NSliderPrivate::Style::drawTrack(const QStyleOptionSlider* option,
                                      QPainter*                 painter,
                                      const QWidget*            widget) const {
    painter->save();

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

void NSliderPrivate::Style::drawProgress(const QStyleOptionSlider* option,
                                         QPainter*                 painter,
                                         const QWidget*            widget) const {
    painter->save();

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
        int   trackY        = (option->rect.height() - d->_pTrackHeight) / 2;
        int   progressWidth = static_cast<int>((option->rect.width() - 2 * padding) * ratio);
        QRect progressRect(padding, trackY, progressWidth, d->_pTrackHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(progressColor);
        painter->drawRoundedRect(progressRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    } else {
        int   trackX         = (option->rect.width() - d->_pTrackHeight) / 2;
        int   progressHeight = static_cast<int>((option->rect.height() - 2 * padding) * ratio);
        int   progressY      = option->rect.height() - padding - progressHeight;
        QRect progressRect(trackX, progressY, d->_pTrackHeight, progressHeight);

        painter->setPen(Qt::NoPen);
        painter->setBrush(progressColor);
        painter->drawRoundedRect(progressRect, d->_pTrackCornerRadius, d->_pTrackCornerRadius);
    }

    painter->restore();
}

void NSliderPrivate::Style::drawHandle(const QStyleOptionSlider* option,
                                       QPainter*                 painter,
                                       const QWidget*            widget) const {
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

    // 绘制外圆
    QColor outerColor = d->_isDark ? d->_pDarkThumbOuterColor : d->_pLightThumbOuterColor;
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerColor);

    // 使用QPainterPath绘制圆形，可以提高绘制质量
    QPainterPath outerPath;
    qreal        outerRadius = d->_pThumbDiameter / 2.0;
    outerPath.addEllipse(thumbCenter, outerRadius, outerRadius);
    painter->drawPath(outerPath);

    // 绘制内圆
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

    // 使用thumbScale属性控制内圆大小，恢复动画效果
    qreal innerRadius = (d->_pThumbInnerDiameter / 2.0) * d->_thumbScale;

    painter->setBrush(innerColor);

    // 同样使用QPainterPath绘制内圆
    QPainterPath innerPath;
    innerPath.addEllipse(thumbCenter, innerRadius, innerRadius);
    painter->drawPath(innerPath);

    painter->restore();
}

void NSliderPrivate::Style::drawTicks(const QStyleOptionSlider* option,
                                      QPainter*                 painter,
                                      const QWidget*            widget) const {
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

NSliderPrivate::~NSliderPrivate() {}

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