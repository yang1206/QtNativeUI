//
// Created by Yang1206 on 2025/4/5.
//

#ifndef NTOOLBUTTON_P_H
#define NTOOLBUTTON_P_H
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NToolButton.h>

class NToolButtonPrivate : public QObject {
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
    Q_PROPERTY_CREATE_D(bool, TransparentBackground)

  public:
    explicit NToolButtonPrivate(QObject* parent = nullptr);
    ~NToolButtonPrivate();
    Q_D_CREATE(NToolButton)

  public:
    enum ButtonType {
        Standard, // 标准按钮
        Accent    // 强调色按钮
    };
    Q_ENUM(ButtonType)

  public:
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
        bool    isFluentIcon = false;
    };

    FluentIconInfo _fluentIcon;

    // 文本图标默认间距
    int _iconTextSpacing{4};
};

#endif // NTOOLBUTTON_P_H