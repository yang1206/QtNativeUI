#include "../../include/QtNativeUI/NNavigationRouter.h"

#include "../private/nnavigationmanager.h"
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NPageComponent.h"

Q_SINGLETON_CREATE_CPP(NNavigationRouter)

NNavigationRouter::NNavigationRouter(QObject* parent) : QObject(parent) { m_maxHistorySize = 25; }

NNavigationRouter::~NNavigationRouter() {}

void NNavigationRouter::setMaxHistorySize(int size) {
    if (size > 0) {
        m_maxHistorySize = size;

        // 如果当前历史记录超过新的最大值，则裁剪
        while (m_history.size() > m_maxHistorySize) {
            m_history.dequeue();
        }
    }
}

int NNavigationRouter::maxHistorySize() const { return m_maxHistorySize; }

NNavigationRouterType::NavigationRouteType NNavigationRouter::navigateTo(const QString&     pageKey,
                                                                         const QVariantMap& params) {
    // 获取目标页面组件
    NPageComponent* toPage = NNavigationManager::getInstance()->getPageComponent(pageKey);
    if (!toPage) {
        return NNavigationRouterType::PageNotFound;
    }

    // 获取当前页面组件
    NPageComponent* fromPage = nullptr;
    if (!m_currentRoute.pageKey.isEmpty()) {
        fromPage = NNavigationManager::getInstance()->getPageComponent(m_currentRoute.pageKey);
    }

    // 执行页面生命周期方法
    if (!executePageLifecycle(fromPage, toPage, params)) {
        return NNavigationRouterType::RouteRejected;
    }

    // 如果有当前路由，将其添加到历史记录中
    if (!m_currentRoute.pageKey.isEmpty()) {
        // 如果历史记录已满，移除最旧的记录
        if (m_history.size() >= m_maxHistorySize) {
            m_history.dequeue();
        }

        m_history.enqueue(m_currentRoute);
    }

    // 更新当前路由
    m_currentRoute.pageKey = pageKey;
    m_currentRoute.params  = params;

    // 发出路由变更信号
    emit routeChanged(pageKey, params);

    // 通知导航栏更新选中状态
    NNavigationBar* navBar = NNavigationManager::getInstance()->currentNavigationBar();
    if (navBar) {
        navBar->navigation(pageKey, false);
    }

    // 调用页面的路由进入方法
    toPage->onRouteEnter(params);

    // 发出导航状态变更信号
    emit navigationStateChanged(!m_history.isEmpty());

    return NNavigationRouterType::Success;
}

NNavigationRouterType::NavigationRouteType NNavigationRouter::navigateBack(const QVariantMap& params) {
    if (m_history.isEmpty()) {
        return NNavigationRouterType::NoHistory;
    }

    // 获取上一个路由
    RouteInfo prevRoute = m_history.last();
    m_history.removeLast();

    // 获取当前页面和目标页面
    NPageComponent* fromPage = NNavigationManager::getInstance()->getPageComponent(m_currentRoute.pageKey);
    NPageComponent* toPage   = NNavigationManager::getInstance()->getPageComponent(prevRoute.pageKey);

    if (!toPage) {
        return NNavigationRouterType::PageNotFound;
    }

    // 合并参数
    QVariantMap mergedParams = prevRoute.params;
    for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
        mergedParams[it.key()] = it.value();
    }

    // 执行页面生命周期方法
    if (!executePageLifecycle(fromPage, toPage, mergedParams)) {
        // 如果被拒绝，恢复历史记录
        m_history.append(prevRoute);
        return NNavigationRouterType::RouteRejected;
    }

    // 保存当前路由信息以便发送信号
    QString fromPageKey = m_currentRoute.pageKey;

    // 更新当前路由
    m_currentRoute        = prevRoute;
    m_currentRoute.params = mergedParams; // 使用合并后的参数

    // 发出返回路由信号
    emit routeBack(fromPageKey, prevRoute.pageKey, mergedParams);

    // 通知导航栏更新选中状态
    NNavigationBar* navBar = NNavigationManager::getInstance()->currentNavigationBar();
    if (navBar) {
        navBar->navigation(prevRoute.pageKey, false);
    }

    // 调用页面的路由进入方法
    toPage->onRouteEnter(mergedParams);

    // 发出导航状态变更信号
    emit navigationStateChanged(!m_history.isEmpty());

    return NNavigationRouterType::Success;
}

void NNavigationRouter::clearHistory() {
    m_history.clear();
    emit navigationStateChanged(false);
}

QString NNavigationRouter::currentRouteKey() const { return m_currentRoute.pageKey; }

QVariantMap NNavigationRouter::currentRouteParams() const { return m_currentRoute.params; }

int NNavigationRouter::historyCount() const { return m_history.size(); }

bool NNavigationRouter::executePageLifecycle(NPageComponent*    fromPage,
                                             NPageComponent*    toPage,
                                             const QVariantMap& params) {
    // 检查目标页面的进入守卫
    if (toPage && !toPage->beforeRouteEnter(params)) {
        return false;
    }

    // 检查当前页面的离开守卫
    if (fromPage) {
        QVariantMap outParams;
        if (!fromPage->beforeRouteLeave(outParams)) {
            return false;
        }

        // 调用离开生命周期方法
        fromPage->onRouteLeave(outParams);
    }

    return true;
}