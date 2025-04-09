//
// Created by Yang1206 on 2025/4/20.
//

#ifndef NDOUBLESPINBOX_H
#define NDOUBLESPINBOX_H

#include <QDoubleSpinBox>
#include "stdafx.h"

class NDoubleSpinBoxPrivate;
class QTNATIVEUI_EXPORT NDoubleSpinBox : public QDoubleSpinBox {
    Q_OBJECT
    Q_Q_CREATE(NDoubleSpinBox)
    // 背景颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundDisabledColor)

    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextDisabledColor)

    // 边框颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderDisabledColor)

    // 底边线颜色
    Q_PROPERTY_CREATE_Q_H(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBottomLineColor)

    // 按钮颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightButtonBgColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkButtonBgColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightButtonHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkButtonHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightButtonPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkButtonPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightButtonDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkButtonDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightSelectionBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSelectionBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightSelectionTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSelectionTextColor)
    // 边框属性
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderWidth)

  public:
    explicit NDoubleSpinBox(QWidget* parent = nullptr);
    ~NDoubleSpinBox();
    QLineEdit* getLineEdit() { return lineEdit(); }

  protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

  private:
    void init();
};

#endif // NDOUBLESPINBOX_H