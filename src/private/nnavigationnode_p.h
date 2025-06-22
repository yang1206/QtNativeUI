#ifndef NNAVIGATIONNODE_H
#define NNAVIGATIONNODE_H

#include <QModelIndex>
#include <QObject>
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/stdafx.h"

class NNavigationNode : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE(QList<NNavigationNode*>, ChildrenNodes)
    Q_PRIVATE_CREATE(NNavigationNode*, ParentNode)
    Q_PROPERTY_CREATE(NRegularIconType::Icon, Icon)
    Q_PROPERTY_CREATE(NFilledIconType::Icon, FilledIcon)
    Q_PROPERTY_CREATE(QModelIndex, ModelIndex)
    Q_PROPERTY_CREATE(int, KeyPoints)
    Q_PROPERTY_CREATE(int, Depth)
    Q_PROPERTY_CREATE(bool, IsRootNode)
    Q_PROPERTY_CREATE(bool, IsFooterNode)
    Q_PROPERTY_CREATE(bool, IsHasFooterPage)
    Q_PROPERTY_CREATE(bool, IsExpanderNode)
    Q_PROPERTY_CREATE(bool, IsVisible)

  public:
    explicit NNavigationNode(QString nodeTitle, NNavigationNode* parent = nullptr);
    ~NNavigationNode();

    QString getNodeKey() const;
    QString getNodeTitle() const;

    void setIsExpanded(bool isExpanded);
    bool getIsExpanded() const;

    void setChildVisible(bool isVisible);
    bool getIsHasChild() const;
    bool getIsHasPageChild() const;

    void appendChildNode(NNavigationNode* childNode);
    void removeChildNode(NNavigationNode* childNode);

    bool getIsChildHasKeyPoints() const;

    NNavigationNode* getOriginalNode();
    bool             getIsChildNode(NNavigationNode* node);

    int getRow() const;

  private:
    QString _nodeKey   = "";
    QString _nodeTitle = "";
    bool    _isExpanded{false};
};

#endif // NNAVIGATIONNODE_H