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

  public:
    explicit NTabWidgetPrivate(QObject* parent = nullptr);
    ~NTabWidgetPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

    NTabBar*        _tabBar{nullptr};
    QStackedWidget* _stack{nullptr};

    void updateLayout();

  private:
};

#endif // QTNATIVEUI_NTABWIDGET_P_H