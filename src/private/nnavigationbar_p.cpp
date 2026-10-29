#include "nnavigationbar_p.h"

#include <QLayout>
#include <QPropertyAnimation>

#include <QMenu>
#include <QTimer>
#include <QKeyEvent>
#include <functional>

#include "../../include/QtNativeUI/NNavigationRouter.h"
#include "QtNativeUI/NAutoSuggestBox.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NNavigationView.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolButton.h"
#include "nnavigationfooterdelegate_p.h"
#include "nnavigationfootermodel_p.h"
#include "nnavigationmanager.h"
#include "nnavigationmodel_p.h"
#include "nnavigationnode_p.h"
#include "nnavigationtreeview_p.h"

NNavigationBarPrivate::NNavigationBarPrivate(QObject* parent) : QObject{parent} {
    _pNavigationViewWidth = 295;
    _pPaneAnimatedWidth = 300;
}

NNavigationBarPrivate::~NNavigationBarPrivate() {}

void NNavigationBarPrivate::syncSelection(const QString& key) {
    if (auto* node = _navigationModel->getNavigationNode(key)) onTreeViewClicked(node->getModelIndex(), false);
    else if (auto* node = _footerModel->getNavigationNode(key)) onFooterViewClicked(node->getModelIndex(), false);
}

bool NNavigationBarPrivate::eventFilter(QObject* watched, QEvent* event) {
    if (watched == _footerView && event->type() == QEvent::KeyPress) {
        auto* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Return || key->key() == Qt::Key_Enter || key->key() == Qt::Key_Space) {
            onFooterViewClicked(_footerView->currentIndex());
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}

void NNavigationBarPrivate::onNavigationButtonClicked() {
    Q_Q(NNavigationBar);
    if (qobject_cast<NNavigationView*>(q->parentWidget())) {
        emit q->paneToggleRequested();
        return;
    }
    if (_currentDisplayMode != NNavigationType::Maximal) {
        q->setDisplayMode(NNavigationType::Maximal);
    } else {
        q->setDisplayMode(NNavigationType::Compact);
    }
}

void NNavigationBarPrivate::onTreeViewClicked(const QModelIndex& index, bool logRoute) {
    Q_Q(NNavigationBar);
    if (!index.isValid() || index.model() != _navigationModel) return;
    auto* node = static_cast<NNavigationNode*>(index.internalPointer());
    if (node->getIsExpanderNode()) {
        _expandOrCollpaseExpanderNode(node, !_navigationView->isExpanded(index));
        _resetNodeSelected();
        return;
    }
    if (logRoute) {
        _router->navigateTo(node->getNodeKey(), {{"source", "treeView"}});
        return;
    }
    if (_navigationModel->getSelectedNode() == node) return;
    node->setKeyPoints(0);
    if (auto* previous = _footerModel->getSelectedNode()) {
        _footerDelegate->navigationNodeStateChange({{"SelectMarkChanged", true},
            {"LastSelectedNode", QVariant::fromValue(previous)}, {"SelectedNode", QVariant::fromValue<NNavigationNode*>(nullptr)}});
        _footerModel->setSelectedNode(nullptr);
        _footerView->clearSelection();
    }
    _navigationModel->setSelectedNode(node);
    if (_currentDisplayMode == NNavigationType::Maximal) _expandSelectedNodeParent();
    _resetNodeSelected();
    emit q->navigationNodeClicked(NNavigationType::PageNode, node->getNodeKey());
}

void NNavigationBarPrivate::onFooterViewClicked(const QModelIndex& index, bool logRoute) {
    Q_Q(NNavigationBar);
    if (!index.isValid() || index.model() != _footerModel) return;
    auto* node = index.data(Qt::UserRole).value<NNavigationNode*>();
    if (!node) return;
    if (!node->getIsHasFooterPage()) {
        emit q->navigationNodeClicked(NNavigationType::FooterNode, node->getNodeKey());
        return;
    }
    if (logRoute) {
        _router->navigateTo(node->getNodeKey(), {{"source", "footerView"}});
        return;
    }
    if (_footerModel->getSelectedNode() == node) return;
    node->setKeyPoints(0);
    auto* previous = _footerModel->getSelectedNode();
    _navigationModel->setSelectedNode(nullptr);
    _resetNodeSelected();
    _footerModel->setSelectedNode(node);
    _footerView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    _footerDelegate->navigationNodeStateChange({{"SelectMarkChanged", true},
        {"LastSelectedNode", QVariant::fromValue(previous)}, {"SelectedNode", QVariant::fromValue(node)}});
    _footerView->viewport()->update();
    emit q->navigationNodeClicked(NNavigationType::FooterNode, node->getNodeKey());
}

void NNavigationBarPrivate::_resetNodeSelected() {
    auto* selected = _navigationModel->getSelectedNode();
    auto* marker = selected;
    if (marker && _currentDisplayMode != NNavigationType::Maximal) marker = marker->getOriginalNode();
    while (marker && !marker->getIsVisible() && !marker->getParentNode()->getIsRootNode()) marker = marker->getParentNode();
    _navigationModel->setSelectedExpandedNode(marker != selected ? marker : nullptr);
    if (marker) {
        _navigationView->selectionModel()->setCurrentIndex(marker->getModelIndex(), QItemSelectionModel::ClearAndSelect);
    } else {
        _navigationView->clearSelection();
        _navigationView->setCurrentIndex({});
    }
    if (_displayedNode != marker) {
        _navigationView->navigationNodeStateChange({{"SelectMarkChanged", true},
            {"LastSelectedNode", QVariant::fromValue(_displayedNode.data())}, {"SelectedNode", QVariant::fromValue(marker)}});
        _displayedNode = marker;
    }
    _navigationView->viewport()->update();
}

void NNavigationBarPrivate::_expandSelectedNodeParent() {
    QList<NNavigationNode*> parents;
    for (auto* parent = _navigationModel->getSelectedNode()->getParentNode();
         parent && !parent->getIsRootNode(); parent = parent->getParentNode()) {
        parents.prepend(parent);
    }
    for (auto* parent : parents) {
        // 已展开的分组只更新选中项，不能重播展开动画。
        _expandOrCollpaseExpanderNode(parent, true);
    }
}

void NNavigationBarPrivate::_expandOrCollpaseExpanderNode(NNavigationNode* node, bool isExpand) {
    if (_currentDisplayMode == NNavigationType::Compact) {
        if (!isExpand) return;
        if (!node->getIsHasPageChild())
            return;
        _showCompactMenu(node);
    } else {
        QModelIndex index      = node->getModelIndex();
        bool        isExpanded = _navigationView->isExpanded(index);
        if (node->getIsHasChild() && isExpand != isExpanded) {
            QVariantMap data;
            if (isExpanded) {
                data.insert("Collapse", QVariant::fromValue(node));
                node->setIsExpanded(isExpand);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->collapse(index);
            } else {
                data.insert("Expand", QVariant::fromValue(node));
                node->setIsExpanded(true);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->expand(index);
            }
        }
    }
}

void NNavigationBarPrivate::_addStackedPage(QWidget* page, QString pageKey) {
    Q_Q(NNavigationBar);
    page->setProperty("NPageKey", pageKey);
    _router->registerPage(pageKey, page);

    NNavigationNode* node = _navigationModel->getNavigationNode(pageKey);
    if (node) {
        NPageComponent* pageComponent = qobject_cast<NPageComponent*>(page);
        if (pageComponent) {
            pageComponent->setRouteKey(pageKey);
            NNavigationManager::getInstance()->registerPageComponent(pageComponent);
        }
        // 添加搜索建议
        if (_navigationSuggestBox && !node->getNodeTitle().isEmpty()) {
            QVariantMap data{{"NNodeType", "Stacked"}, {"NPageKey", pageKey}};
            QString suggestKey = _navigationSuggestBox->addSuggestion(node->getNodeTitle(), data);
            _suggestKeyMap.insert(pageKey, suggestKey);
        }
    }
    connect(page, &QObject::destroyed, this, [q, pageKey] { q->removeNavigationNode(pageKey); });
    Q_EMIT q->navigationNodeAdded(NNavigationType::PageNode, pageKey, page);
    if (_router->currentRouteKey().isEmpty()) _router->navigateTo(pageKey);
}

void NNavigationBarPrivate::_addFooterPage(QWidget* page, QString footerKey) {
    Q_Q(NNavigationBar);
    if (page) {
        page->setProperty("NPageKey", footerKey);
        _router->registerPage(footerKey, page);
        connect(page, &QObject::destroyed, this, [q, footerKey] { q->removeNavigationNode(footerKey); });

        NPageComponent* pageComponent = qobject_cast<NPageComponent*>(page);
        if (pageComponent) {
            pageComponent->setRouteKey(footerKey);
            NNavigationManager::getInstance()->registerPageComponent(pageComponent);
        }
    }
    NNavigationNode* node = _footerModel->getNavigationNode(footerKey);
    if (node && _navigationSuggestBox && !node->getNodeTitle().isEmpty()) {
        QVariantMap suggestData;
        suggestData["NNodeType"] = "Footer";
        suggestData["NPageKey"]  = footerKey;
        QString suggestKey       = _navigationSuggestBox->addSuggestion(node->getNodeTitle(), suggestData);
        _suggestKeyMap.insert(footerKey, suggestKey);
    }
    _footerView->setVisible(_currentDisplayMode != NNavigationType::Minimal);
    _footerView->setFixedHeight(40 * qMin(3, _footerModel->getFooterNodeCount()));
    _footerView->viewport()->update();
    Q_EMIT q->navigationNodeAdded(NNavigationType::FooterNode, footerKey, page);
    if (page && _router->currentRouteKey().isEmpty()) _router->navigateTo(footerKey);
}

void NNavigationBarPrivate::_showCompactMenu(NNavigationNode* node) {
    Q_Q(NNavigationBar);
    auto* menu = new NMenu(q);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    std::function<void(NMenu*, NNavigationNode*)> appendChildren = [&](NMenu* parent, NNavigationNode* group) {
        for (auto* child : group->getChildrenNodes()) {
            if (child->getIsExpanderNode()) {
                auto* submenu = new NMenu(child->getNodeTitle(), parent);
                parent->addMenu(submenu);
                appendChildren(submenu, child);
            } else {
                QIcon icon = child->getIcon() != NRegularIconType::None
                    ? nIcon->fromRegular(child->getIcon()) : nIcon->fromFilled(child->getFilledIcon());
                auto* action = parent->addAction(icon, child->getNodeTitle());
                const QString key = child->getNodeKey();
                connect(action, &QAction::triggered, q, [q, key] { q->navigation(key); });
            }
        }
    };
    appendChildren(menu, node);
    const auto corner = _navigationView->viewport()->mapToGlobal(_navigationView->visualRect(node->getModelIndex()).topRight());
    menu->popup(corner + QPoint(10, 0));
}

void NNavigationBarPrivate::_raiseNavigationBar() {
    Q_Q(NNavigationBar);
    q->raise();
}

void NNavigationBarPrivate::_doComponentAnimation(NNavigationType::NavigationDisplayMode mode, bool animate) {
    Q_Q(NNavigationBar);
    const auto previous = _currentDisplayMode;
    if (previous == NNavigationType::Maximal && mode != previous) _handleNavigationExpandState(true);
    _currentDisplayMode = mode;
    _navigationSuggestBox->setVisible(mode == NNavigationType::Maximal && _pIsSearchVisible);
    _searchButton->setVisible(mode == NNavigationType::Compact && _pIsSearchVisible);
    _navigationView->setVisible(mode != NNavigationType::Minimal);
    _footerView->setVisible(mode != NNavigationType::Minimal && _footerModel->getFooterNodeCount() > 0);
    if (_headerWidget) _headerWidget->setVisible(mode == NNavigationType::Maximal);
    if (mode == NNavigationType::Maximal && previous != mode) _handleNavigationExpandState(false);
    _doNavigationBarWidthAnimation(mode, animate);
    _doNavigationViewWidthAnimation(animate);
    _resetNodeSelected();
    emit q->displayModeChange(mode);
}

void NNavigationBarPrivate::_handleNavigationExpandState(bool save) {
    if (save) {
        _lastExpandedNodeKeys.clear();
        for (auto* node : _navigationModel->getRootExpandedNodes()) {
            _lastExpandedNodeKeys.append(node->getNodeKey());
            node->setIsExpanded(false);
            _navigationView->collapse(node->getModelIndex());
        }
    } else {
        for (const auto& key : _lastExpandedNodeKeys) {
            if (auto* node = _navigationModel->getNavigationNode(key)) {
                node->setIsExpanded(true);
                _navigationView->expand(node->getModelIndex());
            }
        }
        _lastExpandedNodeKeys.clear();
    }
}

void NNavigationBarPrivate::_showSearchAndFocus() {
    Q_Q(NNavigationBar);

    if (_currentDisplayMode != NNavigationType::Maximal) {
        if (auto* view = qobject_cast<NNavigationView*>(q->parentWidget())) view->setPaneOpen(true);
        else q->setDisplayMode(NNavigationType::Maximal, true);
        QTimer::singleShot(0, this, [this]() {
            if (_navigationSuggestBox) {
                _navigationSuggestBox->setFocus();
            }
        });
    } else {
        if (_navigationSuggestBox) {
            _navigationSuggestBox->setFocus();
        }
    }
}

void NNavigationBarPrivate::_doNavigationBarWidthAnimation(NNavigationType::NavigationDisplayMode mode, bool animate) {
    Q_Q(NNavigationBar);
    if (!_widthAnimation) {
        _widthAnimation = new QPropertyAnimation(this, "pPaneAnimatedWidth", this);
        _widthAnimation->setEasingCurve(QEasingCurve::OutCubic);
        connect(_widthAnimation, &QPropertyAnimation::valueChanged, this, [q](const QVariant& value) {
            q->setFixedWidth(value.toInt());
        });
    }
    _widthAnimation->stop();
    _widthAnimation->setStartValue(q->width());
    _widthAnimation->setEndValue(mode == NNavigationType::Maximal ? 300 : 47);
    _widthAnimation->setDuration(animate ? 285 : 0);
    _widthAnimation->start();
}

void NNavigationBarPrivate::_doNavigationViewWidthAnimation(bool animate) {
    if (!_viewWidthAnimation) {
        _viewWidthAnimation = new QPropertyAnimation(this, "pNavigationViewWidth", this);
        _viewWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
        connect(_viewWidthAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
            _navigationView->setColumnWidth(0, value.toInt());
        });
    }
    _viewWidthAnimation->stop();
    _viewWidthAnimation->setStartValue(_navigationView->columnWidth(0));
    _viewWidthAnimation->setEndValue(_currentDisplayMode == NNavigationType::Maximal ? 295 : 40);
    _viewWidthAnimation->setDuration(animate ? 285 : 0);
    _viewWidthAnimation->start();
}
