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

    // 只需要设置禁用状态的颜色
    d->_pDisabledColor =
        NThemeColor(NFluentColorKey::AccentTextFillColorDisabled, d->_isDark ? NThemeType::Dark : NThemeType::Light);

    // 设置背景色
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
    update();
    QPushButton::enterEvent(event);
}

void NHyperlinkButton::leaveEvent(QEvent* event) {
    Q_D(NHyperlinkButton);
    d->_isHovered = false;
    update();
    QPushButton::leaveEvent(event);
}

void NHyperlinkButton::mousePressEvent(QMouseEvent* event) {
    Q_D(NHyperlinkButton);
    d->_isPressed = true;
    update();
    QPushButton::mousePressEvent(event);
}

void NHyperlinkButton::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NHyperlinkButton);
    d->_isPressed = false;
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
        QPainterPath path;
        path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
        painter.fillPath(path, d->_isPressed ? d->_pBackgroundPressColor : d->_pBackgroundHoverColor);
    }

    drawText(&painter);
}

void NHyperlinkButton::drawText(QPainter* painter) {
    Q_D(NHyperlinkButton);
    if (text().isEmpty())
        return;

    // 文字颜色只需要判断是否禁用
    painter->setPen(isEnabled() ? nTheme->accentColor().normal() : d->_pDisabledColor);
    painter->drawText(rect(), Qt::AlignCenter, text());
}

void NHyperlinkButton::changeEvent(QEvent* event) {
    if (event->type() == QEvent::EnabledChange || event->type() == QEvent::PaletteChange ||
        event->type() == QEvent::LanguageChange) {
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