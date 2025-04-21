#ifndef QTNATIVEUI_NCALENDARWIDGET_P_H
#define QTNATIVEUI_NCALENDARWIDGET_P_H

#include <QObject>
#include <QtNativeUI/NCalendarWidget.h>
#include "QtNativeUI/NEnums.h"
#include "ncalendarwidgetstyle.h"

class NCalendarWidgetPrivate : public QObject, public NCalendarWidgetStyleInterface {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightHeaderBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHeaderBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightDateTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDateTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightDateTextDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDateTextDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, LightDateBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDateBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightDateBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDateBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightDateBackgroundSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDateBackgroundSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, LightHeaderTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHeaderTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightWeekdayTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkWeekdayTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightWeekendTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkWeekendTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightTodayBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTodayBackgroundColor)
    Q_PROPERTY_CREATE_D(int, BorderRadius)

  public:
    explicit NCalendarWidgetPrivate(QObject* parent = nullptr);
    ~NCalendarWidgetPrivate();
    Q_D_CREATE(NCalendarWidget)
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

    NCalendarWidgetStyle* _calendarStyle{nullptr};

    QColor backgroundColorForState(bool isDark) const override;
    QColor headerBackgroundColorForState(bool isDark) const override;
    QColor dateTextColorForState(bool isDark, bool isEnabled) const override;
    QColor dateBackgroundColorForState(bool isDark, bool isSelected, bool isHovered) const override;
    QColor headerTextColorForState(bool isDark) const override;
    QColor weekdayTextColorForState(bool isDark) const override;
    QColor weekendTextColorForState(bool isDark) const override;
    QColor todayBackgroundColorForState(bool isDark) const override;
    int    borderRadius() const override;
    bool   isDarkMode() const override;
};

#endif // QTNATIVEUI_NCALENDARWIDGET_P_H