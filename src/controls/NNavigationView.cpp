#include "QtNativeUI/NNavigationView.h"
#include "../private/nnavigationview_p.h"
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NStackedWidget.h"

#include <QResizeEvent>

#include "QtNativeUI/NNavigationRouter.h"

NNavigationView::NNavigationView(QWidget* parent) : QWidget(parent), d_ptr(new NNavigationViewPrivate()) {
    Q_D(NNavigationView);
    d->q_ptr = this;
    d->setupUI();

    setMinimumWidth(300);
    setMinimumHeight(200);

    NNavigationRouter* router = NNavigationRouter::getInstance();
    connect(router, &NNavigationRouter::routeChanged, this, &NNavigationView::routeChanged);
    connect(router, &NNavigationRouter::routeBack, this, &NNavigationView::routeBack);
    connect(router, &NNavigationRouter::navigationStateChanged, this, &NNavigationView::navigationStateChanged);
}

NNavigationView::~NNavigationView() {}

void NNavigationView::setHeaderWidget(QWidget* widget) {
    Q_D(NNavigationView);
    d->_navigationBar->setHeaderWidget(widget);
}

QWidget* NNavigationView::headerWidget() const {
    Q_D(const NNavigationView);
    return d->_navigationBar->headerWidget();
}

void NNavigationView::setSearchVisible(bool visible) {
    Q_D(const NNavigationView);
    d->_navigationBar->setSearchVisible(visible);
}

bool NNavigationView::isSearchVisible() const {
    Q_D(const NNavigationView);
    return d->_navigationBar->isSearchVisible();
}

