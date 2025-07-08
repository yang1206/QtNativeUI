#ifndef NAVIGATION_EXAMPLE_H
#define NAVIGATION_EXAMPLE_H

#include <QWidget>

#include "QtNativeUI/NPageComponent.h"
#include "QtNativeUI/NTextEdit.h"

// 定义演示用的页面组件
class DemoPageComponent : public NPageComponent {
    Q_OBJECT
  public:
    explicit DemoPageComponent(const QString& title, QWidget* parent = nullptr);
    // 重写路由生命周期方法
    void onRouteEnter(const QVariantMap& params) override;
    void onRouteLeave(QVariantMap& outParams) override;
    // 重写路由守卫方法
    bool beforeRouteEnter(const QVariantMap& params) override;
    bool beforeRouteLeave(QVariantMap& outParams) override;
    // 设置是否阻止导航
    void setBlockNavigation(bool block);

    // 获取日志文本
    QString getLogText() const;

  private:
    NTextEdit* m_logTextEdit;
    bool       m_blockNavigation;
};

class NScrollArea;
class NavigationExample : public QWidget {
    Q_OBJECT
  public:
    explicit NavigationExample(QWidget* parent = nullptr);
    ~NavigationExample() override;

  private:
    void     initUI();
    QWidget* createTabBars();
    QWidget* createTabWidgets();
    QWidget* createNavigationBars();
    QWidget* createNavigationViews();
    // 创建路由系统演示部分
    QWidget* createRouterDemo();

  private:
    NScrollArea*       m_scrollArea;
    QMap<QString, int> m_pageKeyMap;

    // 存储路由演示中的页面组件
    DemoPageComponent* m_homePage;
    DemoPageComponent* m_profilePage;
    DemoPageComponent* m_settingsPage;
    DemoPageComponent* m_detailsPage;

    // 存储路由键
    QString m_homeKey;
    QString m_profileKey;
    QString m_settingsKey;
    QString m_detailsKey;
};

#endif // NAVIGATION_EXAMPLE_H