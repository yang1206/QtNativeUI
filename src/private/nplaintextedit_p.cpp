#include "nplaintextedit_p.h"
#include "QtNativeUI/NTheme.h"

NPlainTextEditPrivate::NPlainTextEditPrivate(QObject* parent) : QObject(parent) {}

NPlainTextEditPrivate::~NPlainTextEditPrivate() {}

QColor NPlainTextEditPrivate::backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const {
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

QColor NPlainTextEditPrivate::borderColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkBorderDisabledColor : _pLightBorderDisabledColor;
    } else {
        return isDark ? _pDarkBorderColor : _pLightBorderColor;
    }
}

QColor NPlainTextEditPrivate::bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const {
    if (hasFocus && isEnabled) {
        return nTheme->accentColor().normal();
    } else {
        return isDark ? _pDarkBottomLineColor : _pLightBottomLineColor;
    }
}

QColor NPlainTextEditPrivate::textColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkTextDisabledColor : _pLightTextDisabledColor;
    } else {
        return isDark ? _pDarkTextColor : _pLightTextColor;
    }
}

int NPlainTextEditPrivate::borderRadius() const { return _pBorderRadius; }

int NPlainTextEditPrivate::borderWidth() const { return _pBorderWidth; }

int NPlainTextEditPrivate::bottomLineWidth(bool hasFocus) const { return hasFocus ? 2 : 1; }

bool NPlainTextEditPrivate::isDarkMode() const { return _isDark; }

int NPlainTextEditPrivate::shadowBorderWidth() const { return _shadowBorderWidth; }