#ifndef NSELECTIONINDICATOR_P_H
#define NSELECTIONINDICATOR_P_H

#include <QObject>
#include <QPainter>
#include <QRectF>
#include <functional>

namespace QtNativeUI {
class NPointToPointAnimation;
}

class NSelectionIndicatorMotion : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)

  public:
    enum class Direction { None, Up, Down };

    explicit NSelectionIndicatorMotion(QObject* parent = nullptr);
    ~NSelectionIndicatorMotion() override;

    qreal     progress() const;
    void      setProgress(qreal value);
    Direction direction() const;
    void      setDirection(Direction direction);

    void start(bool animated);
    void finishImmediately();
    void reset();

  signals:
    void progressChanged(qreal value);
    void tick();

  private:
    QtNativeUI::NPointToPointAnimation* m_animation{nullptr};
    qreal                               m_progress{1.0};
    Direction                           m_direction{Direction::None};
};

namespace NSelectionIndicator {

constexpr qreal kMarkWidth      = 3.0;
constexpr qreal kMarkLeftOffset = 3.0;
constexpr qreal kCornerRadius   = 2.0;

qreal verticalInset(const QRectF& itemRect);
QRectF markRectForItem(const QRectF& itemRect);
qreal leadingProgress(qreal progress);
qreal trailingProgress(qreal progress);
QRectF interpolatedMarkRect(const QRectF& previous, const QRectF& target, qreal progress,
                            NSelectionIndicatorMotion::Direction direction);
void paintMark(QPainter* painter, const QRectF& markRect, const QColor& accentColor);

} // namespace NSelectionIndicator

class NSelectionIndicatorSession : public QObject {
    Q_OBJECT

  public:
    using RectFn = std::function<QRectF()>;

    explicit NSelectionIndicatorSession(std::function<void()> repaint, QObject* parent = nullptr);

    void clear();
    void showAt(RectFn currentRect);
    void transition(RectFn previousRect, RectFn currentRect, NSelectionIndicatorMotion::Direction direction);
    QRectF currentRect() const;

  private:
    NSelectionIndicatorMotion m_motion;
    RectFn                    m_previousRect;
    RectFn                    m_currentRect;
    bool                      m_animateFromPrevious{false};
    std::function<void()>     m_repaint;
};

#endif
