//
// Created by Yang1206 on 2025/4/20.
//

#include "ndoublespinbox_p.h"
#include "QtNativeUI/NDoubleSpinBox.h"
#include "QtNativeUI/NTheme.h"

NDoubleSpinBoxPrivate::NDoubleSpinBoxPrivate(QObject* parent) : QObject(parent) {}

NDoubleSpinBoxPrivate::~NDoubleSpinBoxPrivate() {}

QColor NDoubleSpinBoxPrivate::backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const {
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

QColor NDoubleSpinBoxPrivate::borderColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkBorderDisabledColor : _pLightBorderDisabledColor;
    } else {
        return isDark ? _pDarkBorderColor : _pLightBorderColor;
    }
}

QColor NDoubleSpinBoxPrivate::bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const {
    if (hasFocus && isEnabled) {
        return nTheme->accentColor().normal();
    } else {
        return isDark ? _pDarkBottomLineColor : _pLightBottomLineColor;
    }
}

QColor NDoubleSpinBoxPrivate::buttonBackgroundColor(bool isDark, bool isEnabled, bool isHovered, bool isPressed) const {
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

int NDoubleSpinBoxPrivate::borderRadius() const { return _pBorderRadius; }

int NDoubleSpinBoxPrivate::borderWidth() const { return _pBorderWidth; }

int NDoubleSpinBoxPrivate::bottomLineWidth(bool hasFocus) const { return hasFocus ? 2 : 1; }

bool NDoubleSpinBoxPrivate::isDarkMode() const { return _isDark; }

int NDoubleSpinBoxPrivate::shadowBorderWidth() const { return _shadowBorderWidth; } 