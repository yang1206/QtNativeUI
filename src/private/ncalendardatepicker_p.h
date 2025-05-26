#ifndef NCALENDARDATEPICKER_P_H
#define NCALENDARDATEPICKER_P_H

#include <QDate>
#include <QLabel>
#include <QLocale>
#include "QtNativeUI/NCalendarDatePicker.h"
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/NTheme.h"

class NCalendarDatePickerPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NCalendarDatePickerPrivate(NCalendarDatePicker* q);
    ~NCalendarDatePickerPrivate();

    void initUi();
    void updateDisplayText();

    NCalendarDatePicker* q_ptr;
    NPushButton*         button;
    QWidget*             contentWidget;
    QLabel*              dateLabel;
    QLabel*              iconLabel;

    QDate   _pSelectedDate;
    QDate   _pMinimumDate;
    QDate   _pMaximumDate;
    QString _pPlaceholderText;
    QString _pDateFormat;
    int     _pBorderRadius;
    QLocale _locale;
    bool    _isAccentStyle;

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    // 多选相关
    QList<QDate>                       _selectedDates;
    QPair<QDate, QDate>                _selectedDateRange;
    NCalendarWidget::DateSelectionMode _selectionMode;

    void showCalendarFlyout();
    void handleDateSelection(const QDate& date);
    void handleMultiDateSelection(const QList<QDate>& dates);
    void handleDateRangeSelection(const QPair<QDate, QDate>& range);
};

#endif // NCALENDARDATEPICKER_P_H