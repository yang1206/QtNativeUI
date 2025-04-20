#ifndef QTNATIVEUI_NTABWIDGET_P_H
#define QTNATIVEUI_NTABWIDGET_P_H

#include <QObject>
#include <QtNativeUI/NTabWidget.h>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NTabBar.h"

class NTabWidgetPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabWidget)
    Q_PROPERTY_CREATE_D(QColor, LightSelectedBackground)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectedBackground)

  public:
    explicit NTabWidgetPrivate(QObject* parent = nullptr);
    ~NTabWidgetPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

  private:
};

#endif // QTNATIVEUI_NTABWIDGET_P_H