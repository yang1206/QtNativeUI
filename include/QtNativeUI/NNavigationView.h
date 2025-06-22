#ifndef NNAVIGATIONVIEW_H
#define NNAVIGATIONVIEW_H

#include <QWidget>

#include "NIconEnums.h"
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NNavigationViewPrivate;
class QWidget;

class QTNATIVEUI_EXPORT NNavigationView : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NNavigationView)
    Q_PROPERTY_CREATE_Q_H(NNavigationType::NavigationDisplayMode, DisplayMode)
    Q_PROPERTY_CREATE_Q_H(NNavigationType::PageTransitionType, PageTransitionType)

  public:
    explicit NNavigationView(QWidget* parent = nullptr);
    ~NNavigationView();

    // 导航栏相关方法
    void     setHeaderWidget(QWidget* widget);
    QWidget* headerWidget() const;

    // 导航节点操作
    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType addExpanderNode(QString                expanderTitle,
                                                           QString&               expanderKey,
                                                           QString                targetExpanderKey,
                                                           NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, int keyPoints, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, int keyPoints, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType addPageNode(QString                pageTitle,
                                                       QWidget*               page,
                                                       QString                targetExpanderKey,
                                                       int                    keyPoints,
                                                       NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, NFilledIconType::Icon icon);

    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NFilledIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, NRegularIconType::Icon icon);
    NNavigationType::NodeOperateReturnType
    addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, NFilledIconType::Icon icon);

    bool getNavigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
    void collpaseNavigationNode(QString expanderKey);
    void removeNavigationNode(QString nodeKey);

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int  getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey, bool isLogClicked = true);
    void setNavigationBarVisible(bool visible);
    bool isNavigationBarVisible() const;

    // 动画设置
    void setPageTransitionDuration(int duration);
    int  pageTransitionDuration() const;

    // 当前页面
    QWidget* currentWidget() const;
    int      currentIndex() const;
    int      count() const;

  signals:
    void navigationNodeClicked(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void navigationNodeAdded(NNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    void navigationNodeRemoved(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void displayModeChanged(NNavigationType::NavigationDisplayMode displayMode);
    void currentChanged(int index);

  protected:
    void resizeEvent(QResizeEvent* event) override;
};

#endif // NNAVIGATIONVIEW_H