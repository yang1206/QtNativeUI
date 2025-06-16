#include "QtNativeUI/NNavigationBar.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QVBoxLayout>
#include "../navigation/NFooterDelegate.h"
#include "../navigation/NFooterModel.h"
#include "../navigation/NNavigationBarPrivate.h"
#include "../navigation/NNavigationModel.h"
#include "../navigation/NNavigationNode.h"
#include "../navigation/NNavigationView.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NToolButton.h"
#include "nbaselistview.h"

#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NMenu.h"

Q_PROPERTY_CREATE_Q_CPP(NNavigationBar, bool, IsTransparent)

NNavigationBar::NNavigationBar(QWidget* parent) : QWidget{parent}, d_ptr(new NNavigationBarPrivate()) {
    Q_D(NNavigationBar);
    d->q_ptr = this;

    setFixedWidth(300);
    d->_pIsTransparent = true;

    // 导航模型
    d->_navigationModel = new NNavigationModel(this);
    d->_navigationView  = new NNavigationView(this);
    d->_navigationView->setModel(d->_navigationModel);
    connect(d->_navigationView, &NNavigationView::navigationClicked, this, [=](const QModelIndex& index) {
        d->onTreeViewClicked(index);
    });
    connect(d->_navigationView,
            &NNavigationView::navigationOpenNewWindow,
            d,
            &NNavigationBarPrivate::onNavigationOpenNewWindow);

    // 导航按钮组
    d->_navigationButton = new NToolButton(this);
    d->_navigationButton->setFixedSize(40, 38);
    d->_navigationButton->setIcon(nIcon->fromRegular(NRegularIconType::Navigation16Regular));
    connect(d->_navigationButton, &NToolButton::clicked, d, &NNavigationBarPrivate::onNavigationButtonClicked);

    d->_searchButton = new NToolButton(this);
    d->_searchButton->setFixedSize(40, 38);
    d->_searchButton->setIcon(nIcon->fromRegular(NRegularIconType::Search16Regular));
    d->_searchButton->setVisible(false);

    d->_navigationSuggestBox = new NLineEdit(this);
    d->_navigationSuggestBox->setMinimumWidth(0);
    d->_navigationSuggestBox->setPlaceholderText("搜索...");

    // 布局设置
    d->_navigationButtonLayout = new QVBoxLayout();
    d->_navigationButtonLayout->setContentsMargins(0, 0, 0, 0);
    d->_navigationButtonLayout->setSpacing(0);
    d->_navigationButtonLayout->addWidget(d->_navigationButton);
    d->_navigationButtonLayout->addWidget(d->_searchButton);

    d->_navigationSuggestLayout = new QHBoxLayout();
    d->_navigationSuggestLayout->setContentsMargins(0, 0, 10, 0);
    d->_navigationSuggestLayout->setSpacing(6);
    d->_navigationSuggestLayout->addLayout(d->_navigationButtonLayout);
    d->_navigationSuggestLayout->addWidget(d->_navigationSuggestBox);

    // 页脚视图
    d->_footerModel = new NFooterModel(this);
    d->_footerView  = new NBaseListView(this);
    d->_footerView->setFrameShape(QFrame::NoFrame);
    d->_footerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_footerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_footerView->setSelectionMode(QAbstractItemView::SingleSelection);
    d->_footerView->setFixedHeight(0);
    d->_footerView->setModel(d->_footerModel);

    d->_footerDelegate = new NFooterDelegate(this);
    d->_footerDelegate->setListView(d->_footerView);

    connect(
        d->_footerView, &QListView::clicked, this, [=](const QModelIndex& index) { d->onFooterViewClicked(index); });

    // 布局设置
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 10, 5, 0);
    mainLayout->addLayout(d->_navigationSuggestLayout);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);

    // 主题设置
    d->_themeMode = nTheme->themeMode();
    connect(
        nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

NNavigationBar::~NNavigationBar() {}

NNavigationType::NodeOperateReturnType
NNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
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
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page) {
        return NNavigationType::PageInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addPageNode(pageTitle, pageKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page) {
        return NNavigationType::PageInvalid;
    }

    if (targetExpanderKey.isEmpty()) {
        return NNavigationType::TargetNodeInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, icon);
    if (returnType == NNavigationType::Success) {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        NNavigationNode* node         = d->_navigationModel->getNavigationNode(pageKey);
        NNavigationNode* originalNode = node->getOriginalNode();

        if (d->_compactMenuMap.contains(originalNode)) {
            QMenu*   menu   = d->_compactMenuMap.value(originalNode);
            QAction* action = menu->addAction(node->getNodeTitle());
            // 设置图标，如果有的话
            if (node->getIcon() != NRegularIconType::Home12Regular) {
                // 这里需要将Icon转换为QIcon，具体实现可能需要根据实际情况调整
                // action->setIcon(iconFromEnum(node->getIcon()));
            }
            connect(action, &QAction::triggered, this, [=]() { d->onTreeViewClicked(node->getModelIndex()); });
        } else {
            QMenu*   menu   = new QMenu(this);
            QAction* action = menu->addAction(node->getNodeTitle());
            // 设置图标，如果有的话
            if (node->getIcon() != NRegularIconType::Home12Regular) {
                // 这里需要将Icon转换为QIcon，具体实现可能需要根据实际情况调整
                // action->setIcon(iconFromEnum(node->getIcon()));
            }
            connect(action, &QAction::triggered, this, [=]() { d->onTreeViewClicked(node->getModelIndex()); });
            d->_compactMenuMap.insert(originalNode, menu);
        }

        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }

    return returnType;
}

