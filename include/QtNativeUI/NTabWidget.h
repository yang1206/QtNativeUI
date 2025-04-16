#ifndef QTNATIVEUI_NTABWIDGET_H
#define QTNATIVEUI_NTABWIDGET_H

#include <QWidget>
#include "stdafx.h"

class NTabBar;
class QStackedWidget;
class NTabWidgetPrivate;

class QTNATIVEUI_EXPORT NTabWidget : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NTabWidget)
    Q_PROPERTY_CREATE_Q_H(QColor, LightSelectedBackground)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSelectedBackground)
    Q_PROPERTY_CREATE_Q_H(int, TabPosition)

  public:
    enum TabPosition {
        North, // 顶部，默认
        South, // 底部
        West,  // 左侧
        East   // 右侧
    };
    Q_ENUM(TabPosition)

    explicit NTabWidget(QWidget* parent = nullptr);
    ~NTabWidget();

    TabPosition tabPosition() const;
    void        setTabPosition(TabPosition position);

    int  addTab(QWidget* page, const QString& label);
    int  addTab(QWidget* page, const QIcon& icon, const QString& label);
    int  insertTab(int index, QWidget* page, const QString& label);
    int  insertTab(int index, QWidget* page, const QIcon& icon, const QString& label);
    void removeTab(int index);

    QString tabText(int index) const;
    void    setTabText(int index, const QString& text);
    QIcon   tabIcon(int index) const;
    void    setTabIcon(int index, const QIcon& icon);
    QString tabToolTip(int index) const;
    void    setTabToolTip(int index, const QString& tip);

    NTabBar* tabBar() const;
    QWidget* currentWidget() const;
    QWidget* widget(int index) const;
    int      count() const;
    int      indexOf(QWidget* widget) const;

    bool tabsClosable() const;
    void setTabsClosable(bool closable);
    void setMovable(bool movable);
    bool isMovable() const;

    int currentIndex() const;

  public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget* widget);

  signals:
    void currentChanged(int index);
    void tabCloseRequested(int index);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event) override;

  private:
    void init();
    void updateTabBarPosition();
};

#endif // QTNATIVEUI_NTABWIDGET_H