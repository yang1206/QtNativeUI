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
    d->_pBorderRadius      = nTheme->getToken("cornerRadiusDefault").toInt();
    d->_themeMode          = nTheme->themeMode();
    d->_isDark             = nTheme->isDarkMode();
    d->_pLightDefaultColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkDefaultColor  = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightHoverColor   = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Light);
    d->_pDarkHoverColor    = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Dark);
    d->_pLightPressColor   = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Light);
    d->_pDarkPressColor    = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Dark);
    d->_lightTextColor     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_darkTextColor      = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    // 初始化边框颜色
    d->_lightBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_darkBorderColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    setMouseTracking(true);
    setFixedHeight(38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("NPushButton");
    setStyleSheet("#NPushButton{background-color:transparent;}");
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

NPushButton::NPushButton(QString text, QWidget* parent) : NPushButton(parent) { setText(text); }

NPushButton::~NPushButton() {}

void NPushButton::setLightTextColor(QColor color) {
    Q_D(NPushButton);
    d->_lightTextColor = color;
    update();
}

QColor NPushButton::getLightTextColor() const {
    Q_D(const NPushButton);
    return d->_lightTextColor;
}

void NPushButton::setDarkTextColor(QColor color) {
    Q_D(NPushButton);
    d->_darkTextColor = color;
    update();
}

QColor NPushButton::getDarkTextColor() const {
    Q_D(const NPushButton);
    return d->_darkTextColor;
}

void NPushButton::setShowBorder(bool show) {
    Q_D(NPushButton);
    d->_showBorder = show;
    update();
}

bool NPushButton::showBorder() const {
    Q_D(const NPushButton);
    return d->_showBorder;
}

void NPushButton::enterEvent(QEnterEvent* event) {
    Q_D(NPushButton);
    d->_isHovered = true;
    update();
    QPushButton::enterEvent(event);
}

void NPushButton::leaveEvent(QEvent* event) {
    Q_D(NPushButton);
    d->_isHovered = false;
    update();
    QPushButton::leaveEvent(event);
}

void NPushButton::mousePressEvent(QMouseEvent* event) {
    Q_D(NPushButton);
    d->_isPressed = true;
    update();
    QPushButton::mousePressEvent(event);
}

void NPushButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NPushButton);
    d->_isPressed = false;
    update();
    QPushButton::mouseReleaseEvent(event);
}

void NPushButton::paintEvent(QPaintEvent* event) {
    Q_D(NPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 绘制阴影（如果需要）
    nTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 绘制按钮背景
    drawBackground(&painter);

    // 绘制边框
    drawBorder(&painter);

    // 绘制文本
    drawText(&painter);
}

void NPushButton::drawBackground(QPainter* painter) {
    Q_D(NPushButton);
    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    // 根据当前状态和主题选择背景颜色
    QColor bgColor;
    if (!isEnabled()) {
        bgColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode);
    } else if (d->_isPressed) {
        bgColor = d->_isDark ? d->_pDarkPressColor : d->_pLightPressColor;
    } else if (d->_isHovered) {
        bgColor = d->_isDark ? d->_pDarkHoverColor : d->_pLightHoverColor;
    } else {
        bgColor = d->_isDark ? d->_pDarkDefaultColor : d->_pLightDefaultColor;
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    painter->restore();
}

void NPushButton::drawBorder(QPainter* painter) {
    Q_D(NPushButton);
    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    // 设置边框颜色
    QColor borderColor = d->_isDark ? d->_darkBorderColor : d->_lightBorderColor;

    painter->setPen(borderColor);
    painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    if (!d->_isPressed) {
        painter->setPen(borderColor);
        painter->drawLine(foregroundRect.x() + d->_pBorderRadius,
                          height() - d->_shadowBorderWidth,
                          foregroundRect.width(),
                          height() - d->_shadowBorderWidth);
    }

    painter->restore();
}

void NPushButton::drawText(QPainter* painter) {
    Q_D(NPushButton);
    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    // 设置文本颜色
    QColor textColor;
    if (!isEnabled()) {
        textColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
    } else {
        textColor = d->_isDark ? d->_darkTextColor : d->_lightTextColor;
    }

    painter->setPen(textColor);
    painter->drawText(foregroundRect, Qt::AlignCenter, text());

    painter->restore();
}