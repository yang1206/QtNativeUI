#include "ntreeitemdelegate_p.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QEvent>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTreeView>

#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"
#include "ncheckboxindicator_p.h"
#include "nitemeditor_p.h"
#include "ntreeview_p.h"

namespace {

constexpr qreal kChevronAreaW = 20.0;
constexpr qreal kCheckBoxAreaW = 22.0;
constexpr qreal kIconAreaW = 20.0;
constexpr qreal kGap = 4.0;
constexpr qreal kCursorStart = 12.0;

QFont bodyFont() {
    QFont font = QApplication::font();
    font.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt());
    return font;
}

void setCheckRecursive(QAbstractItemModel* model, const QModelIndex& index, Qt::CheckState state) {
    if (!(index.flags() & Qt::ItemIsUserCheckable))
        return;
    model->setData(index, state, Qt::CheckStateRole);
    const int rows = model->rowCount(index);
    for (int r = 0; r < rows; ++r)
        setCheckRecursive(model, model->index(r, index.column(), index), state);
}

void updateAncestorChecks(QAbstractItemModel* model, QModelIndex index) {
    index = index.parent();
    while (index.isValid()) {
        if (!(index.flags() & Qt::ItemIsUserCheckable))
            break;
        int checked = 0;
        int unchecked = 0;
        int partial = 0;
        int checkable = 0;
        const int rows = model->rowCount(index);
        for (int r = 0; r < rows; ++r) {
            const QModelIndex child = model->index(r, index.column(), index);
            if (!(child.flags() & Qt::ItemIsUserCheckable))
                continue;
            ++checkable;
            const auto state = static_cast<Qt::CheckState>(child.data(Qt::CheckStateRole).toInt());
            if (state == Qt::Checked)
                ++checked;
            else if (state == Qt::PartiallyChecked)
                ++partial;
            else
                ++unchecked;
        }
        Qt::CheckState parentState = Qt::Unchecked;
        if (checkable > 0) {
            if (partial > 0 || (checked > 0 && unchecked > 0))
                parentState = Qt::PartiallyChecked;
            else if (checked == checkable)
                parentState = Qt::Checked;
            else
                parentState = Qt::Unchecked;
        }
        model->setData(index, parentState, Qt::CheckStateRole);
        index = index.parent();
    }
}

} // namespace

NTreeItemDelegate::NTreeItemDelegate(NTreeView* view, NTreeViewPrivate* data, QObject* parent)
    : QStyledItemDelegate(parent), m_view(view), m_data(data) {}

QRectF NTreeItemDelegate::backgroundRect(const QStyleOptionViewItem& option) const {
    const int vpWidth = (m_view && m_view->viewport()) ? m_view->viewport()->width() : option.rect.right();
    return QRectF(2, option.rect.top() + 2, vpWidth - 4, option.rect.height() - 4);
}

QRectF NTreeItemDelegate::checkBoxRect(const QStyleOptionViewItem& option) const {
    if (!(option.features & QStyleOptionViewItem::HasCheckIndicator)
        && !(option.index.isValid() && (option.index.flags() & Qt::ItemIsUserCheckable)))
        return {};
    const QRectF bg = backgroundRect(option);
    return QRectF(qreal(option.rect.left()) + kCursorStart, bg.top(), kCheckBoxAreaW, bg.height());
}

QRectF NTreeItemDelegate::chevronRect(const QStyleOptionViewItem& option) const {
    const QRectF bg = backgroundRect(option);
    qreal        x  = qreal(option.rect.left()) + kCursorStart;
    if (option.index.isValid() && (option.index.flags() & Qt::ItemIsUserCheckable))
        x += kCheckBoxAreaW + kGap;
    return QRectF(x, bg.top(), kChevronAreaW, bg.height());
}

void NTreeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                              const QModelIndex& index) const {
    if (!m_view || !m_data || !index.isValid())
        return;
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    if (opt.state.testFlag(QStyle::State_HasFocus))
        opt.state &= ~QStyle::State_HasFocus;
    const bool enabled  = opt.state.testFlag(QStyle::State_Enabled);
    const bool selected = opt.state.testFlag(QStyle::State_Selected);
    const bool hovered  = opt.state.testFlag(QStyle::State_MouseOver);
    const bool pressed  = m_data->pressedIndex.isValid() && m_data->pressedIndex == index;
    const QRectF bgRect = backgroundRect(opt);
    QColor background = Qt::transparent;
    if (!enabled) {
    } else if (selected && pressed) {
        background = m_data->itemPressedColor();
    } else if (selected) {
        background = m_data->itemSelectedColor();
    } else if (pressed) {
        background = m_data->itemPressedColor();
    } else if (hovered) {
        background = m_data->itemHoverColor();
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform
                            | QPainter::TextAntialiasing);
    if (background.alpha() > 0) {
        QPainterPath path;
        path.addRoundedRect(bgRect, m_data->itemBorderRadius(), m_data->itemBorderRadius());
        painter->fillPath(path, background);
    }
    qreal cursorX = qreal(opt.rect.left()) + kCursorStart;
    const bool checkable = index.flags() & Qt::ItemIsUserCheckable;
    if (checkable) {
        const QRectF       area = QRectF(cursorX, bgRect.top(), kCheckBoxAreaW, bgRect.height());
        const QVariant     checkValue = index.data(Qt::CheckStateRole);
        Qt::CheckState     checkState = Qt::Unchecked;
        if (checkValue.isValid())
            checkState = static_cast<Qt::CheckState>(checkValue.toInt());
        constexpr int size = 21;
        const QRect checkRect(qRound(area.center().x() - size / 2.0),
                              qRound(area.center().y() - size / 2.0),
                              size,
                              size);
        NCheckBoxIndicator::paint(painter, checkRect, checkState, enabled, hovered, m_data->isDark, 4);
        cursorX += kCheckBoxAreaW + kGap;
    }
    const qreal chevronLeft = cursorX;
    const bool  hasChildren = m_view->model() && m_view->model()->hasChildren(index);
    if (hasChildren) {
        const QRectF arrowArea(chevronLeft, bgRect.top(), kChevronAreaW, bgRect.height());
        const qreal  rotation = m_view->chevronRotation(index);
        const QIcon  icon =
            nIcon->fromRegular(NRegularIconType::ChevronRight12Regular, 12, m_data->textColor());
        painter->save();
        painter->translate(arrowArea.center());
        painter->rotate(rotation * 90.0);
        painter->translate(-arrowArea.center());
        icon.paint(painter, arrowArea.toRect(), Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
        painter->restore();
    }
    cursorX = chevronLeft + kChevronAreaW + kGap;
    if (!opt.icon.isNull()) {
        const QRect iconRect(qRound(cursorX),
                             qRound(bgRect.center().y() - kIconAreaW / 2.0),
                             qRound(kIconAreaW),
                             qRound(kIconAreaW));
        opt.icon.paint(painter, iconRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
        cursorX += kIconAreaW + kGap;
    }
    const QRectF textRect(cursorX, bgRect.top(), qMax<qreal>(0.0, bgRect.right() - cursorX - 8.0), bgRect.height());
    painter->setFont(bodyFont());
    painter->setPen(enabled ? m_data->textColor()
                            : NThemeColor(NFluentColorKey::TextFillColorDisabled, m_data->themeMode));
    const QString elided = painter->fontMetrics().elidedText(opt.text, Qt::ElideRight, qRound(textRect.width()));
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elided);
    painter->restore();
}

QSize NTreeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(0, m_data ? m_data->itemHeight() : 32);
}

QWidget* NTreeItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const {
    Q_UNUSED(option);
    if (!m_view)
        return QStyledItemDelegate::createEditor(parent, option, index);
    const int radius = m_data ? m_data->itemBorderRadius() : 4;
    QWidget*  editor = NItemEditor::createEditor(parent, index, radius);
    return editor ? editor : QStyledItemDelegate::createEditor(parent, option, index);
}

void NTreeItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    NItemEditor::setEditorData(editor, index);
}

void NTreeItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    NItemEditor::setModelData(editor, model, index);
}

void NTreeItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                             const QModelIndex& index) const {
    NItemEditor::placeEditor(editor, option.rect, NItemEditor::treeCellMargins(index));
}

bool NTreeItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                                    const QModelIndex& index) {
    if (!m_view || !model || !index.isValid() || index.column() != 0)
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    QStyleOptionViewItem opt(option);
    opt.index = index;
    if (event->type() == QEvent::MouseButtonRelease) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() != Qt::LeftButton)
            return QStyledItemDelegate::editorEvent(event, model, option, index);
        const QPointF pos = mouseEvent->position();
        if ((index.flags() & Qt::ItemIsUserCheckable) && checkBoxRect(opt).contains(pos)) {
            const QVariant value = index.data(Qt::CheckStateRole);
            const auto     state = static_cast<Qt::CheckState>(value.toInt());
            const Qt::CheckState next = state == Qt::Checked ? Qt::Unchecked : Qt::Checked;
            if (model->hasChildren(index))
                setCheckRecursive(model, index, next);
            else
                model->setData(index, next, Qt::CheckStateRole);
            updateAncestorChecks(model, index);
            return true;
        }
        if (model->hasChildren(index)) {
            m_view->toggleExpanded(index);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
