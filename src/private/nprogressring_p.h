#ifndef QTNATIVEUI_NPROGRESSRING_P_H
#define QTNATIVEUI_NPROGRESSRING_P_H

#include <QObject>
#include <QPropertyAnimation>
#include <QtNativeUI/NProgressRing.h>
#include "QtNativeUI/NEnums.h"

class QParallelAnimationGroup;
class QSequentialAnimationGroup;

class NProgressRingPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal progressPosition READ progressPosition WRITE setProgressPosition)
    Q_PROPERTY(qreal startAngle READ startAngle WRITE setStartAngle)
    Q_PROPERTY(qreal spanAngle READ spanAngle WRITE setSpanAngle)
    Q_D_CREATE(NProgressRing)
    Q_PROPERTY_CREATE_D(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_D(QColor, LightProgressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkProgressColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledTrackColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledTrackColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledProgressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledProgressColor)
    Q_PROPERTY_CREATE_D(QColor, LightPausedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPausedColor)
    Q_PROPERTY_CREATE_D(QColor, LightErrorColor)
    Q_PROPERTY_CREATE_D(QColor, DarkErrorColor)
    Q_PROPERTY_CREATE_D(int, StrokeWidth)
    Q_PROPERTY_CREATE_D(bool, AnimationEnabled)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)

  public:
    explicit NProgressRingPrivate(QObject* parent = nullptr);
    ~NProgressRingPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

    int     _minimum{0};
    int     _maximum{100};
    int     _value{0};
    QString _format;
    bool    _textVisible{false};

    QColor getTrackColor() const;
    QColor getProgressColor() const;
    QColor getTextColor() const;

    int calculateAngle() const;

    qreal progressPosition() const { return _progressPosition; }
    void  setProgressPosition(qreal position);

    void startProgressAnimation(qreal endValue);
    void startIndeterminateAnimations();
    void stopIndeterminateAnimation();
    void updateAnimations();

    QPropertyAnimation* _progressAnimation{nullptr};

    qreal _progressPosition{0.0};
    bool  _isAnimationRunning{false};

    bool isPaused() const { return _isPaused; }
    void setPaused(bool paused);

    bool isError() const { return _isError; }
    void setError(bool error);

    QColor getBarColorForState() const;

    qreal startAngle() const { return _startAngle; }
    void  setStartAngle(qreal angle);

    qreal spanAngle() const;
    void  setSpanAngle(qreal angle);

  private:
    bool _isPaused{false};
    bool _isError{false};

    QPropertyAnimation*        _startAngleAni1{nullptr};
    QPropertyAnimation*        _startAngleAni2{nullptr};
    QPropertyAnimation*        _spanAngleAni1{nullptr};
    QPropertyAnimation*        _spanAngleAni2{nullptr};
    QSequentialAnimationGroup* _startAngleAniGroup{nullptr};
    QSequentialAnimationGroup* _spanAngleAniGroup{nullptr};
    QParallelAnimationGroup*   _indeterminateAnimationGroup{nullptr};

    qreal _startAngle{0.0};
    qreal _spanAngle{0.0};

    qreal _pausedProgressPosition{0.0};
};

#endif // QTNATIVEUI_NPROGRESSRING_P_H