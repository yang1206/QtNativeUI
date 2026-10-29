#include "nnavigationstyle_p.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>

#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"
#include "nnavigationmodel_p.h"
#include "nnavigationnode_p.h"
#include "nnavigationtreeview_p.h"

NNavigationStyle::NNavigationStyle(QStyle* style) : QProxyStyle(style) {
    _pRotate               = 0;
    _pItemHeight           = 40;
    _pLastSelectMarkTop    = 10.0;
    _pLastSelectMarkBottom = 10.0;
    _pSelectMarkTop        = 10.0;
    _pSelectMarkBottom     = 10.0;
 _lastSelectMarkTopAnimation = new QPropertyAnimation(this, "pLastSelectMarkTop");
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        if (_pNavigationView)
            _pNavigationView->viewport()->update();
    });
    _lastSelectMarkTopAnimation->setDuration(300);
    _lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkBottomAnimation = new QPropertyAnimation(this, "pSelectMarkBottom");
    connect(_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        if (_pNavigationView)
            _pNavigationView->viewport()->update();
    });
    _selectMarkBottomAnimation->setDuration(300);
    _selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode    = nullptr;
        _selectMarkBottomAnimation->setStartValue(0);
        _selectMarkBottomAnimation->setEndValue(10);
        _selectMarkBottomAnimation->start();
    });
 _lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "pLastSelectMarkBottom");
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        if (_pNavigationView)
            _pNavigationView->viewport()->update();
    });
    _lastSelectMarkBottomAnimation->setDuration(300);
    _lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkTopAnimation = new QPropertyAnimation(this, "pSelectMarkTop");
    connect(_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        if (_pNavigationView)
            _pNavigationView->viewport()->update();
    });
    _selectMarkTopAnimation->setDuration(300);
    _selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode    = nullptr;
        _selectMarkTopAnimation->setStartValue(0);
        _selectMarkTopAnimation->setEndValue(10);
        _selectMarkTopAnimation->start();
    });

    _expandAnimation = new QPropertyAnimation(this, "pRotate", this);
    _expandAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_expandAnimation, &QPropertyAnimation::valueChanged, this, [this] {
        if (_pNavigationView && _expandAnimationTargetNode)
            _pNavigationView->viewport()->update(_pNavigationView->visualRect(_expandAnimationTargetNode->getModelIndex()));
    });
    connect(_expandAnimation, &QPropertyAnimation::finished, this, [this] {
        const auto node = _expandAnimationTargetNode;
        _expandAnimationTargetNode.clear();
        if (_pNavigationView && node)
            _pNavigationView->viewport()->update(_pNavigationView->visualRect(node->getModelIndex()));
    });

    _themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NNavigationStyle::~NNavigationStyle() {}

