//
// Created by Yang1206 on 2025/4/10.
//

#ifndef NSPINBOX_P_H
#define NSPINBOX_P_H
#include <QtNativeUI/NSpinBox.h>

class NSpinBoxPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NSpinBoxPrivate(QObject* parent = nullptr);
    ~NSpinBoxPrivate();
    Q_D_CREATE(NSpinBox)
};

#endif // NSPINBOX_P_H