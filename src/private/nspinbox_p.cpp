//
// Created by Yang1206 on 2025/4/10.
//

#include "nspinbox_p.h"
#include "QtNativeUI/NSpinBox.h"
#include "QtNativeUI/NTheme.h"

NSpinBoxPrivate::NSpinBoxPrivate(QObject* parent) : QObject(parent) {}

NSpinBoxPrivate::~NSpinBoxPrivate() {}

QColor NSpinBoxPrivate::backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const {
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

QColor NSpinBoxPrivate::borderColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkBorderDisabledColor : _pLightBorderDisabledColor;
    } else {
        return isDark ? _pDarkBorderColor : _pLightBorderColor;
    }
}

QColor NSpinBoxPrivate::bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const {
    if (hasFocus && isEnabled) {
        return nTheme->accentColor().normal();
    } else {
        return isDark ? _pDarkBottomLineColor : _pLightBottomLineColor;
    }
}

QColor NSpinBoxPrivate::buttonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const {
    if (!isEnabled) {
        return isDark ? _pDarkButtonDisabledColor : _pLightButtonDisabledColor;
    } else if (isPressed) {
        return isDark ? _pDarkButtonPressColor : _pLightButtonPressColor;
    } else if (isHovered) {
        return isDark ? _pDarkButtonHoverColor : _pLightButtonHoverColor;
    } else {
        return isDark ? _pDarkButtonBgColor : _pLightButtonBgColor;
    }
}

int NSpinBoxPrivate::borderRadius() const {
    return _pBorderRadius;
}

int NSpinBoxPrivate::borderWidth() const {
    return _pBorderWidth;
}

int NSpinBoxPrivate::bottomLineWidth(bool hasFocus) const {
    return hasFocus ? 2 : 1;
}

bool NSpinBoxPrivate::isDarkMode() const {
    return _isDark;
}

int NSpinBoxPrivate::shadowBorderWidth() const {
    return _shadowBorderWidth;
}