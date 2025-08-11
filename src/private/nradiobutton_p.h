//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NRADIOBUTTON_P_H
#define NRADIOBUTTON_P_H
#include <QPropertyAnimation>
#include <QtNativeUI/NRadioButton.h>

#include "QtNativeUI/NEnums.h"

class NRadioButtonPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal innerCircleScale READ innerCircleScale WRITE setInnerCircleScale)

    // 状态颜色
    Q_PROPERTY_CREATE_D(QColor, LightOuterCircleDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkOuterCircleDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightOuterCircleCheckedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkOuterCircleCheckedColor)
    Q_PROPERTY_CREATE_D(QColor, LightOuterCircleBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkOuterCircleBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightInnerCircleColor)
    Q_PROPERTY_CREATE_D(QColor, DarkInnerCircleColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledColor)

    // 尺寸和形状
    Q_PROPERTY_CREATE_D(int, OuterCircleSize)
    Q_PROPERTY_CREATE_D(int, OuterCircleBorderWidth)
    Q_PROPERTY_CREATE_D(int, InnerCircleSize)
    Q_PROPERTY_CREATE_D(int, TextPadding)

  public:
    explicit NRadioButtonPrivate(QObject* parent = nullptr);
    ~NRadioButtonPrivate();
    Q_D_CREATE(NRadioButton)

    qreal innerCircleScale() const { return _innerCircleScale; }
    void  setInnerCircleScale(qreal scale) {
        _innerCircleScale = scale;
        q_ptr->update();
    }

    void startInnerCircleAnimation(qreal startScale, qreal endScale);

  public:
    // 状态跟踪
    bool                  _isHovered = false;
    bool                  _isPressed = false;
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark = false;

    // 动画相关
    qreal               _innerCircleScale     = 1.0;
    QPropertyAnimation* _innerCircleAnimation = nullptr;

    // 强调色
    QColor _accentColor;
    QColor _accentHoverColor;
    QColor _accentPressedColor;
    QColor _accentDisabledColor;
    
    mutable QColor _cachedOuterCircleColor;
    mutable QColor _cachedOuterBorderColor;
    mutable QColor _cachedInnerCircleColor;
    mutable QColor _cachedTextColor;
    mutable bool _outerCircleColorCacheValid = false;
    mutable bool _outerBorderColorCacheValid = false;
    mutable bool _innerCircleColorCacheValid = false;
    mutable bool _textColorCacheValid = false;
    
    void invalidateColorCache() {
        _outerCircleColorCacheValid = false;
        _outerBorderColorCacheValid = false;
        _innerCircleColorCacheValid = false;
        _textColorCacheValid = false;
    }
};

#endif // NRADIOBUTTON_P_H
