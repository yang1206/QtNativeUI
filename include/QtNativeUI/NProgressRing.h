#ifndef QTNATIVEUI_NPROGRESSRING_H
#define QTNATIVEUI_NPROGRESSRING_H

#include <QWidget>
#include "stdafx.h"

class NProgressRingPrivate;
class QTNATIVEUI_EXPORT NProgressRing : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NProgressRing)

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

    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString format READ format WRITE setFormat)
    Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)

  public:
    explicit NProgressRing(QWidget* parent = nullptr);
    ~NProgressRing();

    int     minimum() const;
    int     maximum() const;
    int     value() const;
    QString format() const;
    bool    isTextVisible() const;

    void pause();
    void resume();
    void setPaused(bool paused);
    bool isPaused() const;

    void error();
    void setError(bool error);
    bool isError() const;

    void setStrokeWidth(int width);
    int  getStrokeWidth() const;

    void  setProgressPosition(qreal position);
    qreal progressPosition() const;

    bool isIndeterminate() const;
    void setIndeterminate(bool indeterminate);
    void setMinimum(int minimum);
    void setMaximum(int maximum);
    void setRange(int minimum, int maximum);
    void setValue(int value);
    void setFormat(const QString& format);
    void setTextVisible(bool visible);
    void reset();

  signals:
    void valueChanged(int value);
    void strokeWidthChanged();

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;

  private:
    void init();
    void drawTrack(QPainter* painter);
    void drawProgress(QPainter* painter);
    void drawIndeterminateProgress(QPainter* painter);
    void drawText(QPainter* painter);
};

#endif // QTNATIVEUI_NPROGRESSRING_H