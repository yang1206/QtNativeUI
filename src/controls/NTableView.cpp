#include "QtNativeUI/NTableView.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QBrush>
#include <QCursor>
#include <QFrame>
#include <QHeaderView>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QStyledItemDelegate>
#include <QTimer>

#include "../private/naccessible_p.h"
#include "../private/ntableview_p.h"
#include "QtNativeUI/NFluentColors.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightHeaderBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkHeaderBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightHeaderHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkHeaderHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightHeaderPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkHeaderPressedColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkItemHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkItemSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightHeaderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkHeaderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightPlaceholderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkPlaceholderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightDividerColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkDividerColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTableView, QString, PlaceholderText)
Q_PROPERTY_CREATE_Q_CPP(NTableView, bool, BorderVisible)
Q_PROPERTY_CREATE_Q_CPP(NTableView, bool, BackgroundVisible)

namespace {

constexpr int kScrollBarInset = 4;

void paintRowSelectionIndicator(QPainter* painter, const QRect& itemRect, const QColor& accentColor, bool rtl) {
    if (!painter || !itemRect.isValid())
        return;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    const int heightOffset = itemRect.height() / 4;
    const qreal x          = rtl ? itemRect.right() - 6.0 : itemRect.left() + 3.0;
    painter->setPen(Qt::NoPen);
    painter->setBrush(accentColor);
    painter->drawRoundedRect(
        QRectF(x, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 2, 2);
    painter->restore();
}

void suppressNativeScrollBar(QScrollBar* bar) {
    if (!bar)
        return;
    bar->setAttribute(Qt::WA_DontShowOnScreen, true);
    bar->hide();
}

NScrollBar*
createOverlayScrollBar(Qt::Orientation orientation, QWidget* host, QScrollBar* native, const QString& objectName) {
    auto* bar = new NScrollBar(orientation, host);
    bar->setObjectName(objectName);
    QObject::connect(native, &QScrollBar::valueChanged, bar, &QScrollBar::setValue);
    QObject::connect(bar, &QScrollBar::valueChanged, native, &QScrollBar::setValue);
    return bar;
}

bool mirrorNativeScrollBar(NScrollBar* overlay, const QScrollBar* native) {
    if (!overlay || !native)
        return false;
    overlay->setRange(native->minimum(), native->maximum());
    overlay->setPageStep(native->pageStep());
    overlay->setSingleStep(native->singleStep());
    const bool needScroll = native->maximum() > native->minimum();
    overlay->setVisible(needScroll);
    return needScroll;
}

void placeVerticalOverlayScrollBar(NScrollBar* bar,
                                 const QRect& hostRect,
                                 const QRect& viewportRect,
                                 int cornerRadius,
                                 bool reserveHorizontalTrack) {
    if (!bar)
        return;
    const int thickness = qMax(bar->sizeHint().width(), 8);
    const int horizontalReserve =
        reserveHorizontalTrack ? qMax(bar->sizeHint().height(), 8) + kScrollBarInset : 0;
    const int bottomInset = kScrollBarInset + cornerRadius + horizontalReserve;
    const int height      = qMax(8, viewportRect.height() - kScrollBarInset - bottomInset);
    bar->setGeometry(hostRect.right() - thickness - kScrollBarInset,
                     viewportRect.top() + kScrollBarInset,
                     thickness,
                     height);
}

qreal clampRadius(qreal radius, const QRectF& rect) {
    if (radius <= 0)
        return 0;
    return qMin(radius, qMin(rect.width(), rect.height()) / 2.0);
}

QPainterPath topRoundedRectPath(const QRectF& rect, qreal radius) {
    const qreal r = clampRadius(radius, rect);
    if (r <= 0) {
        QPainterPath path;
        path.addRect(rect);
        return path;
    }
    QPainterPath path;
    path.moveTo(rect.left(), rect.bottom());
    path.lineTo(rect.left(), rect.top() + r);
    path.arcTo(rect.left(), rect.top(), 2 * r, 2 * r, 180, -90);
    path.lineTo(rect.right() - r, rect.top());
    path.arcTo(rect.right() - 2 * r, rect.top(), 2 * r, 2 * r, 90, -90);
    path.lineTo(rect.right(), rect.bottom());
    path.closeSubpath();
    return path;
}

QPainterPath leftRoundedRectPath(const QRectF& rect, qreal radius) {
    const qreal r = clampRadius(radius, rect);
    if (r <= 0) {
        QPainterPath path;
        path.addRect(rect);
        return path;
    }
    QPainterPath path;
    path.moveTo(rect.right(), rect.top());
    path.lineTo(rect.left() + r, rect.top());
    path.arcTo(rect.left(), rect.top(), 2 * r, 2 * r, 90, 90);
    path.lineTo(rect.left(), rect.bottom() - r);
    path.arcTo(rect.left(), rect.bottom() - 2 * r, 2 * r, 2 * r, 180, 90);
    path.lineTo(rect.right(), rect.bottom());
    path.closeSubpath();
    return path;
}

QPainterPath tableRoundedClipInWidget(const NTableView* table, const QWidget* widget) {
    if (!table || !widget)
        return {};
    if (!table->getBackgroundVisible() && !table->getBorderVisible())
        return {};
    const int radius = table->getBorderRadius();
    if (radius <= 0)
        return {};
    const QPoint origin = widget->mapFrom(const_cast<NTableView*>(table), QPoint(0, 0));
    QPainterPath path;
    path.addRoundedRect(QRectF(origin, table->size()), radius, radius);
    return path;
}

void intersectTableRoundedClip(QPainter* painter, const NTableView* table, const QWidget* widget) {
    if (!painter || !table || !widget)
        return;
    const QPainterPath clip = tableRoundedClipInWidget(table, widget);
    if (!clip.isEmpty())
        painter->setClipPath(clip, Qt::IntersectClip);
}

void placeHorizontalOverlayScrollBar(NScrollBar* bar,
                                     const QRect& hostRect,
                                     const QRect& viewportRect,
                                     int cornerRadius,
                                     bool reserveVerticalTrack) {
    if (!bar)
        return;
    const int thickness = qMax(bar->sizeHint().height(), 8);
    const int verticalReserve =
        reserveVerticalTrack ? qMax(bar->sizeHint().width(), 8) + kScrollBarInset : 0;
    const int width =
        qMax(8, viewportRect.width() - 2 * kScrollBarInset - cornerRadius - verticalReserve);
    bar->setGeometry(viewportRect.left() + kScrollBarInset,
                     hostRect.bottom() - thickness - kScrollBarInset,
                     width,
                     thickness);
}

class NTableViewHeaderView final : public QHeaderView {
  public:
    NTableViewHeaderView(Qt::Orientation orientation, NTableView* view, NTableViewPrivate* d)
        : QHeaderView(orientation, view), m_view(view), m_d(d) {
        setMouseTracking(true);
        viewport()->setMouseTracking(true);
        viewport()->setAutoFillBackground(false);
        setHighlightSections(false);
    }

  protected:
    void paintEvent(QPaintEvent* event) override {
        if (!m_view || !model())
            return;
        QPainter painter(viewport());
        const int radius = m_view->getBorderRadius();
        if (radius > 0) {
            painter.setRenderHint(QPainter::Antialiasing);
            const QRectF area(viewport()->rect());
            if (orientation() == Qt::Horizontal)
                painter.setClipPath(topRoundedRectPath(area, radius));
            else if (m_view->isVerticalHeaderVisible())
                painter.setClipPath(leftRoundedRectPath(area, radius));
        }
        const QRect clip = event->rect();
        for (int visual = 0; visual < count(); ++visual) {
            const int logical = logicalIndex(visual);
            if (logical < 0 || isSectionHidden(logical))
                continue;
            const int   pos  = sectionViewportPosition(logical);
            const int   size = sectionSize(logical);
            const QRect sectionRect =
                orientation() == Qt::Horizontal ? QRect(pos, 0, size, height()) : QRect(0, pos, width(), size);
            if (!sectionRect.intersects(clip))
                continue;
            paintSection(&painter, sectionRect, logical);
        }
    }

    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override {
        if (!painter || !rect.isValid() || !m_view || !model())
            return;
        const QPoint pointer = viewport()->mapFromGlobal(QCursor::pos());
        const bool   hovered = isEnabled() && underMouse() && rect.contains(pointer);
        const bool   pressed = hovered && QApplication::mouseButtons().testFlag(Qt::LeftButton);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->fillRect(rect, m_d->headerBackgroundColor());
        if (pressed)
            painter->fillRect(rect, m_d->headerPressedColor());
        else if (hovered)
            painter->fillRect(rect, m_d->headerHoverColor());
        const bool horizontal = orientation() == Qt::Horizontal;
        painter->setPen(QPen(m_d->dividerColor(), 1.0));
        if (horizontal) {
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            if (logicalIndex != count() - 1)
                painter->drawLine(rect.right(), rect.top() + 10, rect.right(), rect.bottom() - 10);
        } else {
            painter->drawLine(rect.topRight(), rect.bottomRight());
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        }
        QColor         foreground     = isEnabled() ? m_d->headerTextColor()
                                                    : NThemeColor(NFluentColorKey::TextFillColorDisabled,
                                                      m_d->isDark ? NThemeType::Dark : NThemeType::Light);
        const QVariant foregroundRole = model()->headerData(logicalIndex, orientation(), Qt::ForegroundRole);
        if (foregroundRole.canConvert<QBrush>()) {
            const QBrush brush = qvariant_cast<QBrush>(foregroundRole);
            if (brush.style() != Qt::NoBrush)
                foreground = brush.color();
        }
        painter->setPen(foreground);
        QFont          headerFont = font();
        const QVariant fontRole   = model()->headerData(logicalIndex, orientation(), Qt::FontRole);
        if (fontRole.canConvert<QFont>())
            headerFont = qvariant_cast<QFont>(fontRole);
        painter->setFont(headerFont);
        Qt::Alignment  alignment     = horizontal ? Qt::AlignLeft | Qt::AlignVCenter : Qt::AlignCenter;
        const QVariant alignmentRole = model()->headerData(logicalIndex, orientation(), Qt::TextAlignmentRole);
        if (alignmentRole.isValid())
            alignment = static_cast<Qt::Alignment>(alignmentRole.toInt());
        const bool         sorted   = horizontal && isSortIndicatorShown() && sortIndicatorSection() == logicalIndex;
        const int          pad      = m_view->getCellHorizontalPadding();
        QRect              textRect = rect.adjusted(horizontal ? pad : 8, 0, horizontal ? -(sorted ? 32 : pad) : -8, 0);
        QString text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();
        if (!horizontal && text.isEmpty())
            text = QString::number(logicalIndex + 1);
        const QFontMetrics metrics(headerFont);
        painter->drawText(textRect, alignment, metrics.elidedText(text, Qt::ElideRight, textRect.width()));
        if (sorted) {
            const bool   rtl       = layoutDirection() == Qt::RightToLeft;
            const qreal  centerX   = rtl ? rect.left() + 16 : rect.right() - 16;
            const qreal  centerY   = rect.center().y() + 0.5;
            const qreal  direction = sortIndicatorOrder() == Qt::AscendingOrder ? -1.0 : 1.0;
            QPainterPath chevron;
            chevron.moveTo(centerX - 3.5, centerY - direction * 1.5);
            chevron.lineTo(centerX, centerY + direction * 2.0);
            chevron.lineTo(centerX + 3.5, centerY - direction * 1.5);
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(foreground, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawPath(chevron);
        }
        painter->restore();
    }

  private:
    NTableView*        m_view = nullptr;
    NTableViewPrivate* m_d    = nullptr;
};

class NTableViewChromeUnderlay final : public QWidget {
  public:
    explicit NTableViewChromeUnderlay(NTableView* view, NTableViewPrivate* d) : QWidget(view), m_view(view), m_d(d) {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
        setAutoFillBackground(false);
        setFocusPolicy(Qt::NoFocus);
    }

  protected:
    void paintEvent(QPaintEvent*) override {
        if (!m_view || !m_d || !m_view->getBackgroundVisible())
            return;
        QPainter painter(this);
        if (!painter.isActive())
            return;
        painter.setRenderHint(QPainter::Antialiasing);
        const QColor bg     = m_d->backgroundColor();
        const QRectF host   = QRectF(rect());
        const int    radius = m_view->getBorderRadius();
        painter.setPen(Qt::NoPen);
        painter.setBrush(bg);
        if (radius > 0)
            painter.drawRoundedRect(host, radius, radius);
        else
            painter.fillRect(host, bg);
    }

  private:
    NTableView*        m_view = nullptr;
    NTableViewPrivate* m_d    = nullptr;
};

class NTableViewFrameOverlay final : public QWidget {
  public:
    explicit NTableViewFrameOverlay(NTableView* view, NTableViewPrivate* d) : QWidget(view), m_view(view), m_d(d) {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
        setAutoFillBackground(false);
        setFocusPolicy(Qt::NoFocus);
    }

  protected:
    void paintEvent(QPaintEvent*) override {
        if (!m_view || !m_view->getBorderVisible())
            return;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const int    radius = m_view->getBorderRadius();
        const QRectF bounds = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(m_d->borderColor(), 1.0));
        painter.drawRoundedRect(bounds, radius, radius);
    }

  private:
    NTableView*        m_view = nullptr;
    NTableViewPrivate* m_d    = nullptr;
};

class NTableViewCellDelegate final : public QStyledItemDelegate {
  public:
    explicit NTableViewCellDelegate(NTableView* view, NTableViewPrivate* d)
        : QStyledItemDelegate(view), m_view(view), m_d(d) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (!painter || !m_view || !index.isValid())
            return;
        const bool enabled     = option.state & QStyle::State_Enabled;
        const bool selected    = option.state & QStyle::State_Selected;
        const bool rowSelected = m_view->selectionModel() &&
                                 m_view->selectionBehavior() == QAbstractItemView::SelectRows &&
                                 m_view->selectionModel()->isRowSelected(index.row(), index.parent());
        const bool     hovered        = enabled && m_d->hoveredRow == index.row();
        const bool     current        = index == m_view->currentIndex();
        QColor         background     = m_view->getBackgroundVisible() ? m_d->backgroundColor() : Qt::transparent;
        const QVariant backgroundRole = index.data(Qt::BackgroundRole);
        if (backgroundRole.canConvert<QBrush>()) {
            const QBrush brush = qvariant_cast<QBrush>(backgroundRole);
            if (brush.style() != Qt::NoBrush)
                background = brush.color();
        }
        QColor         foreground     = enabled ? m_d->textColor()
                                                : NThemeColor(NFluentColorKey::TextFillColorDisabled,
                                                  m_d->isDark ? NThemeType::Dark : NThemeType::Light);
        const QVariant foregroundRole = index.data(Qt::ForegroundRole);
        if (foregroundRole.canConvert<QBrush>()) {
            const QBrush brush = qvariant_cast<QBrush>(foregroundRole);
            if (brush.style() != Qt::NoBrush)
                foreground = brush.color();
        }
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        if (m_view->viewport())
            intersectTableRoundedClip(painter, m_view, m_view->viewport());
        painter->fillRect(option.rect, background);
        if (enabled && (selected || rowSelected))
            painter->fillRect(option.rect, m_d->itemSelectedColor());
        else if (hovered)
            painter->fillRect(option.rect, m_d->itemHoverColor());
        painter->setPen(QPen(m_d->dividerColor(), 1.0));
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        QStyleOptionViewItem content = option;
        initStyleOption(&content, index);
        content.state &= ~(QStyle::State_Selected | QStyle::State_MouseOver | QStyle::State_HasFocus);
        content.backgroundBrush        = Qt::NoBrush;
        content.showDecorationSelected = false;
        content.palette.setColor(QPalette::Text, foreground);
        content.palette.setColor(QPalette::WindowText, foreground);
        const int pad = m_view->getCellHorizontalPadding();
        content.rect.adjust(pad, 0, -pad, 0);
        content.font              = m_view->font();
        const QVariant decoration = index.data(Qt::DecorationRole);
        const QVariant checkState = index.data(Qt::CheckStateRole);
        if (decoration.isValid() || checkState.isValid()) {
            QStyledItemDelegate::paint(painter, content, index);
        } else {
            painter->setFont(content.font);
            painter->setPen(foreground);
            painter->drawText(content.rect, Qt::AlignVCenter | Qt::AlignLeft, index.data(Qt::DisplayRole).toString());
        }
        if (rowSelected && isFirstVisibleColumn(index.column())) {
            paintRowSelectionIndicator(
                painter, option.rect, nTheme->accentColor().normal(), m_view->layoutDirection() == Qt::RightToLeft);
        }
        const bool showFocus =
            current && m_view->hasFocus() && enabled &&
            (m_view->selectionBehavior() == QAbstractItemView::SelectItems ||
             (m_view->selectionBehavior() == QAbstractItemView::SelectRows && isFirstVisibleColumn(index.column())));
        if (showFocus) {
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(nTheme->accentColor().normal(), 1.5));
            const int radius = qMax(2, m_view->getBorderRadius() - 1);
            painter->drawRoundedRect(QRectF(option.rect).adjusted(1.5, 1.5, -1.5, -1.5), radius, radius);
        }
        painter->restore();
    }

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (!m_view || !index.isValid())
            return QStyledItemDelegate::createEditor(parent, option, index);
        auto* editor = new NLineEdit(parent);
        editor->setBorderWidth(1);
        editor->setBorderRadius(qMax(4, m_view->getBorderRadius() / 2));
        editor->setText(index.data(Qt::EditRole).toString());
        return editor;
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        auto* line = qobject_cast<NLineEdit*>(editor);
        if (line)
            line->setText(index.data(Qt::EditRole).toString());
        else
            QStyledItemDelegate::setEditorData(editor, index);
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
        auto* line = qobject_cast<NLineEdit*>(editor);
        if (line)
            model->setData(index, line->text(), Qt::EditRole);
        else
            QStyledItemDelegate::setModelData(editor, model, index);
    }

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (!m_view) {
            QStyledItemDelegate::updateEditorGeometry(editor, option, index);
            return;
        }
        const int pad = m_view->getCellHorizontalPadding();
        editor->setGeometry(option.rect.adjusted(pad - 4, 3, -pad + 4, -3));
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QSize hint = QStyledItemDelegate::sizeHint(option, index);
        hint.setHeight(qMax(hint.height(), m_view->getRowHeight()));
        return hint;
    }

  private:
    bool isFirstVisibleColumn(int logicalIndex) const {
        if (!m_view || !m_view->horizontalHeader())
            return false;
        const QHeaderView* header = m_view->horizontalHeader();
        for (int visualIndex = 0; visualIndex < header->count(); ++visualIndex) {
            const int candidate = header->logicalIndex(visualIndex);
            if (!m_view->isColumnHidden(candidate))
                return candidate == logicalIndex;
        }
        return false;
    }
    NTableView*        m_view = nullptr;
    NTableViewPrivate* m_d    = nullptr;
};

} // namespace

