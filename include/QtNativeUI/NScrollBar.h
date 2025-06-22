//
// Created by Yang1206 on 2025/4/6.
//

#ifndef NSCROLLBAR_H
#define NSCROLLBAR_H

#include <QScrollBar>
#include "NColor.h"
#include "stdafx.h"

class QAbstractScrollArea;
class NScrollBarPrivate;
class QTNATIVEUI_EXPORT NScrollBar : public QScrollBar {
    Q_OBJECT
    Q_Q_CREATE(NScrollBar)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightArrowColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkArrowColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightArrowHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkArrowHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightArrowPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkArrowPressColor)
    Q_PROPERTY_CREATE_Q_H(int, ThumbThickness)
    Q_PROPERTY_CREATE_Q_H(int, ThumbNormalThickness)
    Q_PROPERTY_CREATE_Q_H(int, TrackThickness)
    Q_PROPERTY_CREATE_Q_H(int, ThumbCornerRadius)
    Q_PROPERTY_CREATE_Q_H(int, TrackCornerRadius)

  public:
    explicit NScrollBar(QWidget* parent = nullptr);
    explicit NScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr);
    explicit NScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent = nullptr);
    ~NScrollBar();

  signals:
    void rangeAnimationFinished();

  protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void changeEvent(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    void init();
};

#endif // NSCROLLBAR_H