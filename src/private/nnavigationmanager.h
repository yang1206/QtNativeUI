#ifndef NNAVIGATIONMANAGER_P_H
#define NNAVIGATIONMANAGER_P_H

#include <QMap>
#include <QObject>
#include <QPointer>
#include "QtNativeUI/stdafx.h"

class NNavigationBar;
class NPageComponent;
class NNavigationRouter;

class QTNATIVEUI_EXPORT NNavigationManager : public QObject {
    Q_OBJECT
  public:
    static NNavigationManager* getInstance();

  private:
    explicit NNavigationManager(QObject* parent = nullptr);
    ~NNavigationManager();

    static NNavigationManager* m_instance;

  public:
    // 注册导航栏
    void registerNavigationBar(NNavigationBar* navBar, const QString& name = "default");

    // 获取导航栏
    NNavigationBar* getNavigationBar(const QString& name = "default");

    // 获取当前活动的导航栏
    NNavigationBar* currentNavigationBar();

    // 设置当前活动的导航栏
    void setCurrentNavigationBar(const QString& name);

    // 注册页面组件
    void registerPageComponent(NPageComponent* page);

    // 获取页面组件
    NPageComponent* getPageComponent(const QString& routeKey);

    // 获取所有页面组件
    QList<NPageComponent*> getAllPageComponents();

    void registerRoute(const QString& key, NNavigationRouter* router);
    void unregisterRoute(const QString& key, NNavigationRouter* router);
    NNavigationRouter* routerForKey(const QString& key) const;
    void setActiveRouter(NNavigationRouter* router);
    NNavigationRouter* activeRouter() const;

  private:
    QMap<QString, QPointer<NNavigationBar>> m_navigationBars;
    QString                        m_currentNavBarName;
    QMap<QString, QPointer<NPageComponent>> m_pageComponents;
    QMap<QString, QPointer<NNavigationRouter>> m_routers;
    QPointer<NNavigationRouter> m_activeRouter;
};

#endif // NNAVIGATIONMANAGER_P_H
