#include "QtNativeUI/NNavigationRouter.h"

#include <QScopeGuard>
#include "nnavigationmanager.h"

Q_SINGLETON_CREATE_CPP(NNavigationRouter)

NNavigationRouter::NNavigationRouter(QObject* parent) : QObject(parent), m_maxHistorySize(25) {}

NNavigationRouter::~NNavigationRouter() {
    for (auto it = m_pages.cbegin(); it != m_pages.cend(); ++it) {
        NNavigationManager::getInstance()->unregisterRoute(it.key(), this);
    }
}

NNavigationRouter* NNavigationRouter::activeRouter() const {
    if (this == _instance) {
        auto* router = NNavigationManager::getInstance()->activeRouter();
        if (router && router != this) return router;
    }
    return const_cast<NNavigationRouter*>(this);
}

void NNavigationRouter::setMaxHistorySize(int size) {
    if (activeRouter() != this) {
        activeRouter()->setMaxHistorySize(size);
        return;
    }
    if (size <= 0) return;
    m_maxHistorySize = size;
    while (m_history.size() > size) m_history.dequeue();
}

int NNavigationRouter::maxHistorySize() const { return activeRouter()->m_maxHistorySize; }

void NNavigationRouter::registerPage(const QString& pageKey, QWidget* page) {
    if (pageKey.isEmpty() || !page || m_pages.contains(pageKey)) return;
    m_pages.insert(pageKey, page);
    NNavigationManager::getInstance()->registerRoute(pageKey, this);
    connect(page, &QObject::destroyed, this, [this, pageKey] { unregisterPage(pageKey); });
}

bool NNavigationRouter::containsPage(const QString& pageKey) const { return !m_pages.value(pageKey).isNull(); }

void NNavigationRouter::unregisterPage(const QString& pageKey) { unregisterPages({pageKey}); }

void NNavigationRouter::unregisterPages(const QStringList& keys) {
    QPointer<NNavigationRouter> self(this);
    bool removed = false;
    for (const auto& key : keys) {
        if (!m_pages.remove(key)) continue;
        removed = true;
        NNavigationManager::getInstance()->unregisterRoute(key, this);
    }
    if (!removed) return;
    for (int i = m_history.size() - 1; i >= 0; --i) {
        if (!containsPage(m_history.at(i).pageKey)) m_history.removeAt(i);
    }
    if (!m_currentRoute.pageKey.isEmpty() && !containsPage(m_currentRoute.pageKey)) {
        m_currentRoute = {};
        emit routeChanged({}, {});
        if (!self) return;
    }
    emit navigationStateChanged(!m_history.isEmpty());
    if (self && this != _instance && NNavigationManager::getInstance()->activeRouter() == this) {
        emit getInstance()->navigationStateChanged(!m_history.isEmpty());
    }
}

NNavigationRouterType::NavigationRouteType NNavigationRouter::navigateTo(const QString& key, const QVariantMap& params) {
    return navigateToImpl(key, params, true);
}

NNavigationRouterType::NavigationRouteType NNavigationRouter::replaceRoute(const QString& key, const QVariantMap& params) {
    return navigateToImpl(key, params, false);
}

NNavigationRouterType::NavigationRouteType NNavigationRouter::navigateToImpl(const QString& pageKey,
                                                                         const QVariantMap& params, bool recordHistory) {
    if (this == _instance) {
        auto* owner = NNavigationManager::getInstance()->routerForKey(pageKey);
        if (owner && owner != this) return owner->navigateToImpl(pageKey, params, recordHistory);
        // 兼容独立注册的页面组件。
        if (!containsPage(pageKey)) {
            if (auto* page = NNavigationManager::getInstance()->getPageComponent(pageKey)) registerPage(pageKey, page);
        }
    }
    if (!containsPage(pageKey)) return NNavigationRouterType::PageNotFound;
    if (m_isNavigating) return NNavigationRouterType::RouteRejected;
    if (m_currentRoute.pageKey == pageKey && m_currentRoute.params == params) return NNavigationRouterType::Success;

    QPointer<NNavigationRouter> self(this);
    m_isNavigating = true;
    const auto navigating = qScopeGuard([self] { if (self) self->m_isNavigating = false; });
    QPointer<QWidget> target = m_pages.value(pageKey);
    auto* fromPage = qobject_cast<NPageComponent*>(m_pages.value(m_currentRoute.pageKey).data());
    auto* toPage = qobject_cast<NPageComponent*>(target.data());
    if (!executePageLifecycle(fromPage, toPage, params) || !self || !target || !containsPage(pageKey)) {
        return NNavigationRouterType::RouteRejected;
    }

    // 守卫通过后再提交历史、路由和可见页面。
    if (recordHistory && !m_currentRoute.pageKey.isEmpty() && m_currentRoute.pageKey != pageKey) {
        while (m_history.size() >= m_maxHistorySize) m_history.dequeue();
        m_history.enqueue(m_currentRoute);
    }
    m_currentRoute = {pageKey, params};
    NNavigationManager::getInstance()->setActiveRouter(this);
    emit routeChanged(pageKey, params);
    if (!self) return NNavigationRouterType::ObjectInvalid;
    if (target && containsPage(pageKey) && toPage) toPage->onRouteEnter(params);
    if (!self) return NNavigationRouterType::ObjectInvalid;
    const bool hasHistory = !m_history.isEmpty();
    emit navigationStateChanged(hasHistory);
    if (!self) return NNavigationRouterType::ObjectInvalid;
    if (this != getInstance()) {
        emit getInstance()->routeChanged(pageKey, params);
        emit getInstance()->navigationStateChanged(hasHistory);
    }
    return NNavigationRouterType::Success;
}

