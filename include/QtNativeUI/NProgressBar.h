#ifndef QTNATIVEUI_NPROGRESSBAR_H
#define QTNATIVEUI_NPROGRESSBAR_H

#include <QProgressBar>
#include "stdafx.h"

class NProgressBarPrivate;
class QTNATIVEUI_EXPORT NProgressBar : public QProgressBar {
    Q_OBJECT
    Q_Q_CREATE(NProgressBar)

    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPausedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPausedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightErrorColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkErrorColor)
    Q_PROPERTY_CREATE_Q_H(bool, AnimationEnabled)
    Q_PROPERTY_CREATE_Q_H(int, AnimationDuration)

  public:
    explicit NProgressBar(QWidget* parent = nullptr);
    explicit NProgressBar(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~NProgressBar();

    void setValue(int value);

    void  setProgressPosition(qreal position);
    qreal progressPosition() const;
    bool  isIndeterminate() const;
    void  setIndeterminate(bool indeterminate);
    void  pause();
    void  resume();
    void  reset();
    void  setPaused(bool paused);
    bool  isPaused() const;

    void error();
    void setError(bool error);
    bool isError() const;

    void setBorderRadius(int radius);
    int  getBorderRadius() const;
    void setProgressThickness(int thickness);
    int  getProgressThickness() const;

  signals:
    void trackThicknessChanged();
    void borderRadiusChanged();

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;

    void drawTrack(QPainter* painter);
    void drawProgress(QPainter* painter);
    void drawIndeterminateProgress(QPainter* painter);
    void drawText(QPainter* painter);

  private:
    void init();
};

#endif // QTNATIVEUI_NPROGRESSBAR_H