void NTableView::setRowHeight(int height) {
    Q_D(NTableView);
    if (d->_pRowHeight == height)
        return;
    d->_pRowHeight = height;
    verticalHeader()->setDefaultSectionSize(height);
    doItemsLayout();
    Q_EMIT pRowHeightChanged();
}

int NTableView::getRowHeight() const { return d_ptr->_pRowHeight; }

void NTableView::setHeaderHeight(int height) {
    Q_D(NTableView);
    if (d->_pHeaderHeight == height)
        return;
    d->_pHeaderHeight = height;
    horizontalHeader()->setFixedHeight(height);
    Q_EMIT pHeaderHeightChanged();
}

int NTableView::getHeaderHeight() const { return d_ptr->_pHeaderHeight; }

void NTableView::setBorderRadius(int radius) {
    Q_D(NTableView);
    if (d->_pBorderRadius == radius)
        return;
    d->_pBorderRadius = radius;
    updateRoundedMask();
    syncOverlayScrollBars();
    Q_EMIT pBorderRadiusChanged();
}

int NTableView::getBorderRadius() const { return d_ptr->_pBorderRadius; }

void NTableView::setCellHorizontalPadding(int padding) {
    Q_D(NTableView);
    if (d->_pCellHorizontalPadding == padding)
        return;
    d->_pCellHorizontalPadding = padding;
    viewport()->update();
    Q_EMIT pCellHorizontalPaddingChanged();
}

