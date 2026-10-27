#ifndef QTNATIVEUI_NTABWIDGET_H
#define QTNATIVEUI_NTABWIDGET_H

#include <QTabWidget>
#include "NTabBar.h"
#include "stdafx.h"

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
    Q_PROPERTY_CREATE_Q_H(bool, AddButtonVisible)
    Q_PROPERTY_CREATE_Q_H(bool, TransparentBackground)
    Q_PROPERTY(bool tabsClosable READ tabsClosable WRITE setTabsClosable)

  public:
    explicit NTabWidget(QWidget* parent = nullptr);
    ~NTabWidget() override;

    NTabBar* tabBar() const;

    void                            setTabsClosable(bool closable);
    bool                            tabsClosable() const;
    void                            setTabClosable(int index, bool closable);
    bool                            getTabClosable(int index) const;
    void                            setTabWidthMode(NTabBar::TabWidthMode mode);
    NTabBar::TabWidthMode           getTabWidthMode() const;
    void                            setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode mode);
    NTabBar::CloseButtonDisplayMode getCloseButtonDisplayMode() const;
    void                            setTabDragEnabled(bool enabled);
    bool                            getTabDragEnabled() const;
    QSize                           sizeHint() const override;
    QSize                           minimumSizeHint() const override;
    int                             heightForWidth(int width) const override;

  signals:
    void tabAddRequested();

  protected:
    void changeEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void tabInserted(int index) override;
    void showEvent(QShowEvent* event) override;
    bool event(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

  private:
    void init();
    void updatePalette();
    void adjustContentMargins();
};

#endif // QTNATIVEUI_NTABWIDGET_H
