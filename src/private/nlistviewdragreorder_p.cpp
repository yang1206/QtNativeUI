#include "nlistviewdragreorder_p.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QEasingCurve>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QVariantAnimation>

#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NTheme.h>

namespace {
constexpr int kDropLineInset = 12;
constexpr int kDragAnimMs    = 180;
} // namespace

NListViewDragReorderController::NListViewDragReorderController(NListView* view, QObject* parent)
    : QObject(parent), m_view(view) {}

void NListViewDragReorderController::reset() {
    m_dragging            = false;
    m_sourceRow           = -1;
    m_dropTarget          = -1;
    m_pixmap              = QPixmap();
    m_paintingWithOffsets = false;
    m_offsets.clear();
    clearAnimations();
}

void NListViewDragReorderController::clearAnimations() {
    for (QVariantAnimation* animation : m_anims) {
        if (animation) {
            animation->stop();
            animation->deleteLater();
        }
    }
    m_anims.clear();
}

void NListViewDragReorderController::onPress(const QPoint& viewportPos, int sourceRow) {
    if (!m_view || sourceRow < 0)
        return;
    m_sourceRow = sourceRow;
    m_pressPos  = viewportPos;
}

bool NListViewDragReorderController::onMove(const QPoint& viewportPos, Qt::MouseButtons buttons) {
    if (!m_view || m_sourceRow < 0 || !(buttons & Qt::LeftButton))
        return false;
    if (!m_dragging) {
        if ((viewportPos - m_pressPos).manhattanLength() < QApplication::startDragDistance())
            return false;
        const QModelIndex current = m_view->currentIndex();
        if (current.isValid() && current.model() == m_view->model())
            m_sourceRow = current.row();
        m_pixmap     = renderRowPixmap(m_sourceRow);
        m_dragging   = true;
        m_dropTarget = dropIndicatorRow(viewportPos);
        updateDisplacement();
    }
    m_currentPos     = viewportPos;
    const int target = dropIndicatorRow(viewportPos);
    if (target != m_dropTarget) {
        m_dropTarget = target;
        updateDisplacement();
    }
    if (m_view->viewport())
        m_view->viewport()->update();
    return true;
}

bool NListViewDragReorderController::onRelease(const QPoint& viewportPos, Qt::MouseButton button,
                                               int* destinationRowOut) {
    if (!m_dragging || button != Qt::LeftButton)
        return false;
    if (destinationRowOut)
        *destinationRowOut = m_dropTarget >= 0 ? m_dropTarget : dropIndicatorRow(viewportPos);
    m_dragging = false;
    return true;
}

qreal NListViewDragReorderController::offsetForRow(int row) const { return m_offsets.value(row, 0.0); }

QRect NListViewDragReorderController::ghostRect() const {
    if (!m_dragging || m_pixmap.isNull())
        return {};
    const qreal dpr  = qMax(1.0, m_pixmap.devicePixelRatioF());
    const int   pixW = qRound(m_pixmap.width() / dpr);
    const int   pixH = qRound(m_pixmap.height() / dpr);
    if (pixW <= 0 || pixH <= 0)
        return {};
    return QRect(0, m_currentPos.y() - pixH / 2, pixW, pixH);
}

QRect NListViewDragReorderController::adjustedVisualRect(const QModelIndex& index, const QRect& baseRect) const {
    if (!m_paintingWithOffsets || !m_dragging || !index.isValid() || baseRect.isEmpty())
        return baseRect;
    if (index.row() == m_sourceRow) {
        QRect hidden = baseRect;
        hidden.moveTop(-hidden.height() * 2);
        return hidden;
    }
    QRect shifted = baseRect;
    shifted.translate(0, qRound(offsetForRow(index.row())));
    return shifted;
}

QRect NListViewDragReorderController::displacedLayoutRect(int row) const {
    if (!m_view || !m_view->model() || row < 0 || row >= m_view->model()->rowCount())
        return {};
    QRect rect = m_view->layoutVisualRect(m_view->model()->index(row, 0));
    if (m_dragging)
        rect.translate(0, qRound(offsetForRow(row)));
    return rect;
}

