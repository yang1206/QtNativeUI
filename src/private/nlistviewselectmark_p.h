#ifndef NLISTVIEWSELECTMARK_P_H
#define NLISTVIEWSELECTMARK_P_H

#include <QHash>
#include <QModelIndex>
#include <QObject>
#include <QPersistentModelIndex>

#include "nselectionindicator_p.h"

class QItemSelection;
class QVariantAnimation;

class NListView;
class QWidget;

class NListViewSelectMarkController : public QObject {
    Q_OBJECT

  public:
    explicit NListViewSelectMarkController(NListView* view, QObject* parent = nullptr);
    ~NListViewSelectMarkController() override;

    void updateFromSelection(const QModelIndex& previousHint = QModelIndex());
    void syncMultiSelection(const QItemSelection& selected, const QItemSelection& deselected);
    void resetState();
    void onScrolled();
    void onModelAboutToReset();
    void onModelReset();
    void onRowsAboutToBeRemoved();
    void onRowsMoved();
    void onLayoutChanged();
    void paintViewportOverlay(QWidget* viewport) const;

  private:
    void requestViewportUpdate();
    bool useMovingIndicator() const;
    QRectF markRectForIndex(const QModelIndex& index) const;
    QModelIndex normalizedSelectedIndex() const;
    bool isEndpointUsable(const QModelIndex& index) const;
    NSelectionIndicatorMotion::Direction classifyDirection(const QModelIndex& previous,
                                                           const QModelIndex& current) const;
    QRectF movingIndicatorRect() const;
    void clearIndicatorState();
    void snapIndicatorToSelection();
    bool usesRevealIndicators() const;
    qreal revealProgress(const QModelIndex& index) const;
    void clearMultiRevealAnimations();
    void startRevealAnimation(const QModelIndex& index);

    NListView*                                      m_view{nullptr};
    NSelectionIndicatorMotion                       m_motion;
    QPersistentModelIndex                           m_currentIndicatorIndex;
    QPersistentModelIndex                           m_previousIndicatorIndex;
    QHash<QPersistentModelIndex, QVariantAnimation*> m_revealAnimations;
};

#endif
