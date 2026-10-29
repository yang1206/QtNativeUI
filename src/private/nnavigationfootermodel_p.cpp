#include "nnavigationfootermodel_p.h"

NNavigationFooterModel::NNavigationFooterModel(QObject* parent) : QAbstractListModel{parent} {
    _pSelectedNode = nullptr;
}

NNavigationFooterModel::~NNavigationFooterModel() { qDeleteAll(_footerNodeList); }

int NNavigationFooterModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : _footerNodeList.count();
}

QVariant NNavigationFooterModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.model() != this || index.row() < 0 || index.row() >= _footerNodeList.count()) return {};
    if (role == Qt::DisplayRole || role == Qt::AccessibleTextRole || role == Qt::ToolTipRole) return _footerNodeList.at(index.row())->getNodeTitle();
    if (role == Qt::UserRole) {
        if (index.row() < _footerNodeList.count()) {
            return QVariant::fromValue(_footerNodeList[index.row()]);
        }
    }
    return QVariant();
}

NNavigationType::NodeOperateReturnType NNavigationFooterModel::addFooterNode(QString                footerTitle,
                                                                             QString&               footerKey,
                                                                             bool                   isHasFooterPage,
                                                                             int                    keyPoints,
                                                                             NRegularIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(footerTitle);
    node->setKeyPoints(qMax(0, keyPoints));
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setIcon(icon);
    footerKey = node->getNodeKey();

    const int row = _footerNodeList.size();
    beginInsertRows({}, row, row);
    _footerNodeList.append(node);
    endInsertRows();

    node->setModelIndex(this->index(_footerNodeList.count() - 1));
    return NNavigationType::Success;
}

NNavigationType::NodeOperateReturnType NNavigationFooterModel::addFooterNode(QString               footerTitle,
                                                                             QString&              footerKey,
                                                                             bool                  isHasFooterPage,
                                                                             int                   keyPoints,
                                                                             NFilledIconType::Icon icon) {
    NNavigationNode* node = new NNavigationNode(footerTitle);
    node->setKeyPoints(qMax(0, keyPoints));
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setFilledIcon(icon);
    footerKey = node->getNodeKey();

    const int row = _footerNodeList.size();
    beginInsertRows({}, row, row);
    _footerNodeList.append(node);
    endInsertRows();

    node->setModelIndex(this->index(_footerNodeList.count() - 1));
    return NNavigationType::Success;
}

int NNavigationFooterModel::getFooterNodeCount() const { return _footerNodeList.count(); }

NNavigationNode* NNavigationFooterModel::getNavigationNode(QString footerKey) {
    for (auto node : _footerNodeList) {
        if (node->getNodeKey() == footerKey) {
            return node;
        }
    }
    return nullptr;
}

void NNavigationFooterModel::removeNavigationNode(QString footerKey) {
    for (int row = 0; row < _footerNodeList.size(); ++row) {
        auto* node = _footerNodeList.at(row);
        if (node->getNodeKey() != footerKey) continue;
        beginRemoveRows({}, row, row);
        if (_pSelectedNode == node) _pSelectedNode = nullptr;
        _footerNodeList.removeAt(row);
        delete node;
        endRemoveRows();
        return;
    }
}
