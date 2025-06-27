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

    d->_pBorderRadius = NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt();
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
    d->_pCheckAlpha            = 0;

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
    d->_spacing        = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
    setTristate(false);
}

void NCheckBox::setChecked(bool checked) {
    Q_D(NCheckBox);

    if (isChecked() == checked)
        return;

    d->startAlphaAnimation(checked);

    QCheckBox::setChecked(checked);
}

void NCheckBox::nextCheckState() {
    Q_D(NCheckBox);

    Qt::CheckState currentState = checkState();

    Qt::CheckState nextState;
    if (currentState == Qt::Unchecked) {
        nextState = Qt::Checked;
    } else if (currentState == Qt::Checked && isTristate()) {
        nextState = Qt::PartiallyChecked;
    } else {
        nextState = Qt::Unchecked;
    }

    bool needAnimation = (currentState == Qt::Unchecked && nextState != Qt::Unchecked) ||
                         (currentState != Qt::Unchecked && nextState == Qt::Unchecked);

    QCheckBox::nextCheckState();

    if (needAnimation) {
        d->startAlphaAnimation(nextState != Qt::Unchecked);
    } else {
        d->_pCheckAlpha         = 255;
        d->_isAnimationFinished = true;
        update();
    }
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

    bool isCheckedOrPartial = isChecked() || checkState() == Qt::PartiallyChecked;

    if (d->_isAnimationFinished) {
        if (isCheckedOrPartial) {
            if (!isEnabled()) {
                bgColor = d->_accentDisabledColor;
            } else if (d->_isPressed) {
                bgColor = d->_accentPressColor;
            } else if (d->_isHovered) {
                bgColor = d->_accentHoverColor;

            } else {
                bgColor = d->_accentDefaultColor;
            }
            borderColor = Qt::transparent;
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
    } else {
        QColor uncheckedBgColor;
        if (!isEnabled()) {
            uncheckedBgColor = NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode);
            borderColor      = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDisabled, d->_themeMode);
        } else if (d->_isPressed) {
            uncheckedBgColor = d->_isDark ? d->_pDarkPressColor : d->_pLightPressColor;
            borderColor      = d->_isDark ? d->_pDarkBorderPressColor : d->_pLightBorderPressColor;
        } else if (d->_isHovered) {
            uncheckedBgColor = d->_isDark ? d->_pDarkHoverColor : d->_pLightHoverColor;
            borderColor      = d->_isDark ? d->_pDarkBorderHoverColor : d->_pLightBorderHoverColor;
        } else {
            uncheckedBgColor = d->_isDark ? d->_pDarkDefaultColor : d->_pLightDefaultColor;
            borderColor      = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
        }

        QColor checkedBgColor;
        if (!isEnabled()) {
            checkedBgColor = d->_accentDisabledColor;
        } else if (d->_isPressed) {
            checkedBgColor = d->_accentPressColor;
        } else if (d->_isHovered) {
            checkedBgColor = d->_accentHoverColor;
        } else {
            checkedBgColor = d->_accentDefaultColor;
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(uncheckedBgColor);
        painter->drawRoundedRect(innerRect, d->_pBorderRadius, d->_pBorderRadius);

        QColor accentColor = checkedBgColor;
        accentColor.setAlpha(d->_pCheckAlpha);

        painter->setPen(Qt::NoPen);
        painter->setBrush(accentColor);
        painter->drawRoundedRect(innerRect, d->_pBorderRadius, d->_pBorderRadius);

        if (d->_pCheckAlpha < 255) {
            QColor alphaBorderColor = borderColor;
            alphaBorderColor.setAlpha(borderColor.alpha() * (255 - d->_pCheckAlpha) / 255);

            QPen pen(alphaBorderColor);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            painter->drawRoundedRect(innerRect, d->_pBorderRadius, d->_pBorderRadius);
        }
    }

    if (isCheckedOrPartial || d->_pCheckAlpha > 0) {
        QColor iconColor;
        if (!isEnabled()) {
            iconColor = d->_accentDisabledTextColor;
        } else {
            iconColor = d->_accentTextColor;
        }

        if (!d->_isAnimationFinished) {
            iconColor.setAlpha(d->_pCheckAlpha);
        }

        int iconSize       = d->_checkBoxSize * 0.75;
        d->_checkIcon.size = iconSize;

        if (checkState() == Qt::PartiallyChecked) {
            painter->setPen(QPen(iconColor, 2));

            int lineWidth = innerRect.width() * 0.6; // 使用60%的宽度
            int startX    = innerRect.center().x() - lineWidth / 2;
            int endX      = innerRect.center().x() + lineWidth / 2;
            int y         = innerRect.center().y();

            painter->drawLine(startX, y, endX, y);
        } else {
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
    d->_isPressed           = true;
    d->_isAnimationFinished = false;
    update();
    QCheckBox::mousePressEvent(event);
}

void NCheckBox::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NCheckBox);
    d->_isPressed = false;

    bool wasChecked = d->_pCheckAlpha > 127;

    QCheckBox::mouseReleaseEvent(event);

    bool isNowChecked = isChecked() || checkState() == Qt::PartiallyChecked;

    if (isNowChecked != wasChecked) {
        d->startAlphaAnimation(isNowChecked);
    } else {
        d->_pCheckAlpha         = isNowChecked ? 255 : 0;
        d->_isAnimationFinished = true;
        update();
    }
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