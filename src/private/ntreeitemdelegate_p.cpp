#include "ntreeitemdelegate_p.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QEvent>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QTreeView>

#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"
#include "ncheckboxindicator_p.h"
#include "nitemeditor_p.h"
#include "ntreeview_p.h"
#include "ntreecheckcontroller_p.h"

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

} // namespace

NTreeItemDelegate::NTreeItemDelegate(NTreeView* view, NTreeViewPrivate* data, QObject* parent)
    : QStyledItemDelegate(parent), m_view(view), m_data(data) {}

QRectF NTreeItemDelegate::backgroundRect(const QStyleOptionViewItem& option) const {
    const int vpWidth = (m_view && m_view->viewport()) ? m_view->viewport()->width() : option.rect.right();
    return QRectF(2, option.rect.top() + 2, vpWidth - 4, option.rect.height() - 4);
}

QRectF NTreeItemDelegate::checkBoxRect(const QStyleOptionViewItem& option) const {
    if (option.index.column() != 0)
        return {};
    if (!(option.features & QStyleOptionViewItem::HasCheckIndicator)
        && !(option.index.isValid() && (option.index.flags() & Qt::ItemIsUserCheckable)))
        return {};
    const QRectF bg = backgroundRect(option);
    return QRectF(qreal(option.rect.left()) + kCursorStart, bg.top(), kCheckBoxAreaW, bg.height());
}

QRectF NTreeItemDelegate::chevronRect(const QStyleOptionViewItem& option) const {
    if (option.index.column() != 0)
        return {};
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
    const bool hovered  = opt.state.testFlag(QStyle::State_MouseOver);
    const QRectF bgRect = backgroundRect(opt);
    painter->save();
    painter->setClipRect(opt.rect, Qt::IntersectClip);
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform
                            | QPainter::TextAntialiasing);
    qreal cursorX = qreal(opt.rect.left()) + kCursorStart;
    const bool treeColumn = index.column() == 0;
    const bool checkable = treeColumn && (index.flags() & Qt::ItemIsUserCheckable);
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
    const bool  hasChildren = treeColumn && m_view->model() && m_view->model()->hasChildren(index);
    if (hasChildren) {
        const QRectF arrowArea(chevronLeft, bgRect.top(), kChevronAreaW, bgRect.height());
        const qreal  rotation = m_view->chevronRotation(index);
        const QColor color = m_data->textColor();
        if (m_chevron.isNull() || m_chevronColor != color) {
            m_chevronColor = color;
            m_chevron = nIcon->fromRegular(NRegularIconType::ChevronRight12Regular, 12, color);
        }
        painter->save();
        painter->translate(arrowArea.center());
        painter->rotate(rotation * 90.0);
        painter->translate(-arrowArea.center());
        m_chevron.paint(painter, arrowArea.toRect(), Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
        painter->restore();
    }
    if (treeColumn)
        cursorX = chevronLeft + kChevronAreaW + kGap;
    if (!opt.icon.isNull()) {
        const QRect iconRect(qRound(cursorX),
                             qRound(bgRect.center().y() - kIconAreaW / 2.0),
                             qRound(kIconAreaW),
                             qRound(kIconAreaW));
        opt.icon.paint(painter, iconRect, Qt::AlignCenter, enabled ? QIcon::Normal : QIcon::Disabled);
        cursorX += kIconAreaW + kGap;
    }
    const qreal textRight = qMin(qreal(opt.rect.right()), bgRect.right()) - 8.0;
    const QRectF textRect(cursorX, bgRect.top(), qMax<qreal>(0.0, textRight - cursorX), bgRect.height());
    painter->setFont(bodyFont());
    painter->setPen(enabled ? m_data->textColor()
                            : NThemeColor(NFluentColorKey::TextFillColorDisabled, m_data->themeMode));
    const QString elided = painter->fontMetrics().elidedText(opt.text, Qt::ElideRight, qRound(textRect.width()));
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, elided);
    painter->restore();
}

QSize NTreeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    Q_UNUSED(option);
    int width = QFontMetrics(bodyFont()).horizontalAdvance(index.data().toString()) + int(kCursorStart + 8);
    if (index.column() == 0) {
        width += int(kChevronAreaW + kGap);
        if (index.flags() & Qt::ItemIsUserCheckable)
            width += int(kCheckBoxAreaW + kGap);
    }
    if (!index.data(Qt::DecorationRole).isNull())
        width += int(kIconAreaW + kGap);
    return QSize(width, m_data ? m_data->itemHeight() : 32);
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
    if (!m_view || !model || !index.isValid())
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    if (index.column() != 0) {
        if (event->type() == QEvent::MouseButtonRelease && m_data->usesCheckState(index)
            && static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
            m_data->checks->toggle(index.siblingAtColumn(0));
            return true;
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
    if (!(index.flags() & Qt::ItemIsEnabled))
        return false;
    QStyleOptionViewItem opt(option);
    opt.index = index;
    if (event->type() == QEvent::MouseButtonRelease) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() != Qt::LeftButton)
            return QStyledItemDelegate::editorEvent(event, model, option, index);
        const QPointF pos = mouseEvent->position();
        const bool chevronHit = model->hasChildren(index) && chevronRect(opt).contains(pos);
        if ((index.flags() & Qt::ItemIsUserCheckable)
            && (checkBoxRect(opt).contains(pos) || (m_data->usesCheckState(index) && !chevronHit))) {
            m_data->checks->toggle(index);
            return true;
        }
        if (model->hasChildren(index) && (chevronHit || !m_data->usesCheckState(index))) {
            m_view->toggleExpanded(index);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
