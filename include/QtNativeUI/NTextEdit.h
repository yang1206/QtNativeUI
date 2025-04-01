//
// Created by Yang1206 on 2025/4/1.
//

#ifndef NTEXTEDIT_H
#define NTEXTEDIT_H

#include <QTextEdit>
#include "stdafx.h"

class NTextEditPrivate;
class QTNATIVEUI_EXPORT NTextEdit : public QTextEdit {
    Q_OBJECT
    Q_Q_CREATE(NTextEdit)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBottomLineColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderWidth)

  public:
    NTextEdit(QWidget* parent = nullptr);
    NTextEdit(const QString& text, QWidget* parent = nullptr);
    ~NTextEdit();

  protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

  private:
    void init();
};

#endif // NTEXTEDIT_H