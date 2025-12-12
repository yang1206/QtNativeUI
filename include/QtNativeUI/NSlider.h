//
// Created by Yang1206 on 2025/4/10.
//

#ifndef NSLIDER_H
#define NSLIDER_H

#include <QSlider>
#include <functional>

#include "NColor.h"
#include "stdafx.h"

class NSliderPrivate;
class QTNATIVEUI_EXPORT NSlider : public QSlider {
    Q_OBJECT
    Q_Q_CREATE(NSlider)

    // 颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbOuterColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbOuterColor)

    // 尺寸属性
    Q_PROPERTY_CREATE_Q_H(int, TrackHeight)
    Q_PROPERTY_CREATE_Q_H(int, ThumbDiameter)
    Q_PROPERTY_CREATE_Q_H(int, ThumbInnerDiameter)
    Q_PROPERTY_CREATE_Q_H(int, TrackCornerRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTickColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTickColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledTickColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledTickColor)
    Q_PROPERTY_CREATE_Q_H(int, TickLength)
    Q_PROPERTY_CREATE_Q_H(int, TickThickness)

    Q_PROPERTY(bool showTooltip READ showTooltip WRITE setShowTooltip)

  public:
    NSlider(QWidget* parent = nullptr);
    NSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~NSlider();

    void setAccentColor(const QColor& color);
    void setAccentColor(const NAccentColor& color);
    void resetAccentColor();

    bool showTooltip() const;
    void setShowTooltip(bool show);
    void setTooltipFormatter(std::function<QString(int)> formatter);
    using QSlider::setTickInterval;
    using QSlider::tickInterval;
    using QSlider::setTickPosition;
    using QSlider::tickPosition;

  protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

  private:
    void init();
    void updateAccentColors();
};

#endif // NSLIDER_H