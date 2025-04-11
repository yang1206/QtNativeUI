#ifndef QTNATIVEUI_NTABBAR_H
#define QTNATIVEUI_NTABBAR_H

#include <QTabBar>
#include "stdafx.h"

class QDrag;
class QMimeData;
class NTabBarPrivate;

class QTNATIVEUI_EXPORT NTabBar : public QTabBar {
    Q_OBJECT
    Q_Q_CREATE(NTabBar)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, OverlayMode)
    Q_PROPERTY_CREATE_Q_H(QColor, SelectedTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, UnselectedTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, HoverTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DisabledTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, SelectedBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, HoverBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, BorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, SeparatorColor)

  public:
    explicit NTabBar(QWidget* parent = nullptr);
    ~NTabBar();
    int hoverIndex() const;

  signals:
    void tabBarPressed(int index);
    void tabDragStarted(QDrag* drag);
    void tabDropped(QMimeData* data);

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void changeEvent(QEvent* event) override;

  private:
    void init();
};

#endif // QTNATIVEUI_NTABBAR_H