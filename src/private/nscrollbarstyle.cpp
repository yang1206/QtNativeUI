//
// Created by Yang1206 on 2025/4/6.
//

#include "nscrollbarstyle.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSlider>
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NScrollBar.h"

NScrollBarStyle::NScrollBarStyle(QStyle* style) : QProxyStyle(style) {}

NScrollBarStyle::~NScrollBarStyle() {}

void NScrollBarStyle::drawComplexControl(ComplexControl             control,
                                         const QStyleOptionComplex* option,
                                         QPainter*                  painter,
                                         const QWidget*             widget) const {
    if (control == CC_ScrollBar) {
        const QStyleOptionSlider* scrollOption = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (!scrollOption)
            return;

        bool horizontal = scrollOption->orientation == Qt::Horizontal;

        QRect groove = subControlRect(CC_ScrollBar, scrollOption, SC_ScrollBarGroove, widget);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QPainterPath trackPath;
        trackPath.addRoundedRect(groove, _trackCornerRadius, _trackCornerRadius);

        if (_isHovered || _isPressed) {
            painter->fillPath(trackPath, _trackColor);
        }

        painter->restore();

        // 滑块
        if (scrollOption->subControls & SC_ScrollBarSlider) {
            QRect sliderBase = subControlRect(CC_ScrollBar, scrollOption, SC_ScrollBarSlider, widget);

            QRect slider           = sliderBase;
            int   currentThickness = (_isHovered || _isPressed) ? _thumbThickness : _thumbNormalThickness;

            if (horizontal) {
                int heightDiff = sliderBase.height() - currentThickness;
                slider = QRect(sliderBase.x(), sliderBase.y() + heightDiff / 2, sliderBase.width(), currentThickness);
            } else {
                int widthDiff = sliderBase.width() - currentThickness;
                slider = QRect(sliderBase.x() + widthDiff / 2, sliderBase.y(), currentThickness, sliderBase.height());
            }

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);

            QPainterPath path;
            path.addRoundedRect(slider, _thumbCornerRadius, _thumbCornerRadius);

            QColor thumbColor = _thumbColor;
            if (_isPressed) {
                thumbColor = _thumbPressColor;
            } else if (_isHovered) {
                thumbColor = _thumbHoverColor;
            }

            painter->fillPath(path, thumbColor);
            painter->restore();
        }

        // 绘制箭头
        if (_isHovered || _isPressed) {
            // 只在悬浮或按下时绘制箭头
            if (scrollOption->subControls & SC_ScrollBarAddLine) {
                drawArrow(SC_ScrollBarAddLine, scrollOption, painter, widget);
            }
            if (scrollOption->subControls & SC_ScrollBarSubLine) {
                drawArrow(SC_ScrollBarSubLine, scrollOption, painter, widget);
            }
        }

        return;
    }

    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

void NScrollBarStyle::drawArrow(SubControl                subControl,
                                const QStyleOptionSlider* option,
                                QPainter*                 painter,
                                const QWidget*            widget) const {
    QRect arrowRect  = subControlRect(CC_ScrollBar, option, subControl, widget);
    bool  horizontal = option->orientation == Qt::Horizontal;

    QColor arrowColor = _arrowColor;

    bool isArrowHovered = (subControl == SC_ScrollBarAddLine && _addLineHovered) ||
                          (subControl == SC_ScrollBarSubLine && _subLineHovered);
    bool isArrowPressed = (subControl == SC_ScrollBarAddLine && _addLinePressed) ||
                          (subControl == SC_ScrollBarSubLine && _subLinePressed);

    if (isArrowPressed) {
        arrowColor = _arrowPressColor;
    } else if (isArrowHovered) {
        arrowColor = _arrowHoverColor;
    }

    NFilledIconType::Icon arrowIcon;

    if (horizontal) {
        if (subControl == SC_ScrollBarAddLine) {
            arrowIcon = NFilledIconType::CaretRight12Filled;
        } else {
            arrowIcon = NFilledIconType::CaretLeft12Filled;
        }
    } else {
        if (subControl == SC_ScrollBarAddLine) {
            arrowIcon = NFilledIconType::CaretDown12Filled;
        } else {
            arrowIcon = NFilledIconType::CaretUp12Filled;
        }
    }

    QIcon icon = nIcon->fromFilled(arrowIcon, arrowColor);
    icon.paint(painter, arrowRect, Qt::AlignCenter);
}

