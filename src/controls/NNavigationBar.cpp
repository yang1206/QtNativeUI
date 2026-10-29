#include "QtNativeUI/NNavigationBar.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QVBoxLayout>
#include "../../include/QtNativeUI/NNavigationRouter.h"
#include "../private/nnavigationbar_p.h"
#include "../private/nnavigationfooterdelegate_p.h"
#include "../private/nnavigationfootermodel_p.h"
#include "../private/nnavigationmodel_p.h"
#include "../private/nnavigationnode_p.h"
#include "../private/nnavigationtreeview_p.h"
#include "QtNativeUI/NAutoSuggestBox.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolButton.h"
#include "nbaselistview.h"
#include "nnavigationmanager.h"

#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"

Q_PROPERTY_CREATE_Q_CPP(NNavigationBar, bool, IsTransparent)

NNavigationBar::NNavigationBar(QWidget* parent) : QWidget{parent}, d_ptr(new NNavigationBarPrivate()) {
    Q_D(NNavigationBar);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });

    NNavigationManager::getInstance()->registerNavigationBar(this);
    d->_router = new NNavigationRouter(this);
    // 导航状态只订阅本实例；单例仅作兼容分发。
    connect(
        d->_router, &NNavigationRouter::routeChanged, this, &NNavigationBar::handleRouteChanged);
    connect(d->_router, &NNavigationRouter::routeBack, this, &NNavigationBar::handleRouteBack);

    setFixedWidth(300);
    d->_pIsTransparent   = true;
    d->_pIsSearchVisible = true;
    d->_headerLayout     = new QVBoxLayout();
    d->_headerLayout->setContentsMargins(0, 0, 0, 0);
    d->_headerLayout->setSpacing(0);
    d->_headerWidget = nullptr;

    // 导航模型
    d->_navigationModel = new NNavigationModel(this);
    d->_navigationView  = new NNavigationTreeView(this);
    d->_navigationView->setModel(d->_navigationModel);
    connect(d->_navigationView, &NNavigationTreeView::navigationClicked, this, [=](const QModelIndex& index) {
        d->onTreeViewClicked(index);
    });
    auto syncExpanded = [d](const QModelIndex& index, bool expanded) {
        auto* node = static_cast<NNavigationNode*>(index.internalPointer());
        if (node) node->setIsExpanded(expanded);
        d->_resetNodeSelected();
    };
    connect(d->_navigationView, &QTreeView::expanded, this, [syncExpanded](const QModelIndex& index) { syncExpanded(index, true); });
    connect(d->_navigationView, &QTreeView::collapsed, this, [syncExpanded](const QModelIndex& index) { syncExpanded(index, false); });

    // 导航按钮组
    d->_navigationButton = new NToolButton(this);
    d->_navigationButton->setFixedSize(40, 38);
    d->_navigationButton->setTransparentBackground(true);
    d->_navigationButton->setLightHoverColor(NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light));
    d->_navigationButton->setDarkHoverColor(NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark));
    d->_navigationButton->setLightPressColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light));
    d->_navigationButton->setDarkPressColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark));
    d->_navigationButton->setLightBorderColor(Qt::transparent);
    d->_navigationButton->setDarkBorderColor(Qt::transparent);
    d->_navigationButton->setIcon(nIcon->fromRegular(NRegularIconType::Navigation16Regular));
    connect(d->_navigationButton, &NToolButton::clicked, d, &NNavigationBarPrivate::onNavigationButtonClicked);

    d->_searchButton = new NToolButton(this);
    d->_searchButton->setFixedSize(40, 38);
    d->_searchButton->setTransparentBackground(true);
    d->_searchButton->setLightHoverColor(NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light));
    d->_searchButton->setDarkHoverColor(NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark));
    d->_searchButton->setLightPressColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light));
    d->_searchButton->setDarkPressColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark));
    d->_searchButton->setLightBorderColor(Qt::transparent);
    d->_searchButton->setDarkBorderColor(Qt::transparent);
    d->_searchButton->setIcon(nIcon->fromRegular(NRegularIconType::Search16Regular));
    d->_searchButton->setVisible(false);

    d->_navigationSuggestBox = new NAutoSuggestBox(this);
    d->_navigationSuggestBox->setMinimumWidth(0);
    d->_navigationSuggestBox->setPlaceholderText(tr("search"));
    d->_navigationSuggestBox->setVisible(d->_pIsSearchVisible);

    d->_navigationButtonLayout = new QVBoxLayout();
    d->_navigationButtonLayout->setContentsMargins(0, 0, 0, 0);
    d->_navigationButtonLayout->setSpacing(0);
    d->_navigationButtonLayout->addWidget(d->_navigationButton);
    d->_navigationButtonLayout->addWidget(d->_searchButton);

    d->_navigationSuggestLayout = new QVBoxLayout();
    d->_navigationSuggestLayout->setContentsMargins(10, 0, 0, 0);
    d->_navigationSuggestLayout->addWidget(d->_navigationSuggestBox);

    connect(d->_navigationSuggestBox,
            &NAutoSuggestBox::suggestionClicked,
            this,
            [=](QString suggestText, QVariantMap suggestData) {
                NNavigationNode* node = nullptr;
                if (suggestData.value("NNodeType").toString() == "Stacked") {
                    node = d->_navigationModel->getNavigationNode(suggestData.value("NPageKey").toString());
                    if (node) {
                        d->onTreeViewClicked(node->getModelIndex());
                    }
                } else {
                    node = d->_footerModel->getNavigationNode(suggestData.value("NPageKey").toString());
                    if (node) {
                        d->onFooterViewClicked(node->getModelIndex());
                    }
                }
            });

    d->_navigationLayout = new QVBoxLayout();
    d->_navigationLayout->setContentsMargins(0, 0, 0, 0);
    d->_navigationLayout->setSpacing(4);
    d->_navigationLayout->addLayout(d->_navigationButtonLayout);
    d->_navigationLayout->addLayout(d->_navigationSuggestLayout);

    // 页脚视图

    d->_footerView = new NBaseListView(this);
    d->_footerView->setObjectName("navigationFooterView");
    d->_footerView->setFocusPolicy(Qt::StrongFocus);
    d->_footerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_footerView->installEventFilter(d);
    d->_footerView->setFixedHeight(0);
    d->_footerModel = new NNavigationFooterModel(this);
    d->_footerView->setModel(d->_footerModel);
    d->_footerDelegate = new NNavigationFooterDelegate(this);
    d->_footerView->setItemDelegate(d->_footerDelegate);
    d->_footerDelegate->setListView(d->_footerView);
    connect(d->_footerView, &NBaseListView::mousePress, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &NBaseListView::mouseDoubleClick, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &NBaseListView::mouseRelease, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(QModelIndex());
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &NBaseListView::clicked, this, [=](const QModelIndex& index) {
        d->onFooterViewClicked(index);
    });
    connect(d->_searchButton, &NToolButton::clicked, this, [d] { d->_showSearchAndFocus(); });

    // 布局设置
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 10, 5, 0);
    mainLayout->addLayout(d->_headerLayout);
    mainLayout->addLayout(d->_navigationLayout);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);
}

