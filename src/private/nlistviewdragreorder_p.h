#ifndef NLISTVIEWDRAGREORDER_P_H
#define NLISTVIEWDRAGREORDER_P_H

#include <QHash>
#include <QObject>
#include <QPixmap>
#include <QPoint>

class NListView;
class QVariantAnimation;
class QWidget;

class NListViewDragReorderController : public QObject {
    Q_OBJECT

  public:
    explicit NListViewDragReorderController(NListView* view, QObject* parent = nullptr);

    void reset();
    void onPress(const QPoint& viewportPos, int sourceRow);
    bool onMove(const QPoint& viewportPos, Qt::MouseButtons buttons);
    bool onRelease(const QPoint& viewportPos, Qt::MouseButton button, int* destinationRowOut);

    bool isDragging() const { return m_dragging; }
    void setPaintingWithOffsets(bool painting) { m_paintingWithOffsets = painting; }
    bool paintingWithOffsets() const { return m_paintingWithOffsets; }
    int  sourceRow() const { return m_sourceRow; }
    QRect ghostRect() const;
    QRect adjustedVisualRect(const QModelIndex& index, const QRect& baseRect) const;
    void paintOverlay(QWidget* viewport) const;

  private:
    QRect displacedLayoutRect(int row) const;
    int  dropIndicatorRow(const QPoint& viewportPos) const;
    void updateDisplacement();
    void clearAnimations();
    QPixmap renderRowPixmap(int row) const;
    qreal offsetForRow(int row) const;

    NListView*                     m_view{nullptr};
    int                            m_sourceRow{-1};
    int                            m_dropTarget{-1};
    bool                           m_dragging{false};
    QPoint                         m_pressPos;
    QPoint                         m_currentPos;
    QPixmap                        m_pixmap;
    QHash<int, qreal>              m_offsets;
    QHash<int, QVariantAnimation*> m_anims;
    bool                           m_paintingWithOffsets{false};
};

#endif
