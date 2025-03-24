#ifndef QTNATIVEUI_NTOGGLESWITCH_P_H
#define QTNATIVEUI_NTOGGLESWITCH_P_H

#include <QtNativeUI/NToggleSwitch.h>
#include "QtNativeUI/NEnums.h"
#include <QPropertyAnimation>

class NToggleSwitchPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal thumbCenterX READ thumbCenterX WRITE setThumbCenterX)
    Q_PROPERTY(qreal thumbRadius READ thumbRadius WRITE setThumbRadius)
    Q_PROPERTY_CREATE_D(int, TrackBorderRadius)
    Q_PROPERTY_CREATE_D(int, TrackBorderWidth)
    Q_PROPERTY_CREATE_D(QColor, LightTrackDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightTrackBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightTrackCheckedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackCheckedColor)
    Q_PROPERTY_CREATE_D(QColor, LightThumbDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkThumbDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightThumbCheckedColor)
    Q_PROPERTY_CREATE_D(QColor, DarkThumbCheckedColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)

  public:
    explicit NToggleSwitchPrivate(QObject* parent = nullptr);
    ~NToggleSwitchPrivate();
    Q_D_CREATE(NToggleSwitch)

    qreal thumbCenterX() const { return _thumbCenterX; }
    void setThumbCenterX(qreal value) { _thumbCenterX = value; }
    
    qreal thumbRadius() const { return _thumbRadius; }
    void setThumbRadius(qreal value) { _thumbRadius = value; }

  public:
    bool                  _isPressed{false};
    bool                  _isHovered{false};
    bool                  _isDragging{false};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    // 开关轨道尺寸
    int _trackWidth{40};
    int _trackHeight{20};
    int _trackMargin{1};
    // 滑块尺寸
    int _thumbSize{14};
    // 文本间距
    int _spacing{8};
    
    // 动画相关
    qreal _thumbCenterX{0};  // 滑块中心X坐标
    qreal _thumbRadius{0};   // 滑块半径
    int _lastMouseX{0};      // 上次鼠标X坐标
    
    // 动画对象
    QPropertyAnimation* _thumbPosAnimation{nullptr};
    QPropertyAnimation* _thumbRadiusAnimation{nullptr};

    // 强调色
    QColor _accentDefaultColor;
    QColor _accentDisabledColor;
    QColor _accentTextColor;
    QColor _accentDisabledTextColor;
    
    // 辅助方法
    void startThumbPosAnimation(qreal startX, qreal endX, bool isChecked);
    void startThumbRadiusAnimation(qreal startRadius, qreal endRadius);
    void adjustThumbCenterX();
};

#endif // QTNATIVEUI_NTOGGLESWITCH_P_H