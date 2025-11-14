#ifndef NRANGESLIDER_H
#define NRANGESLIDER_H

#include <QWidget>
#include <functional>
#include "NColor.h"
#include "stdafx.h"

class NRangeSliderPrivate;
class QTNATIVEUI_EXPORT NRangeSlider : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NRangeSlider)

    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledTrackColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDisabledProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDisabledProgressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbOuterColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbOuterColor)
    Q_PROPERTY_CREATE_Q_H(int, TrackHeight)
    Q_PROPERTY_CREATE_Q_H(int, ThumbDiameter)
    Q_PROPERTY_CREATE_Q_H(int, ThumbInnerDiameter)
    Q_PROPERTY_CREATE_Q_H(int, TrackCornerRadius)

    Q_PROPERTY_CREATE_Q_H(int, Minimum)
    Q_PROPERTY_CREATE_Q_H(int, Maximum)
    Q_PROPERTY_CREATE_Q_H(int, LowerValue)
    Q_PROPERTY_CREATE_Q_H(int, UpperValue)

    Q_PROPERTY(bool showTooltip READ showTooltip WRITE setShowTooltip)

  public:
    explicit NRangeSlider(QWidget* parent = nullptr);
    explicit NRangeSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~NRangeSlider();

    void            setRange(int min, int max);
    void            setValues(int lower, int upper);
    QPair<int, int> values() const;

    Qt::Orientation orientation() const;
    void            setOrientation(Qt::Orientation orientation);

    void setAccentColor(const QColor& color);
    void setAccentColor(const NAccentColor& color);
    void resetAccentColor();

    bool showTooltip() const;
    void setShowTooltip(bool show);
    void setTooltipFormatter(std::function<QString(int)> formatter);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

  Q_SIGNALS:
    void lowerValueChanged(int value);
    void upperValueChanged(int value);
    void rangeChanged(int lower, int upper);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void changeEvent(QEvent* event) override;

  private:
    void init();
    void updateAccentColors();

    int   valueFromPosition(const QPoint& pos) const;
    QRect lowerHandleRect() const;
    QRect upperHandleRect() const;
    QRect trackRect() const;
    QRect progressRect() const;
    void  drawTrack(QPainter* painter);
    void  drawProgress(QPainter* painter);
    void  drawHandle(QPainter* painter, const QRect& rect, bool isActive, bool isLower);
};

#endif // NRANGESLIDER_H
