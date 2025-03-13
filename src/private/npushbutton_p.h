#ifndef QTNATIVEUI_NPUSHBUTTON_P_H
#define QTNATIVEUI_NPUSHBUTTON_P_H

#include <QtNativeUI/NPushButton.h>

#include "QtNativeUI/NEnums.h"

class NPushButtonPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NPushButton)
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

  private:
    QColor                _lightTextColor;
    QColor                _darkTextColor;
    QColor                _lightBorderColor;
    QColor                _darkBorderColor;
    bool                  _isPressed{false};
    bool                  _isHovered{false};
    bool                  _showBorder{true};
    int                   _shadowBorderWidth{3};
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark;
};

#endif // QTNATIVEUI_NPUSHBUTTON_P_H