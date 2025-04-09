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
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBottomLineColor)
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
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderWidth)

  public:
    explicit NSpinBox(QWidget* parent = nullptr);
    ~NSpinBox();
    QLineEdit* getLineEdit() { return lineEdit(); }

  protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

  private:
    void init();
};

#endif // NSPINBOX_H