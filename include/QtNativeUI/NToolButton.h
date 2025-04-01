//
// Created by Yang1206 on 2025/4/1
//

#ifndef NTOOLBUTTON_H
#define NTOOLBUTTON_H

#include <QToolButton>
#include "stdafx.h"

class NToolButtonPrivate;
class QTNATIVEUI_EXPORT NToolButton : public QToolButton {
    Q_OBJECT
    Q_Q_CREATE(NToolButton)

  public:
    NToolButton(QWidget* parent = nullptr);
    ~NToolButton();

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // NTOOLBUTTON_H