#ifndef NNAVIGATIONROUTER_H
#define NNAVIGATIONROUTER_H

#include <QObject>
#include <QPointer>
#include <QQueue>
#include <QVariantMap>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NPageComponent.h"
#include "QtNativeUI/singleton.h"
#include "QtNativeUI/stdafx.h"

struct RouteInfo {
    QString     pageKey;
    QVariantMap params;
};

class QTNATIVEUI_EXPORT NNavigationRouter : public QObject {
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NNavigationRouter)

  public:
    explicit NNavigationRouter(QObject* parent = nullptr);
    ~NNavigationRouter() override;

  Q_SIGNALS:
    void navigationStateChanged(bool state);
    void routeChanged(const QString& pageKey, const QVariantMap& params);
    void routeBack(const QString& fromPageKey, const QString& toPageKey, const QVariantMap& params);

  public:
    // 设置最大历史记录数
    void setMaxHistorySize(int size);
    int  maxHistorySize() const;
    // 导航到指定页面
    NNavigationRouterType::NavigationRouteType navigateTo(const QString& pageKey, const QVariantMap& params = {});
    NNavigationRouterType::NavigationRouteType replaceRoute(const QString& pageKey, const QVariantMap& params = {});
    // 返回上一页
    NNavigationRouterType::NavigationRouteType navigateBack(const QVariantMap& params = {});
    // 清除路由历史
    void clearHistory();

    // 获取当前路由信息
    QString     currentRouteKey() const;
    QVariantMap currentRouteParams() const;

    // 获取历史记录数量
    int historyCount() const;

    // 每个导航栏维护独立的页面与历史；单例保留为兼容入口。
    void registerPage(const QString& pageKey, QWidget* page);
    void unregisterPage(const QString& pageKey);
    void unregisterPages(const QStringList& pageKeys);
    bool containsPage(const QString& pageKey) const;

  private:
    int               m_maxHistorySize;
    QQueue<RouteInfo> m_history;
    RouteInfo         m_currentRoute;
    QMap<QString, QPointer<QWidget>> m_pages;
    bool m_isNavigating{false};

    bool executePageLifecycle(NPageComponent* fromPage, NPageComponent* toPage, const QVariantMap& params);
    NNavigationRouter* activeRouter() const;
    NNavigationRouterType::NavigationRouteType navigateToImpl(const QString& pageKey, const QVariantMap& params, bool recordHistory);
};

#endif // NNAVIGATIONROUTER_H
