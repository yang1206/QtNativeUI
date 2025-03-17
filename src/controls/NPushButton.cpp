#include <QEnterEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NPushButton.h>
#include "../private/npushbutton_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NPushButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkPressColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightTextDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkTextDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, LightTextPressColor)
Q_PROPERTY_CREATE_Q_CPP(NPushButton, QColor, DarkTextPressColor)

NPushButton::NPushButton(QWidget* parent) : QPushButton(parent), d_ptr(new NPushButtonPrivate()) {
    Q_D(NPushButton);
    d->q_ptr                   = this;
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

    d->_lightBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_darkBorderColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    updateAccentColors();

    setMouseTracking(true);

    setMinimumHeight(32);

    int horizontalSpacing = NDesignToken(NDesignTokenKey::SpacingL).toInt();
    int verticalSpacing   = NDesignToken(NDesignTokenKey::SpacingS).toInt();
    setContentsMargins(horizontalSpacing, verticalSpacing, horizontalSpacing, verticalSpacing);

    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);
    setObjectName("NPushButton");
    setStyleSheet("#NPushButton{background-color:transparent;}");
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NPushButton);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();

        if (d->_buttonType == NPushButtonPrivate::Accent) {
            updateAccentColors();
        }
        updateFluentIcon();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        Q_D(NPushButton);
        if (d->_buttonType == NPushButtonPrivate::Accent) {
            updateAccentColors();
            update();
        }
    });
}

NPushButton::NPushButton(QString text, QWidget* parent) : NPushButton(parent) { setText(text); }

NPushButton::~NPushButton() {}

void NPushButton::setButtonType(ButtonType type) {
    Q_D(NPushButton);
    if (static_cast<NPushButtonPrivate::ButtonType>(type) != d->_buttonType) {
        d->_buttonType = static_cast<NPushButtonPrivate::ButtonType>(type);

        if (type == Accent) {
            updateAccentColors();
        }

        update();
    }
}

NPushButton::ButtonType NPushButton::buttonType() const {
    Q_D(const NPushButton);
    return static_cast<ButtonType>(d->_buttonType);
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

void NPushButton::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange ||
        event->type() == QEvent::LanguageChange) {
        update();
    }
    QPushButton::changeEvent(event);
}

void NPushButton::focusInEvent(QFocusEvent* event) { QPushButton::focusInEvent(event); }

void NPushButton::focusOutEvent(QFocusEvent* event) { QPushButton::focusOutEvent(event); }

void NPushButton::paintEvent([[maybe_unused]] QPaintEvent* event) {
    Q_D(NPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 根据按钮状态选择阴影层级
    NDesignTokenKey::Key elevationKey = NDesignTokenKey::ElevationRest;
    if (!isEnabled()) {
        elevationKey = NDesignTokenKey::ElevationNone;
    } else if (d->_isPressed) {
        elevationKey = NDesignTokenKey::ElevationRest;
    } else if (d->_isHovered) {
        elevationKey = NDesignTokenKey::ElevationHover;
    }

    // 绘制阴影
    nTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius, elevationKey);

    drawBackground(&painter);
    drawBorder(&painter);
    drawIcon(&painter);
    drawText(&painter);
    updateFluentIcon();
}

void NPushButton::drawBackground(QPainter* painter) {
    Q_D(NPushButton);
    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    if (d->_buttonType == NPushButtonPrivate::Accent) {
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
            bgColor = d->_isDark ? d->_pDarkDefaultColor : d->_pLightDefaultColor;
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    }

    if (!d->_isPressed) {
        painter->setPen(d->_isDark ? QColor(0x63, 0x63, 0x63) : QColor(0xD6, 0xD6, 0xD6));
        painter->drawLine(foregroundRect.x() + d->_pBorderRadius,
                          height() - d->_shadowBorderWidth,
                          foregroundRect.width(),
                          height() - d->_shadowBorderWidth);
    }

    painter->restore();
}

void NPushButton::drawBorder(QPainter* painter) {
    Q_D(NPushButton);

    if (d->_buttonType == NPushButtonPrivate::Accent) {
        return;
    }

    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    QColor borderColor = d->_isDark ? d->_darkBorderColor : d->_lightBorderColor;

    painter->setPen(borderColor);
    painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    painter->restore();
}

