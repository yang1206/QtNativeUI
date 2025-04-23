//
// Created by Yang1206 on 2025/4/5.
//

#include <QEnterEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NToolButton.h>
#include "../private/ntoolbutton_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NToolButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, DarkPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, LightTextDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, DarkTextDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, LightTextPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, DarkTextPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToolButton, bool, TransparentBackground)

NToolButton::NToolButton(QWidget* parent) : QToolButton(parent), d_ptr(new NToolButtonPrivate()) {
    Q_D(NToolButton);
    d->q_ptr = this;
    init();
}

NToolButton::NToolButton(QString text, QWidget* parent) : NToolButton(parent) { setText(text); }

NToolButton::~NToolButton() {}

void NToolButton::init() {
    Q_D(NToolButton);

    d->_pBorderRadius          = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_themeMode              = nTheme->themeMode();
    d->_isDark                 = nTheme->isDarkMode();
    d->_pLightDefaultColor     = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkDefaultColor      = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightHoverColor       = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Light);
    d->_pDarkHoverColor        = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Dark);
    d->_pLightPressColor       = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Light);
    d->_pDarkPressColor        = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Dark);
    d->_pLightTextDefaultColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextDefaultColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextPressColor   = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkTextPressColor    = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightBorderColor      = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor       = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);
    d->_pTransparentBackground = false;
    updateAccentColors();

    setMouseTracking(true);

    setMinimumHeight(32);

    int horizontalSpacing = NDesignToken(NDesignTokenKey::SpacingL).toInt();
    int verticalSpacing   = NDesignToken(NDesignTokenKey::SpacingS).toInt();
    setContentsMargins(horizontalSpacing, verticalSpacing, horizontalSpacing, verticalSpacing);

    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);
    setObjectName("NToolButton");
    setStyleSheet("#NToolButton{background-color:transparent;}");

    setToolButtonStyle(Qt::ToolButtonIconOnly);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NToolButton);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();

        if (d->_buttonType == NToolButtonPrivate::Accent) {
            updateAccentColors();
        }
        updateFluentIcon();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        Q_D(NToolButton);
        if (d->_buttonType == NToolButtonPrivate::Accent) {
            updateAccentColors();
            update();
        }
    });
}

void NToolButton::setButtonType(ButtonType type) {
    Q_D(NToolButton);
    if (static_cast<NToolButtonPrivate::ButtonType>(type) != d->_buttonType) {
        d->_buttonType = static_cast<NToolButtonPrivate::ButtonType>(type);

        if (type == Accent) {
            updateAccentColors();
        }

        update();
    }
}

NToolButton::ButtonType NToolButton::buttonType() const {
    Q_D(const NToolButton);
    return static_cast<ButtonType>(d->_buttonType);
}

void NToolButton::enterEvent(QEnterEvent* event) {
    Q_D(NToolButton);
    d->_isHovered = true;
    update();
    QToolButton::enterEvent(event);
}

void NToolButton::leaveEvent(QEvent* event) {
    Q_D(NToolButton);
    d->_isHovered = false;
    update();
    QToolButton::leaveEvent(event);
}

void NToolButton::mousePressEvent(QMouseEvent* event) {
    Q_D(NToolButton);
    d->_isPressed = true;
    update();
    QToolButton::mousePressEvent(event);
}

void NToolButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NToolButton);
    d->_isPressed = false;
    update();
    QToolButton::mouseReleaseEvent(event);
}

void NToolButton::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange ||
        event->type() == QEvent::LanguageChange) {
        update();
    }
    QToolButton::changeEvent(event);
}

void NToolButton::focusInEvent(QFocusEvent* event) { QToolButton::focusInEvent(event); }

void NToolButton::focusOutEvent(QFocusEvent* event) { QToolButton::focusOutEvent(event); }

void NToolButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    Q_D(NToolButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    if (!d->_pTransparentBackground) {
        NDesignTokenKey::Key elevationKey = NDesignTokenKey::ElevationRest;
        if (!isEnabled()) {
            elevationKey = NDesignTokenKey::ElevationNone;
        } else if (d->_isPressed) {
            elevationKey = NDesignTokenKey::ElevationRest;
        } else if (d->_isHovered) {
            elevationKey = NDesignTokenKey::ElevationHover;
        }

        nTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius, elevationKey);
    }
    drawBackground(&painter);
    drawBorder(&painter);
    drawIcon(&painter);
    drawText(&painter);
    updateFluentIcon();
}

