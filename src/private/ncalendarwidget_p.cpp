#include "ncalendarwidget_p.h"
#include "QtNativeUI/NTheme.h"

NCalendarWidgetPrivate::NCalendarWidgetPrivate(QObject* parent) : QObject(parent) {}

NCalendarWidgetPrivate::~NCalendarWidgetPrivate() {}

QColor NCalendarWidgetPrivate::backgroundColorForState(bool isDark) const {
    return isDark ? _pDarkBackgroundColor : _pLightBackgroundColor;
}

QColor NCalendarWidgetPrivate::headerBackgroundColorForState(bool isDark) const {
    return isDark ? _pDarkHeaderBackgroundColor : _pLightHeaderBackgroundColor;
}

QColor NCalendarWidgetPrivate::dateTextColorForState(bool isDark, bool isEnabled) const {
    if (!isEnabled) {
        return isDark ? _pDarkDateTextDisabledColor : _pLightDateTextDisabledColor;
    }
    return isDark ? _pDarkDateTextColor : _pLightDateTextColor;
}

QColor NCalendarWidgetPrivate::dateBackgroundColorForState(bool isDark, bool isSelected, bool isHovered) const {
    if (isSelected) {
        return isDark ? _pDarkDateBackgroundSelectedColor : _pLightDateBackgroundSelectedColor;
    } else if (isHovered) {
        return isDark ? _pDarkDateBackgroundHoverColor : _pLightDateBackgroundHoverColor;
    }
    return isDark ? _pDarkDateBackgroundColor : _pLightDateBackgroundColor;
}

QColor NCalendarWidgetPrivate::headerTextColorForState(bool isDark) const {
    return isDark ? _pDarkHeaderTextColor : _pLightHeaderTextColor;
}

QColor NCalendarWidgetPrivate::weekdayTextColorForState(bool isDark) const {
    return isDark ? _pDarkWeekdayTextColor : _pLightWeekdayTextColor;
}

QColor NCalendarWidgetPrivate::weekendTextColorForState(bool isDark) const {
    return isDark ? _pDarkWeekendTextColor : _pLightWeekendTextColor;
}

QColor NCalendarWidgetPrivate::todayBackgroundColorForState(bool isDark) const {
    return isDark ? _pDarkTodayBackgroundColor : _pLightTodayBackgroundColor;
}

int NCalendarWidgetPrivate::borderRadius() const { return _pBorderRadius; }

bool NCalendarWidgetPrivate::isDarkMode() const { return _isDark; }