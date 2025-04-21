#ifndef QTNATIVEUI_NTABWIDGET_H
#define QTNATIVEUI_NTABWIDGET_H

#include <QTabWidget>
#include "stdafx.h"

class NTabBar;
class NTabWidgetPrivate;

class QTNATIVEUI_EXPORT NTabWidget : public QTabWidget {
    Q_OBJECT
    Q_Q_CREATE(NTabWidget)
    Q_PROPERTY_CREATE_Q_H(QColor, LightSelectedBackground)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSelectedBackground)

  public:
    explicit NTabWidget(QWidget* parent = nullptr);
    ~NTabWidget();

    NTabBar* tabBar() const;

    void setTabsClosable(bool closable);
    bool tabsClosable() const;

  protected:
    void changeEvent(QEvent* event) override;

  private:
    void init();
    void updatePalette();
};

#endif // QTNATIVEUI_NTABWIDGET_H