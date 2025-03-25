#ifndef QTNATIVEUI_NCHECKBOX_P_H
#define QTNATIVEUI_NCHECKBOX_P_H

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

  public:
    explicit NCheckBoxPrivate(QObject* parent = nullptr);
    ~NCheckBoxPrivate();
    Q_D_CREATE(NCheckBox)

  private:
    bool                  _isPressed{false};
    bool                  _isHovered{false};
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
};

#endif // QTNATIVEUI_NCHECKBOX_P_H