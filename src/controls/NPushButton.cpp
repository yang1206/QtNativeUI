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
NPushButton::NPushButton(QWidget* parent) : QPushButton(parent), d_ptr(new NPushButtonPrivate()) {
    Q_D(NPushButton);
    d->q_ptr               = this;
    d->_pBorderRadius      = 3;
    d->_themeMode          = nTheme->themeMode();
    d->_pLightDefaultColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkDefaultColor  = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightHoverColor   = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Light);
    d->_pDarkHoverColor    = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Dark);
    d->_pLightPressColor   = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Light);
    d->_pDarkPressColor    = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Dark);
    d->_lightTextColor     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_darkTextColor      = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

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

NPushButton::NPushButton(QString text, QWidget* parent) : NPushButton(parent) { setText(text); }

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

void NPushButton::mousePressEvent(QMouseEvent* event) {
    Q_D(NPushButton);
    d->_isPressed = true;
    QPushButton::mousePressEvent(event);
}

void NPushButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NPushButton);
    d->_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void NPushButton::paintEvent(QPaintEvent* event) {
    Q_D(NPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    // eTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);
    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);
    if (d->_themeMode == NThemeType::Light) {
        painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light));
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pLightPressColor
                                                     : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor)
                                     : NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Light));
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pDarkPressColor
                                                     : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor)
                                     : NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Dark));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed) {
        painter.setPen(NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius,
                         height() - d->_shadowBorderWidth,
                         foregroundRect.width(),
                         height() - d->_shadowBorderWidth);
    }
    // 文字绘制
    painter.setPen(isEnabled() ? d->_themeMode == NThemeType::Light ? d->_lightTextColor : d->_darkTextColor
                               : NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light));
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}