#ifndef QTNATIVEUI_NDATEPICKER_H
#define QTNATIVEUI_NDATEPICKER_H

#include <QDate>
#include <QPushButton>
#include "stdafx.h"

class NDatePickerPrivate;

/**
 * @brief 日期选择器组件
 */
class QTNATIVEUI_EXPORT NDatePicker : public QPushButton {
    Q_OBJECT
    Q_Q_CREATE(NDatePicker)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QDate, Date)
    Q_PROPERTY_CREATE_Q_H(QDate, MinimumDate)
    Q_PROPERTY_CREATE_Q_H(QDate, MaximumDate)
    Q_PROPERTY_CREATE_Q_H(QLocale, Locale)

  public:
    explicit NDatePicker(QWidget* parent = nullptr);
    ~NDatePicker() override;

  signals:
    void dateChanged(const QDate& date);
    void localeChanged(const QLocale& locale);

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // QTNATIVEUI_NDATEPICKER_H
