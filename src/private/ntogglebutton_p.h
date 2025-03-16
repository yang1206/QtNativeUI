#ifndef QTNATIVEUI_NTOGGLEBUTTON_P_H
#define QTNATIVEUI_NTOGGLEBUTTON_P_H

#include <QtNativeUI/NToggleButton.h>
#include "QtNativeUI/NEnums.h"

class NToggleButtonPrivate : public QObject {
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
    Q_PROPERTY_CREATE_D(QString, Text)

  public:
    explicit NToggleButtonPrivate(QObject* parent = nullptr);
    ~NToggleButtonPrivate();
    Q_D_CREATE(NToggleButton)

  private:
    QIcon                 _icon;
    QSize                 _iconSize{16, 16};
    bool                  _checked{false};
    bool                  _isPressed{false};
    int                   _shadowBorderWidth{3};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;

    QColor _accentDefaultColor;
    QColor _accentHoverColor;
    QColor _accentPressColor;
    QColor _accentDisabledColor;
    QColor _accentTextColor;
    QColor _accentDisabledTextColor;

    // 存储图标信息
    struct FluentIconInfo {
        bool    isRegular = true;
        quint32 iconCode  = 0;
        int     size      = 24;
        QColor  customColor;
    };

    FluentIconInfo _fluentIcon;
};

#endif // QTNATIVEUI_NTOGGLEBUTTON_P_H