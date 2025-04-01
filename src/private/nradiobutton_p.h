//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NRADIOBUTTON_P_H
#define NRADIOBUTTON_P_H
#include <QtNativeUI/NRadioButton.h>

class NRadioButtonPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NRadioButtonPrivate(QObject* parent = nullptr);
    ~NRadioButtonPrivate();
    Q_D_CREATE(NRadioButton)
};

#endif // NRADIOBUTTON_P_H
