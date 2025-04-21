#ifndef QTNATIVEUI_NCALENDARWIDGET_H
#define QTNATIVEUI_NCALENDARWIDGET_H

#include <QCalendarWidget>
#include "stdafx.h"

class NCalendarWidgetPrivate;

class QTNATIVEUI_EXPORT NCalendarWidget : public QCalendarWidget {
    Q_OBJECT
    Q_Q_CREATE(NCalendarWidget)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHeaderBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHeaderBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDateTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDateTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDateTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDateTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDateBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDateBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDateBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDateBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDateBackgroundSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDateBackgroundSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHeaderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHeaderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightWeekdayTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkWeekdayTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightWeekendTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkWeekendTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTodayBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTodayBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)

public:
    explicit NCalendarWidget(QWidget* parent = nullptr);
    ~NCalendarWidget();

protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void init();
};

#endif // QTNATIVEUI_NCALENDARWIDGET_H