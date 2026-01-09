#include <QDesktopServices>
#include <QEnterEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtNativeUI/NHyperlinkButton.h>
#include "../private/nhyperlinkbutton_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NHyperlinkButton, QColor, DisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NHyperlinkButton, QColor, BackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NHyperlinkButton, QColor, BackgroundPressColor)

NHyperlinkButton::NHyperlinkButton(QWidget* parent) : QPushButton(parent), d_ptr(new NHyperlinkButtonPrivate()) {
    Q_D(NHyperlinkButton);
    d->q_ptr          = this;
    d->_isDark        = nTheme->isDarkMode();
    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

    d->_pDisabledColor =
        NThemeColor(NFluentColorKey::AccentTextFillColorDisabled, d->_isDark ? NThemeType::Dark : NThemeType::Light);
    d->_pBackgroundHoverColor =
        NThemeColor(NFluentColorKey::SubtleFillColorSecondary, d->_isDark ? NThemeType::Dark : NThemeType::Light);
    d->_pBackgroundPressColor =
        NThemeColor(NFluentColorKey::SubtleFillColorTertiary, d->_isDark ? NThemeType::Dark : NThemeType::Light);

    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setFlat(true);

    setMinimumHeight(32);
    int horizontalSpacing = NDesignToken(NDesignTokenKey::SpacingL).toInt();
    int verticalSpacing   = NDesignToken(NDesignTokenKey::SpacingS).toInt();
    setContentsMargins(horizontalSpacing, verticalSpacing, horizontalSpacing, verticalSpacing);

    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);

    setObjectName("NHyperlinkButton");
    setStyleSheet("#NHyperlinkButton{background-color:transparent;}");

    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        Q_D(NHyperlinkButton);
        d->_isDark         = nTheme->isDarkMode();
        d->_pDisabledColor = NThemeColor(NFluentColorKey::AccentTextFillColorDisabled,
                                         d->_isDark ? NThemeType::Dark : NThemeType::Light);
        d->_pBackgroundHoverColor =
            NThemeColor(NFluentColorKey::SubtleFillColorSecondary, d->_isDark ? NThemeType::Dark : NThemeType::Light);
        d->_pBackgroundPressColor =
            NThemeColor(NFluentColorKey::SubtleFillColorTertiary, d->_isDark ? NThemeType::Dark : NThemeType::Light);
        
        d->invalidateColorCache();
        update();
    });

    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor&) {
        Q_D(NHyperlinkButton);
        d->_textColorCacheValid = false;
        update();
    });

    connect(this, &QPushButton::clicked, this, [this]() {
        Q_D(NHyperlinkButton);
        if (!d->_url.isEmpty()) {
            bool success = QDesktopServices::openUrl(d->_url);
            if (success) {
                emit linkActivated(d->_url);
            }
        }
    });
}

NHyperlinkButton::NHyperlinkButton(QString text, QWidget* parent) : NHyperlinkButton(parent) { setText(text); }

NHyperlinkButton::~NHyperlinkButton() {}

void NHyperlinkButton::enterEvent(QEnterEvent* event) {
    Q_D(NHyperlinkButton);
    d->_isHovered = true;
    d->invalidateColorCache();
    update();
    QPushButton::enterEvent(event);
}

void NHyperlinkButton::leaveEvent(QEvent* event) {
    Q_D(NHyperlinkButton);
    d->_isHovered = false;
    d->invalidateColorCache();
    update();
    QPushButton::leaveEvent(event);
}

void NHyperlinkButton::mousePressEvent(QMouseEvent* event) {
    Q_D(NHyperlinkButton);
    d->_isPressed = true;
    d->invalidateColorCache();
    update();
    QPushButton::mousePressEvent(event);
}

void NHyperlinkButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NHyperlinkButton);
    d->_isPressed = false;
    d->invalidateColorCache();
    update();
    QPushButton::mouseReleaseEvent(event);
}

void NHyperlinkButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    Q_D(NHyperlinkButton);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 绘制背景
    if ((d->_isPressed || d->_isHovered) && isEnabled()) {
        QColor bgColor;
        if (!d->_backgroundColorCacheValid) {
            bgColor = d->_isPressed ? d->_pBackgroundPressColor : d->_pBackgroundHoverColor;
            d->_cachedBackgroundColor = bgColor;
            d->_backgroundColorCacheValid = true;
        } else {
            bgColor = d->_cachedBackgroundColor;
        }
        
        QPainterPath path;
        path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
        painter.fillPath(path, bgColor);
    }

    drawText(&painter);
}

void NHyperlinkButton::drawText(QPainter* painter) {
    Q_D(NHyperlinkButton);
    if (text().isEmpty())
        return;

    QColor textColor;
    if (!d->_textColorCacheValid) {
        textColor = isEnabled() ? nTheme->accentColor().normal() : d->_pDisabledColor;
        d->_cachedTextColor = textColor;
        d->_textColorCacheValid = true;
    } else {
        textColor = d->_cachedTextColor;
    }

    painter->setPen(textColor);
    painter->drawText(rect(), Qt::AlignCenter, text());
}

void NHyperlinkButton::changeEvent(QEvent* event) {
    Q_D(NHyperlinkButton);
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange ||
        event->type() == QEvent::LanguageChange) {
        d->invalidateColorCache();
        update();
    }
    QPushButton::changeEvent(event);
}

void NHyperlinkButton::setUrl(const QString& url) {
    Q_D(NHyperlinkButton);
    d->_url = QUrl(url);
}

void NHyperlinkButton::setUrl(const QUrl& url) {
    Q_D(NHyperlinkButton);
    d->_url = url;
}

QUrl NHyperlinkButton::url() const {
    Q_D(const NHyperlinkButton);
    return d->_url;
}