void NPushButton::drawIcon(QPainter* painter) {
    Q_D(NPushButton);
    if (icon().isNull())
        return;

    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    // 计算图标位置
    QRect iconRect;
    QSize iconSize = this->iconSize();

    if (text().isEmpty()) {
        iconRect = QRect(foregroundRect.x() + (foregroundRect.width() - iconSize.width()) / 2,
                         foregroundRect.y() + (foregroundRect.height() - iconSize.height()) / 2,
                         iconSize.width(),
                         iconSize.height());
    } else {
        int spacing    = 4;
        int textWidth  = painter->fontMetrics().horizontalAdvance(text());
        int totalWidth = iconSize.width() + spacing + textWidth;

        int startX = foregroundRect.x() + (foregroundRect.width() - totalWidth) / 2;
        iconRect   = QRect(startX,
                         foregroundRect.y() + (foregroundRect.height() - iconSize.height()) / 2,
                         iconSize.width(),
                         iconSize.height());
    }

    // 绘制图标
    qreal dpr        = devicePixelRatio();
    QSize pixmapSize = iconSize * dpr;

    QPixmap pixmap = icon().pixmap(pixmapSize);
    pixmap.setDevicePixelRatio(dpr);
    painter->drawPixmap(iconRect, pixmap);

    painter->restore();
}

void NPushButton::drawText(QPainter* painter) {
    Q_D(NPushButton);
    if (text().isEmpty())
        return;

    painter->save();
    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    QColor textColor;

    if (d->_buttonType == NPushButtonPrivate::Accent) {
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

    if (!icon().isNull()) {
        // 有图标时，文本需要右移
        int   spacing    = 4; // 图标与文本间距
        QSize iconSize   = this->iconSize();
        int   textWidth  = painter->fontMetrics().horizontalAdvance(text());
        int   totalWidth = iconSize.width() + spacing + textWidth;

        int   startX = foregroundRect.x() + (foregroundRect.width() - totalWidth) / 2;
        QRect textRect(startX + iconSize.width() + spacing, foregroundRect.y(), textWidth, foregroundRect.height());

        painter->drawText(textRect, Qt::AlignCenter, text());
    } else {
        // 无图标时居中显示
        painter->drawText(foregroundRect, Qt::AlignCenter, text());
    }

    painter->restore();
}

void NPushButton::updateAccentColors() {
    Q_D(NPushButton);

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

void NPushButton::setAccentColor(const NAccentColor& color) {
    Q_D(NPushButton);

    d->_useCustomAccent   = true;
    d->_customAccentColor = color;

    if (d->_buttonType != NPushButtonPrivate::Accent) {
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

void NPushButton::setAccentColor(const QColor& color) { setAccentColor(NColorUtils::toAccentColor(color)); }

void NPushButton::resetAccentColor() {
    Q_D(NPushButton);
    d->_useCustomAccent = false;
    updateAccentColors();
    update();
}

void NPushButton::setFluentIcon(NRegularIconType::Icon icon, int size, const QColor& color) {
    Q_D(NPushButton);
    d->_fluentIcon.isRegular    = true;
    d->_fluentIcon.iconCode     = static_cast<quint32>(icon);
    d->_fluentIcon.size         = size;
    d->_fluentIcon.customColor  = color;
    d->_fluentIcon.isFluentIcon = true;

    updateFluentIcon();
}

void NPushButton::setFluentIcon(NFilledIconType::Icon icon, int size, const QColor& color) {
    Q_D(NPushButton);
    d->_fluentIcon.isRegular    = false;
    d->_fluentIcon.iconCode     = static_cast<quint32>(icon);
    d->_fluentIcon.size         = size;
    d->_fluentIcon.customColor  = color;
    d->_fluentIcon.isFluentIcon = true;
    updateFluentIcon();
}

void NPushButton::updateFluentIcon() {
    Q_D(NPushButton);
    if (!d->_fluentIcon.isFluentIcon) {
        return;
    }
    if (d->_fluentIcon.iconCode == 0) {
        setIcon(QIcon());
        return;
    }

    // 确定图标颜色
    QColor iconColor;
    if (!d->_fluentIcon.customColor.isValid()) {
        // 如果没有自定义颜色，使用文本颜色
        if (d->_buttonType == NPushButtonPrivate::Accent) {
            if (!isEnabled()) {
                iconColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
            } else {
                if (d->_isPressed) {
                    iconColor = d->_isDark ? d->_pDarkTextPressColor : d->_pLightTextPressColor;
                } else {
                    iconColor = d->_isDark ? d->_accentTextColor : d->_accentDisabledTextColor;
                }
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

    // 设置图标
    if (d->_fluentIcon.isRegular) {
        setIcon(nIcon->fromRegular(
            static_cast<NRegularIconType::Icon>(d->_fluentIcon.iconCode), d->_fluentIcon.size, iconColor));
    } else {
        setIcon(nIcon->fromFilled(
            static_cast<NFilledIconType::Icon>(d->_fluentIcon.iconCode), d->_fluentIcon.size, iconColor));
    }

    setIconSize(QSize(d->_fluentIcon.size, d->_fluentIcon.size));
}
