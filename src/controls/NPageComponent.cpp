#include "QtNativeUI/NPageComponent.h"

#include "../../include/QtNativeUI/NNavigationRouter.h"
#include "QtNativeUI/NNavigationBar.h"
#include "nnavigationmanager.h"

NPageComponent::NPageComponent(QWidget* parent) : QWidget(parent) {}

NPageComponent::~NPageComponent() {}

void NPageComponent::onRouteEnter(const QVariantMap& params) {
    // 默认实现为空，子类可以重写
    Q_UNUSED(params);
}

void NPageComponent::onRouteLeave(QVariantMap& outParams) {
    // 默认实现为空，子类可以重写
    Q_UNUSED(outParams);
}

bool NPageComponent::beforeRouteEnter(const QVariantMap& params) {
    // 默认允许导航
    Q_UNUSED(params);
    return true;
}

bool NPageComponent::beforeRouteLeave(QVariantMap& outParams) {
    // 默认允许导航
    Q_UNUSED(outParams);
    return true;
}

QString NPageComponent::routeKey() const { return _routeKey; }

void NPageComponent::setRouteKey(const QString& key) { _routeKey = key; }

void NPageComponent::navigateTo(const QString& pageKey, const QVariantMap& params) {
    NNavigationRouter::getInstance()->navigateTo(pageKey, params);
}

QString NPageComponent::pageTitle() const { return m_pageTitle; }

void NPageComponent::setPageTitle(const QString& title) { m_pageTitle = title; }

void NPageComponent::navigateBack(const QVariantMap& params) {
    QVariantMap outParams = params;
    if (beforeRouteLeave(outParams)) {
        onRouteLeave(outParams);
        NNavigationRouter::getInstance()->navigateBack(outParams);
    }
}