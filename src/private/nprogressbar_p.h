#ifndef QTNATIVEUI_NPROGRESSBAR_P_H
#define QTNATIVEUI_NPROGRESSBAR_P_H

#include <QPropertyAnimation>
#include <QtNativeUI/NProgressBar.h>
#include "QtNativeUI/NEnums.h"

class QParallelAnimationGroup;
class NProgressBarPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal progressPosition READ progressPosition WRITE setProgressPosition)
    Q_PROPERTY(qreal indeterminateOffset READ indeterminateOffset WRITE setIndeterminateOffset)
    Q_PROPERTY(qreal shortPos READ shortPos WRITE setShortPos)
    Q_PROPERTY(qreal longPos READ longPos WRITE setLongPos)
    Q_D_CREATE(NProgressBar)

    // 颜色属性
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

    // 形状属性
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ProgressThickness)

    // 动画属性
    Q_PROPERTY_CREATE_D(bool, AnimationEnabled)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)

  public:
    explicit NProgressBarPrivate(QObject* parent = nullptr);
    ~NProgressBarPrivate();

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};

    QColor getTrackColor() const;
    QColor getProgressColor() const;
    QColor getTextColor() const;

    // 位置计算辅助方法
    QRect getTrackRect() const;
    QRect getProgressRect() const;
    int   calculateProgressWidth() const;
    int   calculateProgressHeight() const;

    // 动画相关属性和方法
    qreal progressPosition() const { return _progressPosition; }
    void  setProgressPosition(qreal position);

    qreal indeterminateOffset() const { return _indeterminateOffset; }
    void  setIndeterminateOffset(qreal offset);

    void startProgressAnimation(qreal endValue);
    void startIndeterminateAnimation();
    void stopIndeterminateAnimation();
    void updateAnimations();

    QPropertyAnimation* _progressAnimation{nullptr};
    QPropertyAnimation* _indeterminateAnimation{nullptr};

    qreal _progressPosition{0.0};
    qreal _indeterminateOffset{0.0};
    bool  _isAnimationRunning{false};

    // 状态相关
    bool isPaused() const { return _isPaused; }
    void setPaused(bool paused);

    bool isError() const { return _isError; }
    void setError(bool error);

    // 获取基于当前状态的颜色
    QColor getBarColorForState() const;

    // 不确定状态的两个位置
    qreal shortPos() const { return _shortPos; }
    void  setShortPos(qreal pos);

    qreal longPos() const { return _longPos; }
    void  setLongPos(qreal pos);

    // 启动双段式动画
    void startIndeterminateAnimations();

  private:
    bool _isPaused{false};
    bool _isError{false};

    QPropertyAnimation*        _shortPosAnimation{nullptr};
    QPropertyAnimation*        _longPosAnimation{nullptr};
    QSequentialAnimationGroup* _longAnimationGroup{nullptr};
    QParallelAnimationGroup*   _indeterminateAnimationGroup{nullptr};

    qreal _shortPos{0.0};
    qreal _longPos{0.0};

    qreal _pausedProgressPosition{0.0}; // 保存暂停时的进度位置
};

#endif // QTNATIVEUI_NPROGRESSBAR_P_H