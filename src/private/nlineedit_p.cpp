#include "nlineedit_p.h"
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionFrame>
#include "QtNativeUI/NTheme.h"

NLineEditPrivate::NLineEditPrivate(QObject* parent) : QObject(parent) {}

NLineEditPrivate::~NLineEditPrivate() {}

QColor NLineEditPrivate::backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const {
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

QColor NLineEditPrivate::borderColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkBorderDisabledColor : _pLightBorderDisabledColor;
    } else {
        return isDark ? _pDarkBorderColor : _pLightBorderColor;
    }
}

QColor NLineEditPrivate::bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const {
    if (hasFocus && isEnabled) {
        return nTheme->accentColor().normal();
    } else {
        return isDark ? _pDarkBottomLineColor : _pLightBottomLineColor;
    }
}

QColor NLineEditPrivate::textColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkTextDisabledColor : _pLightTextDisabledColor;
    }
    return isDark ? _pDarkTextColor : _pLightTextColor;
}

int NLineEditPrivate::borderRadius() const { return _pBorderRadius; }

int NLineEditPrivate::borderWidth() const { return _pBorderWidth; }

int NLineEditPrivate::bottomLineWidth(bool hasFocus) const { return hasFocus ? 2 : 1; }

bool NLineEditPrivate::isDarkMode() const { return _isDark; }

int NLineEditPrivate::shadowBorderWidth() const { return _shadowBorderWidth; }