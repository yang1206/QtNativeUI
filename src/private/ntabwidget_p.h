#ifndef QTNATIVEUI_NTABWIDGET_P_H
#define QTNATIVEUI_NTABWIDGET_P_H

#include <QObject>
#include <QPropertyAnimation>
#include <QtNativeUI/NTabWidget.h>
#include "QtNativeUI/NEnums.h"

class QStackedWidget;
class NTabBar;

class NTabWidgetPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabWidget)

    // 私有属性
    // ...

  public:
    explicit NTabWidgetPrivate(QObject* parent = nullptr);
    ~NTabWidgetPrivate();

    NThemeType::ThemeMode _themeMode;
    bool _isDark{false};

    // 内部组件
    NTabBar* _tabBar{nullptr};
    QStackedWidget* _stack{nullptr};

    // 辅助方法
    void updateLayout();
    // ...

  private:
    // 私有实现
    // ...
};

#endif // QTNATIVEUI_NTABWIDGET_P_H