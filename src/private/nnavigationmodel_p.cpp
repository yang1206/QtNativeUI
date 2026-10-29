#include "nnavigationmodel_p.h"

NNavigationModel::NNavigationModel(QObject* parent) : QAbstractItemModel(parent) {
    _rootNode = new NNavigationNode("root");
    _rootNode->setIsRootNode(true);
    _rootNode->setIsExpanderNode(true);
    _pSelectedNode = nullptr;
    _pSelectedExpandedNode = nullptr;
}

NNavigationModel::~NNavigationModel() { delete _rootNode; }

QModelIndex NNavigationModel::parent(const QModelIndex& child) const {
    if (!child.isValid() || child.model() != this) return {};
    auto* parentNode = static_cast<NNavigationNode*>(child.internalPointer())->getParentNode();
    return !parentNode || parentNode == _rootNode ? QModelIndex() : createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex NNavigationModel::index(int row, int column, const QModelIndex& parent) const {
    if ((parent.isValid() && parent.model() != this) || !hasIndex(row, column, parent)) return {};
    auto* node = parent.isValid() ? static_cast<NNavigationNode*>(parent.internalPointer()) : _rootNode;
    return createIndex(row, column, node->getChildrenNodes().at(row));
}

int NNavigationModel::rowCount(const QModelIndex& parent) const {
    if (parent.column() > 0 || (parent.isValid() && parent.model() != this)) return 0;
    auto* node = parent.isValid() ? static_cast<NNavigationNode*>(parent.internalPointer()) : _rootNode;
    return node->getChildrenNodes().size();
}

int NNavigationModel::columnCount(const QModelIndex&) const { return 1; }

QVariant NNavigationModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.model() != this) return {};
    auto* node = static_cast<NNavigationNode*>(index.internalPointer());
    if (role == Qt::DisplayRole || role == Qt::AccessibleTextRole || role == Qt::ToolTipRole) return node->getNodeTitle();
    return {};
}

NNavigationType::NodeOperateReturnType NNavigationModel::insertNode(
    const QString& title, QString& key, const QString& parentKey, bool expander, int keyPoints,
    NRegularIconType::Icon regular, NFilledIconType::Icon filled) {
    auto* parent = parentKey.isEmpty() ? _rootNode : _nodesMap.value(parentKey);
    if (!parent) return NNavigationType::TargetNodeInvalid;
    if (!parent->getIsExpanderNode()) return NNavigationType::TargetNodeTypeError;
    if (parent->getDepth() >= 10) return NNavigationType::TargetNodeDepthLimit;
    auto* node = new NNavigationNode(title, parent);
    node->setDepth(parent->getDepth() + 1);
    node->setIsExpanderNode(expander);
    node->setIcon(regular);
    node->setFilledIcon(filled);
    node->setKeyPoints(qMax(0, keyPoints));
    node->setIsVisible(parent == _rootNode || (parent->getIsVisible() && parent->getIsExpanded()));
    const QModelIndex parentIndex = parent == _rootNode ? QModelIndex() : QModelIndex(parent->getModelIndex());
    const int row = parent->getChildrenNodes().size();
    beginInsertRows(parentIndex, row, row);
    parent->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    node->setModelIndex(index(row, 0, parentIndex));
    key = node->getNodeKey();
    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationModel::addExpanderNode(QString title, QString& key, NRegularIconType::Icon icon) {
    return insertNode(title, key, QString(), true, 0, icon, NFilledIconType::None);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addExpanderNode(QString title, QString& key, QString parentKey, NRegularIconType::Icon icon) {
    if (parentKey.isEmpty()) return NNavigationType::TargetNodeInvalid;
    return insertNode(title, key, parentKey, true, 0, icon, NFilledIconType::None);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, NRegularIconType::Icon icon) {
    return insertNode(title, key, QString(), false, 0, icon, NFilledIconType::None);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, int keyPoints, NRegularIconType::Icon icon) {
    return insertNode(title, key, QString(), false, keyPoints, icon, NFilledIconType::None);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, QString parentKey, NRegularIconType::Icon icon) {
    if (parentKey.isEmpty()) return NNavigationType::TargetNodeInvalid;
    return insertNode(title, key, parentKey, false, 0, icon, NFilledIconType::None);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, QString parentKey, int keyPoints, NRegularIconType::Icon icon) {
    if (parentKey.isEmpty()) return NNavigationType::TargetNodeInvalid;
    return insertNode(title, key, parentKey, false, keyPoints, icon, NFilledIconType::None);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addExpanderNode(QString title, QString& key, NFilledIconType::Icon icon) {
    return insertNode(title, key, QString(), true, 0, NRegularIconType::None, icon);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addExpanderNode(QString title, QString& key, QString parentKey, NFilledIconType::Icon icon) {
    if (parentKey.isEmpty()) return NNavigationType::TargetNodeInvalid;
    return insertNode(title, key, parentKey, true, 0, NRegularIconType::None, icon);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, NFilledIconType::Icon icon) {
    return insertNode(title, key, QString(), false, 0, NRegularIconType::None, icon);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, int keyPoints, NFilledIconType::Icon icon) {
    return insertNode(title, key, QString(), false, keyPoints, NRegularIconType::None, icon);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, QString parentKey, NFilledIconType::Icon icon) {
    if (parentKey.isEmpty()) return NNavigationType::TargetNodeInvalid;
    return insertNode(title, key, parentKey, false, 0, NRegularIconType::None, icon);
}

NNavigationType::NodeOperateReturnType NNavigationModel::addPageNode(QString title, QString& key, QString parentKey, int keyPoints, NFilledIconType::Icon icon) {
    if (parentKey.isEmpty()) return NNavigationType::TargetNodeInvalid;
    return insertNode(title, key, parentKey, false, keyPoints, NRegularIconType::None, icon);
}

QStringList NNavigationModel::removeNavigationNode(QString key) {
    auto* node = _nodesMap.value(key);
    if (!node) return {};
    QList<NNavigationNode*> subtree{node};
    for (int i = 0; i < subtree.size(); ++i) subtree.append(subtree.at(i)->getChildrenNodes());
    QStringList removed;
    for (auto* child : subtree) removed.append(child->getNodeKey());
    auto* parent = node->getParentNode();
    const int row = node->getRow();
    beginRemoveRows(parent == _rootNode ? QModelIndex() : QModelIndex(parent->getModelIndex()), row, row);
    if (subtree.contains(_pSelectedNode)) _pSelectedNode = nullptr;
    if (subtree.contains(_pSelectedExpandedNode)) _pSelectedExpandedNode = nullptr;
    for (auto* child : subtree) _nodesMap.remove(child->getNodeKey());
    parent->removeChildNode(node);
    delete node;
    endRemoveRows();
    return removed;
}

NNavigationNode* NNavigationModel::getNavigationNode(QString key) const { return _nodesMap.value(key); }

QList<NNavigationNode*> NNavigationModel::getRootExpanderNodes() const {
    QList<NNavigationNode*> nodes;
    for (auto* node : _rootNode->getChildrenNodes()) if (node->getIsExpanderNode()) nodes.append(node);
    return nodes;
}

QList<NNavigationNode*> NNavigationModel::getRootExpandedNodes() const {
    QList<NNavigationNode*> nodes;
    for (auto* node : getRootExpanderNodes()) if (node->getIsExpanded()) nodes.append(node);
    return nodes;
}