void NToolButton::drawBackground(QPainter* painter) {
    Q_D(NToolButton);
    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    if (d->_buttonType == NToolButtonPrivate::Accent) {
        QColor bgColor;
        if (!isEnabled()) {
            bgColor = d->_accentDisabledColor;
        } else if (d->_isPressed) {
            bgColor = d->_accentPressColor;
        } else if (d->_isHovered) {
            bgColor = d->_accentHoverColor;
        } else {
            bgColor = d->_accentDefaultColor;
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    } else {
        QColor bgColor;
        if (!isEnabled()) {
            bgColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode);
        } else if (d->_isPressed) {
            bgColor = d->_isDark ? d->_pDarkPressColor : d->_pLightPressColor;
        } else if (d->_isHovered) {
            bgColor = d->_isDark ? d->_pDarkHoverColor : d->_pLightHoverColor;
        } else {
            if (d->_pTransparentBackground) {
                bgColor = Qt::transparent;
            } else {
                bgColor = d->_isDark ? d->_pDarkDefaultColor : d->_pLightDefaultColor;
            }
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    }

    if ((!d->_isPressed)) {
        if (d->_buttonType == NToolButtonPrivate::Accent) {
            painter->restore();
            return;
        }

        if (d->_pTransparentBackground && !d->_isHovered) {
            painter->restore();
            return;
        }

        painter->setPen(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, d->_themeMode));
        painter->drawLine(foregroundRect.x() + d->_pBorderRadius,
                          height() - d->_shadowBorderWidth,
                          foregroundRect.width(),
                          height() - d->_shadowBorderWidth);
    }

    painter->restore();
}

void NToolButton::drawBorder(QPainter* painter) {
    Q_D(NToolButton);
    if (d->_pTransparentBackground && !d->_isHovered && !d->_isPressed &&
        d->_buttonType != NToolButtonPrivate::Accent) {
        return;
    }

    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    QColor borderColor = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;

    painter->setPen(borderColor);
    painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    painter->restore();
}

void NToolButton::drawIcon(QPainter* painter) {
    Q_D(NToolButton);
    if (icon().isNull())
        return;

    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    QRect iconRect;
    QSize iconSize = this->iconSize();

    Qt::ToolButtonStyle tbStyle = toolButtonStyle();

    if (tbStyle == Qt::ToolButtonIconOnly) {
        iconRect = QRect(foregroundRect.x() + (foregroundRect.width() - iconSize.width()) / 2,
                         foregroundRect.y() + (foregroundRect.height() - iconSize.height()) / 2,
                         iconSize.width(),
                         iconSize.height());
    } else if (tbStyle == Qt::ToolButtonTextOnly) {
        painter->restore();
        return;
    } else if (tbStyle == Qt::ToolButtonTextBesideIcon) {
        int textWidth  = fontMetrics().horizontalAdvance(text());
        int totalWidth = iconSize.width() + d->_iconTextSpacing + textWidth;

        int startX = foregroundRect.x() + (foregroundRect.width() - totalWidth) / 2;
        iconRect   = QRect(startX,
                         foregroundRect.y() + (foregroundRect.height() - iconSize.height()) / 2,
                         iconSize.width(),
                         iconSize.height());
    } else if (tbStyle == Qt::ToolButtonTextUnderIcon) {
        int textHeight  = fontMetrics().height();
        int totalHeight = iconSize.height() + d->_iconTextSpacing + textHeight;

        int startY = foregroundRect.y() + (foregroundRect.height() - totalHeight) / 2;
        iconRect   = QRect(foregroundRect.x() + (foregroundRect.width() - iconSize.width()) / 2,
                         startY,
                         iconSize.width(),
                         iconSize.height());
    }

    qreal dpr        = devicePixelRatio();
    QSize pixmapSize = iconSize * dpr;

    QPixmap pixmap = icon().pixmap(pixmapSize);
    pixmap.setDevicePixelRatio(dpr);
    painter->drawPixmap(iconRect, pixmap);

    painter->restore();
}

void NToolButton::drawText(QPainter* painter) {
    Q_D(NToolButton);
    if (text().isEmpty())
        return;

    painter->save();
    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    QColor textColor;

    if (d->_buttonType == NToolButtonPrivate::Accent) {
        textColor = isEnabled() ? d->_accentTextColor : d->_accentDisabledTextColor;
    } else {
        if (!isEnabled()) {
            textColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
        } else {
            if (d->_isPressed) {
                textColor = d->_isDark ? d->_pDarkTextPressColor : d->_pLightTextPressColor;
            } else {
                textColor = d->_isDark ? d->_pDarkTextDefaultColor : d->_pLightTextDefaultColor;
            }
        }
    }

    painter->setPen(textColor);

    Qt::ToolButtonStyle tbStyle = toolButtonStyle();
    QRect               textRect;

    if (tbStyle == Qt::ToolButtonTextOnly) {
        textRect = foregroundRect;
    } else if (tbStyle == Qt::ToolButtonIconOnly) {
        painter->restore();
        return;
    } else if (tbStyle == Qt::ToolButtonTextBesideIcon) {
        QSize iconSize   = this->iconSize();
        int   textWidth  = painter->fontMetrics().horizontalAdvance(text());
        int   totalWidth = iconSize.width() + d->_iconTextSpacing + textWidth;

        int startX = foregroundRect.x() + (foregroundRect.width() - totalWidth) / 2;
        textRect   = QRect(
            startX + iconSize.width() + d->_iconTextSpacing, foregroundRect.y(), textWidth, foregroundRect.height());
    } else if (tbStyle == Qt::ToolButtonTextUnderIcon) {
        QSize iconSize    = this->iconSize();
        int   textHeight  = painter->fontMetrics().height();
        int   totalHeight = iconSize.height() + d->_iconTextSpacing + textHeight;

        int startY = foregroundRect.y() + (foregroundRect.height() - totalHeight) / 2;
        textRect   = QRect(
            foregroundRect.x(), startY + iconSize.height() + d->_iconTextSpacing, foregroundRect.width(), textHeight);
    }

    painter->drawText(textRect, Qt::AlignCenter, text());

    painter->restore();
}

