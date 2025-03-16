#ifndef QTNATIVEUI_NPUSHBUTTON_P_H
#define QTNATIVEUI_NPUSHBUTTON_P_H

#include <QtNativeUI/NPushButton.h>

#include "QtNativeUI/NEnums.h"

class NPushButtonPrivate : public QObject {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPressColor)
  public:
    explicit NPushButtonPrivate(QObject* parent = nullptr);
    ~NPushButtonPrivate();
    Q_D_CREATE(NPushButton)

  public:
    enum ButtonType {
        Standard, // 标准按钮
        Accent    // 强调色按钮
    };
    Q_ENUM(ButtonType)

  private:
    QColor                _lightTextColor;
    QColor                _darkTextColor;
    QColor                _lightBorderColor;
    QColor                _darkBorderColor;
    bool                  _isPressed{false};
    bool                  _isHovered{false};
    int                   _shadowBorderWidth{3};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;
    ButtonType            _buttonType{Standard};

    QColor _accentDefaultColor;
    QColor _accentHoverColor;
    QColor _accentPressColor;
    QColor _accentDisabledColor;
    QColor _accentTextColor;
    QColor _accentDisabledTextColor;

    bool         _useCustomAccent{false};
    NAccentColor _customAccentColor = NAccentColor::fromColor(QColor(0x00, 0x6B, 0xD4));

    // 存储图标信息
    struct FluentIconInfo {
        bool    isRegular = true;
        quint32 iconCode  = 0;
        int     size      = 24;
        QColor  customColor;
        bool    isFluentIcon = false;  // 添加这个标志位来区分是否是 Fluent 图标
    };

    FluentIconInfo _fluentIcon;
};

#endif // QTNATIVEUI_NPUSHBUTTON_P_H