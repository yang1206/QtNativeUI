#ifndef QTNATIVEUI_NTOGGLESWITCH_P_H
#define QTNATIVEUI_NTOGGLESWITCH_P_H

#include <QtNativeUI/NToggleSwitch.h>
#include "QtNativeUI/NEnums.h"

class NToggleSwitchPrivate : public QObject {
    Q_OBJECT
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

  public:
    bool                  _isPressed{false};
    bool                  _isHovered{false};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    // 开关轨道尺寸
    int _trackWidth{40};
    int _trackHeight{20};
    // 滑块尺寸
    int _thumbSize{14};
    // 文本间距
    int _spacing{8};
    // 动画参数
    qreal _thumbPosition{0.0}; // 0.0-1.0范围内的滑块位置

    // 强调色
    QColor _accentDefaultColor;
    QColor _accentDisabledColor;
    QColor _accentTextColor;
    QColor _accentDisabledTextColor;
};

#endif // QTNATIVEUI_NTOGGLESWITCH_P_H