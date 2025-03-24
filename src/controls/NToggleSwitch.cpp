#include <QEvent>
#include <QPainter>
#include <QtNativeUI/NToggleSwitch.h>
#include "../private/ntoggleswitch_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, int, TrackBorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, int, TrackBorderWidth)
// 轨道未选中状态
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightTrackDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkTrackDefaultColor)
// 轨道边框颜色
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightTrackBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkTrackBorderColor)
// 轨道选中状态
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightTrackCheckedColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkTrackCheckedColor)
// 滑块未选中状态
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightThumbDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkThumbDefaultColor)
// 滑块选中状态
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightThumbCheckedColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkThumbCheckedColor)
// 文本颜色
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NToggleSwitch, QColor, DarkTextColor)

NToggleSwitch::NToggleSwitch(QWidget* parent) : QAbstractButton(parent), d_ptr(new NToggleSwitchPrivate()) { init(); }

NToggleSwitch::NToggleSwitch(const QString& text, QWidget* parent) : NToggleSwitch(parent) { setText(text); }

NToggleSwitch::~NToggleSwitch() {}

void NToggleSwitch::init() {
    Q_D(NToggleSwitch);
    d->q_ptr = this;

    // 设置轨道圆角，使用圆形轨道（与高度相匹配）
    d->_pTrackBorderRadius = d->_trackHeight / 2;
    d->_pTrackBorderWidth  = 1.5;
    d->_themeMode          = nTheme->themeMode();
    d->_isDark             = nTheme->isDarkMode();

    // 基于FluentAvalonia的设计规范设置颜色

    // 轨道未选中状态颜色（背景）
    d->_pLightTrackDefaultColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkTrackDefaultColor  = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);

    // 轨道边框颜色
    d->_pLightTrackBorderColor = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Light);
    d->_pDarkTrackBorderColor  = NThemeColor(NFluentColorKey::ControlStrokeColorDefault, NThemeType::Dark);

    // 轨道选中状态颜色 - 使用强调色
    // 强调色会通过 updateAccentColors() 设置

    // 滑块未选中状态颜色
    d->_pLightThumbDefaultColor = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Light);
    d->_pDarkThumbDefaultColor  = NThemeColor(NFluentColorKey::ControlStrongFillColorDefault, NThemeType::Dark);

    // 滑块选中状态颜色
    d->_pLightThumbCheckedColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Light); // 白色
    d->_pDarkThumbCheckedColor  = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Dark);  // 黑色

    // 文本颜色
    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    updateAccentColors();

    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setObjectName("NToggleSwitch");
    setCheckable(true);
    setMinimumHeight(d->_trackHeight);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NToggleSwitch);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        updateAccentColors();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        updateAccentColors();
        update();
    });

    // 当checked状态改变时更新
    connect(this, &QAbstractButton::toggled, this, [this](bool checked) {
        Q_D(NToggleSwitch);
        d->_thumbPosition = checked ? 1.0 : 0.0;
        update();
    });
}

void NToggleSwitch::setChecked(bool checked) {
    QAbstractButton::setChecked(checked);
    Q_D(NToggleSwitch);
    d->_thumbPosition = checked ? 1.0 : 0.0;
    update();
}

bool NToggleSwitch::isChecked() const { return QAbstractButton::isChecked(); }

void NToggleSwitch::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    drawTrack(&painter);
    drawThumb(&painter);
    drawText(&painter);
}

void NToggleSwitch::drawTrack(QPainter* painter) {
    Q_D(NToggleSwitch);
    painter->save();

    // 计算轨道位置（垂直居中）
    QRect trackRect(0, (height() - d->_trackHeight) / 2, d->_trackWidth, d->_trackHeight);

    // 根据状态选择颜色
    QColor trackColor;
    QColor borderColor;

    // 根据选中状态和禁用状态选择不同的颜色
    if (isChecked()) {
        if (!isEnabled()) {
            trackColor  = d->_accentDisabledColor;
            borderColor = Qt::transparent; // 选中状态不显示边框
        } else {
            // 选中状态使用强调色
            trackColor  = d->_accentDefaultColor;
            borderColor = Qt::transparent; // 选中状态不显示边框
        }
    } else {
        // 未选中状态下，轨道是白色/深色背景，带边框
        if (!isEnabled()) {
            trackColor  = NThemeColor(NFluentColorKey::ControlFillColorDisabled, d->_themeMode);
            borderColor = NThemeColor(NFluentColorKey::ControlStrokeColorSecondary, d->_themeMode);
        } else {
            // 未选中状态使用背景色+边框
            trackColor  = d->_isDark ? d->_pDarkTrackDefaultColor : d->_pLightTrackDefaultColor;
            borderColor = d->_isDark ? d->_pDarkTrackBorderColor : d->_pLightTrackBorderColor;
        }
    }

    // 先绘制背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(trackColor);
    painter->drawRoundedRect(trackRect, d->_pTrackBorderRadius, d->_pTrackBorderRadius);

    // 再绘制边框（如果需要）
    if (borderColor != Qt::transparent) {
        QPen pen(borderColor);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        QRect borderRect = trackRect.adjusted(d->_pTrackBorderWidth / 2,
                                              d->_pTrackBorderWidth / 2,
                                              -d->_pTrackBorderWidth / 2,
                                              -d->_pTrackBorderWidth / 2);
        painter->drawRoundedRect(borderRect, d->_pTrackBorderRadius, d->_pTrackBorderRadius);
    }

    painter->restore();
}

