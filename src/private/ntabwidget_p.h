#ifndef QTNATIVEUI_NTABWIDGET_P_H
#define QTNATIVEUI_NTABWIDGET_P_H

#include <QObject>
#include <QPropertyAnimation>
#include <QtNativeUI/NTabWidget.h>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NTabBar.h"

class QStackedWidget;
class NTabBar;

class NTabWidgetPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabWidget)
    Q_PROPERTY_CREATE_D(QColor, LightSelectedBackground)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectedBackground)
    Q_PROPERTY_CREATE_D(int, TabPosition)
    Q_PROPERTY_CREATE_D(bool, TabsClosable)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)

  public:
    explicit NTabWidgetPrivate(QObject* parent = nullptr);
    ~NTabWidgetPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

    NTabBar*        _tabBar{nullptr};
    QStackedWidget* _stack{nullptr};

    // 更新布局方法，根据标签位置重新排列组件
    void updateLayout();

    // 转换TabPosition到对应的QTabBar::Shape
    NTabBar::Shape positionToShape(NTabWidget::TabPosition pos) const;

  private:
};

#endif // QTNATIVEUI_NTABWIDGET_P_H