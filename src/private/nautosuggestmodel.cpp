#include "nautosuggestmodel.h"
#include "nautosuggestbox_p.h"

#include <QPainter>
#include <QPainterPath>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NAutoSuggestModel::NAutoSuggestModel(QObject* parent) : QAbstractListModel(parent) {}

NAutoSuggestModel::~NAutoSuggestModel() {}

int NAutoSuggestModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return _suggestions.count();
}

QVariant NAutoSuggestModel::data(const QModelIndex& index, int role) const { return QVariant(); }

void NAutoSuggestModel::setSuggestions(const QVector<NAutoSuggestion*>& suggestions) {
    beginResetModel();
    _suggestions = suggestions;
    endResetModel();
}

void NAutoSuggestModel::clearSuggestions() {
    beginResetModel();
    _suggestions.clear();
    endResetModel();
}

NAutoSuggestion* NAutoSuggestModel::getSuggestion(int row) const {
    if (row < 0 || row >= _suggestions.count())
        return nullptr;
    return _suggestions[row];
}

NAutoSuggestDelegate::NAutoSuggestDelegate(QObject* parent) : QStyledItemDelegate(parent) {
    _themeMode = nTheme->themeMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) { _themeMode = mode; });
}

NAutoSuggestDelegate::~NAutoSuggestDelegate() {}

void NAutoSuggestDelegate::paint(QPainter*                   painter,
                                 const QStyleOptionViewItem& option,
                                 const QModelIndex&          index) const {
    int                  margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    NAutoSuggestModel* model      = dynamic_cast<NAutoSuggestModel*>(const_cast<QAbstractItemModel*>(index.model()));
    NAutoSuggestion*   suggestion = model->getSuggestion(index.row());
    if (option.state.testFlag(QStyle::State_HasFocus)) {
        viewOption.state &= ~QStyle::State_HasFocus;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect        optionRect = option.rect;
    optionRect.adjust(margin * 2, margin, -margin * 2, -margin);
    path.addRoundedRect(optionRect, 8, 8);

    if (option.state & QStyle::State_Selected) {
        if (option.state & QStyle::State_MouseOver) {
            painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorTertiary, _themeMode));
        } else {
            painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
        }
    } else if (option.state & QStyle::State_MouseOver) {
        painter->fillPath(path, NThemeColor(NFluentColorKey::SubtleFillColorSecondary, _themeMode));
    }


    NRegularIconType::Icon icon = suggestion->getIcon();
    if (icon != NRegularIconType::None) {
        QIcon iconObj = nIcon->fromRegular(icon);
        QRect iconRect(optionRect.x() + 8, optionRect.y() + (optionRect.height() - 16) / 2, 16, 16);
        iconObj.paint(
            painter, iconRect, Qt::AlignCenter, option.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled);
    }


    painter->setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode));
    QRect textRect = optionRect;
    textRect.setLeft(icon != NRegularIconType::None ? optionRect.left() + 32 : optionRect.left() + 10);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, suggestion->getText());

    painter->restore();
}

QSize NAutoSuggestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}