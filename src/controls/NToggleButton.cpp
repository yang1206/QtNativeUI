#include <QEnterEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NToggleButton.h>
#include "../private/ntogglebutton_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NToggleButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, DarkPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, LightTextDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, DarkTextDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, LightTextPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, DarkTextPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, AccentDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, AccentHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, AccentPressColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, AccentDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, AccentTextColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QColor, AccentDisabledTextColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleButton, QString, Text)
NToggleButton::NToggleButton(QWidget* parent) : QWidget(parent), d_ptr(new NToggleButtonPrivate()) { init(); }

NToggleButton::NToggleButton(const QString& text, QWidget* parent) : NToggleButton(parent) { setText(text); }

NToggleButton::~NToggleButton() {}

void NToggleButton::init() {
    Q_D(NToggleButton);
    d->q_ptr               = this;
    d->_pBorderRadius      = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_themeMode          = nTheme->themeMode();
    d->_isDark             = nTheme->isDarkMode();
    d->_pLightDefaultColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkDefaultColor  = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightHoverColor   = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Light);
    d->_pDarkHoverColor    = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Dark);
    d->_pLightPressColor   = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Light);
    d->_pDarkPressColor    = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Dark);

    d->_pLightTextDefaultColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextDefaultColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightTextPressColor   = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkTextPressColor    = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);

    d->_pLightBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    updateAccentColors();

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_OpaquePaintEvent);

    setMinimumHeight(32);

    int horizontalSpacing = NDesignToken(NDesignTokenKey::SpacingL).toInt();
    int verticalSpacing   = NDesignToken(NDesignTokenKey::SpacingS).toInt();
    setContentsMargins(horizontalSpacing, verticalSpacing, horizontalSpacing, verticalSpacing);

    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);
    setObjectName("NToggleButton");
    setStyleSheet("#NToggleButton{background-color:transparent;}");
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NToggleButton);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        updateAccentColors();
        updateFluentIcon();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        updateAccentColors();
        update();
    });
}

void NToggleButton::setChecked(bool checked) {
    Q_D(NToggleButton);
    if (d->_checked != checked) {
        d->_checked = checked;
        updateFluentIcon();
        update();
        emit toggled(checked);
    }
}

bool NToggleButton::isChecked() const {
    Q_D(const NToggleButton);
    return d->_checked;
}

void NToggleButton::toggle() { setChecked(!isChecked()); }

QSize NToggleButton::sizeHint() const {
    Q_D(const NToggleButton);
    QFontMetrics fm(font());
    int          width  = 0;
    int          height = 32;

    // 计算文本宽度
    if (!d->_pText.isEmpty()) {
        width += fm.horizontalAdvance(d->_pText);
    }

    // 添加图标宽度
    if (!d->_icon.isNull()) {
        width += d->_iconSize.width();
        if (!d->_pText.isEmpty()) {
            width += 4;
        }
    }

    // 添加内边距
    QMargins margins = contentsMargins();
    width += margins.left() + margins.right();
    height += margins.top() + margins.bottom();

    return QSize(width, height);
}

QSize NToggleButton::minimumSizeHint() const {
    return QSize(32, 32); // 最小尺寸
}

void NToggleButton::setFluentIcon(NRegularIconType::Icon icon, int size, const QColor& color) {
    Q_D(NToggleButton);
    d->_fluentIcon.isRegular   = true;
    d->_fluentIcon.iconCode    = static_cast<quint32>(icon);
    d->_fluentIcon.size        = size;
    d->_fluentIcon.customColor = color;

    updateFluentIcon();
}

void NToggleButton::setFluentIcon(NFilledIconType::Icon icon, int size, const QColor& color) {
    Q_D(NToggleButton);
    d->_fluentIcon.isRegular   = false;
    d->_fluentIcon.iconCode    = static_cast<quint32>(icon);
    d->_fluentIcon.size        = size;
    d->_fluentIcon.customColor = color;
    updateFluentIcon();
}

QIcon NToggleButton::icon() const {
    Q_D(const NToggleButton);
    return d->_icon;
}

void NToggleButton::setIconSize(const QSize& size) {
    Q_D(NToggleButton);
    d->_iconSize = size;
    update();
}

QSize NToggleButton::iconSize() const {
    Q_D(const NToggleButton);
    return d->_iconSize;
}

void NToggleButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    Q_D(NToggleButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 根据按钮状态选择阴影层级
    NDesignTokenKey::Key elevationKey = NDesignTokenKey::ElevationRest;
    if (!isEnabled()) {
        elevationKey = NDesignTokenKey::ElevationNone;
    } else if (d->_isPressed) {
        elevationKey = NDesignTokenKey::ElevationRest;
    } else if (underMouse()) {
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

void NToggleButton::mousePressEvent(QMouseEvent* event) {
    Q_D(NToggleButton);
    d->_isPressed = true;
    update();
    QWidget::mouseReleaseEvent(event);
}

void NToggleButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NToggleButton);
    d->_isPressed = false;
    if (rect().contains(event->pos())) {
        toggle();
        emit clicked(d->_checked);
    }
    QWidget::mouseReleaseEvent(event);
}

