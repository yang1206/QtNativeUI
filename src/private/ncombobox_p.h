#ifndef NCOMBOBOX_P_H
#define NCOMBOBOX_P_H

#include <QtNativeUI/NAnimation.h>
#include <QtNativeUI/NComboBox.h>
#include "ncomboboxstyle.h"

class NComboBoxPrivate : public QObject, public NComboBoxStyleInterface {
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

    Q_PROPERTY_CREATE_D(QColor, LightDropdownButtonBgColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownButtonBgColor)
    Q_PROPERTY_CREATE_D(QColor, LightDropdownButtonHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownButtonHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightDropdownButtonPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownButtonPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightDropdownButtonDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDropdownButtonDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightSelectionBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectionBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightSelectionTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectionTextColor)

    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderWidth)

    Q_PROPERTY(qreal arrowYOffset READ arrowYOffset WRITE setArrowYOffset)

  public:
    explicit NComboBoxPrivate(QObject* parent = nullptr);
    ~NComboBoxPrivate();
    Q_D_CREATE(NComboBox)

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};
    int                   _shadowBorderWidth{3};
    bool                  _isDropdownVisible{false};

    NComboBoxStyle* _comboBoxStyle{nullptr};


    QtNativeUI::NTranslateYAnimation* _arrowAnimation{nullptr};

    qreal arrowYOffset() const { return _arrowYOffset; }
    void  setArrowYOffset(qreal offset) {
        _arrowYOffset = offset;
        if (q_ptr) {
            q_ptr->update();
        }
    }

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
    qreal  getArrowYOffset() const override { return _arrowYOffset; }

  private:
    qreal _arrowYOffset{0};
};

#endif // NCOMBOBOX_P_H