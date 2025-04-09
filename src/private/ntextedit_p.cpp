//
// Created by Yang1206 on 2025/4/1.
//

#include "ntextedit_p.h"
#include "QtNativeUI/NTextEdit.h"
#include "QtNativeUI/NTheme.h"

NTextEditPrivate::NTextEditPrivate(QObject* parent) : QObject(parent) {}

NTextEditPrivate::~NTextEditPrivate() {}

QColor NTextEditPrivate::backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const {
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

QColor NTextEditPrivate::borderColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkBorderDisabledColor : _pLightBorderDisabledColor;
    } else {
        return isDark ? _pDarkBorderColor : _pLightBorderColor;
    }
}

QColor NTextEditPrivate::bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const {
    if (hasFocus && isEnabled) {
        return nTheme->accentColor().normal();
    } else {
        return isDark ? _pDarkBottomLineColor : _pLightBottomLineColor;
    }
}

QColor NTextEditPrivate::textColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkTextDisabledColor : _pLightTextDisabledColor;
    } else {
        return isDark ? _pDarkTextColor : _pLightTextColor;
    }
}

QColor NTextEditPrivate::selectionBackgroundColor(bool isDark) const {
    return isDark ? _pDarkSelectionBackgroundColor : _pLightSelectionBackgroundColor;
}

QColor NTextEditPrivate::selectionTextColor(bool isDark) const {
    return isDark ? _pDarkSelectionTextColor : _pLightSelectionTextColor;
}

int NTextEditPrivate::borderRadius() const { return _pBorderRadius; }

int NTextEditPrivate::borderWidth() const { return _pBorderWidth; }

int NTextEditPrivate::bottomLineWidth(bool hasFocus) const { return hasFocus ? 2 : 1; }

bool NTextEditPrivate::isDarkMode() const { return _isDark; }

int NTextEditPrivate::shadowBorderWidth() const { return _shadowBorderWidth; }