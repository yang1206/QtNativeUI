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
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderWidth)

  public:
    explicit NTabWidget(QWidget* parent = nullptr);
    ~NTabWidget() override;

    NTabBar* tabBar() const;

    void setTabsClosable(bool closable);
    bool tabsClosable() const;

  protected:
    void changeEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void tabInserted(int index) override;
    void showEvent(QShowEvent* event) override;

  private:
    void init();
    void updatePalette();
    void adjustContentMargins();
};

#endif // QTNATIVEUI_NTABWIDGET_H