#include "nnavigationfooterdelegate_p.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "QtNativeUI/NTheme.h"
#include "nnavigationfootermodel_p.h"
#include "nnavigationnode_p.h"

NNavigationFooterDelegate::NNavigationFooterDelegate(QObject* parent) : QStyledItemDelegate{parent} {
    _pListView = nullptr;
    _themeMode = nTheme->themeMode();
    connect(
        nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) { _themeMode = themeMode; });

    setProperty("lastSelectMarkTop", 10.0);
    setProperty("lastSelectMarkBottom", 10.0);
    setProperty("selectMarkTop", 10.0);
    setProperty("selectMarkBottom", 10.0);

    // Mark向上
    _lastSelectMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
        _lastSelectMarkTop = value.toReal();
        if (_pListView)
            _pListView->viewport()->update();
    });
    _lastSelectMarkTopAnimation->setDuration(300);
    _lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
    connect(_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
        _selectMarkBottom = value.toReal();
        if (_pListView)
            _pListView->viewport()->update();
    });
    _selectMarkBottomAnimation->setDuration(300);
    _selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [this]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode    = nullptr;
        _selectMarkBottomAnimation->setStartValue(0);
        _selectMarkBottomAnimation->setEndValue(10);
        _selectMarkBottomAnimation->start();
    });

    // Mark向下
    _lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "lastSelectMarkBottom");
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
        _lastSelectMarkBottom = value.toReal();
        if (_pListView)
            _pListView->viewport()->update();
    });
    _lastSelectMarkBottomAnimation->setDuration(300);
    _lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
    connect(_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
        _selectMarkTop = value.toReal();
        if (_pListView)
            _pListView->viewport()->update();
    });
    _selectMarkTopAnimation->setDuration(300);
    _selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [this]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode    = nullptr;
        _selectMarkTopAnimation->setStartValue(0);
        _selectMarkTopAnimation->setEndValue(10);
        _selectMarkTopAnimation->start();
    });
}

NNavigationFooterDelegate::~NNavigationFooterDelegate() {}

void NNavigationFooterDelegate::navigationNodeStateChange(QVariantMap data) {
    if (data.contains("SelectMarkChanged")) {
        _lastSelectedNode             = data.value("LastSelectedNode").value<NNavigationNode*>();
        NNavigationNode* selectedNode = data.value("SelectedNode").value<NNavigationNode*>();
        bool             direction    = _compareItemY(selectedNode, _lastSelectedNode);
        _lastSelectMarkTop            = 10;
        _lastSelectMarkBottom         = 10;
        _selectMarkTop                = 10;
        _selectMarkBottom             = 10;
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

void NNavigationFooterDelegate::paint(QPainter*                   painter,
                                      const QStyleOptionViewItem& option,
                                      const QModelIndex&          index) const {
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    NNavigationFooterModel* model =
        dynamic_cast<NNavigationFooterModel*>(const_cast<QAbstractItemModel*>(index.model()));
    NNavigationNode* node = index.data(Qt::UserRole).value<NNavigationNode*>();
    if (option.state.testFlag(QStyle::State_HasFocus)) {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, viewOption, index);

    // 背景绘制
    QRect itemRect = option.rect;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect.setTop(itemRect.top() + 2);
    itemRect.setBottom(itemRect.bottom() - 2);
    QPainterPath path;
    path.addRoundedRect(itemRect, 8, 8);

    if (option.state & QStyle::State_Selected) {
        if (index == _pPressIndex) {
            painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
        } else {
            if (option.state & QStyle::State_MouseOver) {
                // 选中时覆盖
                painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorTertiary, _themeMode));
            } else {
                // 选中
                painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
            }
        }
    } else {
        if (index == _pPressIndex) {
            painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorTertiary, _themeMode));
        } else {
            if (option.state & QStyle::State_MouseOver) {
                painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
            }
        }
    }
    painter->restore();

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect = option.rect;

    if (index.row() == 0) {
        painter->setPen(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, _themeMode));
        painter->drawLine(option.rect.x(), itemRect.y() + 1, option.rect.x() + option.rect.width(), itemRect.y() + 1);
    }

    QColor textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);
    painter->setPen(index == _pPressIndex ? NThemeColor(NFluentColorKey::TextFillColorSecondary, _themeMode)
                                          : textColor);

    NRegularIconType::Icon regularIcon = node->getIcon();
    NFilledIconType::Icon  filledIcon  = node->getFilledIcon();
    if (regularIcon != NRegularIconType::None) {
        QIcon iconObj = nIcon->fromRegular(regularIcon);
        QRect iconRect(itemRect.x() + (_iconAreaWidth - 17) / 2, itemRect.y() + (itemRect.height() - 17) / 2, 17, 17);
        iconObj.paint(
            painter, iconRect, Qt::AlignCenter, option.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
    } else if (filledIcon != NFilledIconType::None) {
        QIcon iconObj = nIcon->fromFilled(filledIcon);
        QRect iconRect(itemRect.x() + (_iconAreaWidth - 17) / 2, itemRect.y() + (itemRect.height() - 17) / 2, 17, 17);
        iconObj.paint(
            painter, iconRect, Qt::AlignCenter, option.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
    }

    int keyPoints = node->getKeyPoints();
    if (keyPoints) {
        painter->save();
        painter->setPen(Qt::NoPen);

        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 10, 10);

        painter->setBrush(NThemeColor(NFluentColorKey::SystemFillColorCritical, _themeMode));
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 9, 9);

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
        painter->drawText(
            keyPoints > 9 ? 248 : 251, itemRect.y() + itemRect.height() / 2 + 4, QString::number(keyPoints));
        painter->restore();
    }

    painter->setPen(index == _pPressIndex ? NThemeColor(NFluentColorKey::TextFillColorSecondary, _themeMode)
                                          : textColor);
    QRect textRect;
    if (regularIcon != NRegularIconType::None || filledIcon != NFilledIconType::None) {
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
    QString text = painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

    QColor accentColor = nTheme->isDarkMode() ? nTheme->accentColor().dark() : nTheme->accentColor().light();

    if (_isSelectMarkDisplay && (node == model->getSelectedNode())) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(accentColor);
        painter->drawRoundedRect(QRectF(itemRect.x() + 3,
                                        itemRect.y() + _selectMarkTop,
                                        3,
                                        itemRect.height() - _selectMarkTop - _selectMarkBottom),
                                 3,
                                 3);
    }
    if (node == _lastSelectedNode) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(accentColor);
        painter->drawRoundedRect(QRectF(itemRect.x() + 3,
                                        itemRect.y() + _lastSelectMarkTop,
                                        3,
                                        itemRect.height() - _lastSelectMarkTop - _lastSelectMarkBottom),
                                 3,
                                 3);
    }
    painter->restore();
}

QSize NNavigationFooterDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}

bool NNavigationFooterDelegate::_compareItemY(NNavigationNode* node1, NNavigationNode* node2) {
    if (!node1) {
        return true;
    }
    if (!node2) {
        return false;
    }
    if (node1->getModelIndex().row() < node2->getModelIndex().row()) {
        return true;
    } else {
        return false;
    }
}