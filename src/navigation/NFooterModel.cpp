#include "NFooterModel.h"

NFooterModel::NFooterModel(QObject* parent) : QAbstractListModel{parent} { _pSelectedNode = nullptr; }

NFooterModel::~NFooterModel() { qDeleteAll(_footerNodeList); }

int NFooterModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return _footerNodeList.count();
}

QVariant NFooterModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::UserRole) {
        if (index.row() < _footerNodeList.count()) {
            return QVariant::fromValue(_footerNodeList[index.row()]);
        }
    }
    return QVariant();
}

NNavigationType::NodeOperateReturnType NFooterModel::addFooterNode(QString                footerTitle,
                                                                   QString&               footerKey,
                                                                   bool                   isHasFooterPage,
                                                                   int                    keyPoints,
                                                                   NRegularIconType::Icon icon) {
    if (_footerNodeList.count() >= 3) {
        return NNavigationType::TargetNodeInvalid;
    }

    NNavigationNode* node = new NNavigationNode(footerTitle);
    node->setKeyPoints(keyPoints);
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setIcon(icon);
    footerKey = node->getNodeKey();

    beginResetModel();
    _footerNodeList.append(node);
    endResetModel();

    node->setModelIndex(this->index(_footerNodeList.count() - 1));
    return NNavigationType::Success;
}

int NFooterModel::getFooterNodeCount() const { return _footerNodeList.count(); }

NNavigationNode* NFooterModel::getNavigationNode(QString footerKey) {
    for (auto node : _footerNodeList) {
        if (node->getNodeKey() == footerKey) {
            return node;
        }
    }
    return nullptr;
}

void NFooterModel::removeNavigationNode(QString footerKey) {
    for (auto node : _footerNodeList) {
        if (node->getNodeKey() == footerKey) {
            _footerNodeList.removeOne(node);
            delete node;
            break;
        }
    }
}