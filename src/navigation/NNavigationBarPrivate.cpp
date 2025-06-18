#include "NNavigationBarPrivate.h"

#include <QLayout>
#include <QPropertyAnimation>

#include <QMenu>
#include "NFooterDelegate.h"
#include "NFooterModel.h"
#include "NNavigationModel.h"
#include "NNavigationNode.h"
#include "NNavigationRouter.h"
#include "NNavigationView.h"
#include "QtNativeUI/NAutoSuggestBox.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolButton.h"

NNavigationBarPrivate::NNavigationBarPrivate(QObject* parent) : QObject{parent} {}

NNavigationBarPrivate::~NNavigationBarPrivate() {}

void NNavigationBarPrivate::onNavigationButtonClicked() {
    Q_Q(NNavigationBar);
    if (_currentDisplayMode == NNavigationType::Compact) {
        q->setDisplayMode(NNavigationType::Maximal);
    } else {
        q->setDisplayMode(NNavigationType::Compact);
    }
}

void NNavigationBarPrivate::onNavigationRouteBack(QVariantMap routeData) {
    Q_Q(NNavigationBar);
    QString pageKey = routeData.value("NPageKey").toString();
    q->navigation(pageKey, false);
}

void NNavigationBarPrivate::onTreeViewClicked(const QModelIndex& index, bool isLogRoute) {
    Q_Q(NNavigationBar);
    if (index.isValid()) {
        NNavigationNode* node = static_cast<NNavigationNode*>(index.internalPointer());
        if (!node) {
            return;
        }

        if (node->getIsExpanderNode()) {
            if (_currentDisplayMode == NNavigationType::Compact) {
                if (node->getIsHasPageChild()) {
                    // 展开菜单
                    QMenu* menu = _compactMenuMap.value(node);
                    if (menu) {
                        QPoint nodeTopRight =
                            _navigationView->mapToGlobal(_navigationView->visualRect(node->getModelIndex()).topRight());
                        menu->popup(QPoint(nodeTopRight.x() + 10, nodeTopRight.y()));
                    }
                }
            } else {
                if (node->getIsHasChild()) {
                    QVariantMap data;
                    if (_navigationView->isExpanded(index)) {
                        // 收起
                        data.insert("Collapse", QVariant::fromValue(node));
                        node->setIsExpanded(false);
                        _navigationView->navigationNodeStateChange(data);
                        _navigationView->collapse(index);
                    } else {
                        // 展开
                        data.insert("Expand", QVariant::fromValue(node));
                        node->setIsExpanded(true);
                        _navigationView->navigationNodeStateChange(data);
                        _navigationView->expand(index);
                    }
                }
            }
        } else {
            if (node->getKeyPoints()) {
                node->setKeyPoints(0);
                _navigationView->update();
            }

            NNavigationNode* selectedNode = _navigationModel->getSelectedNode();
            if (selectedNode != node) {
                // 记录跳转
                if (isLogRoute) {
                    QVariantMap routeData = QVariantMap();
                    QString     pageKey;
                    if (selectedNode) {
                        pageKey.append(selectedNode->getNodeKey());
                    } else {
                        if (_footerModel->getSelectedNode()) {
                            pageKey.append(_footerModel->getSelectedNode()->getNodeKey());
                        }
                    }
                    routeData.insert("NPageKey", pageKey);

                    NNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
                }

                Q_EMIT q->navigationNodeClicked(NNavigationType::PageNode, node->getNodeKey());

                if (_footerModel->getSelectedNode()) {
                    _footerView->clearSelection();
                    QVariantMap footerPostData = QVariantMap();
                    footerPostData.insert("SelectMarkChanged", true);
                    footerPostData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
                    footerPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                    _footerModel->setSelectedNode(nullptr);
                    _footerDelegate->navigationNodeStateChange(footerPostData);
                }

                QVariantMap postData = QVariantMap();
                postData.insert("SelectMarkChanged", true);
                if (_navigationModel->getSelectedExpandedNode()) {
                    postData.insert("LastSelectedNode",
                                    QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
                } else {
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()));
                }

                if (_currentDisplayMode == NNavigationType::Compact) {
                    NNavigationNode* originNode = node->getOriginalNode();
                    if (originNode == node) {
                        postData.insert("SelectedNode", QVariant::fromValue(node));
                    } else {
                        if (originNode == _navigationModel->getSelectedExpandedNode()) {
                            _navigationModel->setSelectedNode(node);
                            _resetNodeSelected();
                            return;
                        }
                        _navigationModel->setSelectedExpandedNode(originNode);
                        postData.insert("SelectedNode", QVariant::fromValue(originNode));
                    }
                } else {
                    postData.insert("SelectedNode", QVariant::fromValue(node));
                }

                _navigationModel->setSelectedNode(node);
                _navigationView->navigationNodeStateChange(postData);

                if (!node->getIsVisible() && _currentDisplayMode != NNavigationType::Compact) {
                    _expandSelectedNodeParent();
                }
            }
        }
        _resetNodeSelected();
    }
}

