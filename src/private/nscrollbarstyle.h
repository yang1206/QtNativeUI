//
// Created by Yang1206 on 2025/4/6.
//

#ifndef NSCROLLBARSTYLE_H
#define NSCROLLBARSTYLE_H

#include <QColor>
#include <QProxyStyle>
#include <QStyleOptionSlider>

class NScrollBar;
class NScrollBarStyle : public QProxyStyle {
    Q_OBJECT

  public:
    explicit NScrollBarStyle(QStyle* style = nullptr);
    ~NScrollBarStyle();

    void drawComplexControl(ComplexControl             control,
                            const QStyleOptionComplex* option,
                            QPainter*                  painter,
                            const QWidget*             widget = nullptr) const override;

    QRect subControlRect(ComplexControl             control,
                         const QStyleOptionComplex* option,
                         SubControl                 subControl,
                         const QWidget*             widget = nullptr) const override;

    int pixelMetric(PixelMetric         metric,
                    const QStyleOption* option = nullptr,
                    const QWidget*      widget = nullptr) const override;

    // 设置样式颜色和尺寸
    void setThumbColor(const QColor& color);
    void setThumbHoverColor(const QColor& color);
    void setThumbPressColor(const QColor& color);
    void setTrackColor(const QColor& color);
    void setArrowColor(const QColor& color);
    void setArrowHoverColor(const QColor& color);
    void setArrowPressColor(const QColor& color);
    void setThumbThickness(int thickness);
    void setThumbNormalThickness(int thickness);
    void setTrackThickness(int thickness);
    void setThumbCornerRadius(int radius);
    void setTrackCornerRadius(int radius);
    void setHovered(bool hovered);
    void setPressed(bool pressed);
    void setLineHovered(SubControl subControl, bool hovered);
    void setLinePressed(SubControl subControl, bool pressed);

  private:
    QColor _thumbColor;
    QColor _thumbHoverColor;
    QColor _thumbPressColor;
    QColor _trackColor;
    QColor _arrowColor;
    QColor _arrowHoverColor;
    QColor _arrowPressColor;
    int    _thumbThickness{8};       // 悬浮/按下时的厚度
    int    _thumbNormalThickness{4}; // 正常状态下的厚度
    int    _trackThickness{8};
    int    _thumbCornerRadius{4};
    int    _trackCornerRadius{4};
    bool   _isHovered{false};
    bool   _isPressed{false};
    bool   _addLineHovered{false};
    bool   _subLineHovered{false};
    bool   _addLinePressed{false};
    bool   _subLinePressed{false};

    void
    drawArrow(SubControl subControl, const QStyleOptionSlider* option, QPainter* painter, const QWidget* widget) const;
};

#endif // NSCROLLBARSTYLE_H