int NTableView::getCellHorizontalPadding() const { return d_ptr->_pCellHorizontalPadding; }

NTableView::NTableView(QWidget* parent) : QTableView(parent), d_ptr(new NTableViewPrivate()) {
    Q_D(NTableView);
    d->q_ptr = this;
    init();
}

NTableView::~NTableView() {
    Q_D(NTableView);
    for (const QMetaObject::Connection& connection : d->modelConnections)
        disconnect(connection);
}

void NTableView::init() {
    Q_D(NTableView);
    setObjectName(QStringLiteral("NTableView"));
    d->isDark    = nTheme->isDarkMode();
    d->themeMode = nTheme->themeMode();

    d->_pLightBackgroundColor       = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor        = NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Dark);
    d->_pLightHeaderBackgroundColor = NThemeColor(NFluentColorKey::LayerFillColorAlt, NThemeType::Light);
    d->_pDarkHeaderBackgroundColor  = NThemeColor(NFluentColorKey::LayerFillColorAlt, NThemeType::Dark);
    d->_pLightHeaderHoverColor      = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkHeaderHoverColor       = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightHeaderPressedColor    = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkHeaderPressedColor     = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightItemHoverColor        = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHoverColor         = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightItemSelectedColor     = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkItemSelectedColor      = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightTextColor             = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor              = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightHeaderTextColor       = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkHeaderTextColor        = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightPlaceholderTextColor  = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkPlaceholderTextColor   = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightDividerColor          = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pDarkDividerColor           = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);
    d->_pLightBorderColor           = NThemeColor(NFluentColorKey::CardStrokeColorDefaultSolid, NThemeType::Light);
    d->_pDarkBorderColor            = NThemeColor(NFluentColorKey::CardStrokeColorDefaultSolid, NThemeType::Dark);
    d->_pRowHeight                  = 36;
    d->_pHeaderHeight               = 36;
    d->_pBorderRadius               = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pCellHorizontalPadding      = 12;
    d->_pBorderVisible              = true;
    d->_pBackgroundVisible          = true;

    setFocusPolicy(Qt::StrongFocus);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setHorizontalHeader(new NTableViewHeaderView(Qt::Horizontal, this, d));
    setVerticalHeader(new NTableViewHeaderView(Qt::Vertical, this, d));
    horizontalHeader()->setAutoFillBackground(false);
    horizontalHeader()->viewport()->setAutoFillBackground(false);
    verticalHeader()->setAutoFillBackground(false);
    verticalHeader()->viewport()->setAutoFillBackground(false);
    setFrameShape(QFrame::NoFrame);
    setAlternatingRowColors(false);
    setCornerButtonEnabled(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSortingEnabled(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    QTableView::setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    viewport()->setAutoFillBackground(false);
    setShowGrid(false);
    setWordWrap(false);
    horizontalHeader()->setDefaultSectionSize(120);
    horizontalHeader()->setMinimumSectionSize(56);
    horizontalHeader()->setFixedHeight(d->_pHeaderHeight);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setSectionsClickable(true);
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setDefaultSectionSize(d->_pRowHeight);
    verticalHeader()->setMinimumSectionSize(32);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setFixedWidth(48);
    verticalHeader()->hide();
    QTableView::setItemDelegate(new NTableViewCellDelegate(this, d));
    suppressNativeScrollBar(verticalScrollBar());
    suppressNativeScrollBar(horizontalScrollBar());
    d->overlayVerticalScrollBar =
        createOverlayScrollBar(Qt::Vertical, this, verticalScrollBar(), QStringLiteral("NTableViewVerticalScrollBar"));
    d->overlayHorizontalScrollBar = createOverlayScrollBar(
        Qt::Horizontal, this, horizontalScrollBar(), QStringLiteral("NTableViewHorizontalScrollBar"));
    d->overlayVerticalScrollBar->setAttribute(Qt::WA_TranslucentBackground);
    d->overlayHorizontalScrollBar->setAttribute(Qt::WA_TranslucentBackground);
    d->overlayVerticalScrollBar->setAutoFillBackground(false);
    d->overlayHorizontalScrollBar->setAutoFillBackground(false);
    connect(verticalScrollBar(), &QScrollBar::rangeChanged, this, [this]() { syncOverlayScrollBars(); });
    connect(horizontalScrollBar(), &QScrollBar::rangeChanged, this, [this]() { syncOverlayScrollBars(); });
    d->chromeUnderlay = new NTableViewChromeUnderlay(this, d);
    d->chromeUnderlay->setGeometry(rect());
    d->chromeUnderlay->lower();
    d->borderOverlay = new NTableViewFrameOverlay(this, d);
    setStyleSheet(QStringLiteral(
        "NTableView { background: transparent; border: none; outline: none; gridline-color: transparent; }"
        "NTableView::item { background: transparent; border: none; padding: 0px; }"
        "NTableView::item:selected { background: transparent; }"
        "NTableView::item:focus { outline: none; }"
        "QHeaderView::section { background: transparent; border: none; padding: 0px; margin: 0px; }"));
    applyThemePalette();
    d->updatePalette();
    syncOverlayScrollBars();

    connect(this, &NTableView::pPlaceholderTextChanged, this, [this]() {
        updateAutomaticAccessibleDescription();
        if (viewport())
            viewport()->update();
    });
    connect(this, &NTableView::pBorderVisibleChanged, this, [this]() {
        Q_D(NTableView);
        if (d->borderOverlay) {
            d->borderOverlay->setVisible(getBorderVisible());
            d->borderOverlay->update();
        }
        updateRoundedMask();
    });
    connect(this, &NTableView::pBackgroundVisibleChanged, this, [this]() {
        Q_D(NTableView);
        applyThemePalette();
        if (d->chromeUnderlay) {
            d->chromeUnderlay->setVisible(getBackgroundVisible());
            d->chromeUnderlay->update();
        }
        updateRoundedMask();
        if (viewport())
            viewport()->update();
    });
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NTableView);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        applyThemePalette();
        d->updatePalette();
        if (d->chromeUnderlay)
            d->chromeUnderlay->update();
        if (d->borderOverlay)
            d->borderOverlay->update();
    });
    connect(nTheme, &NTheme::accentColorChanged, this, [this]() {
        if (viewport())
            viewport()->update();
    });
    updateRoundedMask();
}

