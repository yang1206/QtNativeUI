#include "nlistitemdelegate_p.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QFontMetrics>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>
#include <QWidget>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"
#include "ncheckboxindicator_p.h"
#include "nitemeditor_p.h"
#include "nlistview_p.h"
#include "nlistviewitemlayout_p.h"
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

bool NListItemDelegate::rowStartsSection(const QModelIndex& index) const {
    return NListViewItemLayout::rowStartsSection(m_view, index);
}

int NListItemDelegate::sectionHeaderHeight() const {
    return NListViewItemLayout::sectionHeaderHeight(m_view);
}

void NListItemDelegate::paintSectionHeader(QPainter* painter, const QRect& itemRect, const QString& title,
                                           const QColor& textColor) const {
    const int height = sectionHeaderHeight();
    if (!m_data || title.isEmpty() || height <= 0)
        return;
    QRect headerRect(itemRect.left() + 12, itemRect.top(), itemRect.width() - 24, height);
    painter->setFont(m_data->sectionHeaderFont());
    painter->setPen(textColor.isValid() ? textColor : m_data->sectionTextColor());
    painter->drawText(headerRect, Qt::AlignLeft | Qt::AlignVCenter, title);
}

void NListItemDelegate::paintStickySection(QWidget* viewport) const {
    if (!m_view || !m_data || !viewport)
        return;
    if (!m_view->getSectionsEnabled() || m_view->viewMode() == QListView::IconMode)
        return;
    QAbstractItemModel* model = m_view->model();
    if (!model || model->rowCount() <= 0)
        return;
    const int headerH = sectionHeaderHeight();
    if (headerH <= 0)
        return;
    const int probeX = qBound(0, viewport->width() / 2, qMax(0, viewport->width() - 1));
    QModelIndex topIndex = m_view->indexAt(QPoint(probeX, 0));
    if (!topIndex.isValid())
        topIndex = m_view->indexAt(QPoint(probeX, 1));
    if (!topIndex.isValid() || topIndex.model() != model)
        return;
    const QString section = topIndex.data(NListViewType::SectionRole).toString();
    if (section.isEmpty())
        return;
    const int column = topIndex.column();
    int startRow = topIndex.row();
    while (startRow > 0) {
        const QModelIndex previous = model->index(startRow - 1, column);
        if (previous.data(NListViewType::SectionRole).toString() != section)
            break;
        --startRow;
    }
    const QModelIndex startIndex = model->index(startRow, column);
    if (m_view->visualRect(startIndex).top() >= 0)
        return;
    int stickyY = 0;
    for (int row = startRow + 1; row < model->rowCount(); ++row) {
        const QModelIndex index = model->index(row, column);
        if (!rowStartsSection(index))
            continue;
        const int nextTop = m_view->visualRect(index).top();
        if (nextTop < headerH)
            stickyY = nextTop - headerH;
        break;
    }
    const QRect bar(0, stickyY, viewport->width(), headerH);
    QPainter painter(viewport);
    if (!painter.isActive())
        return;
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.fillRect(bar, m_data->backgroundColor());
    QColor titleColor = startIndex.data(NListViewType::SectionForegroundRole).value<QColor>();
    paintSectionHeader(&painter, bar, section, titleColor);
}

QSize NListItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QSize hint = QStyledItemDelegate::sizeHint(option, index);
    if (!m_data)
        return hint;
    if (m_view && m_view->viewMode() == QListView::IconMode) {
        const QSize grid = m_view->gridSize();
        if (grid.width() > 0 && grid.height() > 0)
            return grid;
        QSize icon = m_view->iconSize();
        if (!icon.isValid() || icon.isEmpty())
            icon = QSize(48, 48);
        const int textH = QFontMetrics(captionFont()).height() * 2 + 16;
        return QSize(qMax(icon.width() + 24, 96), icon.height() + textH);
    }
    const QString subtitle = index.data(NListViewType::SubtitleRole).toString();
    int           height   = m_data->itemHeight();
    if (!subtitle.isEmpty())
        height = qMax(height, m_data->itemHeight() + 18);
    if (rowStartsSection(index))
        height += sectionHeaderHeight();
    hint.setHeight(height);
    return hint;
}