void NNavigationStyle::drawPrimitive(PrimitiveElement    element,
                                     const QStyleOption* option,
                                     QPainter*           painter,
                                     const QWidget*      widget) const {
    switch (element) {
        case QStyle::PE_PanelItemViewItem: {
            if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option)) {
                painter->save();
                QModelIndex index = vopt->index;
                painter->setRenderHint(QPainter::Antialiasing);
                QRect itemRect = vopt->rect;
                itemRect.setTop(itemRect.top() + 2);
                itemRect.setBottom(itemRect.bottom() - 2);
                QPainterPath path;
                path.addRoundedRect(itemRect, 8, 8);
 QColor background;
                if (vopt->state & QStyle::State_Selected) {
                    if (index == _pPressIndex) {
                        background = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode);
                    } else if (vopt->state & QStyle::State_MouseOver) {
                        background = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, _themeMode);
                    } else {
                        background = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode);
                    }
                } else {
                    if (index == _pPressIndex) {
                        background = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, _themeMode);
                    } else if (vopt->state & QStyle::State_MouseOver) {
                        background = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode);
                    } else {
                        background = NThemeColor(NFluentColorKey::SubtleFillColorTransparent, _themeMode);
                    }
                }

                if (background.alpha() > 0) {
                    painter->fillPath(path, background);
                }
                painter->restore();
            }
            return;
        }
        case QStyle::PE_PanelItemViewRow: {
            return;
        }
        case QStyle::PE_IndicatorBranch: {
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NNavigationStyle::drawControl(ControlElement      element,
                                   const QStyleOption* option,
                                   QPainter*           painter,
                                   const QWidget*      widget) const {
    switch (element) {
        case QStyle::CE_ShapedFrame: {
            return;
        }
        case QStyle::CE_ItemViewItem: {
            if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option)) {
                this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);
                QRect itemRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                        QPainter::TextAntialiasing);
                NNavigationNode*  node = static_cast<NNavigationNode*>(vopt->index.internalPointer());
                NNavigationModel* model =
                    dynamic_cast<NNavigationModel*>(const_cast<QAbstractItemModel*>(vopt->index.model()));

                QColor accentColor =
                    nTheme->isDarkMode() ? nTheme->accentColor().dark() : nTheme->accentColor().light();
                if (_isSelectMarkDisplay &&
                    (node == model->getSelectedNode() || node == model->getSelectedExpandedNode())) {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(accentColor);
                    painter->drawRoundedRect(QRectF(itemRect.x() + 3,
                                                    itemRect.y() + _pSelectMarkTop,
                                                    3,
                                                    itemRect.height() - _pSelectMarkTop - _pSelectMarkBottom),
                                             3,
                                             3);
                }
                if (node == _lastSelectedNode) {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(accentColor);
                    painter->drawRoundedRect(QRectF(itemRect.x() + 3,
                                                    itemRect.y() + _pLastSelectMarkTop,
                                                    3,
                                                    itemRect.height() - _pLastSelectMarkTop - _pLastSelectMarkBottom),
                                             3,
                                             3);
                }
 QColor textColor;
                if (!(vopt->state & QStyle::State_Enabled)) {
                    textColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, _themeMode);
                } else if (vopt->state & QStyle::State_Selected) {
                    textColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, _themeMode);
                } else {
                    textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);
                }
 painter->setPen(vopt->index == _pPressIndex ? textColor.darker(120) : textColor);
                NRegularIconType::Icon icon = node->getIcon();
 painter->save();
                NRegularIconType::Icon regularIcon = node->getIcon();
                NFilledIconType::Icon  filledIcon  = node->getFilledIcon();
                if (regularIcon != NRegularIconType::None) {
                    QIcon iconObj = nIcon->fromRegular(regularIcon);
                    QRect iconRect(
                        itemRect.x() + (_iconAreaWidth - 17) / 2, itemRect.y() + (itemRect.height() - 17) / 2, 17, 17);
                    iconObj.paint(painter,
                                  iconRect,
                                  Qt::AlignCenter,
                                  vopt->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
                } else if (filledIcon != NFilledIconType::None) {
                    QIcon iconObj = nIcon->fromFilled(filledIcon);
                    QRect iconRect(
                        itemRect.x() + (_iconAreaWidth - 17) / 2, itemRect.y() + (itemRect.height() - 17) / 2, 17, 17);
                    iconObj.paint(painter,
                                  iconRect,
                                  Qt::AlignCenter,
                                  vopt->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
                }
                painter->restore();

                int viewWidth = widget->width();
                painter->setPen(vopt->index == _pPressIndex ? textColor.darker(120) : textColor);
                QRect textRect;
                if (icon != NRegularIconType::None || filledIcon != NFilledIconType::None) {
                    textRect = QRect(itemRect.x() + _iconAreaWidth,
                                     itemRect.y(),
                                     itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _iconAreaWidth,
                                     itemRect.height());
                } else {
                    textRect = QRect(itemRect.x() + _leftPadding,
                                     itemRect.y(),
                                     itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _leftPadding,
                                     itemRect.height());
                }
                QString text =
                    painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
                painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

                if (viewWidth > 260) {
                    if (node->getIsExpanderNode()) {
                        if (node->getIsHasChild()) {
                            QRect expandIconRect(
                                itemRect.right() - _indicatorIconAreaWidth, itemRect.y(), 17, itemRect.height());

                            painter->save();
                            QIcon downIcon = nIcon->fromRegular(NRegularIconType::ChevronDown24Regular);
                            QRect downRect = expandIconRect;
                            painter->translate(expandIconRect.x() + (qreal) expandIconRect.width() / 2,
                                               expandIconRect.y() + (qreal) expandIconRect.height() / 2);
                            if (node == _expandAnimationTargetNode) {
                                painter->rotate(_pRotate);
                            } else {
                                if (node->getIsExpanded()) {
painter->rotate(-180);
                                } else {
                                    painter->rotate(0);
                                }
                            }
                            painter->translate(-expandIconRect.x() - (qreal) expandIconRect.width() / 2,
                                               -expandIconRect.y() - (qreal) expandIconRect.height() / 2);
                            downIcon.paint(painter,
                                           downRect,
                                           Qt::AlignCenter,
                                           vopt->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
                            painter->restore();
                        }
                        if (node->getIsChildHasKeyPoints()) {
                            painter->save();
                            painter->setPen(Qt::NoPen);
                            painter->setBrush(accentColor);
                            painter->drawEllipse(QPoint(itemRect.right() - 17, itemRect.y() + 12), 3, 3);
                            painter->restore();
                        }
                    } else {
                        int keyPoints = node->getKeyPoints();
                        if (keyPoints) {
                            painter->save();
                            painter->setPen(Qt::NoPen);
                            painter->setBrush(Qt::white);
                            painter->drawEllipse(
                                QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2), 10, 10);
                            painter->setBrush(NThemeColor(NFluentColorKey::SystemFillColorCritical, _themeMode));
                            painter->drawEllipse(
                                QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2), 9, 9);
                            painter->setPen(QPen(Qt::white, 2));
                            QFont font = painter->font();
                            font.setBold(true);
                            if (keyPoints > 99) {
                                keyPoints = 99;
                            }
                            if (keyPoints > 9) {
                                font.setPixelSize(11);
                            } else {
                                font.setPixelSize(12);
                            }
                            painter->setFont(font);
                            painter->drawText(keyPoints > 9 ? itemRect.right() - 33 : itemRect.right() - 30,
                                              itemRect.y() + itemRect.height() / 2 + 4,
                                              QString::number(keyPoints));
                            painter->restore();
                        }
                    }
                }

                QPen borderPen(NThemeColor(NFluentColorKey::SubtleFillColorTransparent, _themeMode));
                painter->setPen(borderPen);

                painter->restore();
            }
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize NNavigationStyle::sizeFromContents(ContentsType        type,
                                         const QStyleOption* option,
                                         const QSize&        size,
                                         const QWidget*      widget) const {
    switch (type) {
        case QStyle::CT_ItemViewItem: {
            QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            itemSize.setHeight(_pItemHeight);
            return itemSize;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void NNavigationStyle::navigationNodeStateChange(QVariantMap data) {
    if (data.contains("Expand") || data.contains("Collapse")) {
        const bool expanding = data.contains("Expand");
        auto* node = data.value(expanding ? "Expand" : "Collapse").value<NNavigationNode*>();
        if (!node) return;
        const qreal start = _expandAnimationTargetNode == node ? _pRotate : (expanding ? 0.0 : -180.0);
        const qreal end = expanding ? -180.0 : 0.0;
        // 子项可见性由 QTreeView 的展开动画管理，快照不能再叠加透明遮蔽。
        // 反向操作从当前角度接续，旧动画不能在完成时清掉新目标。
        _expandAnimation->stop();
        _expandAnimationTargetNode = node;
        _expandAnimation->setStartValue(start);
        _expandAnimation->setEndValue(end);
        _expandAnimation->setDuration(qMax(1, qRound(300 * qAbs(end - start) / 180)));
        _expandAnimation->start();
    } else if (data.contains("SelectMarkChanged")) {
        _lastSelectedNode             = data.value("LastSelectedNode").value<NNavigationNode*>();
        NNavigationNode* selectedNode = data.value("SelectedNode").value<NNavigationNode*>();
        bool             direction    = _compareItemY(selectedNode, _lastSelectedNode);
        _pLastSelectMarkTop           = 10;
        _pLastSelectMarkBottom        = 10;
        _pSelectMarkTop               = 10;
        _pSelectMarkBottom            = 10;
        if (direction) {
            _lastSelectMarkTopAnimation->setStartValue(10);
            _lastSelectMarkTopAnimation->setEndValue(0);
            _lastSelectMarkTopAnimation->start();
            _lastSelectMarkBottomAnimation->stop();
            _selectMarkTopAnimation->stop();
            _isSelectMarkDisplay = false;
        } else {
            _lastSelectMarkBottomAnimation->setStartValue(10);
            _lastSelectMarkBottomAnimation->setEndValue(0);
            _lastSelectMarkBottomAnimation->start();
            _lastSelectMarkTopAnimation->stop();
            _selectMarkBottomAnimation->stop();
            _isSelectMarkDisplay = false;
        }
    }
}

bool NNavigationStyle::_compareItemY(NNavigationNode* node1, NNavigationNode* node2) {
    if (!node1) {
        return false;
    }
    if (!node2) {
        return true;
    }

    if (node1->getParentNode() == node2->getParentNode()) {
        if (node1->getModelIndex().row() < node2->getModelIndex().row()) {
            return true;
        } else {
            return false;
        }
    } else {
        NNavigationNode* node1OriginalNode = node1->getOriginalNode();
        NNavigationNode* node2OriginalNode = node2->getOriginalNode();

        if (node1OriginalNode == node2OriginalNode) {
            int node1Depth = node1->getDepth();
            int node2Depth = node2->getDepth();

            if (node1Depth == node2Depth) {
                NNavigationNode* node1ParentNode = node1->getParentNode();
                NNavigationNode* node2ParentNode = node2->getParentNode();
                if (node1ParentNode->getModelIndex().row() < node2ParentNode->getModelIndex().row()) {
                    return true;
                } else {
                    return false;
                }
            } else {
                if (node1Depth < node2Depth) {
                    NNavigationNode* node2ParentNode = node2->getParentNode();
                    while (node2ParentNode->getDepth() != node1Depth) {
                        node2ParentNode = node2ParentNode->getParentNode();
                    }

                    if (node1 == node2ParentNode) {
                        return true;
                    }
                    if (node1->getModelIndex().row() < node2ParentNode->getModelIndex().row()) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    NNavigationNode* node1ParentNode = node1->getParentNode();
                    while (node1ParentNode->getDepth() != node2Depth) {
                        node1ParentNode = node1ParentNode->getParentNode();
                    }
                    if (node2 == node1ParentNode) {
                        return false;
                    }
                    if (node1ParentNode->getModelIndex().row() < node2->getModelIndex().row()) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }
        } else {
            if (node1OriginalNode->getModelIndex().row() < node2OriginalNode->getModelIndex().row()) {
                return true;
            } else {
                return false;
            }
        }
    }
}
