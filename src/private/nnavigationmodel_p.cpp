#include "nnavigationmodel_p.h"

NNavigationModel::NNavigationModel(QObject* parent) : QAbstractItemModel{parent} {
    _rootNode = new NNavigationNode("root");
    _rootNode->setIsRootNode(true);
    _rootNode->setIsExpanderNode(true);
    _pSelectedNode         = nullptr;
    _pSelectedExpandedNode = nullptr;
}

NNavigationModel::~NNavigationModel() { delete _rootNode; }

QModelIndex NNavigationModel::parent(const QModelIndex& child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }

    NNavigationNode* childNode  = static_cast<NNavigationNode*>(child.internalPointer());
    NNavigationNode* parentNode = childNode->getParentNode();

    if (parentNode == _rootNode) {
        return QModelIndex();
    } else if (parentNode == nullptr) {
        return QModelIndex();
    }

    return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex NNavigationModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    NNavigationNode* parentNode;
    if (!parent.isValid()) {
        parentNode = _rootNode;
    } else {
        parentNode = static_cast<NNavigationNode*>(parent.internalPointer());
    }

    NNavigationNode* childNode = nullptr;
    if (parentNode->getChildrenNodes().count() > row) {
        childNode = parentNode->getChildrenNodes().at(row);
    }

    if (childNode) {
        return createIndex(row, column, childNode);
    }

    return QModelIndex();
}

int NNavigationModel::rowCount(const QModelIndex& parent) const {
    NNavigationNode* parentNode;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentNode = _rootNode;
    } else {
        parentNode = static_cast<NNavigationNode*>(parent.internalPointer());
    }

    return parentNode->getChildrenNodes().count();
}

int NNavigationModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return 1;
}

QVariant NNavigationModel::data(const QModelIndex& index, int role) const {
    Q_UNUSED(index)
    Q_UNUSED(role)
    return QVariant();
}