void NTableView::updateRoundedMask() {
    setAttribute(Qt::WA_TranslucentBackground, false);
    clearMask();
    if (viewport())
        viewport()->clearMask();
    if (horizontalHeader() && horizontalHeader()->viewport())
        horizontalHeader()->viewport()->clearMask();
    if (verticalHeader() && verticalHeader()->viewport())
        verticalHeader()->viewport()->clearMask();
    Q_D(NTableView);
    if (d->chromeUnderlay) {
        d->chromeUnderlay->lower();
        d->chromeUnderlay->update();
    }
    if (d->borderOverlay)
        d->borderOverlay->update();
}

void NTableView::setModel(QAbstractItemModel* model) {
    Q_D(NTableView);
    if (QTableView::model() == model)
        return;
    for (const QMetaObject::Connection& connection : d->modelConnections)
        disconnect(connection);
    d->modelConnections.clear();
    QTableView::setModel(model);
    setHoveredRow(-1);
    if (model)
        setSortingEnabled(true);
    connectModelSignals(model);
    refreshModelPresentation();
}

void NTableView::setSelectionModel(QItemSelectionModel* selectionModel) {
    QTableView::setSelectionModel(selectionModel);
    if (viewport())
        viewport()->update();
}

bool NTableView::isShowingPlaceholder() const {
    return !getPlaceholderText().isEmpty() && (!model() || model()->rowCount() == 0 || model()->columnCount() == 0);
}

