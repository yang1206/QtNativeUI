#ifndef QTNATIVEUI_NTABBAR_P_H
#define QTNATIVEUI_NTABBAR_P_H

#include <QObject>
#include <QPropertyAnimation>
#include <QtNativeUI/NTabBar.h>
#include "QtNativeUI/NEnums.h"

class NTabBarPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabBar)

  public:
    explicit NTabBarPrivate(QObject* parent = nullptr);
    ~NTabBarPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

  private:
};

#endif // QTNATIVEUI_NTABBAR_P_H