#ifndef QTNATIVEUI_NTOGGLESWITCH_P_H
#define QTNATIVEUI_NTOGGLESWITCH_P_H

#include <QPropertyAnimation>
#include <QtNativeUI/NToggleSwitch.h>
#include "QtNativeUI/NEnums.h"

class NToggleSwitchPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal thumbCenterX READ thumbCenterX WRITE setThumbCenterX)
    Q_PROPERTY(qreal thumbRadius READ thumbRadius WRITE setThumbRadius)
    Q_PROPERTY(qreal thumbStretchFactor READ thumbStretchFactor WRITE setThumbStretchFactor)
    Q_PROPERTY_CREATE_D(int, TrackBorderRadius)
    Q_PROPERTY_CREATE_D(int, TrackBorderWidth)
    Q_PROPERTY_CREATE_D(QColor, LightTrackDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightTrackBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackBorderColor)
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
    void  setThumbCenterX(qreal value) { _thumbCenterX = value; }

    qreal thumbRadius() const { return _thumbRadius; }
    void  setThumbRadius(qreal value) { _thumbRadius = value; }

    qreal thumbStretchFactor() const { return _thumbStretchFactor; }
    void  setThumbStretchFactor(qreal value) {
        _thumbStretchFactor = value;
        q_ptr->update();
    }

  private:
    bool                  _isPressed{false};
    bool                  _isHovered{false};
    bool                  _isDragging{false};
    bool                  _isChecked{false};
    bool                  _visualChecked{false};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    int _trackWidth{40};
    int _trackHeight{20};
    int _trackMargin{1};
    int _thumbSize{14};
    int _spacing{8};

    qreal _thumbCenterX{0};
    qreal _thumbRadius{0};
    int   _lastMouseX{0};

    QPropertyAnimation* _thumbPosAnimation{nullptr};
    QPropertyAnimation* _thumbRadiusAnimation{nullptr};

    QColor _accentDefaultColor;
    QColor _accentDisabledColor;
    QColor _accentHoverColor;
    QColor _accentPressedColor;
    QColor _accentTextColor;
    QColor _accentDisabledTextColor;

    qreal               _thumbStretchFactor{1.0};
    QPropertyAnimation* _thumbStretchAnimation{nullptr};

    void startThumbPosAnimation(qreal startX, qreal endX, bool isChecked);
    void startThumbRadiusAnimation(qreal startRadius, qreal endRadius);
    void adjustThumbCenterX();
    void startThumbStretchAnimation(bool pressed);
};

#endif // QTNATIVEUI_NTOGGLESWITCH_P_H