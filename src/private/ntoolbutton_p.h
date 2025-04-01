//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NTOOLBUTTON_P_H
#define NTOOLBUTTON_P_H
#include <QtNativeUI/NToolButton.h>

class NToolButtonPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NToolButtonPrivate(QObject* parent = nullptr);
    ~NToolButtonPrivate();
    Q_D_CREATE(NToolButton)
};

#endif // NTOOLBUTTON_P_H