void NToolButton::updateAccentColors() {
    Q_D(NToolButton);

    if (d->_useCustomAccent) {
        return;
    }

    NAccentColor accentColor = nTheme->accentColor();

    d->_accentDefaultColor  = accentColor.normal();
    d->_accentHoverColor    = accentColor.light();
    d->_accentPressColor    = accentColor.dark();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_accentTextColor         = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, d->_themeMode);
    d->_accentDisabledTextColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorDisabled, d->_themeMode);
}

void NToolButton::setAccentColor(const NAccentColor& color) {
    Q_D(NToolButton);

    d->_useCustomAccent   = true;
    d->_customAccentColor = color;

    if (d->_buttonType != NToolButtonPrivate::Accent) {
        setButtonType(Accent);
    }

    d->_accentDefaultColor  = color.normal();
    d->_accentHoverColor    = color.light();
    d->_accentPressColor    = color.dark();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_accentTextColor         = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, d->_themeMode);
    d->_accentDisabledTextColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorDisabled, d->_themeMode);

    update();
}

void NToolButton::setAccentColor(const QColor& color) { setAccentColor(NColorUtils::toAccentColor(color)); }

void NToolButton::resetAccentColor() {
    Q_D(NToolButton);
    d->_useCustomAccent = false;
    updateAccentColors();
    update();
}

void NToolButton::setFluentIcon(NRegularIconType::Icon icon, int size, const QColor& color) {
    Q_D(NToolButton);
    d->_fluentIcon.isRegular    = true;
    d->_fluentIcon.iconCode     = static_cast<quint32>(icon);
    d->_fluentIcon.size         = size;
    d->_fluentIcon.customColor  = color;
    d->_fluentIcon.isFluentIcon = true;

    updateFluentIcon();
}

void NToolButton::setFluentIcon(NFilledIconType::Icon icon, int size, const QColor& color) {
    Q_D(NToolButton);
    d->_fluentIcon.isRegular    = false;
    d->_fluentIcon.iconCode     = static_cast<quint32>(icon);
    d->_fluentIcon.size         = size;
    d->_fluentIcon.customColor  = color;
    d->_fluentIcon.isFluentIcon = true;
    updateFluentIcon();
}

void NToolButton::updateFluentIcon() {
    Q_D(NToolButton);
    if (!d->_fluentIcon.isFluentIcon) {
        return;
    }
    if (d->_fluentIcon.iconCode == 0) {
        setIcon(QIcon());
        return;
    }

    QColor iconColor;
    if (!d->_fluentIcon.customColor.isValid()) {
        if (d->_buttonType == NToolButtonPrivate::Accent) {
            if (!isEnabled()) {
                iconColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
            } else {
                iconColor = d->_accentTextColor;
            }
        } else {
            if (!isEnabled()) {
                iconColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
            } else {
                if (d->_isPressed) {
                    iconColor = d->_isDark ? d->_pDarkTextPressColor : d->_pLightTextPressColor;
                } else {
                    iconColor = d->_isDark ? d->_pDarkTextDefaultColor : d->_pLightTextDefaultColor;
                }
            }
        }
    } else {
        iconColor = d->_fluentIcon.customColor;
    }

    if (d->_fluentIcon.isRegular) {
        setIcon(nIcon->fromRegular(
            static_cast<NRegularIconType::Icon>(d->_fluentIcon.iconCode), d->_fluentIcon.size, iconColor));
    } else {
        setIcon(nIcon->fromFilled(
            static_cast<NFilledIconType::Icon>(d->_fluentIcon.iconCode), d->_fluentIcon.size, iconColor));
    }

    setIconSize(QSize(d->_fluentIcon.size, d->_fluentIcon.size));
}
