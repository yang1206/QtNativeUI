//
// Created by Yang1206 on 2025/4/10.
//

#ifndef NSLIDER_P_H
#define NSLIDER_P_H
#include <QPropertyAnimation>
#include <QProxyStyle>
#include <QtNativeUI/NSlider.h>
#include "QtNativeUI/NEnums.h"

class NSliderPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal thumbScale READ thumbScale WRITE setThumbScale)

    // 颜色属性
    Q_PROPERTY_CREATE_D(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_D(QColor, LightProgressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkProgressColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledTrackColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledTrackColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledProgressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledProgressColor)
    Q_PROPERTY_CREATE_D(QColor, LightThumbOuterColor)
    Q_PROPERTY_CREATE_D(QColor, DarkThumbOuterColor)

    // 尺寸属性
    Q_PROPERTY_CREATE_D(int, TrackHeight)
    Q_PROPERTY_CREATE_D(int, ThumbDiameter)
    Q_PROPERTY_CREATE_D(int, ThumbInnerDiameter)
    Q_PROPERTY_CREATE_D(int, TrackCornerRadius)

    Q_PROPERTY_CREATE_D(QColor, LightTickColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTickColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledTickColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledTickColor)
    Q_PROPERTY_CREATE_D(int, TickLength)
    Q_PROPERTY_CREATE_D(int, TickThickness)

  public:
    class Style : public QProxyStyle {
    public:
        explicit Style(NSliderPrivate* parent, QStyle* style = nullptr);

        void drawComplexControl(ComplexControl control,
                               const QStyleOptionComplex* option,
                               QPainter* painter,
                               const QWidget* widget = nullptr) const override;

        void drawControl(ControlElement element,
                        const QStyleOption* option,
                        QPainter* painter,
                        const QWidget* widget = nullptr) const override;

        SubControl hitTestComplexControl(ComplexControl control,
                                        const QStyleOptionComplex* option,
                                        const QPoint& pos,
                                        const QWidget* widget = nullptr) const override;

    private:
        NSliderPrivate* d;
        void drawTrack(const QStyleOptionSlider* option, QPainter* painter, const QWidget* widget) const;
        void drawProgress(const QStyleOptionSlider* option, QPainter* painter, const QWidget* widget) const;
        void drawHandle(const QStyleOptionSlider* option, QPainter* painter, const QWidget* widget) const;
        void drawTicks(const QStyleOptionSlider* option, QPainter* painter, const QWidget* widget) const;
    };

    explicit NSliderPrivate(QObject* parent = nullptr);
    ~NSliderPrivate();
    Q_D_CREATE(NSlider)

    // 缩放属性访问器
    qreal thumbScale() const { return _thumbScale; }
    void  setThumbScale(qreal scale) {
        _thumbScale = scale;
        q_ptr->update();
    }

    // 启动缩放动画
    void startThumbAnimation(qreal startScale, qreal endScale);

  public:
    // 状态跟踪
    bool                  _isHovered  = false;
    bool                  _isPressed  = false;
    bool                  _isDragging = false;
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark = false;

    // 动画
    qreal               _thumbScale     = 1.0;
    QPropertyAnimation* _thumbAnimation = nullptr;

    // 强调色
    QColor _accentColor;
    QColor _accentHoverColor;
    QColor _accentPressedColor;
    QColor _accentDisabledColor;
    
    // 样式
    Style* _sliderStyle = nullptr;
};

#endif // NSLIDER_P_H