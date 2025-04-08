//
// Created by Yang1206 on 2025/4/10.
//

#ifndef NSPINBOX_P_H
#define NSPINBOX_P_H
#include <QtNativeUI/NSpinBox.h>
#include "nspinboxstyle.h"

class NSpinBoxPrivate : public QObject, public NSpinBoxStyleInterface {
    Q_OBJECT
    // 背景颜色属性
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundFocusColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundFocusColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundDisabledColor)

    // 文字颜色属性
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextDisabledColor)

    // 边框颜色属性
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderDisabledColor)

    // 底边线颜色
    Q_PROPERTY_CREATE_D(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBottomLineColor)

    // 按钮颜色属性
    Q_PROPERTY_CREATE_D(QColor, LightButtonBgColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonBgColor)
    Q_PROPERTY_CREATE_D(QColor, LightButtonHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightButtonPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightButtonDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonDisabledColor)

    // 边框属性
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderWidth)

  public:
    explicit NSpinBoxPrivate(QObject* parent = nullptr);
    ~NSpinBoxPrivate();
    Q_D_CREATE(NSpinBox)

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};
    int                   _shadowBorderWidth{3};

    NSpinBoxStyle* _spinBoxStyle{nullptr};

    QColor backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const override;
    QColor borderColorForState(bool isDark, bool isEnabled) const override;
    QColor bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const override;
    QColor buttonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const override;
    QColor textColorForState(bool isDark, bool isEnabled) const override;
    int    borderRadius() const override;
    int    borderWidth() const override;
    int    bottomLineWidth(bool hasFocus) const override;
    bool   isDarkMode() const override;
    int    shadowBorderWidth() const override;
};

#endif // NSPINBOX_P_H