NNavigationType::NodeOperateReturnType
NNavigationView::addExpanderNode(QString expanderTitle, QString& expanderKey, NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addExpanderNode(QString expanderTitle, QString& expanderKey, NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addExpanderNode(QString                expanderTitle,
                                                                        QString&               expanderKey,
                                                                        QString                targetExpanderKey,
                                                                        NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addExpanderNode(QString               expanderTitle,
                                                                        QString&              expanderKey,
                                                                        QString               targetExpanderKey,
                                                                        NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageNode(QString pageTitle, QWidget* page, NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageNode(QString pageTitle, QWidget* page, NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageNode(QString pageTitle, QWidget* page, int keyPoints, NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageNode(QString pageTitle, QWidget* page, int keyPoints, NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addPageNode(QString                pageTitle,
                                                                    QWidget*               page,
                                                                    QString                targetExpanderKey,
                                                                    int                    keyPoints,
                                                                    NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addPageNode(QString               pageTitle,
                                                                    QWidget*              page,
                                                                    QString               targetExpanderKey,
                                                                    int                   keyPoints,
                                                                    NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addFooterNode(footerTitle, footerKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addFooterNode(footerTitle, footerKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addFooterNode(QString                footerTitle,
                                                                      QWidget*               page,
                                                                      QString&               footerKey,
                                                                      int                    keyPoints,
                                                                      NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addFooterNode(QString               footerTitle,
                                                                      QWidget*              page,
                                                                      QString&              footerKey,
                                                                      int                   keyPoints,
                                                                      NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addFooterNode(footerTitle, page, footerKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addPageComponent(NPageComponent*        page,
                                                                         NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageComponent(page, icon);
}

NNavigationType::NodeOperateReturnType NNavigationView::addPageComponent(NPageComponent*       page,
                                                                         NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageComponent(page, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageComponent(NPageComponent* page, QString targetExpanderKey, NRegularIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageComponent(page, targetExpanderKey, icon);
}

NNavigationType::NodeOperateReturnType
NNavigationView::addPageComponent(NPageComponent* page, QString targetExpanderKey, NFilledIconType::Icon icon) {
    Q_D(NNavigationView);
    return d->_navigationBar->addPageComponent(page, targetExpanderKey, icon);
}

bool NNavigationView::getNavigationNodeIsExpanded(QString expanderKey) const {
    Q_D(const NNavigationView);
    return d->_navigationBar->getNavigationNodeIsExpanded(expanderKey);
}

void NNavigationView::expandNavigationNode(QString expanderKey) {
    Q_D(NNavigationView);
    d->_navigationBar->expandNavigationNode(expanderKey);
}

void NNavigationView::collpaseNavigationNode(QString expanderKey) {
    Q_D(NNavigationView);
    d->_navigationBar->collpaseNavigationNode(expanderKey);
}

void NNavigationView::removeNavigationNode(QString nodeKey) {
    Q_D(NNavigationView);
    d->_navigationBar->removeNavigationNode(nodeKey);
}

void NNavigationView::setNodeKeyPoints(QString nodeKey, int keyPoints) {
    Q_D(NNavigationView);
    d->_navigationBar->setNodeKeyPoints(nodeKey, keyPoints);
}

int NNavigationView::getNodeKeyPoints(QString nodeKey) const {
    Q_D(const NNavigationView);
    return d->_navigationBar->getNodeKeyPoints(nodeKey);
}

void NNavigationView::navigation(QString pageKey, bool isLogClicked) {
    Q_D(NNavigationView);
    d->_navigationBar->navigation(pageKey, isLogClicked);
}

void NNavigationView::navigateTo(const QString& pageKey, const QVariantMap& params) {
    NNavigationRouter::getInstance()->navigateTo(pageKey, params);
}

void NNavigationView::navigateBack(const QVariantMap& params) {
    NNavigationRouter::getInstance()->navigateBack(params);
}

QString NNavigationView::currentRouteKey() const { return NNavigationRouter::getInstance()->currentRouteKey(); }

QVariantMap NNavigationView::currentRouteParams() const {
    return NNavigationRouter::getInstance()->currentRouteParams();
}

int NNavigationView::historyCount() const { return NNavigationRouter::getInstance()->historyCount(); }

void NNavigationView::clearNavigationHistory() { NNavigationRouter::getInstance()->clearHistory(); }

bool NNavigationView::hasNavigationHistory() const { return NNavigationRouter::getInstance()->historyCount() > 0; }

void NNavigationView::setNavigationBarVisible(bool visible) {
    Q_D(NNavigationView);
    d->_isNavigationBarVisible = visible;
    d->_navigationBar->setVisible(visible);
    d->updateLayout();
}

bool NNavigationView::isNavigationBarVisible() const {
    Q_D(const NNavigationView);
    return d->_isNavigationBarVisible;
}

void NNavigationView::setPageTransitionDuration(int duration) {
    Q_D(NNavigationView);
    d->_pageTransitionDuration = duration;
}

int NNavigationView::pageTransitionDuration() const {
    Q_D(const NNavigationView);
    return d->_pageTransitionDuration;
}

QWidget* NNavigationView::currentWidget() const {
    Q_D(const NNavigationView);
    return d->_stackedWidget->currentWidget();
}

int NNavigationView::currentIndex() const {
    Q_D(const NNavigationView);
    return d->_stackedWidget->currentIndex();
}

int NNavigationView::count() const {
    Q_D(const NNavigationView);
    return d->_stackedWidget->count();
}

NNavigationType::NavigationDisplayMode NNavigationView::getDisplayMode() const {
    Q_D(const NNavigationView);
    return d->_pDisplayMode;
}

void NNavigationView::setDisplayMode(NNavigationType::NavigationDisplayMode mode) {
    Q_D(NNavigationView);
    if (d->_pDisplayMode != mode) {
        d->setDisplayMode(mode);
        emit pDisplayModeChanged();
    }
}

NNavigationType::PageTransitionType NNavigationView::getPageTransitionType() const {
    Q_D(const NNavigationView);
    return d->_pPageTransitionType;
}

void NNavigationView::setPageTransitionType(NNavigationType::PageTransitionType type) {
    Q_D(NNavigationView);
    if (d->_pPageTransitionType != type) {
        d->_pPageTransitionType = type;
        emit pPageTransitionTypeChanged();
    }
}

void NNavigationView::resizeEvent(QResizeEvent* event) {
    Q_D(NNavigationView);
    QWidget::resizeEvent(event);

    d->handleNavigationDisplayMode();
}