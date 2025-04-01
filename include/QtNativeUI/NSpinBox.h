//
// Created by Yang1206 on 2025/4/10.
//

#ifndef NSPINBOX_H
#define NSPINBOX_H

#include <QSpinBox>
#include "stdafx.h"

class NSpinBoxPrivate;
class QTNATIVEUI_EXPORT NSpinBox : public QSpinBox {
    Q_OBJECT
    Q_Q_CREATE(NSpinBox)

  public:
    NSpinBox(QWidget* parent = nullptr);
    ~NSpinBox();

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // NSPINBOX_H