NNavigationBar::~NNavigationBar() {}

void NNavigationBar::setHeaderWidget(QWidget* widget) {
    Q_D(NNavigationBar);
    if (d->_headerWidget == widget) return;
    if (d->_headerWidget) {
        d->_headerLayout->removeWidget(d->_headerWidget);
        d->_headerWidget->setParent(nullptr);
    }
    d->_headerWidget = widget;

    if (widget) {
        d->_headerLayout->addWidget(widget);
        widget->setVisible(d->_currentDisplayMode == NNavigationType::Maximal);
    }
}

QWidget* NNavigationBar::headerWidget() const {
    Q_D(const NNavigationBar);
    return d->_headerWidget;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_resetNodeSelected();
        emit navigationNodeAdded(NNavigationType::ExpanderNode, expanderKey, nullptr);
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType NNavigationBar::addExpanderNode(QString                expanderTitle,
                                                                       QString&               expanderKey,
                                                                       QString                targetExpanderKey,
                                                                       NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_resetNodeSelected();
        emit navigationNodeAdded(NNavigationType::ExpanderNode, expanderKey, nullptr);
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addPageNode(pageTitle, pageKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    if (targetExpanderKey.isEmpty()) {
        return NNavigationType::TargetNodeInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }

    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, int keyPoints, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }

    return returnType;
}

NNavigationType::NodeOperateReturnType NNavigationBar::addPageNode(QString                pageTitle,
                                                                   QWidget*               page,
                                                                   QString                targetExpanderKey,
                                                                   int                    keyPoints,
                                                                   NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    if (targetExpanderKey.isEmpty()) {
        return NNavigationType::TargetNodeInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }

    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NRegularIconType::Icon icon) {
    return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType NNavigationBar::addFooterNode(QString                footerTitle,
                                                                     QWidget*               page,
                                                                     QString&               footerKey,
                                                                     int                    keyPoints,
                                                                     NRegularIconType::Icon icon) {
    if (page && NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) return NNavigationType::PageInvalid;
    NNavigationType::NodeOperateReturnType returnType =
        d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d_ptr->_addFooterPage(page, footerKey);
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, NFilledIconType::Icon icon) {
    Q_D(NNavigationBar);
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_resetNodeSelected();
        emit navigationNodeAdded(NNavigationType::ExpanderNode, expanderKey, nullptr);
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType NNavigationBar::addExpanderNode(QString               expanderTitle,
                                                                       QString&              expanderKey,
                                                                       QString               targetExpanderKey,
                                                                       NFilledIconType::Icon icon) {
    Q_D(NNavigationBar);
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_resetNodeSelected();
        emit navigationNodeAdded(NNavigationType::ExpanderNode, expanderKey, nullptr);
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, NFilledIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addPageNode(pageTitle, pageKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NFilledIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    if (targetExpanderKey.isEmpty()) {
        return NNavigationType::TargetNodeInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }

    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, int keyPoints, NFilledIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }

    return returnType;
}

NNavigationType::NodeOperateReturnType NNavigationBar::addPageNode(QString               pageTitle,
                                                                   QWidget*              page,
                                                                   QString               targetExpanderKey,
                                                                   int                   keyPoints,
                                                                   NFilledIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page || NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) {
        return NNavigationType::PageInvalid;
    }

    if (targetExpanderKey.isEmpty()) {
        return NNavigationType::TargetNodeInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d->_addStackedPage(page, pageKey);
        d->_resetNodeSelected();
    }

    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NFilledIconType::Icon icon) {
    return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, icon);
}

NNavigationType::NodeOperateReturnType NNavigationBar::addFooterNode(QString               footerTitle,
                                                                     QWidget*              page,
                                                                     QString&              footerKey,
                                                                     int                   keyPoints,
                                                                     NFilledIconType::Icon icon) {
    if (page && NNavigationManager::getInstance()->routerForKey(page->property("NPageKey").toString())) return NNavigationType::PageInvalid;
    NNavigationType::NodeOperateReturnType returnType =
        d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d_ptr->_addFooterPage(page, footerKey);
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType NNavigationBar::addPageComponent(NPageComponent* page, NRegularIconType::Icon icon) {
    if (!page) return NNavigationType::PageInvalid;
    return addPageNode(page->pageTitle(), page, icon);
}

NNavigationType::NodeOperateReturnType NNavigationBar::addPageComponent(NPageComponent* page, const QString& expanderKey, NRegularIconType::Icon icon) {
    if (!page) return NNavigationType::PageInvalid;
    return addPageNode(page->pageTitle(), page, expanderKey, icon);
}

NNavigationType::NodeOperateReturnType NNavigationBar::addPageComponent(NPageComponent* page, NFilledIconType::Icon icon) {
    if (!page) return NNavigationType::PageInvalid;
    return addPageNode(page->pageTitle(), page, icon);
}

NNavigationType::NodeOperateReturnType NNavigationBar::addPageComponent(NPageComponent* page, const QString& expanderKey, NFilledIconType::Icon icon) {
    if (!page) return NNavigationType::PageInvalid;
    return addPageNode(page->pageTitle(), page, expanderKey, icon);
}

bool NNavigationBar::getNavigationNodeIsExpanded(QString expanderKey) const {
    Q_D(const NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode()) {
        return false;
    }
    return d->_navigationView->isExpanded(node->getModelIndex());
}

void NNavigationBar::expandNavigationNode(QString expanderKey) {
    Q_D(NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode()) {
        return;
    }
    d->_expandOrCollpaseExpanderNode(node, true);
    d->_resetNodeSelected();
}

void NNavigationBar::collpaseNavigationNode(QString expanderKey) {
    Q_D(NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode()) {
        return;
    }
    d->_expandOrCollpaseExpanderNode(node, false);
    d->_resetNodeSelected();
}

void NNavigationBar::removeNavigationNode(QString nodeKey) {
    Q_D(NNavigationBar);
    auto* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node) node = d->_footerModel->getNavigationNode(nodeKey);
    if (!node) return;
    QList<NNavigationNode*> subtree{node};
    for (int i = 0; i < subtree.size(); ++i) subtree.append(subtree.at(i)->getChildrenNodes());
    QList<QPair<QString, NNavigationType::NavigationNodeType>> removed;
    for (auto* child : subtree) {
        const auto type = child->getIsFooterNode() ? NNavigationType::FooterNode
            : (child->getIsExpanderNode() ? NNavigationType::ExpanderNode : NNavigationType::PageNode);
        removed.append({child->getNodeKey(), type});
    }
    const bool footer = node->getIsFooterNode();
    if (footer) d->_footerModel->removeNavigationNode(nodeKey);
    else d->_navigationModel->removeNavigationNode(nodeKey);
    // 先注销整棵子树，随后发出的移除信号才允许选择剩余页面。
    QStringList routeKeys;
    for (const auto& entry : removed) routeKeys.append(entry.first);
    d->_router->unregisterPages(routeKeys);
    for (const auto& entry : removed) {
        d->_lastExpandedNodeKeys.removeAll(entry.first);
        const QString suggestion = d->_suggestKeyMap.take(entry.first);
        if (!suggestion.isEmpty()) d->_navigationSuggestBox->removeSuggestion(suggestion);
    }
    d->_footerView->setFixedHeight(40 * qMin(3, d->_footerModel->getFooterNodeCount()));
    d->_resetNodeSelected();
    for (const auto& entry : removed) emit navigationNodeRemoved(entry.second, entry.first);
}

void NNavigationBar::setNodeKeyPoints(QString nodeKey, int keyPoints) {
    Q_D(NNavigationBar);
    if (keyPoints < 0) return;
    NNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node) {
        node = d->_footerModel->getNavigationNode(nodeKey);
    } else if (node->getIsExpanderNode()) {
        return;
    }

    if (!node) {
        return;
    }

    node->setKeyPoints(keyPoints);
    if (node->getIsFooterNode()) {
        if (d->_footerView) {
            d->_footerView->viewport()->update();
        }
    } else {
        d->_navigationView->viewport()->update();
    }
}

