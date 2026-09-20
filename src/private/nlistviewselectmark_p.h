#ifndef NLISTVIEWSELECTMARK_P_H
#define NLISTVIEWSELECTMARK_P_H

#include <QModelIndex>
#include <QObject>
#include <QPersistentModelIndex>

#include "nselectionindicator_p.h"

class NListView;
class QWidget;

class NListViewSelectMarkController : public QObject {
    Q_OBJECT

  public:
    explicit NListViewSelectMarkController(NListView* view, QObject* parent = nullptr);
    ~NListViewSelectMarkController() override;

    void updateFromSelection(const QModelIndex& previousHint = QModelIndex());
    void resetState();
    void onScrolled();
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
    void refreshGeometry();

    NListView*                    m_view{nullptr};
    NSelectionIndicatorMotion     m_motion;
    QPersistentModelIndex         m_currentIndicatorIndex;
    QPersistentModelIndex         m_previousIndicatorIndex;
};

#endif
