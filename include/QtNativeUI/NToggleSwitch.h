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
    // 轨道未选中状态
    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackDefaultColor)
    // 轨道边框颜色
    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackBorderColor)
    // 轨道选中状态
    Q_PROPERTY_CREATE_Q_H(QColor, LightTrackCheckedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTrackCheckedColor)
    // 滑块未选中状态
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbDefaultColor)
    // 滑块选中状态
    Q_PROPERTY_CREATE_Q_H(QColor, LightThumbCheckedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkThumbCheckedColor)
    // 文本颜色
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)

  public:
    explicit NToggleSwitch(QWidget* parent = nullptr);
    explicit NToggleSwitch(const QString& text, QWidget* parent = nullptr);
    ~NToggleSwitch();

    // 设置/获取是否处于打开状态
    void setChecked(bool checked);
    bool isChecked() const;

  protected:
    void  paintEvent(QPaintEvent* event) override;
    void  enterEvent(QEnterEvent* event) override;
    void  leaveEvent(QEvent* event) override;
    void  mousePressEvent(QMouseEvent* event) override;
    void  mouseReleaseEvent(QMouseEvent* event) override;
    void  changeEvent(QEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    // 绘制函数
    void drawTrack(QPainter* painter);
    void drawThumb(QPainter* painter);
    void drawText(QPainter* painter);

  private:
    void init();
    void updateAccentColors();
};

#endif // QTNATIVEUI_NTOGGLESWITCH_H