int NNavigationBar::getNodeKeyPoints(QString nodeKey) const {
    Q_D(const NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node) {
        node = d->_footerModel->getNavigationNode(nodeKey);
    } else if (node->getIsExpanderNode()) {
        return -1;
    }

    if (!node) {
        return -1;
    }

    return node->getKeyPoints();
}

void NNavigationBar::setSearchVisible(bool visible) {
    Q_D(NNavigationBar);
    if (d->_pIsSearchVisible != visible) {
        d->_pIsSearchVisible = visible;

        d->_navigationSuggestBox->setVisible(visible && d->_currentDisplayMode == NNavigationType::Maximal);
        d->_searchButton->setVisible(visible && d->_currentDisplayMode == NNavigationType::Compact);
    }
}

bool NNavigationBar::isSearchVisible() const {
    Q_D(const NNavigationBar);
    return d->_pIsSearchVisible;
}

void NNavigationBar::navigation(QString pageKey, bool isLogClicked) {
    Q_D(NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
    if (!node) {
        node = d->_footerModel->getNavigationNode(pageKey);
    }

    if (node) {
        if (!isLogClicked && !node->getIsExpanderNode() && (!node->getIsFooterNode() || node->getIsHasFooterPage())) {
            d->_router->replaceRoute(pageKey);
            return;
        }
        if (node->getIsFooterNode()) {
            d->onFooterViewClicked(node->getModelIndex(), isLogClicked);
        } else {
            if (!node->getIsExpanderNode()) {
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked);
            }
        }
    }
}

