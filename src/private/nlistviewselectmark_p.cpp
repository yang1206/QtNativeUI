#include "nlistviewselectmark_p.h"

#include <QAbstractItemView>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QListView>
#include <QPainter>
#include <QSet>
#include <QVariantAnimation>

#include <QEasingCurve>

#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NTheme.h>
#include "nlistviewitemlayout_p.h"

NListViewSelectMarkController::NListViewSelectMarkController(NListView* view, QObject* parent)
    : QObject(parent), m_view(view) {
    connect(&m_motion, &NSelectionIndicatorMotion::tick, this, [this]() { requestViewportUpdate(); });
}

NListViewSelectMarkController::~NListViewSelectMarkController() { clearMultiRevealAnimations(); }

void NListViewSelectMarkController::requestViewportUpdate() {
    if (m_view && m_view->viewport())
        m_view->viewport()->update();
}

bool NListViewSelectMarkController::usesRevealIndicators() const {
    if (!m_view || !m_view->getSelectionIndicatorVisible())
        return false;
    if (m_view->viewMode() == QListView::IconMode)
        return false;
    const auto mode = m_view->selectionMode();
    return mode == QAbstractItemView::MultiSelection || mode == QAbstractItemView::ExtendedSelection;
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
    const QRect content = NListViewItemLayout::rowBackgroundRect(
        NListViewItemLayout::rowContentRect(m_view, index, m_view->indicatorItemRect(index)));
    if (content.isEmpty())
        return {};
    return NSelectionIndicator::markRectForItem(QRectF(content));
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

NSelectionIndicatorMotion::Direction
NListViewSelectMarkController::classifyDirection(const QModelIndex& previous, const QModelIndex& current) const {
    if (!previous.isValid() || !current.isValid() || previous == current)
        return NSelectionIndicatorMotion::Direction::None;
    const QRectF previousRect = markRectForIndex(previous);
    const QRectF currentRect  = markRectForIndex(current);
    if (previousRect.isEmpty() || currentRect.isEmpty())
        return NSelectionIndicatorMotion::Direction::None;
    if (m_view->flow() == QListView::LeftToRight)
        return NSelectionIndicatorMotion::Direction::None;
    if (!qFuzzyCompare(previousRect.center().y() + 1.0, currentRect.center().y() + 1.0)) {
        return currentRect.center().y() > previousRect.center().y() ? NSelectionIndicatorMotion::Direction::Down
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
    m_currentIndicatorIndex  = QModelIndex();
    m_previousIndicatorIndex = QModelIndex();
    requestViewportUpdate();
}

void NListViewSelectMarkController::resetState() {
    m_motion.finishImmediately();
    clearIndicatorState();
    clearMultiRevealAnimations();
}

void NListViewSelectMarkController::clearMultiRevealAnimations() {
    for (QVariantAnimation* animation : m_revealAnimations) {
        if (animation) {
            animation->stop();
            animation->deleteLater();
        }
    }
    m_revealAnimations.clear();
}

void NListViewSelectMarkController::startRevealAnimation(const QModelIndex& index) {
    if (!m_view || !index.isValid() || !m_view->getSelectionIndicatorAnimated())
        return;
    const QPersistentModelIndex key(index);
    if (m_revealAnimations.contains(key))
        return;
    auto* animation = new QVariantAnimation(this);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setDuration(200);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    m_revealAnimations.insert(key, animation);
    connect(animation, &QVariantAnimation::valueChanged, this, [this]() { requestViewportUpdate(); });
    connect(animation, &QVariantAnimation::finished, this, [this, key, animation]() {
        m_revealAnimations.remove(key);
        animation->deleteLater();
        requestViewportUpdate();
    });
    animation->start();
}

qreal NListViewSelectMarkController::revealProgress(const QModelIndex& index) const {
    const auto it = m_revealAnimations.constFind(QPersistentModelIndex(index));
    if (it == m_revealAnimations.constEnd() || !it.value())
        return 1.0;
    return qBound(0.0, it.value()->currentValue().toReal(), 1.0);
}

void NListViewSelectMarkController::syncMultiSelection(const QItemSelection& selected,
                                                       const QItemSelection& deselected) {
    if (!usesRevealIndicators()) {
        clearMultiRevealAnimations();
        return;
    }
    const QItemSelectionModel* selection = m_view ? m_view->selectionModel() : nullptr;
    if (!selection) {
        clearMultiRevealAnimations();
        return;
    }
    for (auto it = m_revealAnimations.begin(); it != m_revealAnimations.end();) {
        const QModelIndex index = it.key();
        if (!index.isValid() || index.model() != m_view->model() || !selection->isSelected(index)) {
            if (it.value()) {
                it.value()->stop();
                it.value()->deleteLater();
            }
            it = m_revealAnimations.erase(it);
        } else {
            ++it;
        }
    }
    for (const QModelIndex& index : deselected.indexes()) {
        const QPersistentModelIndex key(index);
        if (QVariantAnimation* animation = m_revealAnimations.take(key)) {
            animation->stop();
            animation->deleteLater();
        }
    }
    for (const QModelIndex& index : selected.indexes()) {
        if (index.isValid() && index.column() == 0 && selection->isSelected(index))
            startRevealAnimation(index);
    }
    if (selection->selectedIndexes().isEmpty())
        clearMultiRevealAnimations();
    requestViewportUpdate();
}

void NListViewSelectMarkController::snapIndicatorToSelection() {
    if (!m_view || !m_view->getSelectionIndicatorVisible() || !useMovingIndicator()) {
        requestViewportUpdate();
        return;
    }
    m_motion.finishImmediately();
    m_previousIndicatorIndex  = QModelIndex();
    const QModelIndex current = normalizedSelectedIndex();
    if (!current.isValid() || !isEndpointUsable(current)) {
        clearIndicatorState();
        return;
    }
    m_motion.setDirection(NSelectionIndicatorMotion::Direction::None);
    m_currentIndicatorIndex = QPersistentModelIndex(current);
    requestViewportUpdate();
}

QRectF NListViewSelectMarkController::movingIndicatorRect() const {
    if (!m_currentIndicatorIndex.isValid())
        return {};
    const QRectF target = markRectForIndex(m_currentIndicatorIndex);
    if (target.isEmpty())
        return {};
    if (!m_previousIndicatorIndex.isValid() || m_motion.direction() == NSelectionIndicatorMotion::Direction::None) {
        return target;
    }
    const QRectF previous = markRectForIndex(m_previousIndicatorIndex);
    if (previous.isEmpty())
        return target;
    return NSelectionIndicator::interpolatedMarkRect(previous, target, m_motion.progress(), m_motion.direction());
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
        requestViewportUpdate();
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
    const bool hasUsablePrevious = previousIndex.isValid() && isEndpointUsable(previousIndex) && useMovingIndicator();
    const auto direction =
        hasUsablePrevious ? classifyDirection(previousIndex, current) : NSelectionIndicatorMotion::Direction::None;
    const bool canAnimate    = hasUsablePrevious && direction != NSelectionIndicatorMotion::Direction::None;
    m_previousIndicatorIndex = canAnimate ? previousIndex : QPersistentModelIndex();
    m_currentIndicatorIndex  = QPersistentModelIndex(current);
    m_motion.setDirection(canAnimate ? direction : NSelectionIndicatorMotion::Direction::None);
    const bool animated = m_view->getSelectionIndicatorAnimated();
    m_motion.start(canAnimate && animated);
}

void NListViewSelectMarkController::onScrolled() { requestViewportUpdate(); }

void NListViewSelectMarkController::onModelAboutToReset() {
    m_motion.finishImmediately();
    clearIndicatorState();
    clearMultiRevealAnimations();
}

void NListViewSelectMarkController::onModelReset() { updateFromSelection(); }

void NListViewSelectMarkController::onRowsAboutToBeRemoved() {
    m_motion.finishImmediately();
    clearIndicatorState();
}

void NListViewSelectMarkController::onRowsMoved() {
    m_motion.finishImmediately();
    m_previousIndicatorIndex = QModelIndex();
    updateFromSelection();
}

void NListViewSelectMarkController::onLayoutChanged() {
    if (useMovingIndicator())
        snapIndicatorToSelection();
    else
        requestViewportUpdate();
}

void NListViewSelectMarkController::paintViewportOverlay(QWidget* viewport) const {
    if (!m_view || !viewport || !m_view->getSelectionIndicatorVisible())
        return;
    if (m_view->viewMode() == QListView::IconMode)
        return;
    const QColor accent = nTheme->accentColor().normal();
    QPainter     painter(viewport);
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
        const QRectF base = markRectForIndex(index);
        if (base.isEmpty())
            continue;
        const QRectF mark =
            usesRevealIndicators() ? NSelectionIndicator::revealedMarkRect(base, revealProgress(index)) : base;
        if (!mark.isEmpty())
            NSelectionIndicator::paintMark(&painter, mark, accent);
    }
}
