#include "nlistviewselectmark_p.h"

#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QListView>
#include <QPainter>
#include <QSet>

#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NTheme.h>

NListViewSelectMarkController::NListViewSelectMarkController(NListView* view, QObject* parent)
    : QObject(parent), m_view(view) {
    connect(&m_motion, &NSelectionIndicatorMotion::tick, this, [this]() { requestViewportUpdate(); });
}

NListViewSelectMarkController::~NListViewSelectMarkController() = default;

void NListViewSelectMarkController::requestViewportUpdate() {
    if (m_view && m_view->viewport())
        m_view->viewport()->update();
}

bool NListViewSelectMarkController::useMovingIndicator() const {
    if (!m_view || !m_view->getSelectionIndicatorVisible())
        return false;
    if (m_view->viewMode() == QListView::IconMode)
        return false;
    return m_view->selectionMode() == QAbstractItemView::SingleSelection;
}

QRectF NListViewSelectMarkController::markRectForIndex(const QModelIndex& index) const {
    if (!m_view || !index.isValid())
        return {};
    return NSelectionIndicator::markRectForItem(QRectF(m_view->visualRect(index)));
}

bool NListViewSelectMarkController::isEndpointUsable(const QModelIndex& index) const {
    if (!index.isValid() || index.model() != m_view->model())
        return false;
    return !markRectForIndex(index).isEmpty();
}

QModelIndex NListViewSelectMarkController::normalizedSelectedIndex() const {
    if (!m_view || !m_view->model())
        return {};
    const QItemSelectionModel* selection = m_view->selectionModel();
    if (!selection)
        return {};
    const QModelIndex current = m_view->currentIndex();
    if (current.isValid() && selection->isSelected(current))
        return current;
    const QModelIndexList rows = selection->selectedIndexes();
    return rows.isEmpty() ? QModelIndex() : rows.first();
}

NSelectionIndicatorMotion::Direction NListViewSelectMarkController::classifyDirection(
    const QModelIndex& previous, const QModelIndex& current) const {
    if (!previous.isValid() || !current.isValid() || previous == current)
        return NSelectionIndicatorMotion::Direction::None;
    const QRectF previousRect = markRectForIndex(previous);
    const QRectF currentRect    = markRectForIndex(current);
    if (previousRect.isEmpty() || currentRect.isEmpty())
        return NSelectionIndicatorMotion::Direction::None;
    if (!qFuzzyCompare(previousRect.center().y() + 1.0, currentRect.center().y() + 1.0)) {
        return currentRect.center().y() > previousRect.center().y()
                   ? NSelectionIndicatorMotion::Direction::Down
                   : NSelectionIndicatorMotion::Direction::Up;
    }
    if (current.row() != previous.row()) {
        return current.row() > previous.row() ? NSelectionIndicatorMotion::Direction::Down
                                              : NSelectionIndicatorMotion::Direction::Up;
    }
    return NSelectionIndicatorMotion::Direction::None;
}

void NListViewSelectMarkController::clearIndicatorState() {
    m_motion.reset();
    m_currentIndicatorIndex = QModelIndex();
    m_previousIndicatorIndex = QModelIndex();
    requestViewportUpdate();
}

void NListViewSelectMarkController::resetState() {
    m_motion.finishImmediately();
    clearIndicatorState();
}

void NListViewSelectMarkController::refreshGeometry() { requestViewportUpdate(); }

QRectF NListViewSelectMarkController::movingIndicatorRect() const {
    if (!m_currentIndicatorIndex.isValid())
        return {};
    const QRectF target = markRectForIndex(m_currentIndicatorIndex);
    if (target.isEmpty())
        return {};
    if (!m_previousIndicatorIndex.isValid() ||
        m_motion.direction() == NSelectionIndicatorMotion::Direction::None) {
        return target;
    }
    const QRectF previous = markRectForIndex(m_previousIndicatorIndex);
    if (previous.isEmpty())
        return target;
    return NSelectionIndicator::interpolatedMarkRect(previous, target, m_motion.progress(),
                                                     m_motion.direction());
}

void NListViewSelectMarkController::updateFromSelection(const QModelIndex& previousHint) {
    if (!m_view || !m_view->getSelectionIndicatorVisible()) {
        resetState();
        return;
    }
    if (!useMovingIndicator()) {
        m_motion.finishImmediately();
        m_currentIndicatorIndex  = QModelIndex();
        m_previousIndicatorIndex = QModelIndex();
        requestViewportUpdate();
        return;
    }
    const QModelIndex current = normalizedSelectedIndex();
    if (!current.isValid() || !isEndpointUsable(current)) {
        clearIndicatorState();
        return;
    }
    if (m_currentIndicatorIndex.isValid() && current == m_currentIndicatorIndex) {
        refreshGeometry();
        return;
    }
    QPersistentModelIndex previousIndex = m_currentIndicatorIndex;
    if (!previousIndex.isValid() && previousHint.isValid())
        previousIndex = QPersistentModelIndex(previousHint);
    const QRectF targetRect = markRectForIndex(current);
    if (targetRect.isEmpty()) {
        clearIndicatorState();
        return;
    }
    const bool hasUsablePrevious =
        previousIndex.isValid() && isEndpointUsable(previousIndex) && useMovingIndicator();
    const auto direction =
        hasUsablePrevious ? classifyDirection(previousIndex, current)
                          : NSelectionIndicatorMotion::Direction::None;
    const bool canAnimate =
        hasUsablePrevious && direction != NSelectionIndicatorMotion::Direction::None;
    m_previousIndicatorIndex = canAnimate ? previousIndex : QPersistentModelIndex();
    m_currentIndicatorIndex  = QPersistentModelIndex(current);
    m_motion.setDirection(canAnimate ? direction : NSelectionIndicatorMotion::Direction::None);
    m_motion.start(canAnimate);
}

void NListViewSelectMarkController::onScrolled() {
    if (useMovingIndicator())
        refreshGeometry();
    else
        requestViewportUpdate();
}

void NListViewSelectMarkController::paintViewportOverlay(QWidget* viewport) const {
    if (!m_view || !viewport || !m_view->getSelectionIndicatorVisible())
        return;
    if (m_view->viewMode() == QListView::IconMode)
        return;
    const QColor accent = nTheme->accentColor().normal();
    QPainter   painter(viewport);
    if (!painter.isActive())
        return;
    if (useMovingIndicator()) {
        const QRectF rect = movingIndicatorRect();
        if (!rect.isEmpty())
            NSelectionIndicator::paintMark(&painter, rect, accent);
        return;
    }
    const QItemSelectionModel* selection = m_view->selectionModel();
    if (!selection)
        return;
    QSet<QPersistentModelIndex> painted;
    for (const QModelIndex& index : selection->selectedIndexes()) {
        if (!index.isValid())
            continue;
        const QPersistentModelIndex key(index);
        if (painted.contains(key))
            continue;
        painted.insert(key);
        const QRectF mark = markRectForIndex(index);
        if (!mark.isEmpty())
            NSelectionIndicator::paintMark(&painter, mark, accent);
    }
}
