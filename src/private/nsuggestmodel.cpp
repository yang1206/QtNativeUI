#include "nsuggestmodel.h"
#include "nsuggestbox_p.h"

#include <QPainter>
#include <QPainterPath>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

// NSuggestModel 实现
NSuggestModel::NSuggestModel(QObject* parent) : QAbstractListModel(parent) {}

NSuggestModel::~NSuggestModel() {}

int NSuggestModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return _suggestions.count();
}

QVariant NSuggestModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= _suggestions.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return _suggestions[index.row()]->getText();

    if (role == Qt::UserRole)
        return QVariant::fromValue(_suggestions[index.row()]);

    return QVariant();
}

void NSuggestModel::setSuggestions(const QVector<NSuggestion*>& suggestions) {
    beginResetModel();
    _suggestions = suggestions;
    endResetModel();
}

void NSuggestModel::clearSuggestions() {
    beginResetModel();
    _suggestions.clear();
    endResetModel();
}

NSuggestion* NSuggestModel::getSuggestion(int row) const {
    if (row < 0 || row >= _suggestions.count())
        return nullptr;
    return _suggestions[row];
}

// NSuggestDelegate 实现
NSuggestDelegate::NSuggestDelegate(QObject* parent) : QStyledItemDelegate(parent) {
    _themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) { _themeMode = mode; });
}

NSuggestDelegate::~NSuggestDelegate() {}

void NSuggestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (!index.isValid())
        return;

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // 获取建议项
    NSuggestion* suggestion = index.data(Qt::UserRole).value<NSuggestion*>();
    if (!suggestion) {
        painter->restore();
        return;
    }

    // 绘制背景
    QPainterPath path;
    QRect        itemRect = option.rect;
    itemRect.adjust(4, 2, -4, -2);
    path.addRoundedRect(itemRect, 6, 6);

    if (option.state & QStyle::State_Selected) {
        if (option.state & QStyle::State_MouseOver) {
            painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorTertiary, _themeMode));
        } else {
            painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
        }
    } else if (option.state & QStyle::State_MouseOver) {
        painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
    }

    // 绘制图标
    NRegularIconType::Icon icon = suggestion->getIcon();
    if (icon != NRegularIconType::None) {
        QIcon iconObj = nIcon->fromRegular(icon);
        QRect iconRect(itemRect.x() + 8, itemRect.y() + (itemRect.height() - 16) / 2, 16, 16);
        iconObj.paint(
            painter, iconRect, Qt::AlignCenter, option.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
    }

    // 绘制文字
    painter->setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode));
    QRect textRect = itemRect;
    textRect.setLeft(icon != NRegularIconType::None ? itemRect.left() + 32 : itemRect.left() + 10);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, suggestion->getText());

    painter->restore();
}

QSize NSuggestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(200, 40);
}