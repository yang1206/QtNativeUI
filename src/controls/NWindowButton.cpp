#include "QtNativeUI/NWindowButton.h"
#include <QMouseEvent>
#include <QPainter>
#include "../private/nwindowbutton_p.h"
#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NTheme.h"

NWindowButtonPrivate::NWindowButtonPrivate()  = default;
NWindowButtonPrivate::~NWindowButtonPrivate() = default;

NWindowButton::NWindowButton(QWidget* parent) : QPushButton(parent), d_ptr(new NWindowButtonPrivate()) {
    d_ptr->q_ptr = this;
    setFixedSize(46, 32);
    setFlat(true);
    setCursor(Qt::ArrowCursor);
}

NWindowButton::NWindowButton(SystemButtonType type, QWidget* parent) : NWindowButton(parent) { setButtonType(type); }

NWindowButton::~NWindowButton() = default;

void NWindowButton::setButtonType(SystemButtonType type) {
    Q_D(NWindowButton);
    d->buttonType = type;
    update();
}

NWindowButton::SystemButtonType NWindowButton::buttonType() const {
    Q_D(const NWindowButton);
    return d->buttonType;
}

void NWindowButton::setMaximized(bool maximized) {
    Q_D(NWindowButton);
    if (d->maximized != maximized) {
        d->maximized = maximized;
        update();
    }
}

bool NWindowButton::isMaximized() const {
    Q_D(const NWindowButton);
    return d->maximized;
}

void NWindowButton::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    Q_D(NWindowButton);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    bool   isDark    = nTheme->isDarkMode();
    QColor bgColor   = Qt::transparent;
    QColor iconColor = isDark ? QColor(255, 255, 255) : QColor(0, 0, 0);

    if (d->hovered) {
        if (d->buttonType == Close) {
            bgColor   = QColor(232, 17, 35);
            iconColor = Qt::white;
        } else {
            bgColor = isDark ? QColor(255, 255, 255, 26) : QColor(0, 0, 0, 26);
        }
    }

    if (isDown()) {
        if (d->buttonType == Close) {
            bgColor   = QColor(241, 112, 122);
            iconColor = Qt::white;
        } else {
            bgColor = isDark ? QColor(255, 255, 255, 13) : QColor(0, 0, 0, 13);
        }
    }

    painter.fillRect(rect(), bgColor);

    constexpr int iconSize = 16;
    QIcon         icon;
    switch (d->buttonType) {
        case Minimize:
            icon = nIcon->fromRegular(NRegularIconType::Subtract20Regular, iconSize, iconColor);
            break;
        case Maximize:
            if (d->maximized) {
                icon = nIcon->fromRegular(NRegularIconType::SquareMultiple20Regular, iconSize, iconColor);
            } else {
                icon = nIcon->fromRegular(NRegularIconType::Square20Regular, iconSize, iconColor);
            }
            break;
        case Close:
            icon = nIcon->fromRegular(NRegularIconType::Dismiss20Regular, iconSize, iconColor);
            break;
        case Pin:
            if (isChecked()) {
                icon = nIcon->fromRegular(NRegularIconType::PinOff20Regular, iconSize, iconColor);
            } else {
                icon = nIcon->fromRegular(NRegularIconType::Pin20Regular, iconSize, iconColor);
            }
            break;
        case Theme:
            if (isDark) {
                icon = nIcon->fromRegular(NRegularIconType::WeatherSunny20Regular, iconSize, iconColor);
            } else {
                icon = nIcon->fromRegular(NRegularIconType::WeatherMoon20Regular, iconSize, iconColor);
            }
            break;
        default:
            break;
    }

    if (!icon.isNull()) {
        QSize sz(iconSize, iconSize);
        QRect iconRect(rect().center() - QPoint(sz.width() / 2, sz.height() / 2), sz);
        icon.paint(&painter, iconRect);
    }
}

void NWindowButton::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked();
    }
    QPushButton::mouseDoubleClickEvent(event);
}

void NWindowButton::enterEvent(QEnterEvent* event) {
    Q_D(NWindowButton);
    d->hovered = true;
    update();
    QPushButton::enterEvent(event);
}

void NWindowButton::leaveEvent(QEvent* event) {
    Q_D(NWindowButton);
    d->hovered = false;
    update();
    QPushButton::leaveEvent(event);
}