int NListViewDragReorderController::dropIndicatorRow(const QPoint& viewportPos) const {
    if (!m_view || !m_view->model())
        return 0;
    const int count = m_view->model()->rowCount();
    for (int row = 0; row < count; ++row) {
        if (row == m_sourceRow)
            continue;
        const QRect rect = displacedLayoutRect(row);
        if (!rect.isValid())
            continue;
        if (viewportPos.y() < rect.center().y())
            return row;
    }
    return count;
}

QPixmap NListViewDragReorderController::renderRowPixmap(int row) const {
    if (!m_view || !m_view->model())
        return {};
    const QModelIndex index = m_view->model()->index(row, 0);
    if (!index.isValid())
        return {};
    const QRect rect = m_view->layoutVisualRect(index);
    if (!rect.isValid() || rect.width() <= 0 || rect.height() <= 0)
        return {};
    const qreal dpr = m_view->devicePixelRatioF();
    QPixmap     pix(rect.size() * dpr);
    pix.setDevicePixelRatio(dpr);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);
    QStyleOptionViewItem option;
    option.initFrom(m_view);
    option.rect                   = QRect(0, 0, rect.width(), rect.height());
    option.state                  = QStyle::State_Enabled;
    option.showDecorationSelected = true;
    if (m_view->selectionModel() && m_view->selectionModel()->isSelected(index))
        option.state |= QStyle::State_Selected;
    if (QAbstractItemDelegate* delegate = m_view->itemDelegate())
        delegate->paint(&painter, option, index);
    return pix;
}

void NListViewDragReorderController::updateDisplacement() {
    if (!m_view || !m_view->model() || m_sourceRow < 0 || m_dropTarget < 0) {
        clearAnimations();
        return;
    }
    QAbstractItemModel* model   = m_view->model();
    const int           count   = model->rowCount();
    const int           src     = m_sourceRow;
    const int           dst     = m_dropTarget;
    const int           srcH    = m_view->layoutVisualRect(model->index(src, 0)).height();
    if (srcH <= 0)
        return;
    for (int row = 0; row < count; ++row) {
        qreal target = 0.0;
        if (src < dst && row > src && row < dst)
            target = -srcH;
        else if (src > dst && row >= dst && row < src)
            target = srcH;
        const qreal current = m_offsets.value(row, 0.0);
        if (qFuzzyCompare(current + 1.0, target + 1.0) && !m_anims.contains(row))
            continue;
        if (QVariantAnimation* old = m_anims.take(row)) {
            old->stop();
            old->deleteLater();
        }
        if (qFuzzyCompare(current + 1.0, target + 1.0)) {
            m_offsets.insert(row, target);
            continue;
        }
        auto* anim = new QVariantAnimation(this);
        anim->setDuration(kDragAnimMs);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        anim->setStartValue(current);
        anim->setEndValue(target);
        m_anims.insert(row, anim);
        connect(anim, &QVariantAnimation::valueChanged, this, [this, row](const QVariant& value) {
            m_offsets.insert(row, value.toReal());
            if (m_view && m_view->viewport())
                m_view->viewport()->update();
        });
        connect(anim, &QVariantAnimation::finished, this, [this, row, anim]() {
            m_anims.remove(row);
            anim->deleteLater();
        });
        anim->start();
    }
}

void NListViewDragReorderController::paintOverlay(QWidget* viewport) const {
    if (!m_dragging || !viewport || !m_view || !m_view->model())
        return;
    QPainter painter(viewport);
    painter.setRenderHint(QPainter::Antialiasing);
    if (m_dropTarget >= 0) {
        int y = 0;
        if (m_dropTarget < m_view->model()->rowCount())
            y = displacedLayoutRect(m_dropTarget).top();
        else if (m_view->model()->rowCount() > 0)
            y = displacedLayoutRect(m_view->model()->rowCount() - 1).bottom() + 1;
        const QColor accent = nTheme->accentColor().normal();
        painter.setPen(QPen(accent, 2.0));
        painter.drawLine(kDropLineInset, y, viewport->width() - kDropLineInset, y);
        constexpr int circleR = 3;
        painter.setBrush(accent);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(kDropLineInset, y), circleR, circleR);
        painter.drawEllipse(QPoint(viewport->width() - kDropLineInset, y), circleR, circleR);
    }
    const QRect ghost = ghostRect();
    if (!ghost.isEmpty()) {
        painter.setOpacity(0.85);
        painter.drawPixmap(ghost.topLeft(), m_pixmap);
    }
}
