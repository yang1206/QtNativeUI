#ifndef QTNATIVEUI_NTIMEPICKER_H
#define QTNATIVEUI_NTIMEPICKER_H

#include <QPushButton>
#include <QTime>
#include "stdafx.h"

class NTimePickerPrivate;

/**
 * @brief 时间选择器组件
 */
class QTNATIVEUI_EXPORT NTimePicker : public QPushButton {
    Q_OBJECT
    Q_Q_CREATE(NTimePicker)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QTime, Time)
    Q_PROPERTY_CREATE_Q_H(bool, Use24HourFormat)
    Q_PROPERTY_CREATE_Q_H(bool, ShowSeconds)

  public:
    explicit NTimePicker(QWidget* parent = nullptr);
    ~NTimePicker() override;

  signals:
    void timeChanged(const QTime& time);

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // QTNATIVEUI_NTIMEPICKER_H
