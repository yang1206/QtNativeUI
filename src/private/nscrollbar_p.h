//
// Created by Yang1206 on 2025/4/6.
//

#ifndef NSCROLLBAR_P_H
#define NSCROLLBAR_P_H

#include <QtNativeUI/NScrollBar.h>
#include "QtNativeUI/NEnums.h"
#include "nscrollbarstyle.h"

class NScrollBarPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, LightTrackColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTrackColor)
    Q_PROPERTY_CREATE_D(QColor, LightThumbColor)
    Q_PROPERTY_CREATE_D(QColor, DarkThumbColor)
    Q_PROPERTY_CREATE_D(QColor, LightThumbHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkThumbHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightThumbPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkThumbPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightArrowColor)
    Q_PROPERTY_CREATE_D(QColor, DarkArrowColor)
    Q_PROPERTY_CREATE_D(QColor, LightArrowHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkArrowHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightArrowPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkArrowPressColor)
    Q_PROPERTY_CREATE_D(int, ThumbThickness)
    Q_PROPERTY_CREATE_D(int, ThumbNormalThickness)
    Q_PROPERTY_CREATE_D(int, TrackThickness)
    Q_PROPERTY_CREATE_D(int, ThumbCornerRadius)
    Q_PROPERTY_CREATE_D(int, TrackCornerRadius)

  public:
    explicit NScrollBarPrivate(QObject* parent = nullptr);
    ~NScrollBarPrivate();
    Q_D_CREATE(NScrollBar)

    NScrollBarStyle*      style{nullptr};
    bool                  isHovered{false};
    bool                  isPressed{false};
    bool                  isDark{false};
    NThemeType::ThemeMode themeMode;

    QStyle::SubControl   hoveredSubControl{QStyle::SC_None};
    QStyle::SubControl   pressedSubControl{QStyle::SC_None};
    QScrollBar*          _originScrollBar{nullptr};
    QAbstractScrollArea* _originScrollArea{nullptr};

    void updateStyle();
    void initScrollBarStyle();
    void _initAllConfig();
    void _handleScrollBarValueChanged(QScrollBar* scrollBar, int value);
    void _handleScrollBarRangeChanged(int min, int max);
    void _handleScrollBarGeometry();
};

#endif // NSCROLLBAR_P_H