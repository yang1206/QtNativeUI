#ifndef NNAVIGATIONBAR_H
#define NNAVIGATIONBAR_H

#include <QWidget>

#include "NIconEnums.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NNavigationBarPrivate;
class NNavigationNode;

class QTNATIVEUI_EXPORT NNavigationBar : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NNavigationBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)

  public:
    explicit NNavigationBar(QWidget* parent = nullptr);
    ~NNavigationBar();

    // 导航节点操作函数
    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType addExpanderNode(QString                expanderTitle,
                                                           QString&               expanderKey,
                                                           QString                targetExpanderKey,
                                                           NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, int keyPoints, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType addPageNode(QString                pageTitle,
                                                       QWidget*               page,
                                                       QString                targetExpanderKey,
                                                       int                    keyPoints,
                                                       NRegularIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, NRegularIconType::Icon icon);

    bool getNavigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
    void collpaseNavigationNode(QString expanderKey);
    void removeNavigationNode(QString nodeKey);

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int  getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey, bool isLogClicked = true);
    void setDisplayMode(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);

  signals:
    void navigationNodeClicked(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void navigationNodeAdded(NNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    void navigationNodeRemoved(NNavigationType::NavigationNodeType nodeType, QString nodeKey);

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // NNAVIGATIONBAR_H