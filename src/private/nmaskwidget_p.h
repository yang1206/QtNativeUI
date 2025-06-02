#ifndef QTNATIVEUI_NMASKWIDGET_P_H
#define QTNATIVEUI_NMASKWIDGET_P_H

#include <QWidget>
#include "QtNativeUI/stdafx.h"

class QTNATIVEUI_EXPORT NMaskWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MaskAlpha)
  public:
    explicit NMaskWidget(QWidget* parent = nullptr);
    ~NMaskWidget();
    void doMaskAnimation(int endValue);

  protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // QTNATIVEUI_NMASKWIDGET_P_H