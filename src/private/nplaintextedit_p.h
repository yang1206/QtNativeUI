#ifndef QTNATIVEUI_NPLAINTEXTEDIT_P_H
#define QTNATIVEUI_NPLAINTEXTEDIT_P_H

#include <QtNativeUI/NPlainTextEdit.h>
#include "QtNativeUI/NEnums.h"
#include "neditstyle.h"

class NPlainTextEditPrivate : public QObject, public NEditStyleInterface {
    Q_OBJECT
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundFocusColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundFocusColor)
    Q_PROPERTY_CREATE_D(QColor, LightBackgroundDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderDisabledColor)

    Q_PROPERTY_CREATE_D(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBottomLineColor)

    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextDisabledColor)

    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, BorderWidth)

  public:
    explicit NPlainTextEditPrivate(QObject* parent = nullptr);
    ~NPlainTextEditPrivate();
    Q_D_CREATE(NPlainTextEdit)

    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};
    int                   _shadowBorderWidth{3};

    NEditStyle* _plainTextEditStyle{nullptr};

    QColor backgroundColorForState(bool isDark, bool isEnabled, bool hasFocus, bool hasHover) const override;
    QColor borderColorForState(bool isDark, bool isEnabled) const override;
    QColor bottomLineColorForState(bool isDark, bool isEnabled, bool hasFocus) const override;
    QColor textColorForState(bool isDark, bool isEnabled) const override;
    int    borderRadius() const override;
    int    borderWidth() const override;
    int    bottomLineWidth(bool hasFocus) const override;
    bool   isDarkMode() const override;
    int    shadowBorderWidth() const override;
};

#endif // QTNATIVEUI_NPLAINTEXTEDIT_P_H