#ifndef QTNATIVEUI_NTABBAR_P_H
#define QTNATIVEUI_NTABBAR_P_H

#include <QObject>
#include <QPixmap>
#include <QtNativeUI/NTabBar.h>
#include "QtNativeUI/NEnums.h"

class NTabBarPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabBar)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(bool, OverlayMode)
    Q_PROPERTY_CREATE_D(QColor, SelectedTextColor)
    Q_PROPERTY_CREATE_D(QColor, UnselectedTextColor)
    Q_PROPERTY_CREATE_D(QColor, HoverTextColor)
    Q_PROPERTY_CREATE_D(QColor, DisabledTextColor)
    Q_PROPERTY_CREATE_D(QColor, SelectedBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, HoverBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, BorderColor)
    Q_PROPERTY_CREATE_D(QColor, SeparatorColor)

  public:
    explicit NTabBarPrivate(QObject* parent = nullptr);
    ~NTabBarPrivate();

    // 主题相关
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

    // 交互状态
    int     _hoverIndex{-1};
    QPixmap _lastDragPix; // 保存拖拽的标签图像
};

#endif // QTNATIVEUI_NTABBAR_P_H