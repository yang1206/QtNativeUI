#ifndef NNAVIGATIONBAR_H
#define NNAVIGATIONBAR_H

#include <QWidget>

#include "NIconEnums.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NPageComponent;
class NNavigationBarPrivate;
class NNavigationNode;

class QTNATIVEUI_EXPORT NNavigationBar : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NNavigationBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
  public:
    explicit NNavigationBar(QWidget* parent = nullptr);
    ~NNavigationBar();

    void     setHeaderWidget(QWidget* widget);
    QWidget* headerWidget() const;

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

    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, int keyPoints, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addPageComponent(NPageComponent*        page,
                                                            NRegularIconType::Icon icon = NRegularIconType::None);
    NNavigationType::NodeOperateReturnType addPageComponent(NPageComponent* page, NFilledIconType::Icon icon);
    // 添加页面组件到展开节点
    NNavigationType::NodeOperateReturnType addPageComponent(NPageComponent*        page,
                                                            const QString&         expanderKey,
                                                            NRegularIconType::Icon icon = NRegularIconType::None);
    NNavigationType::NodeOperateReturnType
    addPageComponent(NPageComponent* page, const QString& expanderKey, NFilledIconType::Icon icon);

    bool getNavigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
    void collpaseNavigationNode(QString expanderKey);
    void removeNavigationNode(QString nodeKey);

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int  getNodeKeyPoints(QString nodeKey) const;

    void setSearchVisible(bool visible);
    bool isSearchVisible() const;

    void navigation(QString pageKey, bool isLogClicked = true);
    void setDisplayMode(NNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);

  signals:
    void navigationNodeClicked(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void navigationNodeAdded(NNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    void navigationNodeRemoved(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void displayModeChange(NNavigationType::NavigationDisplayMode displayMode);

  protected:
    void paintEvent(QPaintEvent* event) override;

  private slots:
    void handleRouteChanged(const QString& pageKey, const QVariantMap& params);
    void handleRouteBack(const QString& fromPageKey, const QString& toPageKey, const QVariantMap& params);
};

#endif // NNAVIGATIONBAR_H