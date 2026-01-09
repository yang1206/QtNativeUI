#ifndef QTNATIVEUI_NTOGGLEBUTTON_P_H
#define QTNATIVEUI_NTOGGLEBUTTON_P_H

#include <QtNativeUI/NToggleButton.h>
#include "QtNativeUI/NEnums.h"

class QPropertyAnimation;
class NToggleButtonPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextPressColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, AccentDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, AccentHoverColor)
    Q_PROPERTY_CREATE_D(QColor, AccentPressColor)
    Q_PROPERTY_CREATE_D(QColor, AccentDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, AccentTextColor)
    Q_PROPERTY_CREATE_D(QColor, AccentDisabledTextColor)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE(int, CheckAlpha)

  public:
    explicit NToggleButtonPrivate(QObject* parent = nullptr);
    ~NToggleButtonPrivate();
    Q_D_CREATE(NToggleButton)

    void startAlphaAnimation(bool checked);

  private:
    QIcon                 _icon;
    QSize                 _iconSize{16, 16};
    bool                  _checked{false};
    bool                  _isPressed{false};
    bool                  _isAnimationFinished{true};
    int                   _shadowBorderWidth{3};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    QPropertyAnimation* _alphaAnimation{nullptr};

    struct FluentIconInfo {
        bool    isRegular = true;
        quint32 iconCode  = 0;
        int     size      = 24;
        QColor  customColor;
    };

    FluentIconInfo _fluentIcon;
    

    mutable QColor _cachedBackgroundColor;
    mutable QColor _cachedTextColor;
    mutable QColor _cachedBorderColor;
    mutable bool _backgroundColorCacheValid = false;
    mutable bool _textColorCacheValid = false;
    
    void invalidateColorCache() { 
        _backgroundColorCacheValid = false; 
        _textColorCacheValid = false; 
    }
};

#endif // QTNATIVEUI_NTOGGLEBUTTON_P_H