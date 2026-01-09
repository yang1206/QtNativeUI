#include "nnavigationnode_p.h"
#include <QUuid>

NNavigationNode::NNavigationNode(QString nodeTitle, NNavigationNode* parent) : QObject(parent) {
    _pDepth           = 0;
    _pKeyPoints       = 0;
    _nodeKey          = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _nodeTitle        = nodeTitle;
    _pIsRootNode      = false;
    _pIsFooterNode    = false;
    _pIsHasFooterPage = false;
    _pParentNode      = parent;
    _pIsExpanderNode  = false;
    _pIsVisible       = false;
    _pIcon            = NRegularIconType::None;
    _pFilledIcon      = NFilledIconType::None;
}

NNavigationNode::~NNavigationNode() { qDeleteAll(_pChildrenNodes); }

QString NNavigationNode::getNodeKey() const { return _nodeKey; }

QString NNavigationNode::getNodeTitle() const { return _nodeTitle; }

void NNavigationNode::setIsExpanded(bool isExpanded) {
    _isExpanded = isExpanded;
    setChildVisible(isExpanded);
}

bool NNavigationNode::getIsExpanded() const { return _isExpanded; }

void NNavigationNode::setChildVisible(bool isVisible) {
    if (isVisible) {
        for (auto node : _pChildrenNodes) {
            node->setIsVisible(isVisible);
            if (node->getIsExpanderNode() && !node->getIsExpanded()) {
                continue;
            }
            node->setChildVisible(isVisible);
        }
    } else {
        for (auto node : _pChildrenNodes) {
            node->setChildVisible(isVisible);
            node->setIsVisible(isVisible);
        }
    }
}

bool NNavigationNode::getIsHasChild() const {
    if (_pChildrenNodes.count() > 0) {
        return true;
    }
    return false;
}

bool NNavigationNode::getIsHasPageChild() const {
    if (_pChildrenNodes.count() == 0) {
        return false;
    }
    for (auto childNode : _pChildrenNodes) {
        if (!childNode->getIsExpanderNode()) {
            return true;
        }
        if (childNode->getIsHasPageChild()) {
            return true;
        }
    }
    return false;
}

void NNavigationNode::appendChildNode(NNavigationNode* childNode) {
    if (_pIsExpanderNode) {
        _pChildrenNodes.append(childNode);
    }
}

void NNavigationNode::removeChildNode(NNavigationNode* childNode) {
    if (_pIsExpanderNode) {
        _pChildrenNodes.removeOne(childNode);
    }
}

bool NNavigationNode::getIsChildHasKeyPoints() const {
    for (auto childNode : _pChildrenNodes) {
        if (childNode->getKeyPoints()) {
            return true;
        }
        if (childNode->getIsChildHasKeyPoints()) {
            return true;
        }
    }
    return false;
}

NNavigationNode* NNavigationNode::getOriginalNode() {
    if (this->getParentNode()->getIsRootNode()) {
        return this;
    } else {
        NNavigationNode* originalNode = this->getParentNode();
        while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode()) {
            originalNode = originalNode->getParentNode();
        }
        return originalNode;
    }
}

bool NNavigationNode::getIsChildNode(NNavigationNode* node) {
    if (_pChildrenNodes.count() > 0) {
        if (_pChildrenNodes.contains(node)) {
            return true;
        }
        for (auto childNode : _pChildrenNodes) {
            if (childNode->getIsChildNode(node)) {
                return true;
            }
        }
    }
    return false;
}

int NNavigationNode::getRow() const {
    if (_pParentNode) {
        return _pParentNode->getChildrenNodes().indexOf(const_cast<NNavigationNode*>(this));
    }
    return 0;
}