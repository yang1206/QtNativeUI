//
// Created by Yang1206 on 2025/12/29.
//

#ifndef NLISTVIEW_P_H
#define NLISTVIEW_P_H

#include <QMetaObject>
#include <QObject>
#include <QPersistentModelIndex>
#include <QVector>

#include <QtNativeUI/NListView.h>
#include "QtNativeUI/NEnums.h"
#include "nlistviewstyle_p.h"

class NListItemDelegate;
class NListViewSelectMarkController;

class NListViewPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemSelectedColor)
    Q_PROPERTY_CREATE_D(QColor, LightItemPressedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkItemPressedColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightPlaceholderTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPlaceholderTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, ItemBorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, PlaceholderText)
    Q_PROPERTY_CREATE_D(bool, BorderVisible)
    Q_PROPERTY_CREATE_D(bool, BackgroundVisible)
    Q_PROPERTY_CREATE_D(bool, SelectionIndicatorVisible)

  public:
    explicit NListViewPrivate(QObject* parent = nullptr);
    ~NListViewPrivate() override;
    Q_D_CREATE(NListView)

  public:
    QString                          automaticAccessibleDescription;
    QPersistentModelIndex            pressedIndex;
    QVector<QMetaObject::Connection> modelConnections;
    NListViewStyle*                  style{nullptr};
    NListItemDelegate*               itemDelegate{nullptr};
    NListViewSelectMarkController*   selectMarkController{nullptr};
    bool                             isDark{false};
    NThemeType::ThemeMode            themeMode{NThemeType::Light};

    QColor itemHoverColor() const;
    QColor itemSelectedColor() const;
    QColor itemPressedColor() const;
    QColor textColor() const;
    QColor placeholderTextColor() const;
    int    itemHeight() const { return _pItemHeight; }
    int    itemBorderRadius() const { return _pItemBorderRadius; }

    void updateStyle();
    void initStyle();
};

#endif // NLISTVIEW_P_H
