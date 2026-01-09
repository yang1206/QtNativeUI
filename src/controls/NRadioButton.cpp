//
// Created by Yang1206 on 2025/4/1.
//

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionButton>
#include <QtNativeUI/NRadioButton.h>
#include "../private/nradiobutton_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, LightOuterCircleDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, DarkOuterCircleDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, LightOuterCircleCheckedColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, DarkOuterCircleCheckedColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, LightOuterCircleBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, DarkOuterCircleBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, LightInnerCircleColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, DarkInnerCircleColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, LightDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, QColor, DarkDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, int, OuterCircleSize)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, int, OuterCircleBorderWidth)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, int, InnerCircleSize)
Q_PROPERTY_CREATE_Q_CPP(NRadioButton, int, TextPadding)

NRadioButton::NRadioButton(QWidget* parent) : QRadioButton(parent), d_ptr(new NRadioButtonPrivate()) {
    Q_D(NRadioButton);
    d->q_ptr = this;
    init();
}

NRadioButton::NRadioButton(const QString& text, QWidget* parent) : NRadioButton(parent) { setText(text); }

NRadioButton::~NRadioButton() {}

void NRadioButton::init() {
    Q_D(NRadioButton);

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    d->_pOuterCircleSize        = 20;
    d->_pInnerCircleSize        = 10;
    d->_pOuterCircleBorderWidth = 1;
    d->_pTextPadding            = 8;

    d->_pLightOuterCircleDefaultColor = NThemeColor(NFluentColorKey::ControlAltFillColorSecondary, NThemeType::Light);
    d->_pDarkOuterCircleDefaultColor  = NThemeColor(NFluentColorKey::ControlAltFillColorSecondary, NThemeType::Dark);

    d->_pLightOuterCircleCheckedColor = nTheme->accentColor().normal();
    d->_pDarkOuterCircleCheckedColor  = nTheme->accentColor().normal();

    d->_pLightOuterCircleBorderColor = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Light);
    d->_pDarkOuterCircleBorderColor  = NThemeColor(NFluentColorKey::ControlStrongStrokeColorDefault, NThemeType::Dark);

    d->_pLightInnerCircleColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Light);
    d->_pDarkInnerCircleColor  = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Dark);

    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pLightDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkDisabledColor  = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    updateAccentColors();

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NRadioButton);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        
        d->invalidateColorCache();
        
        updateAccentColors();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        Q_D(NRadioButton);
        
        d->_outerCircleColorCacheValid = false;
        d->_innerCircleColorCacheValid = false;
        
        updateAccentColors();
        update();
    });

    connect(this, &QRadioButton::toggled, this, [this](bool checked) {
        Q_D(NRadioButton);
        
        d->invalidateColorCache();
        
        if (checked) {
            d->startInnerCircleAnimation(0.6, 1.0);
        } else {
            d->startInnerCircleAnimation(d->_innerCircleScale, 0.0);
        }
    });
}

void NRadioButton::updateAccentColors() {
    Q_D(NRadioButton);
    NAccentColor accentColor = nTheme->accentColor();

    d->_accentColor         = accentColor.normal();
    d->_accentHoverColor    = accentColor.light();
    d->_accentPressedColor  = accentColor.dark();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_pLightOuterCircleCheckedColor = d->_accentColor;
    d->_pDarkOuterCircleCheckedColor  = d->_accentColor;
}

bool NRadioButton::event(QEvent* event) {
    Q_D(NRadioButton);

    switch (event->type()) {
        case QEvent::Enter:
            if (isEnabled()) {
                d->_isHovered = true;
                d->invalidateColorCache();
                if (isChecked()) {
                    d->startInnerCircleAnimation(d->_innerCircleScale, 1.3);
                }
            }
            break;

        case QEvent::Leave:
            if (isEnabled()) {
                d->_isHovered = false;
                d->invalidateColorCache();
                if (isChecked()) {
                    d->startInnerCircleAnimation(d->_innerCircleScale, 1.0);
                }
            }
            break;

        case QEvent::EnabledChange:
            d->invalidateColorCache(); 
            break;

        default:
            break;
    }

    return QRadioButton::event(event);
}

void NRadioButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    drawRadioButton(&painter);

    drawText(&painter);
}

