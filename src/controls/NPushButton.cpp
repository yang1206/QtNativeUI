#include <QEnterEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NPushButton.h>
#include "../private/npushbutton_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NPushButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkPressColor)
NPushButton::NPushButton(QWidget* parent) : QPushButton(parent), d_ptr(new NPushButtonPrivate(this)) {
    Q_D(NPushButton);
    d->q_ptr               = this;
    d->_pBorderRadius      = 3;
    d->_themeMode          = nTheme->themeMode();
    d->_pLightDefaultColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    // d->_pDarkDefaultColor  = ElaThemeColor(ElaThemeType::Dark, BasicBase);
    // d->_pLightHoverColor   = ElaThemeColor(ElaThemeType::Light, BasicHover);
    // d->_pDarkHoverColor    = ElaThemeColor(ElaThemeType::Dark, BasicHover);
    // d->_pLightPressColor   = ElaThemeColor(ElaThemeType::Light, BasicPress);
    // d->_pDarkPressColor    = ElaThemeColor(ElaThemeType::Dark, BasicPress);
    // d->_lightTextColor     = ElaThemeColor(ElaThemeType::Light, BasicText);
    // d->_darkTextColor      = ElaThemeColor(ElaThemeType::Dark, BasicText);
    setMouseTracking(true);
    setFixedHeight(38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("NPushButton");
    setStyleSheet("#NPushButton{background-color:transparent;}");
    connect(
        nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

NPushButton::NPushButton(const QString& text, QWidget* parent) : QPushButton(text, parent) { setText(text); }

NPushButton::~NPushButton() {}

void NPushButton::setLightTextColor(QColor color) {
    Q_D(NPushButton);
    d->_lightTextColor = color;
}

QColor NPushButton::getLightTextColor() const {
    Q_D(const NPushButton);
    return d->_lightTextColor;
}

void NPushButton::setDarkTextColor(QColor color) {
    Q_D(NPushButton);
    d->_darkTextColor = color;
}

QColor NPushButton::getDarkTextColor() const {
    Q_D(const NPushButton);
    return d->_darkTextColor;
}

void NPushButton::paintEvent(QPaintEvent* event) {
    Q_D(NPushButton);
    QPushButton::paintEvent(event);
}