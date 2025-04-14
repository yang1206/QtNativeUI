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
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderBackground)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderBackground)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderBackgroundSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderBackgroundSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderBackgroundPointerOver)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderBackgroundPointerOver)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderBackgroundPressed)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderBackgroundPressed)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderBackgroundDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderBackgroundDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderForeground)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderForeground)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderForegroundSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderForegroundSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderForegroundPointerOver)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderForegroundPointerOver)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderForegroundPressed)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderForegroundPressed)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderForegroundDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderForegroundDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemIconForeground)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemIconForeground)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemIconForegroundSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemIconForegroundSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemIconForegroundPointerOver)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemIconForegroundPointerOver)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemIconForegroundPressed)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemIconForegroundPressed)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemIconForegroundDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemIconForegroundDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemSeparator)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemSeparator)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHeaderBorderColorSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHeaderBorderColorSelected)

  public:
    explicit NTabBar(QWidget* parent = nullptr);
    ~NTabBar();
    int hoverIndex() const;

    // 重写关闭按钮相关方法
    bool tabsClosable() const;
    void setTabsClosable(bool closable);

    // 用于设置自定义关闭按钮的方法
    void setupCustomCloseButton(int index);

  signals:
    void tabBarPressed(int index);
    void tabDragStarted(QDrag* drag);
    void tabDropped(QMimeData* data);

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void changeEvent(QEvent* event) override;

    // 重写标签插入方法，以便添加关闭按钮
    void tabInserted(int index) override;

  private:
    void init();
};

#endif // QTNATIVEUI_NTABBAR_H