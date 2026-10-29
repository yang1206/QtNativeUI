#include "nnavigationmanager.h"
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NPageComponent.h"
#include "QtNativeUI/NNavigationRouter.h"

NNavigationManager* NNavigationManager::m_instance = nullptr;

NNavigationManager* NNavigationManager::getInstance() {
    if (!m_instance) {
        m_instance = new NNavigationManager();
    }
    return m_instance;
}

NNavigationManager::NNavigationManager(QObject* parent) : QObject(parent) { m_currentNavBarName = "default"; }

NNavigationManager::~NNavigationManager() {}

void NNavigationManager::registerNavigationBar(NNavigationBar* navBar, const QString& name) {
    if (navBar) {
        m_navigationBars[name] = navBar;

        // 如果是第一个注册的导航栏，设为当前活动导航栏
        if (m_navigationBars.size() == 1) {
            m_currentNavBarName = name;
        }
    }
}

NNavigationBar* NNavigationManager::getNavigationBar(const QString& name) {
    if (m_navigationBars.contains(name)) {
        return m_navigationBars[name];
    }
    return nullptr;
}

NNavigationBar* NNavigationManager::currentNavigationBar() { return getNavigationBar(m_currentNavBarName); }

void NNavigationManager::setCurrentNavigationBar(const QString& name) {
    if (m_navigationBars.contains(name)) {
        m_currentNavBarName = name;
    }
}

void NNavigationManager::registerPageComponent(NPageComponent* page) {
    if (page && !page->routeKey().isEmpty()) {
        m_pageComponents[page->routeKey()] = page;
    }
}

NPageComponent* NNavigationManager::getPageComponent(const QString& routeKey) {
    if (m_pageComponents.contains(routeKey)) {
        return m_pageComponents[routeKey];
    }
    return nullptr;
}

QList<NPageComponent*> NNavigationManager::getAllPageComponents() {
    QList<NPageComponent*> pages;
    for (const auto& page : m_pageComponents) {
        if (page) pages.append(page.data());
    }
    return pages;
}

void NNavigationManager::registerRoute(const QString& key, NNavigationRouter* router) { m_routers[key] = router; }

void NNavigationManager::unregisterRoute(const QString& key, NNavigationRouter* router) {
    if (m_routers.value(key) == router) {
        m_routers.remove(key);
        m_pageComponents.remove(key);
    }
}

NNavigationRouter* NNavigationManager::routerForKey(const QString& key) const { return m_routers.value(key); }
void NNavigationManager::setActiveRouter(NNavigationRouter* router) { m_activeRouter = router; }
NNavigationRouter* NNavigationManager::activeRouter() const { return m_activeRouter; }
