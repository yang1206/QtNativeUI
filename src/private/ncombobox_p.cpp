#include "ncombobox_p.h"
#include "QtNativeUI/NComboBox.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NAnimation.h"

NComboBoxPrivate::NComboBoxPrivate(QObject* parent) : QObject(parent), _arrowYOffset(0) {}

NComboBoxPrivate::~NComboBoxPrivate() {
    if (_arrowAnimation) {
        delete _arrowAnimation;
        _arrowAnimation = nullptr;
    }
}

QColor NComboBoxPrivate::backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const {
    if (!isEnabled) {
        return isDark ? _pDarkBackgroundDisabledColor : _pLightBackgroundDisabledColor;
    } else if (hasFocus) {
        return isDark ? _pDarkBackgroundFocusColor : _pLightBackgroundFocusColor;
    } else if (hasHover) {
        return isDark ? _pDarkBackgroundHoverColor : _pLightBackgroundHoverColor;
    } else {
        return isDark ? _pDarkBackgroundColor : _pLightBackgroundColor;
    }
}

QColor NComboBoxPrivate::borderColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkBorderDisabledColor : _pLightBorderDisabledColor;
    } else {
        return isDark ? _pDarkBorderColor : _pLightBorderColor;
    }
}

QColor NComboBoxPrivate::bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const {
    if (hasFocus && isEnabled) {
        return nTheme->accentColor().normal();
    } else {
        return isDark ? _pDarkBottomLineColor : _pLightBottomLineColor;
    }
}

QColor
NComboBoxPrivate::dropdownButtonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const {
    if (!isEnabled) {
        return isDark ? _pDarkDropdownButtonDisabledColor : _pLightDropdownButtonDisabledColor;
    } else if (isPressed) {
        return isDark ? _pDarkDropdownButtonPressColor : _pLightDropdownButtonPressColor;
    } else if (isHovered) {
        return isDark ? _pDarkDropdownButtonHoverColor : _pLightDropdownButtonHoverColor;
    } else {
        return isDark ? _pDarkDropdownButtonBgColor : _pLightDropdownButtonBgColor;
    }
}

QColor NComboBoxPrivate::textColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkTextDisabledColor : _pLightTextDisabledColor;
    }
    return isDark ? _pDarkTextColor : _pLightTextColor;
}

QColor NComboBoxPrivate::selectionBackgroundColor(bool isDark) const {
    return isDark ? _pDarkSelectionBackgroundColor : _pLightSelectionBackgroundColor;
}

QColor NComboBoxPrivate::selectionTextColor(bool isDark) const {
    return isDark ? _pDarkSelectionTextColor : _pLightSelectionTextColor;
}

int NComboBoxPrivate::borderRadius() const { return _pBorderRadius; }

int NComboBoxPrivate::borderWidth() const { return _pBorderWidth; }

int NComboBoxPrivate::bottomLineWidth(bool hasFocus) const { return hasFocus ? 2 : 1; }

bool NComboBoxPrivate::isDarkMode() const { return _isDark; }

int NComboBoxPrivate::shadowBorderWidth() const { return _shadowBorderWidth; }

bool NComboBoxPrivate::isDropdownVisible() const { return _isDropdownVisible; }