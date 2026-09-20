#include "nlistitemdelegate_p.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"
#include "ncheckboxindicator_p.h"
#include "nitemeditor_p.h"
#include "nlistview_p.h"
namespace {

QFont captionFont() {
    QFont font = QApplication::font();
    font.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt());
    return font;
}

QFont bodyFont() {
    QFont font = QApplication::font();
    font.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt());
    return font;
}

} // namespace

NListItemDelegate::NListItemDelegate(NListView* view, NListViewPrivate* data, QObject* parent)
    : QStyledItemDelegate(parent), m_view(view), m_data(data) {}

void NListItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (!m_view || !m_data || !index.isValid())
        return;
    if (m_view->viewMode() == QListView::IconMode)
        paintIconRow(painter, option, index);
    else
        paintListRow(painter, option, index);
}

QSize NListItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QSize hint = QStyledItemDelegate::sizeHint(option, index);
    if (!m_data)
        return hint;
    if (m_view && m_view->viewMode() == QListView::IconMode)
        return hint;
    const QString subtitle = index.data(NListViewType::SubtitleRole).toString();
    int           height   = m_data->itemHeight();
    if (!subtitle.isEmpty())
        height = qMax(height, m_data->itemHeight() + 18);
    hint.setHeight(height);
    return hint;
}

bool NListItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                                    const QModelIndex& index) {
    if (!model || !index.isValid() || !(index.flags() & Qt::ItemIsUserCheckable))
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    if (event->type() == QEvent::MouseButtonRelease) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton && checkBoxRect(option.rect).contains(mouseEvent->pos())) {
            const QVariant value   = index.data(Qt::CheckStateRole);
            const auto     state   = static_cast<Qt::CheckState>(value.toInt());
            const Qt::CheckState next = state == Qt::Checked ? Qt::Unchecked : Qt::Checked;
            model->setData(index, next, Qt::CheckStateRole);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget* NListItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
    Q_UNUSED(option);
    if (!m_view)
        return QStyledItemDelegate::createEditor(parent, option, index);
    const int radius = m_data ? m_data->itemBorderRadius() : 4;
    QWidget*  editor = NItemEditor::createEditor(parent, index, radius);
    return editor ? editor : QStyledItemDelegate::createEditor(parent, option, index);
}

void NListItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    NItemEditor::setEditorData(editor, index);
}

void NListItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    NItemEditor::setModelData(editor, model, index);
}

void NListItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                             const QModelIndex& index) const {
    NItemEditor::placeEditor(editor, option.rect, NItemEditor::listCellMargins(index));
}

QRect NListItemDelegate::checkBoxRect(const QRect& itemRect) const {
    constexpr int size = 18;
    QRect         inner(itemRect);
    inner.adjust(2, 2, -2, -2);
    return QRect(inner.left() + 10, inner.center().y() - size / 2, size, size);
}