QRect NScrollBarStyle::subControlRect(ComplexControl             control,
                                      const QStyleOptionComplex* option,
                                      SubControl                 subControl,
                                      const QWidget*             widget) const {
    if (control == CC_ScrollBar) {
        const QStyleOptionSlider* scrollOption = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (!scrollOption)
            return QProxyStyle::subControlRect(control, option, subControl, widget);

        QRect rect       = option->rect;
        bool  horizontal = scrollOption->orientation == Qt::Horizontal;
        int   arrowSize  = _trackThickness;

        if (subControl == SC_ScrollBarGroove) {
            if (horizontal) {
                return QRect(rect.x() + arrowSize, rect.y(), rect.width() - 2 * arrowSize, rect.height());
            } else {
                return QRect(rect.x(), rect.y() + arrowSize, rect.width(), rect.height() - 2 * arrowSize);
            }
        } else if (subControl == SC_ScrollBarSlider) {
            // 计算滑块位置和大小
            int minLength = pixelMetric(PM_ScrollBarSliderMin, scrollOption, widget);
            int maxValue  = scrollOption->maximum;
            int minValue  = scrollOption->minimum;
            int range     = maxValue - minValue;
            int value     = scrollOption->sliderValue;

            // 获取轨道区域
            QRect grooveRect = subControlRect(CC_ScrollBar, scrollOption, SC_ScrollBarGroove, widget);

            // 滑块大小
            int sliderLength;
            if (range > 0) {
                int pageStep    = scrollOption->pageStep;
                int totalLength = horizontal ? grooveRect.width() : grooveRect.height();
                sliderLength    = (totalLength * pageStep) / (range + pageStep);
                if (sliderLength < minLength || range > INT_MAX / 2)
                    sliderLength = minLength;
                if (sliderLength > totalLength)
                    sliderLength = totalLength;
            } else {
                sliderLength = horizontal ? grooveRect.width() : grooveRect.height();
            }

            // 滑块位置
            int sliderPos;
            if (range > 0) {
                int totalLength = horizontal ? grooveRect.width() : grooveRect.height();
                sliderPos       = ((totalLength - sliderLength) * (value - minValue)) / range;
                if (scrollOption->upsideDown) {
                    sliderPos = totalLength - sliderPos - sliderLength;
                }
            } else {
                sliderPos = 0;
            }

            // 返回滑块的整个区域，不做尺寸调整，尺寸调整在绘制时进行
            if (horizontal) {
                return QRect(grooveRect.x() + sliderPos, grooveRect.y(), sliderLength, grooveRect.height());
            } else {
                return QRect(grooveRect.x(), grooveRect.y() + sliderPos, grooveRect.width(), sliderLength);
            }
        } else if (subControl == SC_ScrollBarAddLine) {
            if (horizontal) {
                return QRect(rect.right() - arrowSize + 1, rect.y(), arrowSize, rect.height());
            } else {
                return QRect(rect.x(), rect.bottom() - arrowSize + 1, rect.width(), arrowSize);
            }
        } else if (subControl == SC_ScrollBarSubLine) {
            if (horizontal) {
                return QRect(rect.x(), rect.y(), arrowSize, rect.height());
            } else {
                return QRect(rect.x(), rect.y(), rect.width(), arrowSize);
            }
        }

        // 隐藏页面按钮
        if (subControl == SC_ScrollBarAddPage || subControl == SC_ScrollBarSubPage) {
            return QRect();
        }
    }

    return QProxyStyle::subControlRect(control, option, subControl, widget);
}

int NScrollBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const {
    if (metric == PM_ScrollBarExtent) {
        return _trackThickness;
    }

    if (metric == PM_ScrollBarSliderMin) {
        return 40; // 最小滑块长度
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}

void NScrollBarStyle::setThumbColor(const QColor& color) { _thumbColor = color; }

void NScrollBarStyle::setThumbHoverColor(const QColor& color) { _thumbHoverColor = color; }

void NScrollBarStyle::setThumbPressColor(const QColor& color) { _thumbPressColor = color; }

void NScrollBarStyle::setTrackColor(const QColor& color) { _trackColor = color; }

void NScrollBarStyle::setArrowColor(const QColor& color) { _arrowColor = color; }

void NScrollBarStyle::setArrowHoverColor(const QColor& color) { _arrowHoverColor = color; }

void NScrollBarStyle::setArrowPressColor(const QColor& color) { _arrowPressColor = color; }

void NScrollBarStyle::setThumbThickness(int thickness) { _thumbThickness = thickness; }

void NScrollBarStyle::setThumbNormalThickness(int thickness) { _thumbNormalThickness = thickness; }

void NScrollBarStyle::setTrackThickness(int thickness) { _trackThickness = thickness; }

void NScrollBarStyle::setThumbCornerRadius(int radius) { _thumbCornerRadius = radius; }

void NScrollBarStyle::setTrackCornerRadius(int radius) { _trackCornerRadius = radius; }

void NScrollBarStyle::setHovered(bool hovered) { _isHovered = hovered; }

void NScrollBarStyle::setPressed(bool pressed) { _isPressed = pressed; }

void NScrollBarStyle::setLineHovered(SubControl subControl, bool hovered) {
    if (subControl == SC_ScrollBarAddLine) {
        _addLineHovered = hovered;
    } else if (subControl == SC_ScrollBarSubLine) {
        _subLineHovered = hovered;
    }
}

void NScrollBarStyle::setLinePressed(SubControl subControl, bool pressed) {
    if (subControl == SC_ScrollBarAddLine) {
        _addLinePressed = pressed;
    } else if (subControl == SC_ScrollBarSubLine) {
        _subLinePressed = pressed;
    }
}