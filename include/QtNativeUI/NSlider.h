//
// Created by Yang1206 on 2025/4/10.
//

#ifndef NSLIDER_H
#define NSLIDER_H

#include <QSlider>
#include "stdafx.h"

class NSliderPrivate;
class QTNATIVEUI_EXPORT NSlider : public QSlider {
    Q_OBJECT
    Q_Q_CREATE(NSlider)

  public:
    NSlider(QWidget* parent = nullptr);
    NSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~NSlider();

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // NSLIDER_H