void NTableView::setVerticalHeaderVisible(bool visible) {
    QHeaderView* vertical = verticalHeader();
    if (!vertical)
        return;
    vertical->setVisible(visible);
    if (visible) {
        vertical->setFixedWidth(48);
        vertical->viewport()->update();
    }
    if (horizontalHeader())
        horizontalHeader()->viewport()->update();
    updateRoundedMask();
    if (viewport())
        viewport()->update();
}

bool NTableView::isVerticalHeaderVisible() const {
    return verticalHeader() && verticalHeader()->isVisible();
}

void NTableView::keyPressEvent(QKeyEvent* event) {
    if (event && event->key() == Qt::Key_F2 && editTriggers().testFlag(QAbstractItemView::EditKeyPressed) &&
        currentIndex().isValid() && edit(currentIndex(), QAbstractItemView::EditKeyPressed, event)) {
        event->accept();
        return;
    }
    QTableView::keyPressEvent(event);
}

void NTableView::leaveEvent(QEvent* event) {
    setHoveredRow(-1);
    QTableView::leaveEvent(event);
}

void NTableView::mouseMoveEvent(QMouseEvent* event) {
    if (event)
        setHoveredRow(indexAt(event->position().toPoint()).row());
    QTableView::mouseMoveEvent(event);
}

