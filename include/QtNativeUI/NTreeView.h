//
// Created by Yang1206 on 2025/12/30.
//

#ifndef NTREEVIEW_H
#define NTREEVIEW_H

#include <QTreeView>
#include "NColor.h"
#include "stdafx.h"

class NTreeViewPrivate;
class QTNATIVEUI_EXPORT NTreeView : public QTreeView {
    Q_OBJECT
    Q_Q_CREATE(NTreeView)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemPressedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemPressedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, ItemBorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, Indentation)

  public:
    explicit NTreeView(QWidget* parent = nullptr);
    ~NTreeView();

  private:
    void init();
};

#endif // NTREEVIEW_H
