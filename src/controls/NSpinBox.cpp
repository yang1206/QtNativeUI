//
// Created by Yang1206 on 2025/4/10.
//

#include <QtNativeUI/NSpinBox.h>
#include "../private/nspinbox_p.h"
#include "QtNativeUI/NLineEdit.h"
#include "QtNativeUI/NTheme.h"

// 属性创建
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonBgColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonPressColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, LightButtonDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, QColor, DarkButtonDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NSpinBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NSpinBox, int, BorderWidth)

NSpinBox::NSpinBox(QWidget* parent) : QSpinBox(parent), d_ptr(new NSpinBoxPrivate()) { init(); }

NSpinBox::~NSpinBox() {}

void NSpinBox::init() {
    Q_D(NSpinBox);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    // 初始化颜色
    d->_pLightBackgroundColor         = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor          = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightBackgroundHoverColor    = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Light);
    d->_pDarkBackgroundHoverColor     = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Dark);
    d->_pLightBackgroundFocusColor    = NThemeColor(NFluentColorKey::ControlFillColorInputActive, NThemeType::Light);
    d->_pDarkBackgroundFocusColor     = NThemeColor(NFluentColorKey::ControlFillColorInputActive, NThemeType::Dark);
    d->_pLightBackgroundDisabledColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Light);
    d->_pDarkBackgroundDisabledColor  = NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Dark);
    d->_pLightBorderColor             = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor              = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pLightBorderDisabledColor     = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderDisabledColor      = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    d->_pLightBottomLineColor = nTheme->accentColor().normal();
    d->_pDarkBottomLineColor  = nTheme->accentColor().normal();
    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    // 按钮颜色
    d->_pLightButtonBgColor       = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkButtonBgColor        = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightButtonHoverColor    = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Light);
    d->_pDarkButtonHoverColor     = NThemeColor(NFluentColorKey::SubtleFillColorTertiary, NThemeType::Dark);
    d->_pLightButtonPressColor    = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkButtonPressColor     = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightButtonDisabledColor = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Light);
    d->_pDarkButtonDisabledColor  = NThemeColor(NFluentColorKey::SubtleFillColorDisabled, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;

    d->_spinBoxStyle = new NSpinBoxStyle(d, style());
    setStyle(d->_spinBoxStyle);
    setObjectName("NSpinBox");
    lineEdit()->setStyleSheet("border: none; background-color:transparent;");
    lineEdit()->setAlignment(Qt::AlignCenter);

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);

    setButtonSymbols(QAbstractSpinBox::UpDownArrows);

    setMinimumHeight(35);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NSpinBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}