void NTableView::paintEvent(QPaintEvent* event) {
    Q_D(NTableView);
    QTableView::paintEvent(event);
    if (isShowingPlaceholder() && viewport()) {
        QPainter placeholder(viewport());
        if (placeholder.isActive()) {
            placeholder.setRenderHint(QPainter::Antialiasing);
            placeholder.setPen(d->placeholderTextColor());
            placeholder.drawText(
                viewport()->rect().adjusted(32, 32, -32, -32), Qt::AlignCenter | Qt::TextWordWrap, getPlaceholderText());
        }
    }
}

void NTableView::resizeEvent(QResizeEvent* event) {
    Q_D(NTableView);
    QTableView::resizeEvent(event);
    if (d->chromeUnderlay) {
        d->chromeUnderlay->setGeometry(rect());
        d->chromeUnderlay->lower();
        d->chromeUnderlay->update();
    }
    if (d->borderOverlay) {
        d->borderOverlay->setGeometry(rect());
        d->borderOverlay->raise();
    }
    updateRoundedMask();
    syncOverlayScrollBars();
}

void NTableView::showEvent(QShowEvent* event) {
    QTableView::showEvent(event);
    QTimer::singleShot(0, this, [this]() { syncOverlayScrollBars(); });
}

void NTableView::wheelEvent(QWheelEvent* event) {
    QTableView::wheelEvent(event);
    syncOverlayScrollBars();
}