bool NToggleButton::event(QEvent* event) {
    switch (event->type()) {
        case QEvent::Enter:
        case QEvent::Leave: {
            update();
            break;
        }
        default: {
            break;
        }
    }
    return QWidget::event(event);
}

void NToggleButton::drawBackground(QPainter* painter) {
    Q_D(NToggleButton);
    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    QColor bgColor;
    if (d->_checked) {
        // 选中状态使用强调色
        if (!isEnabled()) {
            bgColor = d->_pAccentDisabledColor;
        } else if (d->_isPressed) {
            bgColor = d->_pAccentPressColor;
        } else if (underMouse()) {
            bgColor = d->_pAccentHoverColor;
        } else {
            bgColor = d->_pAccentDefaultColor;
        }
    } else {
        // 未选中状态使用普通颜色
        if (!isEnabled()) {
            bgColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode);
        } else if (d->_isPressed) {
            bgColor = d->_isDark ? d->_pDarkPressColor : d->_pLightPressColor;
        } else if (underMouse()) {
            bgColor = d->_isDark ? d->_pDarkHoverColor : d->_pLightHoverColor;
        } else {
            bgColor = d->_isDark ? d->_pDarkDefaultColor : d->_pLightDefaultColor;
        }
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    if (!d->_isPressed && !d->_checked) {
        painter->setPen(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, d->_themeMode));
        painter->drawLine(foregroundRect.x() + d->_pBorderRadius,
                          height() - d->_shadowBorderWidth,
                          foregroundRect.width(),
                          height() - d->_shadowBorderWidth);
    }

    painter->restore();
}

void NToggleButton::drawBorder(QPainter* painter) {
    Q_D(NToggleButton);

    // 选中状态不绘制边框
    if (d->_checked) {
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

void NToggleButton::drawIcon(QPainter* painter) {
    Q_D(NToggleButton);
    if (d->_icon.isNull())
        return;

    painter->save();

    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    // 计算图标位置
    QRect iconRect;
    QSize iconSize = d->_iconSize;

    if (d->_pText.isEmpty()) {
        iconRect = QRect(foregroundRect.x() + (foregroundRect.width() - iconSize.width()) / 2,
                         foregroundRect.y() + (foregroundRect.height() - iconSize.height()) / 2,
                         iconSize.width(),
                         iconSize.height());
    } else {
        int spacing    = 4;
        int textWidth  = painter->fontMetrics().horizontalAdvance(d->_pText);
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

    QPixmap pixmap = d->_icon.pixmap(pixmapSize);
    pixmap.setDevicePixelRatio(dpr);
    painter->drawPixmap(iconRect, pixmap);

    painter->restore();
}

void NToggleButton::drawText(QPainter* painter) {
    Q_D(NToggleButton);
    if (d->_pText.isEmpty())
        return;

    painter->save();
    QRect foregroundRect(d->_shadowBorderWidth,
                         d->_shadowBorderWidth,
                         width() - 2 * (d->_shadowBorderWidth),
                         height() - 2 * d->_shadowBorderWidth);

    QColor textColor;

    if (d->_checked) {
        // 选中状态使用强调色文本颜色
        textColor = isEnabled() ? d->_pAccentTextColor : d->_pAccentDisabledTextColor;
    } else {
        // 未选中状态使用普通文本颜色
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

    if (d->_fluentIcon.iconCode != 0) {
        int   spacing    = 4;
        QSize iconSize   = d->_iconSize;
        int   textWidth  = painter->fontMetrics().horizontalAdvance(d->_pText);
        int   totalWidth = iconSize.width() + spacing + textWidth;

        int   startX = foregroundRect.x() + (foregroundRect.width() - totalWidth) / 2;
        QRect textRect(startX + iconSize.width() + spacing, foregroundRect.y(), textWidth, foregroundRect.height());

        painter->drawText(textRect, Qt::AlignCenter, d->_pText);
    } else {
        // 无图标时居中显示
        painter->drawText(foregroundRect, Qt::AlignCenter, d->_pText);
    }

    painter->restore();
}

void NToggleButton::updateAccentColors() {
    Q_D(NToggleButton);

    NAccentColor accentColor = nTheme->accentColor();

    d->_pAccentDefaultColor  = accentColor.normal();
    d->_pAccentHoverColor    = accentColor.light();
    d->_pAccentPressColor    = accentColor.dark();
    d->_pAccentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_pAccentTextColor         = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, d->_themeMode);
    d->_pAccentDisabledTextColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorDisabled, d->_themeMode);
}

void NToggleButton::updateFluentIcon() {
    Q_D(NToggleButton);

    // 确定图标颜色
    QColor iconColor;
    if (!d->_fluentIcon.customColor.isValid()) {
        // 如果没有自定义颜色，使用文本颜色
        if (d->_checked) {
            iconColor = isEnabled() ? d->_pAccentTextColor : d->_pAccentDisabledTextColor;
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
        d->_icon = (nIcon->fromRegular(
            static_cast<NRegularIconType::Icon>(d->_fluentIcon.iconCode), d->_fluentIcon.size, iconColor));
    } else {
        d->_icon = (nIcon->fromFilled(
            static_cast<NFilledIconType::Icon>(d->_fluentIcon.iconCode), d->_fluentIcon.size, iconColor));
    }

    setIconSize(QSize(d->_fluentIcon.size, d->_fluentIcon.size));
}