#ifndef QTNATIVEUI_NTABBAR_P_H
#define QTNATIVEUI_NTABBAR_P_H

#include <QObject>
#include <QPropertyAnimation>
#include <QtNativeUI/NTabBar.h>
#include "QtNativeUI/NEnums.h"

class NTabBarPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabBar)

    // 私有属性
    // ...

  public:
    explicit NTabBarPrivate(QObject* parent = nullptr);
    ~NTabBarPrivate();

    NThemeType::ThemeMode _themeMode;
    bool _isDark{false};

    // 数据结构和状态
    // ...

    // 辅助方法
    // ...

  private:
    // 私有实现
    // ...
};

#endif // QTNATIVEUI_NTABBAR_P_H