void NNavigationBarPrivate::onFooterViewClicked(const QModelIndex& index, bool isLogRoute) {
    Q_Q(NNavigationBar);
    NNavigationNode* node = index.data(Qt::UserRole).value<NNavigationNode*>();
    if (!node)
        return;

    if (node->getKeyPoints()) {
        node->setKeyPoints(0);
        _footerView->viewport()->update();
    }

    NNavigationNode* selectedNode = _footerModel->getSelectedNode();

    if (selectedNode != node) {
        // 记录跳转
        if (isLogRoute && node->getIsHasFooterPage()) {
            QVariantMap routeData = QVariantMap();
            QString     pageKey;
            if (selectedNode) {
                pageKey.append(selectedNode->getNodeKey());
            } else {
                if (_navigationModel->getSelectedNode()) {
                    pageKey.append(_navigationModel->getSelectedNode()->getNodeKey());
                }
            }
            routeData.insert("NPageKey", pageKey);

            NNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
        }

        Q_EMIT q->navigationNodeClicked(NNavigationType::FooterNode, node->getNodeKey());

        if (node->getIsHasFooterPage()) {
            if (_navigationModel->getSelectedNode() || _navigationModel->getSelectedExpandedNode()) {
                QVariantMap mainPostData = QVariantMap();
                mainPostData.insert("SelectMarkChanged", true);
                mainPostData.insert("LastSelectedNode",
                                    QVariant::fromValue(_navigationModel->getSelectedExpandedNode()
                                                            ? _navigationModel->getSelectedExpandedNode()
                                                            : _navigationModel->getSelectedNode()));
                mainPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                _navigationView->clearSelection();
                _navigationView->navigationNodeStateChange(mainPostData);
                _navigationModel->setSelectedExpandedNode(nullptr);
                _navigationModel->setSelectedNode(nullptr);
            }

            _footerView->clearSelection();
            _footerView->selectionModel()->select(index, QItemSelectionModel::Select);
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(node));
            _footerDelegate->navigationNodeStateChange(postData);
            _footerModel->setSelectedNode(node);
        }
    }
}

void NNavigationBarPrivate::_resetNodeSelected() {
    _navigationView->clearSelection();
    NNavigationNode* selectedNode = _navigationModel->getSelectedNode();
    if (!selectedNode || !selectedNode->getModelIndex().isValid()) {
        return;
    }

    if (selectedNode->getParentNode()->getIsRootNode() || selectedNode->getIsVisible()) {
        _navigationView->selectionModel()->select(selectedNode->getModelIndex(), QItemSelectionModel::Select);
        if (_navigationModel->getSelectedExpandedNode()) {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(selectedNode));
            _navigationView->navigationNodeStateChange(postData);
        }
        _navigationModel->setSelectedExpandedNode(nullptr);
    } else {
        NNavigationNode* parentNode = selectedNode->getParentNode();
        while (parentNode && !parentNode->getParentNode()->getIsRootNode() && !parentNode->getIsVisible()) {
            parentNode = parentNode->getParentNode();
        }

        if (!parentNode) {
            return;
        }

        // 单级节点展开/收起时Mark变化
        if (!_navigationModel->getSelectedExpandedNode()) {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(parentNode));
            _navigationView->navigationNodeStateChange(postData);
        } else {
            // 多级节点
            if (_navigationModel->getSelectedExpandedNode() != parentNode) {
                // 同一起源节点展开/收起时的Mark变化
                if (_navigationModel->getSelectedExpandedNode()->getOriginalNode() == parentNode->getOriginalNode()) {
                    QVariantMap postData = QVariantMap();
                    postData.insert("SelectMarkChanged", true);
                    postData.insert("LastSelectedNode",
                                    QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
                    postData.insert("SelectedNode", QVariant::fromValue(parentNode));
                    _navigationView->navigationNodeStateChange(postData);
                }
            }
        }

        _navigationModel->setSelectedExpandedNode(parentNode);
        _navigationView->selectionModel()->select(parentNode->getModelIndex(), QItemSelectionModel::Select);
    }
}