void NToggleSwitch::drawThumb(QPainter* painter) {
    Q_D(NToggleSwitch);
    painter->save();

    // 计算滑块移动范围
    int trackPadding = (d->_trackHeight - d->_thumbSize) / 2;
    int minX         = trackPadding;
    int maxX         = d->_trackWidth - d->_thumbSize - trackPadding;
    int x            = minX + d->_thumbPosition * (maxX - minX);
    int y            = (height() - d->_thumbSize) / 2;

    QRect thumbRect(x, y, d->_thumbSize, d->_thumbSize);

    // 选择滑块颜色，根据状态选择不同的颜色
    QColor thumbColor;
    if (!isEnabled()) {
        // 禁用状态
        thumbColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, d->_themeMode);
    } else if (isChecked()) {
        // 选中状态
        thumbColor = d->_isDark ? d->_pDarkThumbCheckedColor : d->_pLightThumbCheckedColor;
    } else {
        // 未选中状态
        thumbColor = d->_isDark ? d->_pDarkThumbDefaultColor : d->_pLightThumbDefaultColor;
    }

    // 绘制滑块阴影效果（悬停状态或按下状态）
    if (d->_isHovered || d->_isPressed) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 0, 0, 10)); // 半透明阴影
        painter->drawEllipse(thumbRect.adjusted(-2, -2, 2, 2));
    }

    // 绘制滑块
    painter->setPen(Qt::NoPen);
    painter->setBrush(thumbColor);
    painter->drawEllipse(thumbRect);

    painter->restore();
}

void NToggleSwitch::drawText(QPainter* painter) {
    Q_D(NToggleSwitch);
    if (text().isEmpty())
        return;

    painter->save();

    QRect textRect(d->_trackWidth + d->_spacing, 0, width() - d->_trackWidth - d->_spacing, height());

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

void NToggleSwitch::enterEvent(QEnterEvent* event) {
    Q_D(NToggleSwitch);
    d->_isHovered = true;
    update();
    QAbstractButton::enterEvent(event);
}

void NToggleSwitch::leaveEvent(QEvent* event) {
    Q_D(NToggleSwitch);
    d->_isHovered = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void NToggleSwitch::mousePressEvent(QMouseEvent* event) {
    Q_D(NToggleSwitch);
    d->_isPressed = true;
    update();
    QAbstractButton::mousePressEvent(event);
}

void NToggleSwitch::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NToggleSwitch);
    d->_isPressed = false;
    update();
    QAbstractButton::mouseReleaseEvent(event);
}

void NToggleSwitch::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange) {
        update();
    }
    QAbstractButton::changeEvent(event);
}

QSize NToggleSwitch::sizeHint() const {
    Q_D(const NToggleSwitch);
    QFontMetrics fm(font());
    int          textWidth = text().isEmpty() ? 0 : fm.horizontalAdvance(text()) + d->_spacing;
    int          width     = d->_trackWidth + textWidth;
    int          height    = qMax(d->_trackHeight, fm.height());
    return QSize(width, height);
}

QSize NToggleSwitch::minimumSizeHint() const {
    Q_D(const NToggleSwitch);
    return QSize(d->_trackWidth, d->_trackHeight);
}

void NToggleSwitch::updateAccentColors() {
    Q_D(NToggleSwitch);
    NAccentColor accentColor = nTheme->accentColor();

    d->_accentDefaultColor  = accentColor.normal();
    d->_accentDisabledColor = NThemeColor(NFluentColorKey::AccentFillColorDisabled, d->_themeMode);

    d->_accentTextColor         = NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, d->_themeMode);
    d->_accentDisabledTextColor = NThemeColor(NFluentColorKey::TextOnAccentFillColorDisabled, d->_themeMode);

    // 轨道选中状态颜色
    d->_pLightTrackCheckedColor = accentColor.light();
    d->_pDarkTrackCheckedColor  = accentColor.dark();
}