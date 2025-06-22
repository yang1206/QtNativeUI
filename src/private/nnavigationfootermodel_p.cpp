#include "nnavigationfootermodel_p.h"

NNavigationFooterModel::NNavigationFooterModel(QObject* parent) : QAbstractListModel{parent} {
    _pSelectedNode = nullptr;
}

NNavigationFooterModel::~NNavigationFooterModel() { qDeleteAll(_footerNodeList); }

int NNavigationFooterModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return _footerNodeList.count();
}

QVariant NNavigationFooterModel::data(const QModelIndex& index, int role) const {
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
    if (_footerNodeList.count() >= 3) {
        return NNavigationType::FooterUpperLimit;
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

NNavigationType::NodeOperateReturnType NNavigationFooterModel::addFooterNode(QString               footerTitle,
                                                                             QString&              footerKey,
                                                                             bool                  isHasFooterPage,
                                                                             int                   keyPoints,
                                                                             NFilledIconType::Icon icon) {
    if (_footerNodeList.count() >= 3) {
        return NNavigationType::FooterUpperLimit;
    }

    NNavigationNode* node = new NNavigationNode(footerTitle);
    node->setKeyPoints(keyPoints);
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setFilledIcon(icon);
    footerKey = node->getNodeKey();

    beginResetModel();
    _footerNodeList.append(node);
    endResetModel();

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
    for (auto node : _footerNodeList) {
        if (node->getNodeKey() == footerKey) {
            _footerNodeList.removeOne(node);
            break;
        }
    }
}