NNavigationRouterType::NavigationRouteType NNavigationRouter::navigateBack(const QVariantMap& params) {
    if (activeRouter() != this) return activeRouter()->navigateBack(params);
    if (m_isNavigating) return NNavigationRouterType::RouteRejected;
    // 已移除页面不再参与返回；拒绝守卫时保持有效历史不变。
    while (!m_history.isEmpty() && !containsPage(m_history.last().pageKey)) m_history.removeLast();
    if (m_history.isEmpty()) return NNavigationRouterType::NoHistory;

    QPointer<NNavigationRouter> self(this);
    m_isNavigating = true;
    const auto navigating = qScopeGuard([self] { if (self) self->m_isNavigating = false; });
    const RouteInfo previous = m_history.last();
    QVariantMap merged = previous.params;
    for (auto it = params.cbegin(); it != params.cend(); ++it) merged.insert(it.key(), it.value());
    QPointer<QWidget> target = m_pages.value(previous.pageKey);
    auto* fromPage = qobject_cast<NPageComponent*>(m_pages.value(m_currentRoute.pageKey).data());
    auto* toPage = qobject_cast<NPageComponent*>(target.data());
    if (!executePageLifecycle(fromPage, toPage, merged) || !self || !target || !containsPage(previous.pageKey)) {
        return NNavigationRouterType::RouteRejected;
    }
    // 生命周期回调可以移除节点，所以通过页面键查找，避免弹出错误的历史项。
    for (int i = m_history.size() - 1; i >= 0; --i) {
        if (m_history.at(i).pageKey == previous.pageKey) {
            m_history.removeAt(i);
            break;
        }
    }
    const QString fromKey = m_currentRoute.pageKey;
    m_currentRoute = {previous.pageKey, merged};
    NNavigationManager::getInstance()->setActiveRouter(this);
    emit routeBack(fromKey, previous.pageKey, merged);
    if (!self) return NNavigationRouterType::ObjectInvalid;
    if (target && containsPage(previous.pageKey) && toPage) toPage->onRouteEnter(merged);
    if (!self) return NNavigationRouterType::ObjectInvalid;
    const bool hasHistory = !m_history.isEmpty();
    emit navigationStateChanged(hasHistory);
    if (!self) return NNavigationRouterType::ObjectInvalid;
    if (this != getInstance()) {
        emit getInstance()->routeBack(fromKey, previous.pageKey, merged);
        emit getInstance()->navigationStateChanged(hasHistory);
    }
    return NNavigationRouterType::Success;
}

void NNavigationRouter::clearHistory() {
    if (activeRouter() != this) {
        activeRouter()->clearHistory();
        return;
    }
    m_history.clear();
    emit navigationStateChanged(false);
    if (this != _instance && NNavigationManager::getInstance()->activeRouter() == this) {
        emit getInstance()->navigationStateChanged(false);
    }
}

QString NNavigationRouter::currentRouteKey() const { return activeRouter()->m_currentRoute.pageKey; }
QVariantMap NNavigationRouter::currentRouteParams() const { return activeRouter()->m_currentRoute.params; }
int NNavigationRouter::historyCount() const { return activeRouter()->m_history.size(); }

bool NNavigationRouter::executePageLifecycle(NPageComponent* fromPage, NPageComponent* toPage,
                                             const QVariantMap& params) {
    QPointer<NPageComponent> from = fromPage;
    QPointer<NPageComponent> to = toPage;
    if (to && !to->beforeRouteEnter(params)) return false;
    if (toPage && !to) return false;
    QVariantMap outParams;
    if (from && !from->beforeRouteLeave(outParams)) return false;
    if (toPage && !to) return false;
    if (from) from->onRouteLeave(outParams);
    return !toPage || !to.isNull();
}
