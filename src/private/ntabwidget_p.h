#ifndef QTNATIVEUI_NTABWIDGET_P_H
#define QTNATIVEUI_NTABWIDGET_P_H

#include <QObject>
#include <QtNativeUI/NTabWidget.h>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NTabBar.h"

class QToolButton;
class QShortcut;

class NTabWidgetPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NTabWidget)
    Q_PROPERTY_CREATE_D(QColor, LightSelectedBackground)
    Q_PROPERTY_CREATE_D(QColor, DarkSelectedBackground)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderWidth)
    Q_PROPERTY_CREATE_D(bool, AddButtonVisible)
    Q_PROPERTY_CREATE_D(bool, TransparentBackground)

  public:
    explicit NTabWidgetPrivate(QObject* parent = nullptr);
    ~NTabWidgetPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};
    bool                  updatingPalette{false};
    QToolButton*          addButton{nullptr};
    QShortcut*            closeShortcut{nullptr};
    QShortcut*            closeAlternativeShortcut{nullptr};
    QShortcut*            addShortcut{nullptr};
    void                  initStyle();
    void                  updateAddButtonGeometry();
};

#endif // QTNATIVEUI_NTABWIDGET_P_H