void NNavigationBarPrivate::_expandSelectedNodeParent() {
    NNavigationNode* parentNode = _navigationModel->getSelectedNode()->getParentNode();
    while (parentNode && !parentNode->getIsRootNode()) {
        QVariantMap data;
        data.insert("Expand", QVariant::fromValue(parentNode));
        _navigationView->navigationNodeStateChange(data);
        parentNode->setIsExpanded(true);
        _navigationView->expand(parentNode->getModelIndex());
        parentNode = parentNode->getParentNode();
    }
}

void NNavigationBarPrivate::_expandOrCollpaseExpanderNode(NNavigationNode* node, bool isExpand) {
    if (_currentDisplayMode == NNavigationType::Compact) {
        if (node->getIsHasPageChild()) {
            // 展开菜单
            NMenu* menu = _compactMenuMap.value(node);
            if (menu) {
                QPoint nodeTopRight =
                    _navigationView->mapToGlobal(_navigationView->visualRect(node->getModelIndex()).topRight());
                menu->popup(QPoint(nodeTopRight.x() + 10, nodeTopRight.y()));
            }
        }
    } else {
        QModelIndex index      = node->getModelIndex();
        bool        isExpanded = _navigationView->isExpanded(index);
        if (node->getIsHasChild() && isExpand != isExpanded) {
            QVariantMap data;
            if (isExpanded) {
                // 收起
                data.insert("Collapse", QVariant::fromValue(node));
                node->setIsExpanded(isExpand);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->collapse(index);
            } else {
                // 展开
                data.insert("Expand", QVariant::fromValue(node));
                node->setIsExpanded(true);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->expand(index);
            }
        }
    }
}

void NNavigationBarPrivate::_initNodeModelIndex(const QModelIndex& parentIndex) {
    int rowCount = _navigationModel->rowCount(parentIndex);
    for (int row = 0; row < rowCount; ++row) {
        QModelIndex      index     = _navigationModel->index(row, 0, parentIndex);
        NNavigationNode* childNode = static_cast<NNavigationNode*>(index.internalPointer());
        childNode->setModelIndex(index);

        if (_navigationModel->hasChildren(index)) {
            _initNodeModelIndex(index);
        }
    }
}

void NNavigationBarPrivate::_addStackedPage(QWidget* page, QString pageKey) {
    Q_Q(NNavigationBar);
    page->setProperty("NPageKey", pageKey);
    Q_EMIT q->navigationNodeAdded(NNavigationType::PageNode, pageKey, page);

    NNavigationNode* node = _navigationModel->getNavigationNode(pageKey);

    QVariantMap suggestData;
    suggestData.insert("NNodeType", "Stacked");
    suggestData.insert("NPageKey", pageKey);
    QString suggestKey = _navigationSuggestBox->addSuggestion(node->getIcon(), node->getNodeTitle(), suggestData);
    _suggestKeyMap.insert(pageKey, suggestKey);
}

void NNavigationBarPrivate::_addFooterPage(QWidget* page, QString footerKey) {
    Q_Q(NNavigationBar);
    Q_EMIT q->navigationNodeAdded(NNavigationType::FooterNode, footerKey, page);

    if (page) {
        page->setProperty("NPageKey", footerKey);
    }
    _footerView->setFixedHeight(40 * _footerModel->getFooterNodeCount());
    NNavigationNode* node = _footerModel->getNavigationNode(footerKey);
    QVariantMap      suggestData;
    suggestData.insert("NNodeType", "Footer");
    suggestData.insert("NPageKey", footerKey);
    QString suggestKey = _navigationSuggestBox->addSuggestion(node->getIcon(), node->getNodeTitle(), suggestData);
    _suggestKeyMap.insert(footerKey, suggestKey);
}

