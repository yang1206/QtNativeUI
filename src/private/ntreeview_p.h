//
// Created by Yang1206 on 2025/12/30.
//

#ifndef NTREEVIEW_P_H
#define NTREEVIEW_P_H

#include <QtNativeUI/NTreeView.h>
#include "QtNativeUI/NEnums.h"
#include "ntreeviewstyle_p.h"

class NTreeViewPrivate : public QObject {
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
    Q_PROPERTY_CREATE_D(int, Indentation)

  public:
    explicit NTreeViewPrivate(QObject* parent = nullptr);
    ~NTreeViewPrivate();
    Q_D_CREATE(NTreeView)

    NTreeViewStyle*       style{nullptr};
    bool                  isDark{false};
    NThemeType::ThemeMode themeMode;

    void updateStyle();
    void initStyle();
};

#endif // NTREEVIEW_P_H
