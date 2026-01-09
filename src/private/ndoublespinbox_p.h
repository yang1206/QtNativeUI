//
// Created by Yang1206 on 2025/4/20.
//

#ifndef NDOUBLESPINBOX_P_H
#define NDOUBLESPINBOX_P_H
#include <QtNativeUI/NDoubleSpinBox.h>
#include "nspinboxstyle.h"

class NDoubleSpinBoxPrivate : public QObject, public NSpinBoxStyleInterface {
    Q_OBJECT

    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundFocusColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundFocusColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBottomLineColor)

    Q_PROPERTY_CREATE_D(QColor, LightButtonBgColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonBgColor)
    Q_PROPERTY_CREATE_D(QColor, LightButtonHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightButtonPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightButtonDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkButtonDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightSelectionBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectionBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightSelectionTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectionTextColor)

    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderWidth)

  public:
    explicit NDoubleSpinBoxPrivate(QObject* parent = nullptr);
    ~NDoubleSpinBoxPrivate();
    Q_D_CREATE(NDoubleSpinBox)

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};
    int                   _shadowBorderWidth{3};

    NSpinBoxStyle* _spinBoxStyle{nullptr};

    QColor backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const override;
    QColor borderColorForState(bool isDark, bool isEnabled) const override;
    QColor bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const override;
    QColor buttonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const override;
    QColor textColorForState(bool isDark, bool isEnabled) const override;
    QColor selectionBackgroundColor(bool isDark) const override;
    QColor selectionTextColor(bool isDark) const override;
    int    borderRadius() const override;
    int    borderWidth() const override;
    int    bottomLineWidth(bool hasFocus) const override;
    bool   isDarkMode() const override;
    int    shadowBorderWidth() const override;
};

#endif // NDOUBLESPINBOX_P_H