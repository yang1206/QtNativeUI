#include <QEnterEvent>
#include <QFocusEvent>
#include <QPainter>
#include <QStyle>
#include <QtNativeUI/NLineEdit.h>
#include "../private/nlineedit_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundFocusColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBackgroundDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBackgroundDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBorderDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBorderDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightBottomLineColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkBottomLineColor)

Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, LightTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, QColor, DarkTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NLineEdit, int, BorderWidth)

NLineEdit::NLineEdit(QWidget* parent) : QLineEdit(parent), d_ptr(new NLineEditPrivate()) { init(); }

NLineEdit::NLineEdit(const QString& text, QWidget* parent) : NLineEdit(parent) { setText(text); }

NLineEdit::~NLineEdit() {}

void NLineEdit::init() {
    Q_D(NLineEdit);
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

    d->_pLightBorderColor         = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor          = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pLightBorderDisabledColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderDisabledColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    d->_pLightBottomLineColor = nTheme->accentColor().normal();
    d->_pDarkBottomLineColor  = nTheme->accentColor().normal();
    d->_pLightBottomLineColor = QColor(0x86, 0x86, 0x86);
    d->_pDarkBottomLineColor  = QColor(0x9A, 0x9A, 0x9A);

    d->_pLightTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor          = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkTextDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;

    d->_lineEditStyle = new NLineEditPrivate::Style(d, style());
    setStyle(d->_lineEditStyle);

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);
    setMinimumHeight(35);
    int horizontalSpacing = NDesignToken(NDesignTokenKey::SpacingM).toInt();
    int verticalSpacing   = NDesignToken(NDesignTokenKey::SpacingS).toInt();
    setTextMargins(horizontalSpacing, verticalSpacing, horizontalSpacing, verticalSpacing);
    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);

    setObjectName("NLineEdit");
    setStyleSheet("#QLineEdit{border: none;}");

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NLineEdit);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

void NLineEdit::focusInEvent(QFocusEvent* event) {
    Q_EMIT focusIn(text());
    QLineEdit::focusInEvent(event);
}

void NLineEdit::focusOutEvent(QFocusEvent* event) {
    Q_EMIT focusOut(text());
    QLineEdit::focusOutEvent(event);
}
