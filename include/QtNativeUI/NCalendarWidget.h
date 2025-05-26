#ifndef QTNATIVEUI_NCALENDARWIDGET_H
#define QTNATIVEUI_NCALENDARWIDGET_H

#include <QDate>
#include <QWidget>
#include "stdafx.h"
class NCalendarWidgetPrivate;
class QTNATIVEUI_EXPORT NCalendarWidget : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NCalendarWidget)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QDate, SelectedDate)
    Q_PROPERTY_CREATE_Q_H(QDate, MinimumDate)
    Q_PROPERTY_CREATE_Q_H(QDate, MaximumDate)

  public:
    explicit NCalendarWidget(QWidget* parent = nullptr);
    ~NCalendarWidget();

    void    setLocale(const QLocale& locale);
    QLocale locale() const;

    enum DateSelectionMode {
        SingleDate,   // 单日期选择
        MultipleDate, // 多日期选择
        DateRange     // 日期范围选择
    };
    Q_ENUM(DateSelectionMode)

    void              setDateSelectionMode(DateSelectionMode mode);
    DateSelectionMode dateSelectionMode() const;

    // 多选日期支持
    void         setSelectedDates(const QList<QDate>& dates);
    QList<QDate> selectedDates() const;

    // 日期范围支持
    void                setDateRange(const QDate& startDate, const QDate& endDate);
    QPair<QDate, QDate> selectedDateRange() const;

  Q_SIGNALS:
    void clicked(QDate date);
    void localeChanged(const QLocale& locale);
    void dateSelectionModeChanged(DateSelectionMode mode);
    void selectedDatesChanged(const QList<QDate>& dates);
    void selectedDateRangeChanged(const QPair<QDate, QDate>& range);

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    void init();
};

#endif // QTNATIVEUI_NCALENDARWIDGET_H