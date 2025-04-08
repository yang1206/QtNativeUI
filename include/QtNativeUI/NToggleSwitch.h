#ifndef QTNATIVEUI_NTOGGLESWITCH_H
#define QTNATIVEUI_NTOGGLESWITCH_H

#include <QAbstractButton>
#include "NIconEnums.h"
#include "stdafx.h"

class NToggleSwitchPrivate;
class QTNATIVEUI_EXPORT NToggleSwitch : public QAbstractButton {
    Q_OBJECT
    Q_Q_CREATE(NToggleSwitch)
    Q_PROPERTY_CREATE_Q_H(int, TrackBorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, TrackBorderWidth)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbCheckedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbCheckedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)

  public:
    explicit NToggleSwitch(QWidget* parent = nullptr);
    explicit NToggleSwitch(const QString& text, QWidget* parent = nullptr);
    ~NToggleSwitch();

    void setChecked(bool checked);
    bool isChecked() const;

  Q_SIGNALS:
    void checkedChanged(bool checked);

  protected:
    void  paintEvent(QPaintEvent* event) override;
    void  mousePressEvent(QMouseEvent* event) override;
    void  mouseReleaseEvent(QMouseEvent* event) override;
    void  mouseMoveEvent(QMouseEvent* event) override;
    bool  event(QEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void drawTrack(QPainter* painter);
    void drawThumb(QPainter* painter);
    void drawText(QPainter* painter);

  private:
    void init();
    void updateAccentColors();
};

#endif // QTNATIVEUI_NTOGGLESWITCH_H