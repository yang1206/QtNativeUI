#ifndef NCOMBOBOX_P_H
#define NCOMBOBOX_P_H

#include <QtNativeUI/NComboBox.h>
#include "ncomboboxstyle.h"

class NComboBoxPrivate : public QObject, public NComboBoxStyleInterface {
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

    // 下拉按钮颜色属性
    Q_PROPERTY_CREATE_D(QColor, LightDropdownBgColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownBgColor)
    Q_PROPERTY_CREATE_D(QColor, LightDropdownHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightDropdownPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightDropdownDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightSelectionBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectionBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightSelectionTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectionTextColor)

    // 边框属性
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderWidth)

  public:
    explicit NComboBoxPrivate(QObject* parent = nullptr);
    ~NComboBoxPrivate();
    Q_D_CREATE(NComboBox)

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};
    int                   _shadowBorderWidth{3};
    bool                  _isDropdownVisible{false};

    NComboBoxStyle* _comboBoxStyle{nullptr};

    // NComboBoxStyleInterface实现
    QColor backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const override;
    QColor borderColorForState(bool isDark, bool isEnabled) const override;
    QColor bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const override;
    QColor dropdownButtonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const override;
    QColor textColorForState(bool isDark, bool isEnabled) const override;
    QColor selectionBackgroundColor(bool isDark) const override;
    QColor selectionTextColor(bool isDark) const override;
    int    borderRadius() const override;
    int    borderWidth() const override;
    int    bottomLineWidth(bool hasFocus) const override;
    bool   isDarkMode() const override;
    int    shadowBorderWidth() const override;
    bool   isDropdownVisible() const override;
};

#endif // NCOMBOBOX_P_H