#include <QEvent>
#include <QPainter>
#include <QtNativeUI/NCheckBox.h>
#include "../private/ncheckbox_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NCheckBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, DarkPressColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, LightBorderHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, DarkBorderHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, LightBorderPressColor)
Q_PROPERTY_CREATE_Q_CPP(NCheckBox, QColor, DarkBorderPressColor)

NCheckBox::NCheckBox(QWidget* parent) : QCheckBox(parent), d_ptr(new NCheckBoxPrivate()) { init(); }

NCheckBox::NCheckBox(const QString& text, QWidget* parent) : NCheckBox(parent) { setText(text); }

NCheckBox::~NCheckBox() {}

void NCheckBox::init() {
    Q_D(NCheckBox);
    d->q_ptr = this;

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_themeMode     = nTheme->themeMode();
    d->_isDark        = nTheme->isDarkMode();

    d->_pLightDefaultColor     = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkDefaultColor      = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    d->_pLightHoverColor       = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Light);
    d->_pDarkHoverColor        = NThemeColor(NFluentColorKey::ControlFillColorSecondary, NThemeType::Dark);
    d->_pLightPressColor       = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Light);
    d->_pDarkPressColor        = NThemeColor(NFluentColorKey::ControlFillColorTertiary, NThemeType::Dark);
    d->_pLightTextColor        = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor         = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightBorderColor      = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor       = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Dark);
    d->_pLightBorderHoverColor = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderHoverColor  = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Dark);
    d->_pLightBorderPressColor = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderPressColor  = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Dark);

    updateAccentColors();

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setObjectName("NCheckBox");
    setStyleSheet(R"(
        #NCheckBox {
            background-color: transparent;
            spacing: 0px;  /* 清除默认间距 */
        }
        #NCheckBox::indicator {
            width: 0px;    /* 隐藏默认的指示器 */
            height: 0px;
        }
    )");
    setMinimumHeight(d->_checkBoxSize);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NCheckBox);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        updateAccentColors();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        updateAccentColors();
        update();
    });
    d->_checkIcon.size = 12;
    d->_spacing        = 8;
    setTristate(false);
}

void NCheckBox::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    drawCheckBox(&painter);
    drawText(&painter);
}

void NCheckBox::drawCheckBox(QPainter* painter) {
    Q_D(NCheckBox);
    painter->save();

    QRect checkBoxRect(0, (height() - d->_checkBoxSize) / 2, d->_checkBoxSize, d->_checkBoxSize);

    QRect innerRect = checkBoxRect.adjusted(1, 1, -1, -1);

    QColor bgColor;
    QColor borderColor;

    if (isChecked() || checkState() == Qt::PartiallyChecked) {
        if (!isEnabled()) {
            bgColor     = d->_accentDisabledColor;
            borderColor = Qt::transparent;
        } else if (d->_isPressed) {
            bgColor     = d->_accentPressColor;
            borderColor = Qt::transparent;
        } else if (d->_isHovered) {
            bgColor     = d->_accentHoverColor;
            borderColor = Qt::transparent;
        } else {
            bgColor     = d->_accentDefaultColor;
            borderColor = Qt::transparent;
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(innerRect, d->_pBorderRadius, d->_pBorderRadius);
    } else {
        if (!isEnabled()) {
            bgColor     = NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode);
            borderColor = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDisabled, d->_themeMode);
        } else if (d->_isPressed) {
            bgColor     = d->_isDark ? d->_pDarkPressColor : d->_pLightPressColor;
            borderColor = d->_isDark ? d->_pDarkBorderPressColor : d->_pLightBorderPressColor;
        } else if (d->_isHovered) {
            bgColor     = d->_isDark ? d->_pDarkHoverColor : d->_pLightHoverColor;
            borderColor = d->_isDark ? d->_pDarkBorderHoverColor : d->_pLightBorderHoverColor;
        } else {
            bgColor     = d->_isDark ? d->_pDarkDefaultColor : d->_pLightDefaultColor;
            borderColor = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(innerRect, d->_pBorderRadius, d->_pBorderRadius);

        QPen pen(borderColor);
        pen.setWidth(1);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(innerRect, d->_pBorderRadius, d->_pBorderRadius);
    }

    if (isChecked() || checkState() == Qt::PartiallyChecked) {
        QColor iconColor;
        if (!isEnabled()) {
            iconColor = d->_accentDisabledTextColor;
        } else {
            iconColor = d->_accentTextColor;
        }

        // 使用像素大小相对于复选框大小的图标
        int iconSize       = d->_checkBoxSize * 0.75;
        d->_checkIcon.size = iconSize;

        if (checkState() == Qt::PartiallyChecked) {
            // 半选状态绘制减号
            painter->setPen(QPen(iconColor, 2));
            painter->drawLine(innerRect.x() + innerRect.width() * 0.25,
                              innerRect.center().y(),
                              innerRect.right() - innerRect.width() * 0.25,
                              innerRect.center().y());
        } else {
            // 选中状态绘制勾选图标
            NFilledIconType::Icon iconType = NFilledIconType::Checkmark24Filled;
            QIcon                 icon     = nIcon->fromFilled(iconType, d->_checkIcon.size, iconColor);

            QRect iconRect(innerRect.x() + (innerRect.width() - d->_checkIcon.size) / 2,
                           innerRect.y() + (innerRect.height() - d->_checkIcon.size) / 2,
                           d->_checkIcon.size,
                           d->_checkIcon.size);

            qreal   dpr        = devicePixelRatio();
            QSize   pixmapSize = QSize(d->_checkIcon.size, d->_checkIcon.size) * dpr;
            QPixmap pixmap     = icon.pixmap(pixmapSize);
            pixmap.setDevicePixelRatio(dpr);
            painter->drawPixmap(iconRect, pixmap);
        }
    }

    painter->restore();
}

void NCheckBox::drawText(QPainter* painter) {
    Q_D(NCheckBox);
    if (text().isEmpty())
        return;

    painter->save();

    QRect textRect(d->_checkBoxSize + d->_spacing, 0, width() - d->_checkBoxSize - d->_spacing, height());

    QColor textColor;
    if (!isEnabled()) {
        textColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
    } else {
        textColor = d->_isDark ? d->_pDarkTextColor : d->_pLightTextColor;
    }

    painter->setPen(textColor);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());

    painter->restore();
}

void NCheckBox::enterEvent(QEnterEvent* event) {
    Q_D(NCheckBox);
    d->_isHovered = true;
    update();
    QCheckBox::enterEvent(event);
}

void NCheckBox::leaveEvent(QEvent* event) {
    Q_D(NCheckBox);
    d->_isHovered = false;
    update();
    QCheckBox::leaveEvent(event);
}

void NCheckBox::mousePressEvent(QMouseEvent* event) {
    Q_D(NCheckBox);
    d->_isPressed = true;
    update();
    QCheckBox::mousePressEvent(event);
}

void NCheckBox::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NCheckBox);
    d->_isPressed = false;
    update();
    QCheckBox::mouseReleaseEvent(event);
}

void NCheckBox::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange) {
        update();
    }
    QCheckBox::changeEvent(event);
}

void NCheckBox::updateAccentColors() {
    Q_D(NCheckBox);
    NAccentColor accentColor = nTheme->accentColor();

    d->_accentDefaultColor  = accentColor.normal();
    d->_accentHoverColor    = accentColor.light();
    d->_accentPressColor    = accentColor.dark();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_accentTextColor         = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, d->_themeMode);
    d->_accentDisabledTextColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorDisabled, d->_themeMode);
}