NNavigationRouter* NNavigationBar::router() const { return d_ptr->_router; }

NNavigationType::NavigationDisplayMode NNavigationBar::displayMode() const { return d_ptr->_currentDisplayMode; }

void NNavigationBar::setDisplayMode(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation) {
    Q_D(NNavigationBar);
    if (d->_currentDisplayMode == displayMode || displayMode == NNavigationType::Auto) {
        return;
    }

    d->_doComponentAnimation(displayMode, isAnimation);
    d->_raiseNavigationBar();
}

void NNavigationBar::handleRouteChanged(const QString& pageKey, const QVariantMap& params) {
    Q_UNUSED(params);
    d_ptr->syncSelection(pageKey);
}

void NNavigationBar::handleRouteBack(const QString& fromPageKey, const QString& toPageKey, const QVariantMap& params) {
    Q_UNUSED(fromPageKey);
    Q_UNUSED(params);
    d_ptr->syncSelection(toPageKey);
}

void NNavigationBar::paintEvent(QPaintEvent* event) {
    Q_D(NNavigationBar);
    if (!d->_pIsTransparent) {
        QPainter painter(this);
        painter.save();
        painter.setPen(NThemeColor(NFluentColorKey::SurfaceStrokeColorDefault, d->_themeMode));
        painter.setBrush(NThemeColor(NFluentColorKey::LayerFillColorDefault, d->_themeMode));

        QRect baseRect = this->rect();
        baseRect.adjust(-1, 0, -1, 0);
        QPainterPath path;
        path.moveTo(baseRect.topLeft());
        path.lineTo(QPoint(baseRect.right() - 8, baseRect.y()));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.y(), 16, 16), 90, -90);
        path.lineTo(QPoint(baseRect.right(), baseRect.bottom() - 8));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.bottom() - 16, 16, 16), 0, -90);
        path.lineTo(baseRect.bottomLeft());
        path.closeSubpath();
        painter.drawPath(path);
        painter.restore();
    }
    QWidget::paintEvent(event);
}