void NTableView::scrollContentsBy(int dx, int dy) {
    QTableView::scrollContentsBy(dx, dy);
    syncOverlayScrollBars();
}

void NTableView::applyThemePalette() {
    Q_D(NTableView);
    const QColor transparent = Qt::transparent;
    const QColor base        = getBackgroundVisible() ? transparent : d->backgroundColor();
    QPalette     viewPalette = palette();
    viewPalette.setColor(QPalette::Base, base);
    viewPalette.setColor(QPalette::AlternateBase, base);
    viewPalette.setColor(QPalette::Window, base);
    viewPalette.setColor(QPalette::Text, d->textColor());
    viewPalette.setColor(QPalette::WindowText, d->textColor());
    viewPalette.setColor(QPalette::Highlight, d->itemSelectedColor());
    viewPalette.setColor(QPalette::HighlightedText, d->textColor());
    viewPalette.setColor(
        QPalette::Disabled,
        QPalette::Text,
        NThemeColor(NFluentColorKey::TextFillColorDisabled, d->isDark ? NThemeType::Dark : NThemeType::Light));
    viewPalette.setColor(
        QPalette::Disabled,
        QPalette::WindowText,
        NThemeColor(NFluentColorKey::TextFillColorDisabled, d->isDark ? NThemeType::Dark : NThemeType::Light));
    setPalette(viewPalette);
    viewport()->setPalette(viewPalette);
    QPalette headerPalette = viewPalette;
    headerPalette.setColor(QPalette::Button, d->headerBackgroundColor());
    headerPalette.setColor(QPalette::ButtonText, d->headerTextColor());
    horizontalHeader()->setPalette(headerPalette);
    verticalHeader()->setPalette(headerPalette);
}

