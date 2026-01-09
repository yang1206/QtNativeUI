#ifndef NRANGESLIDER_P_H
#define NRANGESLIDER_P_H

#include <QPropertyAnimation>
#include <QtNativeUI/NRangeSlider.h>
#include <QtNativeUI/NToolTip.h>
#include <functional>
#include "QtNativeUI/NEnums.h"

class NRangeSliderPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal lowerThumbScale READ lowerThumbScale WRITE setLowerThumbScale)
    Q_PROPERTY(qreal upperThumbScale READ upperThumbScale WRITE setUpperThumbScale)

    Q_PROPERTY_CREATE_D(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_D(QColor, LightProgressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkProgressColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledTrackColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledTrackColor)
    Q_PROPERTY_CREATE_D(QColor, LightDisabledProgressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDisabledProgressColor)
    Q_PROPERTY_CREATE_D(QColor, LightThumbOuterColor)
    Q_PROPERTY_CREATE_D(QColor, DarkThumbOuterColor)

    Q_PROPERTY_CREATE_D(int, TrackHeight)
    Q_PROPERTY_CREATE_D(int, ThumbDiameter)
    Q_PROPERTY_CREATE_D(int, ThumbInnerDiameter)
    Q_PROPERTY_CREATE_D(int, TrackCornerRadius)

    Q_PROPERTY_CREATE_D(int, Minimum)
    Q_PROPERTY_CREATE_D(int, Maximum)
    Q_PROPERTY_CREATE_D(int, LowerValue)
    Q_PROPERTY_CREATE_D(int, UpperValue)

  public:
    enum DragHandle { None, Lower, Upper };

    explicit NRangeSliderPrivate(QObject* parent = nullptr);
    ~NRangeSliderPrivate();
    Q_D_CREATE(NRangeSlider)

    qreal lowerThumbScale() const { return _lowerThumbScale; }
    void  setLowerThumbScale(qreal scale) {
        _lowerThumbScale = scale;
        q_ptr->update();
    }

    qreal upperThumbScale() const { return _upperThumbScale; }
    void  setUpperThumbScale(qreal scale) {
        _upperThumbScale = scale;
        q_ptr->update();
    }

    void startThumbAnimation(DragHandle handle, qreal startScale, qreal endScale);
    void updateTooltip(DragHandle handle);
    void hideTooltips();

  public:
    Qt::Orientation       _orientation   = Qt::Horizontal;
    DragHandle            _dragHandle    = None;
    DragHandle            _hoveredHandle = None;
    DragHandle            _focusedHandle = Lower;
    bool                  _isPressed     = false;
    bool                  _isDragging    = false;
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark = false;

    qreal _lowerThumbScale = 1.0;
    qreal _upperThumbScale = 1.0;

    QPropertyAnimation* _lowerThumbAnimation = nullptr;
    QPropertyAnimation* _upperThumbAnimation = nullptr;

    QColor _accentColor;
    QColor _accentHoverColor;
    QColor _accentPressedColor;
    QColor _accentDisabledColor;

    // 拖动时的偏移量
    int                         _dragOffset   = 0;
    NToolTip*                   _lowerTooltip = nullptr;
    NToolTip*                   _upperTooltip = nullptr;
    bool                        _showTooltip  = true;
    std::function<QString(int)> _tooltipFormatter;
};

#endif // NRANGESLIDER_P_H
