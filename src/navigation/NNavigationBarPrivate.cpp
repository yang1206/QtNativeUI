#include "NNavigationBarPrivate.h"

#include <QLayout>
#include <QPropertyAnimation>

#include <QMenu>
#include "NFooterDelegate.h"
#include "NFooterModel.h"
#include "NNavigationModel.h"
#include "NNavigationNode.h"
#include "NNavigationView.h"
#include "QtNativeUI/NLineEdit.h"
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

void NNavigationBarPrivate::onNavigationOpenNewWindow(QString nodeKey) {
    Q_Q(NNavigationBar);
    const QMetaObject* meta = _pageMetaMap.value(nodeKey);
    if (!meta) {
        return;
    }

    QWidget* widget = static_cast<QWidget*>(meta->newInstance());
    if (widget) {
        // 这里可以创建一个新窗口并设置widget作为其中心部件
        // 暂时简化处理
        widget->show();
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
                    // 这里应该使用NavigationRouter，暂时保留空实现
                    // NNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
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
    if (node->getKeyPoints()) {
        node->setKeyPoints(0);
        _navigationView->update();
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
            // 这里应该使用NavigationRouter，暂时保留空实现
            // NNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
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

    // 如果有建议框功能，这里应该添加建议
    // QVariantMap suggestData;
    // suggestData.insert("NNodeType", "Stacked");
    // suggestData.insert("NPageKey", pageKey);
    // QString suggestKey = _navigationSuggestBox->addSuggestion(node->getIcon(), node->getNodeTitle(), suggestData);
    // _suggestKeyMap.insert(pageKey, suggestKey);
}

void NNavigationBarPrivate::_addFooterPage(QWidget* page, QString footKey) {
    Q_Q(NNavigationBar);
    Q_EMIT q->navigationNodeAdded(NNavigationType::FooterNode, footKey, page);

    if (page) {
        page->setProperty("NPageKey", footKey);
    }

    // 如果有_footerView，更新其高度
    // _footerView->setFixedHeight(40 * _footerModel->getFooterNodeCount());

    NNavigationNode* node = _footerModel->getNavigationNode(footKey);

    // 如果有建议框功能，这里应该添加建议
    // QVariantMap suggestData;
    // suggestData.insert("NNodeType", "Footer");
    // suggestData.insert("NPageKey", footKey);
    // QString suggestKey = _navigationSuggestBox->addSuggestion(node->getIcon(), node->getNodeTitle(), suggestData);
    // _suggestKeyMap.insert(footKey, suggestKey);
}

void NNavigationBarPrivate::_raiseNavigationBar() {
    Q_Q(NNavigationBar);
    q->raise();
}

void NNavigationBarPrivate::_doComponentAnimation(NNavigationType::NavigationDisplayMode displayMode,
                                                  bool                                   isAnimation) {
    switch (displayMode) {
        case NNavigationType::Minimal: {
            _doNavigationBarWidthAnimation(displayMode, isAnimation);
            if (_currentDisplayMode == NNavigationType::Maximal) {
                _searchButton->setVisible(true);
                _navigationSuggestBox->setVisible(false);
                _handleNavigationExpandState(true);
            }
            _currentDisplayMode = displayMode;
            break;
        }
        case NNavigationType::Compact: {
            _doNavigationBarWidthAnimation(displayMode, isAnimation);
            _doNavigationViewWidthAnimation(isAnimation);
            if (_currentDisplayMode != NNavigationType::Minimal) {
                _handleMaximalToCompactLayout();
                _doNavigationButtonAnimation(true, isAnimation);
                _doSearchButtonAnimation(true, isAnimation);
                _navigationSuggestBox->setVisible(false);
                _handleNavigationExpandState(true);
            }
            _currentDisplayMode = displayMode;
            break;
        }
        case NNavigationType::Maximal: {
            _resetLayout();
            _handleCompactToMaximalLayout();
            _doNavigationBarWidthAnimation(displayMode, isAnimation);
            _doNavigationButtonAnimation(false, isAnimation);
            _doSearchButtonAnimation(false, isAnimation);
            _navigationSuggestBox->setVisible(true);
            _currentDisplayMode = displayMode;
            _handleNavigationExpandState(false);
            break;
        }
        default: {
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

void NNavigationBarPrivate::_handleMaximalToCompactLayout() {
    // 动画过程布局
    while (_navigationButtonLayout->count()) {
        _navigationButtonLayout->takeAt(0);
    }
    _navigationButtonLayout->addSpacing(40);
    _navigationSuggestLayout->addStretch();
}

void NNavigationBarPrivate::_handleCompactToMaximalLayout() {
    // 动画过程布局
    while (_navigationButtonLayout->count()) {
        _navigationButtonLayout->takeAt(0);
    }
    _navigationButtonLayout->addSpacing(38);
    _navigationSuggestLayout->insertSpacing(0, 46);
}

void NNavigationBarPrivate::_resetLayout() {
    // 恢复初始布局
    while (_navigationButtonLayout->count()) {
        _navigationButtonLayout->takeAt(0);
    }
    _navigationButtonLayout->addWidget(_navigationButton);
    _navigationButtonLayout->addWidget(_searchButton);

    while (_navigationSuggestLayout->count()) {
        _navigationSuggestLayout->takeAt(0);
    }
    _navigationSuggestLayout->addLayout(_navigationButtonLayout);
    _navigationSuggestLayout->addWidget(_navigationSuggestBox);
}

void NNavigationBarPrivate::_doNavigationBarWidthAnimation(NNavigationType::NavigationDisplayMode displayMode,
                                                           bool                                   isAnimation) {
    Q_Q(NNavigationBar);
    QPropertyAnimation* navigationBarWidthAnimation = new QPropertyAnimation(q, "maximumWidth");
    navigationBarWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
    navigationBarWidthAnimation->setStartValue(q->width());
    navigationBarWidthAnimation->setDuration(isAnimation ? 285 : 0);
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
            connect(navigationBarWidthAnimation, &QPropertyAnimation::finished, this, [=]() { _resetLayout(); });
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

void NNavigationBarPrivate::_doNavigationButtonAnimation(bool isCompact, bool isAnimation) {
    if (isCompact) {
        // 导航按钮
        QPropertyAnimation* navigationButtonAnimation = new QPropertyAnimation(_navigationButton, "pos");
        connect(navigationButtonAnimation, &QPropertyAnimation::finished, this, [=]() { _resetLayout(); });

        QPoint navigationButtonPos = _navigationButton->pos();
        navigationButtonAnimation->setStartValue(navigationButtonPos);
        navigationButtonAnimation->setEndValue(QPoint(0, 10));
        navigationButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        navigationButtonAnimation->setDuration(isAnimation ? 285 : 0);
        navigationButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        QPropertyAnimation* navigationButtonAnimation = new QPropertyAnimation(_navigationButton, "pos");
        QPoint              navigationButtonPos       = _navigationButton->pos();
        navigationButtonAnimation->setStartValue(navigationButtonPos);
        navigationButtonAnimation->setEndValue(QPoint(0, 10));
        navigationButtonAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationButtonAnimation->setDuration(isAnimation ? 130 : 0);
        navigationButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void NNavigationBarPrivate::_doSearchButtonAnimation(bool isCompact, bool isAnimation) {
    if (isCompact) {
        QPoint navigationButtonPos = _navigationButton->pos();
        // 搜索按钮
        QPropertyAnimation* searchButtonAnimation = new QPropertyAnimation(_searchButton, "pos");
        searchButtonAnimation->setStartValue(QPoint(200, navigationButtonPos.y()));
        searchButtonAnimation->setEndValue(QPoint(0, navigationButtonPos.y() + 38));
        searchButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        searchButtonAnimation->setDuration(isAnimation ? 285 : 0);
        searchButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _searchButton->setVisible(true);
    } else {
        _searchButton->setVisible(false);
    }
}