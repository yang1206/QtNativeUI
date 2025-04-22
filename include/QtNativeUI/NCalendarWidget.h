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

  Q_SIGNALS:
    void clicked(QDate date);

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // QTNATIVEUI_NCALENDARWIDGET_H