void NTableView::connectModelSignals(QAbstractItemModel* model) {
    Q_D(NTableView);
    if (!model)
        return;
    const auto refresh = [this]() { refreshModelPresentation(); };
    d->modelConnections.append(connect(model, &QAbstractItemModel::modelReset, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::layoutChanged, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsInserted, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::rowsRemoved, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsInserted, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::columnsRemoved, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::dataChanged, this, refresh));
    d->modelConnections.append(connect(model, &QAbstractItemModel::headerDataChanged, this, refresh));
    d->modelConnections.append(connect(model, &QObject::destroyed, this, refresh));
}

void NTableView::refreshModelPresentation() {
    updateAutomaticAccessibleDescription();
    if (viewport())
        viewport()->update();
    if (horizontalHeader())
        horizontalHeader()->viewport()->update();
    if (verticalHeader())
        verticalHeader()->viewport()->update();
    QTimer::singleShot(0, this, [this]() { syncOverlayScrollBars(); });
}

void NTableView::setHoveredRow(int row) {
    Q_D(NTableView);
    if (d->hoveredRow == row)
        return;
    const int previous = d->hoveredRow;
    d->hoveredRow      = row;
    if (!viewport())
        return;
    QRect      dirty;
    const auto includeRow = [this, &dirty](int candidate) {
        if (!model() || candidate < 0 || candidate >= model()->rowCount())
            return;
        const QRect rowRect(0, rowViewportPosition(candidate), viewport()->width(), rowHeight(candidate));
        dirty = dirty.isNull() ? rowRect : dirty.united(rowRect);
    };
    includeRow(previous);
    includeRow(d->hoveredRow);
    if (!dirty.isNull())
        viewport()->update(dirty.adjusted(0, -1, 0, 1));
}

void NTableView::updateAutomaticAccessibleDescription() {
    Q_D(NTableView);
    const QString automatic = isShowingPlaceholder() ? getPlaceholderText() : QString();
    NA11y::applyAutomaticAccessibleDescription(this, d->automaticAccessibleDescription, automatic);
}

void NTableView::syncOverlayScrollBars() {
    Q_D(NTableView);
    if (!d->overlayVerticalScrollBar || !d->overlayHorizontalScrollBar || !viewport())
        return;
    const QRect viewportRect = viewport()->geometry();
    const int   radius       = getBorderRadius();
    const bool  verticalVisible =
        mirrorNativeScrollBar(d->overlayVerticalScrollBar, verticalScrollBar());
    const bool horizontalVisible =
        mirrorNativeScrollBar(d->overlayHorizontalScrollBar, horizontalScrollBar());
    if (verticalVisible)
        placeVerticalOverlayScrollBar(
            d->overlayVerticalScrollBar, rect(), viewportRect, radius, horizontalVisible);
    if (horizontalVisible)
        placeHorizontalOverlayScrollBar(
            d->overlayHorizontalScrollBar, rect(), viewportRect, radius, verticalVisible);
    d->overlayVerticalScrollBar->raise();
    d->overlayHorizontalScrollBar->raise();
    if (d->borderOverlay)
        d->borderOverlay->raise();
}
