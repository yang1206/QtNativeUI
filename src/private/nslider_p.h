//
// Created by Yang1206 on 2025/4/10.
//

#ifndef NSLIDER_P_H
#define NSLIDER_P_H
#include <QtNativeUI/NSlider.h>

class NSliderPrivate : public QObject {
    Q_OBJECT
  public:
    explicit NSliderPrivate(QObject* parent = nullptr);
    ~NSliderPrivate();
    Q_D_CREATE(NSlider)
};

#endif // NSLIDER_P_H