//
// Created by Yang1206 on 2025/12/29.
//

#ifndef NLISTVIEW_H
#define NLISTVIEW_H

#include <QListView>
#include "NColor.h"
#include "stdafx.h"

class NListViewPrivate;
class QTNATIVEUI_EXPORT NListView : public QListView {
    Q_OBJECT
    Q_Q_CREATE(NListView)
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

  public:
    explicit NListView(QWidget* parent = nullptr);
    ~NListView();

  private:
    void init();
};

#endif // NLISTVIEW_H