NNavigationType::NodeOperateReturnType
NNavigationBar::addPageNode(QString pageTitle, QWidget* page, int keyPoints, NRegularIconType::Icon icon) {
    Q_D(NNavigationBar);
    if (!page) {
        return NNavigationType::PageInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
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
    if (!page) {
        return NNavigationType::PageInvalid;
    }

    if (targetExpanderKey.isEmpty()) {
        return NNavigationType::TargetNodeInvalid;
    }

    QString                                pageKey;
    NNavigationType::NodeOperateReturnType returnType =
        d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        NNavigationNode* node         = d->_navigationModel->getNavigationNode(pageKey);
        NNavigationNode* originalNode = node->getOriginalNode();

        if (d->_compactMenuMap.contains(originalNode)) {
            QMenu*   menu   = d->_compactMenuMap.value(originalNode);
            QAction* action = menu->addAction(node->getNodeTitle());
            // 设置图标，如果有的话
            if (node->getIcon() != NRegularIconType::Home12Regular) {
                // 这里需要将Icon转换为QIcon
                // action->setIcon(iconFromEnum(node->getIcon()));
            }
            connect(action, &QAction::triggered, this, [=]() { d->onTreeViewClicked(node->getModelIndex()); });
        } else {
            QMenu*   menu   = new QMenu(this);
            QAction* action = menu->addAction(node->getNodeTitle());
            // 设置图标，如果有的话
            if (node->getIcon() != NRegularIconType::Home12Regular) {
                // 这里需要将Icon转换为QIcon
                // action->setIcon(iconFromEnum(node->getIcon()));
            }
            connect(action, &QAction::triggered, this, [=]() { d->onTreeViewClicked(node->getModelIndex()); });
            d->_compactMenuMap.insert(originalNode, menu);
        }

        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
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
    Q_D(NNavigationBar);
    NNavigationType::NodeOperateReturnType returnType =
        d->_footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, icon);
    if (returnType == NNavigationType::Success) {
        d->_addFooterPage(page, footerKey);
    }
    return returnType;
}

void NNavigationBar::removeNavigationNode(QString nodeKey) {
    Q_D(NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node) {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    if (!node) {
        return;
    }
    if (node->getIsFooterNode()) {
        Q_EMIT navigationNodeRemoved(NNavigationType::FooterNode, nodeKey);
        d->_footerModel->removeNavigationNode(nodeKey);
        // 更新 Footer 视图高度
        d->_footerView->setFixedHeight(40 * d->_footerModel->getFooterNodeCount());
    } else {
        QStringList removeKeyList = d->_navigationModel->removeNavigationNode(nodeKey);
        d->_initNodeModelIndex(QModelIndex());
        for (const auto& removeKey : removeKeyList) {
            Q_EMIT navigationNodeRemoved(NNavigationType::PageNode, removeKey);
        }
    }

    // 处理搜索建议
    // if (_navigationSuggestBox && d->_suggestKeyMap.contains(nodeKey)) {
    //     _navigationSuggestBox->removeSuggestion(d->_suggestKeyMap.value(nodeKey));
    //     d->_suggestKeyMap.remove(nodeKey);
    // }
}

void NNavigationBar::setNodeKeyPoints(QString nodeKey, int keyPoints) {
    Q_D(NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node) {
        node = d->_footerModel->getNavigationNode(nodeKey);
    } else if (node->getIsExpanderNode() || keyPoints < 0) {
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

void NNavigationBar::navigation(QString pageKey, bool isLogClicked) {
    Q_D(NNavigationBar);
    NNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
    if (!node) {
        node = d->_footerModel->getNavigationNode(pageKey);
    }

    if (node) {
        if (node->getIsFooterNode()) {
            d->onFooterViewClicked(node->getModelIndex(), isLogClicked);
        } else {
            if (!node->getIsExpanderNode()) {
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked);
            }
        }
    }
}

void NNavigationBar::setDisplayMode(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation) {
    Q_D(NNavigationBar);
    if (d->_currentDisplayMode == displayMode || displayMode == NNavigationType::Auto) {
        return;
    }

    d->_doComponentAnimation(displayMode, isAnimation);
    d->_raiseNavigationBar();
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