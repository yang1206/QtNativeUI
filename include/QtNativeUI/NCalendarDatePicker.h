#ifndef QTNATIVEUI_NCALENDARDATEPICKER_H
#define QTNATIVEUI_NCALENDARDATEPICKER_H

#include <QDate>
#include <QWidget>
#include "NCalendarWidget.h"
#include "stdafx.h"

class NCalendarDatePickerPrivate;
class QTNATIVEUI_EXPORT NCalendarDatePicker : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NCalendarDatePicker)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QDate, SelectedDate)
    Q_PROPERTY_CREATE_Q_H(QDate, MinimumDate)
    Q_PROPERTY_CREATE_Q_H(QDate, MaximumDate)
    Q_PROPERTY_CREATE_Q_H(QString, PlaceholderText)
    Q_PROPERTY_CREATE_Q_H(QString, DateFormat)

  public:
    explicit NCalendarDatePicker(QWidget* parent = nullptr);
    ~NCalendarDatePicker();

    void                               setLocale(const QLocale& locale);
    QLocale                            locale() const;
    void                               setDateSelectionMode(NCalendarWidget::DateSelectionMode mode);
    NCalendarWidget::DateSelectionMode dateSelectionMode() const;
    // 多选日期支持
    void         setSelectedDates(const QList<QDate>& dates);
    QList<QDate> selectedDates() const;

    // 日期范围支持
    void                setDateRange(const QDate& startDate, const QDate& endDate);
    QPair<QDate, QDate> selectedDateRange() const;

  Q_SIGNALS:
    void dateSelected(QDate date);
    void dateSelectionModeChanged(NCalendarWidget::DateSelectionMode mode);
    void selectedDatesChanged(const QList<QDate>& dates);
    void selectedDateRangeChanged(const QPair<QDate, QDate>& range);
    void popupOpened();
    void popupClosed();

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // QTNATIVEUI_NCALENDARDATEPICKER_H