void NListItemDelegate::paintListRow(QPainter* painter, const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    if (opt.state.testFlag(QStyle::State_HasFocus))
        opt.state &= ~QStyle::State_HasFocus;
    const bool enabled   = opt.state.testFlag(QStyle::State_Enabled);
    const bool selected  = opt.state.testFlag(QStyle::State_Selected);
    const bool hovered   = opt.state.testFlag(QStyle::State_MouseOver);
    const bool pressed   = m_data->pressedIndex.isValid() && m_data->pressedIndex == index;
    QRect      itemRect  = opt.rect;
    itemRect.adjust(2, 2, -2, -2);
    QPainterPath path;
    path.addRoundedRect(itemRect, m_data->itemBorderRadius(), m_data->itemBorderRadius());
    QColor background = Qt::transparent;
    if (selected) {
        if (pressed)
            background = m_data->itemPressedColor();
        else if (hovered)
            background = m_data->itemHoverColor();
        else
            background = m_data->itemSelectedColor();
    } else if (pressed)
        background = m_data->itemHoverColor();
    else if (hovered)
        background = m_data->itemHoverColor();
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    if (background.alpha() > 0)
        painter->fillPath(path, background);
    const bool checkable = index.flags() & Qt::ItemIsUserCheckable;
    int        contentLeft = itemRect.left() + 12;
    if (checkable) {
        const QRect        checkRect = checkBoxRect(opt.rect);
        const QVariant     checkValue = index.data(Qt::CheckStateRole);
        Qt::CheckState     checkState = Qt::Unchecked;
        if (checkValue.isValid())
            checkState = static_cast<Qt::CheckState>(checkValue.toInt());
        NCheckBoxIndicator::paint(painter, checkRect, checkState, enabled, hovered, m_data->isDark, 4);
        contentLeft = checkRect.right() + 8;
    }
    if (!opt.icon.isNull()) {
        QRect iconRect(contentLeft, itemRect.center().y() - 10, 20, 20);
        QIcon::Mode mode = QIcon::Normal;
        if (!enabled)
            mode = QIcon::Disabled;
        else if (selected)
            mode = QIcon::Selected;
        opt.icon.paint(painter, iconRect, Qt::AlignCenter, mode);
        contentLeft = iconRect.right() + 8;
    }
    const QString subtitle = index.data(NListViewType::SubtitleRole).toString();
    const bool    twoLine  = !subtitle.isEmpty();
    const bool showChevron = index.data(NListViewType::ShowChevronRole).toBool();
    int rightReserve = 8;
    if (showChevron)
        rightReserve += 20;
    QRect textRect(contentLeft, itemRect.top(), itemRect.right() - contentLeft - rightReserve, itemRect.height());
    const NThemeType::ThemeMode themeMode = m_data->themeMode;
    if (twoLine) {
        QRect primaryRect = textRect;
        primaryRect.setHeight(textRect.height() / 2 + 2);
        QRect secondaryRect = textRect;
        secondaryRect.setTop(primaryRect.bottom() - 2);
        painter->setFont(bodyFont());
        painter->setPen(enabled ? m_data->textColor()
                              : NThemeColor(NFluentColorKey::TextFillColorDisabled, themeMode));
        painter->drawText(primaryRect, Qt::AlignLeft | Qt::AlignVCenter, opt.text);
        painter->setFont(captionFont());
        painter->setPen(NThemeColor(NFluentColorKey::TextFillColorSecondary, themeMode));
        painter->drawText(secondaryRect, Qt::AlignLeft | Qt::AlignVCenter, subtitle);
    } else {
        painter->setFont(bodyFont());
        painter->setPen(enabled ? m_data->textColor()
                              : NThemeColor(NFluentColorKey::TextFillColorDisabled, themeMode));
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, opt.text);
    }
    if (showChevron) {
        const QIcon chevron = nIcon->fromRegular(NRegularIconType::ChevronRight20Regular);
        QRect       chevronRect(itemRect.right() - 24, itemRect.center().y() - 8, 16, 16);
        chevron.paint(painter, chevronRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
    }
    painter->restore();
}

void NListItemDelegate::paintIconRow(QPainter* painter, const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    if (opt.state.testFlag(QStyle::State_HasFocus))
        opt.state &= ~QStyle::State_HasFocus;
    const bool selected = opt.state.testFlag(QStyle::State_Selected);
    const bool hovered  = opt.state.testFlag(QStyle::State_MouseOver);
    const bool pressed  = m_data->pressedIndex.isValid() && m_data->pressedIndex == index;
    QRect      itemRect = opt.rect.adjusted(4, 4, -4, -4);
    QPainterPath path;
    path.addRoundedRect(itemRect, m_data->itemBorderRadius(), m_data->itemBorderRadius());
    QColor background = Qt::transparent;
    if (selected) {
        if (pressed)
            background = m_data->itemPressedColor();
        else if (hovered)
            background = m_data->itemHoverColor();
        else
            background = m_data->itemSelectedColor();
    } else if (hovered || pressed)
        background = m_data->itemHoverColor();
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    if (background.alpha() > 0)
        painter->fillPath(path, background);
    QRect iconRect = itemRect;
    iconRect.setHeight(itemRect.height() * 2 / 3);
    if (!opt.icon.isNull()) {
        QIcon::Mode mode = opt.state.testFlag(QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled;
        opt.icon.paint(painter, iconRect, Qt::AlignCenter, mode);
    }
    QRect textRect = itemRect;
    textRect.setTop(iconRect.bottom());
    painter->setFont(captionFont());
    painter->setPen(m_data->textColor());
    painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, opt.text);
    painter->restore();
}
