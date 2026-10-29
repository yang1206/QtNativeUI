#ifndef NNAVIGATIONANIMATION_P_H
#define NNAVIGATIONANIMATION_P_H

#include <QObject>
#include <QPixmap>
#include <QPointer>
#include <QWidget>
#include "QtNativeUI/NEnums.h"

class QStackedWidget;
class QPropertyAnimation;
class QGraphicsBlurEffect;

class NNavigationAnimationManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress)

  public:
    explicit NNavigationAnimationManager(QStackedWidget* stackedWidget, QObject* parent = nullptr);
    ~NNavigationAnimationManager() override;

    void executeTransition(NNavigationType::PageTransitionType type, int targetIndex, bool isRouteBack, int duration = 300);
    void paintTransition(QPainter* painter, const QRect& rect, int borderRadius);
    void cancelTransition();
    qreal progress() const { return m_progress; }
    void setProgress(qreal progress);

  protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    QStackedWidget* m_stackedWidget{nullptr};
    NNavigationType::PageTransitionType m_transitionType{NNavigationType::NoTransition};
    QPixmap m_targetStackPix;
    QPixmap m_currentStackPix;
    QPointer<QGraphicsBlurEffect> m_blurEffect;
    QPropertyAnimation* m_animation{nullptr};
    qreal m_progress{0};
    bool m_isRouteBack{false};
    quint64 m_requestId{0};
    QPointer<QWidget> m_hiddenPage;
};

#endif // NNAVIGATIONANIMATION_P_H
