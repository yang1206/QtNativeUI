#ifndef QTNATIVEUI_NCHECKBOX_P_H
#define QTNATIVEUI_NCHECKBOX_P_H

#include <QPropertyAnimation>
#include <QtNativeUI/NCheckBox.h>
#include "QtNativeUI/NEnums.h"

class NCheckBoxPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderPressColor)
    Q_PROPERTY_CREATE(int, CheckAlpha)

  public:
    explicit NCheckBoxPrivate(QObject* parent = nullptr);
    ~NCheckBoxPrivate();
    Q_D_CREATE(NCheckBox)

    // 启动透明度动画
    void startAlphaAnimation(bool checked);

  private:
    bool                  _isPressed{false};
    bool                  _isHovered{false};
    bool                  _isAnimationFinished{true};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    int _checkBoxSize{21};
    int _spacing{8};

    QColor _accentDefaultColor;
    QColor _accentHoverColor;
    QColor _accentPressColor;
    QColor _accentDisabledColor;
    QColor _accentTextColor;
    QColor _accentDisabledTextColor;

    struct CheckIconInfo {
        NFilledIconType::Icon iconCode = NFilledIconType::Checkmark12Filled;
        int                   size     = 12;
        QColor                color;
    };

    CheckIconInfo _checkIcon;

    QPropertyAnimation* _alphaAnimation{nullptr};
    
    mutable QColor _cachedBackgroundColor;
    mutable QColor _cachedBorderColor;
    mutable QColor _cachedTextColor;
    mutable QColor _cachedIconColor;
    mutable QIcon _cachedCheckIcon;
    mutable bool _backgroundColorCacheValid = false;
    mutable bool _borderColorCacheValid = false;
    mutable bool _textColorCacheValid = false;
    mutable bool _iconColorCacheValid = false;
    mutable bool _checkIconCacheValid = false;
    
    void invalidateColorCache() {
        _backgroundColorCacheValid = false;
        _borderColorCacheValid = false;
        _textColorCacheValid = false;
        _iconColorCacheValid = false;
    }
    
    void invalidateIconCache() {
        _checkIconCacheValid = false;
        _iconColorCacheValid = false;
    }
};

#endif // QTNATIVEUI_NCHECKBOX_P_H