void NRadioButton::drawRadioButton(QPainter* painter) {
    Q_D(NRadioButton);

    painter->save();

    int outerSize   = d->_pOuterCircleSize;
    int outerRadius = outerSize / 2;
    int centerX     = outerRadius;
    int centerY     = height() / 2;


    QColor outerCircleColor;
    if (!d->_outerCircleColorCacheValid) {
        if (!isEnabled()) {
            outerCircleColor = d->_isDark ? NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Dark)
                                          : NThemeColor(NFluentColorKey::ControlFillColorDisabled, NThemeType::Light);
        } else if (isChecked()) {
            if (d->_isPressed) {
                outerCircleColor = d->_accentPressedColor;
            } else if (d->_isHovered) {
                outerCircleColor = d->_accentHoverColor;
            } else {
                outerCircleColor = d->_accentColor;
            }
        } else {
            if (d->_isPressed) {
                outerCircleColor = NThemeColor(NFluentColorKey::ControlAltFillColorQuarternary, d->_themeMode);
            } else if (d->_isHovered) {
                outerCircleColor = NThemeColor(NFluentColorKey::ControlAltFillColorTertiary, d->_themeMode);
            } else {
                outerCircleColor = d->_isDark ? d->_pDarkOuterCircleDefaultColor : d->_pLightOuterCircleDefaultColor;
            }
        }
        d->_cachedOuterCircleColor = outerCircleColor;
        d->_outerCircleColorCacheValid = true;
    } else {
        outerCircleColor = d->_cachedOuterCircleColor;
    }

    QColor outerBorderColor;
    if (!d->_outerBorderColorCacheValid) {
        if (!isEnabled()) {
            outerBorderColor = d->_isDark ? d->_pDarkDisabledColor : d->_pLightDisabledColor;
        } else if (isChecked()) {
            outerBorderColor = Qt::transparent;
        } else {
            outerBorderColor = d->_isDark ? d->_pDarkOuterCircleBorderColor : d->_pLightOuterCircleBorderColor;
        }
        d->_cachedOuterBorderColor = outerBorderColor;
        d->_outerBorderColorCacheValid = true;
    } else {
        outerBorderColor = d->_cachedOuterBorderColor;
    }

    QColor innerCircleColor;
    if (!d->_innerCircleColorCacheValid) {
        if (!isEnabled()) {
            innerCircleColor = d->_isDark ? d->_pDarkDisabledColor : d->_pLightDisabledColor;
        } else if (isChecked()) {
            innerCircleColor = d->_isDark ? d->_pDarkInnerCircleColor : d->_pLightInnerCircleColor;
        } else {
            innerCircleColor = Qt::transparent;
        }
        d->_cachedInnerCircleColor = innerCircleColor;
        d->_innerCircleColorCacheValid = true;
    } else {
        innerCircleColor = d->_cachedInnerCircleColor;
    }

    painter->setPen(outerBorderColor == Qt::transparent ? Qt::NoPen
                                                        : QPen(outerBorderColor, d->_pOuterCircleBorderWidth));
    painter->setBrush(outerCircleColor);
    painter->drawEllipse(QPointF(centerX, centerY),
                         outerRadius - d->_pOuterCircleBorderWidth / 2.0,
                         outerRadius - d->_pOuterCircleBorderWidth / 2.0);

    if (isChecked() && d->_innerCircleScale > 0) {
        int innerSize = d->_pInnerCircleSize * d->_innerCircleScale;
        painter->setPen(Qt::NoPen);
        painter->setBrush(innerCircleColor);
        painter->drawEllipse(QPointF(centerX, centerY), innerSize / 2.0, innerSize / 2.0);
    }

    painter->restore();
}

void NRadioButton::drawText(QPainter* painter) {
    Q_D(NRadioButton);

    if (text().isEmpty())
        return;

    painter->save();

    QRect textRect(
        d->_pOuterCircleSize + d->_pTextPadding, 0, width() - d->_pOuterCircleSize - d->_pTextPadding, height());

    QColor textColor;
    if (!d->_textColorCacheValid) {
        if (!isEnabled()) {
            textColor = d->_isDark ? d->_pDarkDisabledColor : d->_pLightDisabledColor;
        } else {
            textColor = d->_isDark ? d->_pDarkTextColor : d->_pLightTextColor;
        }
        d->_cachedTextColor = textColor;
        d->_textColorCacheValid = true;
    } else {
        textColor = d->_cachedTextColor;
    }

    painter->setPen(textColor);
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text());

    painter->restore();
}

void NRadioButton::mousePressEvent(QMouseEvent* event) {
    Q_D(NRadioButton);

    if (event->button() == Qt::LeftButton) {
        d->_isPressed = true;
        d->invalidateColorCache(); 

        if (isChecked()) {
            d->startInnerCircleAnimation(d->_innerCircleScale, 0.8);
        }
    }

    QRadioButton::mousePressEvent(event);
}

void NRadioButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NRadioButton);

    if (event->button() == Qt::LeftButton) {
        d->_isPressed = false;
        d->invalidateColorCache();

        if (isChecked()) {
            if (rect().contains(event->pos())) {
                d->startInnerCircleAnimation(d->_innerCircleScale, 1.2);
            } else {
                d->startInnerCircleAnimation(d->_innerCircleScale, 1.0);
            }
        }
    }

    QRadioButton::mouseReleaseEvent(event);
}

QSize NRadioButton::sizeHint() const {
    Q_D(const NRadioButton);
    QFontMetrics fm(font());
    int          textWidth = text().isEmpty() ? 0 : fm.horizontalAdvance(text()) + d->_pTextPadding;
    int          width     = d->_pOuterCircleSize + textWidth;
    int          height    = qMax(d->_pOuterCircleSize, fm.height() + 6); // 额外的垂直间距
    return QSize(width, height);
}

QSize NRadioButton::minimumSizeHint() const {
    Q_D(const NRadioButton);
    return QSize(d->_pOuterCircleSize, d->_pOuterCircleSize);
}