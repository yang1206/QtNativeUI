//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NRADIOBUTTON_H
#define NRADIOBUTTON_H

#include <QRadioButton>
#include "stdafx.h"

class NRadioButtonPrivate;
class QTNATIVEUI_EXPORT NRadioButton : public QRadioButton {
    Q_OBJECT
    Q_Q_CREATE(NRadioButton)

  public:
    NRadioButton(QWidget* parent = nullptr);
    NRadioButton(const QString& text, QWidget* parent = nullptr);
    ~NRadioButton();

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // NRADIOBUTTON_H
