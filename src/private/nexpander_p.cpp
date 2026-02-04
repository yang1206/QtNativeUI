#include "nexpander_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

NExpanderHeaderWidget::NExpanderHeaderWidget(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
}

void NExpanderHeaderWidget::setHeader(const QString& header) {
    if (m_header != header) {
        m_header = header;
        update();
    }
}

void NExpanderHeaderWidget::setHeaderIcon(bool isRegular, quint32 iconCode, int size) {
    m_headerIcon.isRegular = isRegular;
    m_headerIcon.iconCode  = iconCode;
    m_headerIcon.size      = size;
    m_headerIcon.hasIcon   = true;
    update();
}

void NExpanderHeaderWidget::clearHeaderIcon() {
    m_headerIcon.hasIcon = false;
    update();
}

void NExpanderHeaderWidget::setChevronRotation(qreal rotation) {
    m_chevronRotation = rotation;
    update();
}

void NExpanderHeaderWidget::setExpanded(bool expanded) {
    m_expanded = expanded;
    update();
}

void NExpanderHeaderWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    bool   isDark    = nTheme->isDarkMode();
    QColor bgColor   = isDark ? NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Dark)
                              : NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Light);
    QColor textColor = isDark ? NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark)
                              : NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);

    if (!isEnabled()) {
        textColor = isDark ? NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark)
                           : NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    } else if (m_isPressed) {
        bgColor = bgColor.darker(105);
    } else if (m_isHovered) {
        bgColor = bgColor.lighter(102);
    }

    int radius = NRadiusToken(NDesignTokenKey::CornerRadiusMedium).toInt();

    QPainterPath path;
    if (m_expanded) {
        path.moveTo(0, radius);
        path.arcTo(0, 0, radius * 2, radius * 2, 180, -90);
        path.lineTo(width() - radius, 0);
        path.arcTo(width() - radius * 2, 0, radius * 2, radius * 2, 90, -90);
        path.lineTo(width(), height());
        path.lineTo(0, height());
        path.closeSubpath();
    } else {
        path.addRoundedRect(rect(), radius, radius);
    }

    painter.fillPath(path, bgColor);

    if (!m_header.isEmpty()) {
        painter.setPen(textColor);
        QFont headerFont = font();
        headerFont.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt());
        painter.setFont(headerFont);

        int textLeft = 16;

        if (m_headerIcon.hasIcon) {
            QIcon icon;
            if (m_headerIcon.isRegular) {
                icon = nIcon->fromRegular(
                    static_cast<NRegularIconType::Icon>(m_headerIcon.iconCode), m_headerIcon.size, textColor);
            } else {
                icon = nIcon->fromFilled(
                    static_cast<NFilledIconType::Icon>(m_headerIcon.iconCode), m_headerIcon.size, textColor);
            }

            QRect iconRect(textLeft, (height() - m_headerIcon.size) / 2, m_headerIcon.size, m_headerIcon.size);
            icon.paint(&painter, iconRect);
            textLeft += m_headerIcon.size + 8;
        }

        int   chevronSize = 12;
        int   rightMargin = 16;
        QRect textRect(textLeft, 0, width() - textLeft - rightMargin - chevronSize - 8, height());
        painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, m_header);
    }

    painter.save();
    int   chevronSize = 12;
    int   rightMargin = 16;
    QRect chevronRect(width() - rightMargin - chevronSize, (height() - chevronSize) / 2, chevronSize, chevronSize);
    painter.translate(chevronRect.center());
    painter.rotate(m_chevronRotation);

    QColor chevronColor =
        isEnabled() ? (nTheme->isDarkMode() ? NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark)
                                            : NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light))
                    : (nTheme->isDarkMode() ? NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark)
                                            : NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light));

    QIcon chevronIcon = nIcon->fromRegular(NRegularIconType::ChevronDown12Regular, 12, chevronColor);
    QRect iconRect(-6, -6, 12, 12);
    chevronIcon.paint(&painter, iconRect);

    painter.restore();
}

void NExpanderHeaderWidget::mousePressEvent(QMouseEvent* event) {
    if (isEnabled()) {
        m_isPressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void NExpanderHeaderWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (m_isPressed && rect().contains(event->pos()) && isEnabled()) {
        Q_EMIT clicked();
    }
    m_isPressed = false;
    update();
    QWidget::mouseReleaseEvent(event);
}

void NExpanderHeaderWidget::enterEvent(QEnterEvent* event) {
    m_isHovered = true;
    update();
    QWidget::enterEvent(event);
}

void NExpanderHeaderWidget::leaveEvent(QEvent* event) {
    m_isHovered = false;
    m_isPressed = false;
    update();
    QWidget::leaveEvent(event);
}

NExpanderContentWidget::NExpanderContentWidget(QWidget* parent) : QWidget(parent) {
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(16, 16, 16, 16);
    m_layout->setSpacing(8);
}

void NExpanderContentWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    bool   isDark  = nTheme->isDarkMode();
    QColor bgColor = isDark ? NThemeColor(NFluentColorKey::CardBackgroundFillColorSecondary, NThemeType::Dark)
                            : NThemeColor(NFluentColorKey::CardBackgroundFillColorSecondary, NThemeType::Light);

    int radius = NRadiusToken(NDesignTokenKey::CornerRadiusMedium).toInt();

    QPainterPath path;
    path.moveTo(0, 0);
    path.lineTo(width(), 0);
    path.lineTo(width(), height() - radius);
    path.arcTo(width() - radius * 2, height() - radius * 2, radius * 2, radius * 2, 0, -90);
    path.lineTo(radius, height());
    path.arcTo(0, height() - radius * 2, radius * 2, radius * 2, 270, -90);
    path.closeSubpath();

    painter.fillPath(path, bgColor);
}

NExpanderPrivate::NExpanderPrivate(QObject* parent) : QObject(parent) {
    _themeMode = nTheme->themeMode();
    _isDark    = nTheme->isDarkMode();
}

NExpanderPrivate::~NExpanderPrivate() {}