bool NListItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                                    const QModelIndex& index) {
    if (!model || !index.isValid() || !(index.flags() & Qt::ItemIsUserCheckable))
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    if (event->type() == QEvent::MouseButtonRelease) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        const QRect hitRect = (m_view && m_view->viewMode() == QListView::IconMode)
                                  ? iconCheckBoxRect(option.rect.adjusted(4, 4, -4, -4))
                                  : checkBoxRect(option.rect);
        if (mouseEvent->button() == Qt::LeftButton && hitRect.contains(mouseEvent->pos())) {
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
    if (m_view && m_view->viewMode() == QListView::IconMode)
        return nullptr;
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
    QRect itemRect = NListViewItemLayout::rowContentRect(m_view, index, opt.rect);
    if (rowStartsSection(index)) {
        QColor sectionColor = index.data(NListViewType::SectionForegroundRole).value<QColor>();
        if (!sectionColor.isValid())
            sectionColor = m_data->sectionTextColor();
        paintSectionHeader(painter, opt.rect, index.data(NListViewType::SectionRole).toString(), sectionColor);
    }
    itemRect = NListViewItemLayout::rowBackgroundRect(itemRect);
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

QRect NListItemDelegate::iconCheckBoxRect(const QRect& tileRect) const {
    constexpr int size = 18;
    return QRect(tileRect.right() - size - 6, tileRect.top() + 6, size, size);
}

void NListItemDelegate::paintIconRow(QPainter* painter, const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    if (opt.state.testFlag(QStyle::State_HasFocus))
        opt.state &= ~QStyle::State_HasFocus;
    const bool enabled  = opt.state.testFlag(QStyle::State_Enabled);
    const bool selected = opt.state.testFlag(QStyle::State_Selected);
    const bool hovered  = opt.state.testFlag(QStyle::State_MouseOver);
    const bool pressed  = m_data->pressedIndex.isValid() && m_data->pressedIndex == index;
    const QRect tile    = opt.rect.adjusted(4, 4, -4, -4);
    QPainterPath path;
    path.addRoundedRect(tile, m_data->itemBorderRadius(), m_data->itemBorderRadius());
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
    QSize iconSize = m_view ? m_view->iconSize() : QSize();
    if (!iconSize.isValid() || iconSize.isEmpty())
        iconSize = QSize(48, 48);
    constexpr int kPad     = 8;
    constexpr int kTextGap = 6;
    QRect iconRect(0, 0, iconSize.width(), iconSize.height());
    iconRect.moveCenter(QPoint(tile.center().x(), tile.top() + kPad + iconSize.height() / 2));
    if (!opt.icon.isNull()) {
        QIcon::Mode mode = QIcon::Normal;
        if (!enabled)
            mode = QIcon::Disabled;
        else if (selected)
            mode = QIcon::Selected;
        opt.icon.paint(painter, iconRect, Qt::AlignCenter, mode);
    }
    const bool checkable = index.flags() & Qt::ItemIsUserCheckable;
    if (checkable) {
        const QVariant checkValue = index.data(Qt::CheckStateRole);
        Qt::CheckState checkState = Qt::Unchecked;
        if (checkValue.isValid())
            checkState = static_cast<Qt::CheckState>(checkValue.toInt());
        NCheckBoxIndicator::paint(painter, iconCheckBoxRect(tile), checkState, enabled, hovered, m_data->isDark, 4);
    }
    QRect textRect = tile.adjusted(kPad, 0, -kPad, -kPad);
    textRect.setTop(iconRect.bottom() + kTextGap);
    const NThemeType::ThemeMode themeMode = m_data->themeMode;
    const QString subtitle = index.data(NListViewType::SubtitleRole).toString();
    painter->setFont(captionFont());
    painter->setPen(enabled ? m_data->textColor()
                           : NThemeColor(NFluentColorKey::TextFillColorDisabled, themeMode));
    if (subtitle.isEmpty()) {
        painter->drawText(textRect, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, opt.text);
    } else {
        const QFontMetrics titleMetrics(painter->font());
        QRect              titleRect = textRect;
        titleRect.setHeight(titleMetrics.height());
        painter->drawText(titleRect, Qt::AlignHCenter | Qt::AlignVCenter,
                          titleMetrics.elidedText(opt.text, Qt::ElideRight, titleRect.width()));
        QRect subRect = textRect;
        subRect.setTop(titleRect.bottom() + 2);
        painter->setFont(captionFont());
        painter->setPen(NThemeColor(NFluentColorKey::TextFillColorSecondary, themeMode));
        const QFontMetrics subMetrics(painter->font());
        painter->drawText(subRect, Qt::AlignHCenter | Qt::AlignTop,
                          subMetrics.elidedText(subtitle, Qt::ElideRight, subRect.width()));
    }
    painter->restore();
}
