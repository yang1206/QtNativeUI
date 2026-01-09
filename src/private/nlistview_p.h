//
// Created by Yang1206 on 2025/12/29.
//

#ifndef NLISTVIEW_P_H
#define NLISTVIEW_P_H

#include <QtNativeUI/NListView.h>
#include "QtNativeUI/NEnums.h"
#include "nlistviewstyle_p.h"

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
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, ItemBorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderRadius)

  public:
    explicit NListViewPrivate(QObject* parent = nullptr);
    ~NListViewPrivate();
    Q_D_CREATE(NListView)

    NListViewStyle*       style{nullptr};
    bool                  isDark{false};
    NThemeType::ThemeMode themeMode;

    void updateStyle();
    void initStyle();
};

#endif // NLISTVIEW_P_H