void NNavigationBarPrivate::_raiseNavigationBar() {
    Q_Q(NNavigationBar);
    q->raise();
}

void NNavigationBarPrivate::_doComponentAnimation(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation) {
    // 设置动画状态，防止多次调用
    if (_isAnimating)
        return;
        
    _isAnimating = true;
    
    switch (displayMode) {
        case NNavigationType::Minimal: {
            // 隐藏搜索按钮和搜索框
            _searchButton->setVisible(false);
            _navigationSuggestBox->setVisible(false);
            
            // 执行动画
            _doNavigationBarWidthAnimation(displayMode, isAnimation);
            if (_currentDisplayMode == NNavigationType::Maximal) {
                _handleNavigationExpandState(true);
            }
            _currentDisplayMode = displayMode;
            break;
        }
        case NNavigationType::Compact: {
            // 首先调整组件可见性
            _navigationSuggestBox->setVisible(false);
            _searchButton->setVisible(true);
            
            // 执行宽度动画
            _doNavigationBarWidthAnimation(displayMode, isAnimation);
            _doNavigationViewWidthAnimation(isAnimation);
            
            if (_currentDisplayMode != NNavigationType::Minimal) {
                _handleNavigationExpandState(true);
            }
            
            _currentDisplayMode = displayMode;
            break;
        }
        case NNavigationType::Maximal: {
            // 调整组件可见性
            _searchButton->setVisible(false);
            _navigationSuggestBox->setVisible(true);
            
            // 执行宽度动画
            _doNavigationBarWidthAnimation(displayMode, isAnimation);
            
            _currentDisplayMode = displayMode;
            _handleNavigationExpandState(false);
            break;
        }
        default: {
            _isAnimating = false;
            break;
        }
    }
}

void NNavigationBarPrivate::_handleNavigationExpandState(bool isSave) {
    if (isSave) {
        // 保存展开状态 收起根节点所有子树
        _lastExpandedNodesList = _navigationModel->getRootExpandedNodes();
        for (auto node : _lastExpandedNodesList) {
            onTreeViewClicked(node->getModelIndex(), false);
        }
    } else {
        for (auto node : _lastExpandedNodesList) {
            // 修正动画覆盖
            _navigationView->resize(295, _navigationView->height());
            onTreeViewClicked(node->getModelIndex(), false);
        }
    }
}

void NNavigationBarPrivate::_doNavigationBarWidthAnimation(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation) {
    Q_Q(NNavigationBar);
    QPropertyAnimation* navigationBarWidthAnimation = new QPropertyAnimation(q, "maximumWidth");
    navigationBarWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
    navigationBarWidthAnimation->setStartValue(q->width());
    navigationBarWidthAnimation->setDuration(isAnimation ? 285 : 0);
    
    // 添加动画完成时的回调
    connect(navigationBarWidthAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isAnimating = false;
    });
    
    switch (displayMode) {
        case NNavigationType::Minimal: {
            connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                q->setFixedWidth(value.toInt());
            });
            navigationBarWidthAnimation->setEndValue(0);
            break;
        }
        case NNavigationType::Compact: {
            connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                q->setFixedWidth(value.toInt());
            });
            navigationBarWidthAnimation->setEndValue(47);
            break;
        }
        case NNavigationType::Maximal: {
            connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                q->setFixedWidth(value.toInt());
            });
            navigationBarWidthAnimation->setEndValue(300);
            break;
        }
        default: {
            break;
        }
    }
    navigationBarWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NNavigationBarPrivate::_doNavigationViewWidthAnimation(bool isAnimation) {
    QPropertyAnimation* navigationViewWidthAnimation = new QPropertyAnimation(this, "pNavigationViewWidth");
    connect(navigationViewWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _navigationView->setColumnWidth(0, value.toInt());
    });

    navigationViewWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
    navigationViewWidthAnimation->setStartValue(_navigationView->columnWidth(0));
    navigationViewWidthAnimation->setEndValue(40);
    navigationViewWidthAnimation->setDuration(isAnimation ? 285 : 0);
    navigationViewWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}