NNavigationType::NodeOperateReturnType
NNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, NRegularIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(expanderTitle, _rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsExpanderNode(true);
    node->setIcon(icon);

    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    expanderKey = node->getNodeKey();
    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationModel::addExpanderNode(QString                expanderTitle,
                                                                         QString&               expanderKey,
                                                                         QString                targetExpanderKey,
                                                                         NRegularIconType::Icon icon) {
    if (!_nodesMap.contains(targetExpanderKey)) {
        return NNavigationType::TargetNodeInvalid;
    }

    NNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode()) {
        return NNavigationType::TargetNodeTypeError;
    }

    if (parentNode->getDepth() > 10) {
        return NNavigationType::TargetNodeDepthLimit;
    }

    NNavigationNode* node = new NNavigationNode(expanderTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIsExpanderNode(true);
    node->setIcon(icon);

    if (parentNode->getIsVisible() && parentNode->getIsExpanded()) {
        node->setIsVisible(true);
    }

    beginInsertRows(
        parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    expanderKey = node->getNodeKey();
    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType
NNavigationModel::addPageNode(QString pageTitle, QString& pageKey, NRegularIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(pageTitle, _rootNode);
    node->setIcon(icon);
    node->setDepth(1);
    node->setIsVisible(true);

    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString                pageTitle,
                                                                     QString&               pageKey,
                                                                     QString                targetExpanderKey,
                                                                     NRegularIconType::Icon icon) {
    if (!_nodesMap.contains(targetExpanderKey)) {
        return NNavigationType::TargetNodeInvalid;
    }

    NNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode()) {
        return NNavigationType::TargetNodeTypeError;
    }

    if (parentNode->getDepth() > 10) {
        return NNavigationType::TargetNodeDepthLimit;
    }

    NNavigationNode* node = new NNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIcon(icon);

    if (parentNode->getIsVisible() && parentNode->getIsExpanded()) {
        node->setIsVisible(true);
    }

    beginInsertRows(
        parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType
NNavigationModel::addPageNode(QString pageTitle, QString& pageKey, int keyPoints, NRegularIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(pageTitle, _rootNode);
    node->setIcon(icon);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setKeyPoints(keyPoints);

    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString                pageTitle,
                                                                     QString&               pageKey,
                                                                     QString                targetExpanderKey,
                                                                     int                    keyPoints,
                                                                     NRegularIconType::Icon icon) {
    if (!_nodesMap.contains(targetExpanderKey)) {
        return NNavigationType::TargetNodeInvalid;
    }

    NNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode()) {
        return NNavigationType::TargetNodeTypeError;
    }

    if (parentNode->getDepth() > 10) {
        return NNavigationType::TargetNodeDepthLimit;
    }

    NNavigationNode* node = new NNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIcon(icon);
    node->setKeyPoints(keyPoints);

    if (parentNode->getIsVisible() && parentNode->getIsExpanded()) {
        node->setIsVisible(true);
    }

    beginInsertRows(
        parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType
NNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, NFilledIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(expanderTitle, _rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsExpanderNode(true);
    node->setFilledIcon(icon);

    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    expanderKey = node->getNodeKey();
    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationModel::addExpanderNode(QString               expanderTitle,
                                                                         QString&              expanderKey,
                                                                         QString               targetExpanderKey,
                                                                         NFilledIconType::Icon icon) {
    if (!_nodesMap.contains(targetExpanderKey)) {
        return NNavigationType::TargetNodeInvalid;
    }

    NNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode()) {
        return NNavigationType::TargetNodeTypeError;
    }

    if (parentNode->getDepth() > 10) {
        return NNavigationType::TargetNodeDepthLimit;
    }

    NNavigationNode* node = new NNavigationNode(expanderTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIsExpanderNode(true);
    node->setFilledIcon(icon);

    if (parentNode->getIsVisible() && parentNode->getIsExpanded()) {
        node->setIsVisible(true);
    }

    beginInsertRows(
        parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    expanderKey = node->getNodeKey();
    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType
NNavigationModel::addPageNode(QString pageTitle, QString& pageKey, NFilledIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(pageTitle, _rootNode);
    node->setFilledIcon(icon);
    node->setDepth(1);
    node->setIsVisible(true);

    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString               pageTitle,
                                                                     QString&              pageKey,
                                                                     QString               targetExpanderKey,
                                                                     NFilledIconType::Icon icon) {
    if (!_nodesMap.contains(targetExpanderKey)) {
        return NNavigationType::TargetNodeInvalid;
    }

    NNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode()) {
        return NNavigationType::TargetNodeTypeError;
    }

    if (parentNode->getDepth() > 10) {
        return NNavigationType::TargetNodeDepthLimit;
    }

    NNavigationNode* node = new NNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setFilledIcon(icon);

    if (parentNode->getIsVisible() && parentNode->getIsExpanded()) {
        node->setIsVisible(true);
    }

    beginInsertRows(
        parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType
NNavigationModel::addPageNode(QString pageTitle, QString& pageKey, int keyPoints, NFilledIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(pageTitle, _rootNode);
    node->setFilledIcon(icon);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setKeyPoints(keyPoints);

    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString               pageTitle,
                                                                     QString&              pageKey,
                                                                     QString               targetExpanderKey,
                                                                     int                   keyPoints,
                                                                     NFilledIconType::Icon icon) {
    if (!_nodesMap.contains(targetExpanderKey)) {
        return NNavigationType::TargetNodeInvalid;
    }

    NNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode()) {
        return NNavigationType::TargetNodeTypeError;
    }

    if (parentNode->getDepth() > 10) {
        return NNavigationType::TargetNodeDepthLimit;
    }

    NNavigationNode* node = new NNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setFilledIcon(icon);
    node->setKeyPoints(keyPoints);

    if (parentNode->getIsVisible() && parentNode->getIsExpanded()) {
        node->setIsVisible(true);
    }

    beginInsertRows(
        parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();

    pageKey = node->getNodeKey();
    if (!_pSelectedNode) {
        _pSelectedNode = node;
    }

    return NNavigationType::Success;
}

QStringList NNavigationModel::removeNavigationNode(QString nodeKey) {
    QList<QString> removeKeyList;
    if (!_nodesMap.contains(nodeKey)) {
        return removeKeyList;
    }

    NNavigationNode* node       = _nodesMap.value(nodeKey);
    NNavigationNode* parentNode = node->getParentNode();

    if (node->getIsExpanderNode()) {
        QList<NNavigationNode*> childNodeList = node->getChildrenNodes();
        for (int i = 0; i < childNodeList.count(); i++) {
            NNavigationNode* childNode          = childNodeList[i];
            QList<QString>   childRemoveKeyList = removeNavigationNode(childNode->getNodeKey());
            removeKeyList.append(childRemoveKeyList);
        }
    } else {
        removeKeyList.append(node->getNodeKey());
    }

    beginRemoveRows(parentNode->getModelIndex(),
                    parentNode->getChildrenNodes().count() - 1,
                    parentNode->getChildrenNodes().count() - 1);
    parentNode->removeChildNode(node);
    _nodesMap.remove(node->getNodeKey());
    endRemoveRows();

    return removeKeyList;
}

NNavigationNode* NNavigationModel::getNavigationNode(QString nodeKey) const {
    if (_nodesMap.contains(nodeKey)) {
        return _nodesMap.value(nodeKey);
    }
    return nullptr;
}

QList<NNavigationNode*> NNavigationModel::getRootExpanderNodes() const {
    QList<NNavigationNode*> expanderNodeList;
    for (auto node : _rootNode->getChildrenNodes()) {
        if (node->getIsExpanderNode()) {
            expanderNodeList.append(node);
        }
    }
    return expanderNodeList;
}

QList<NNavigationNode*> NNavigationModel::getRootExpandedNodes() const {
    QList<NNavigationNode*> expandedNodeList;
    for (auto node : _rootNode->getChildrenNodes()) {
        if (node->getIsExpanderNode() && node->getIsExpanded()) {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}