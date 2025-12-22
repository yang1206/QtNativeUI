#include "QtNativeUI/NFlyout.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include "../private/nflyout_p.h"
#include "QtNativeUI/NTheme.h"
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, int, BorderWidth)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, Qt::Edge, Placement)
NFlyout::NFlyout(QWidget* parent) : QWidget(parent), d_ptr(new NFlyoutPrivate()) {
    Q_D(NFlyout);
    d->q_ptr = this;

    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;
    d->_pPlacement    = Qt::BottomEdge;

    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose, false);

    setMinimumSize(100, 50);

    setMouseTracking(true);

    d->setupUI();

    setupEventFilter();

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NFlyout);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

NFlyout::~NFlyout() { QApplication::instance()->removeEventFilter(this); }

void NFlyout::setContent(QWidget* content) {
    Q_D(NFlyout);

    if (content == d->_pContent)
        return;
    if (d->_pContent && d->_mainLayout) {
        d->_mainLayout->removeWidget(d->_pContent);
        d->_pContent->setParent(nullptr);
    }

    d->_pContent = content;
    if (content && d->_mainLayout) {
        content->setParent(this);
        d->_mainLayout->addWidget(content);
    }

    update();
}

QWidget* NFlyout::content() const {
    Q_D(const NFlyout);
    return d->_pContent;
}

void NFlyout::setTarget(QWidget* target) {
    Q_D(NFlyout);
    d->_pTarget = target;
}

QWidget* NFlyout::target() const {
    Q_D(const NFlyout);
    return d->_pTarget;
}

bool NFlyout::isOpen() const {
    Q_D(const NFlyout);
    return d->_isOpen;
}

void NFlyout::setAnimationType(NFlyoutAnimationType type) {
    Q_D(NFlyout);
    if (d->_animationType != type) {
        d->_animationType = type;

        if (d->_animManager) {
            delete d->_animManager;
            d->_animManager = nullptr;
        }
    }
}

NFlyoutAnimationType NFlyout::animationType() const {
    Q_D(const NFlyout);
    return d->_animationType;
}

void NFlyout::setLightDismissMode(LightDismissMode mode) {
    Q_D(NFlyout);
    d->_lightDismissMode = mode;

    setupEventFilter();
}

NFlyout::LightDismissMode NFlyout::lightDismissMode() const {
    Q_D(const NFlyout);
    return d->_lightDismissMode;
}

void NFlyout::setShadowEffect(int blurRadius, const QPoint& offset) {
    Q_D(NFlyout);
    if (d->_shadowEffect) {
        d->_shadowEffect->setBlurRadius(blurRadius);
        d->_shadowEffect->setOffset(offset.x(), offset.y());

        QColor shadowColor = d->_isDark ? QColor(0, 0, 0, 80) : QColor(0, 0, 0, 30);
        d->_shadowEffect->setColor(shadowColor);
    }
}

NFlyout* NFlyout::createWithContent(QWidget*             content,
                                    QWidget*             target,
                                    QWidget*             parent,
                                    NFlyoutAnimationType animType,
                                    bool                 isDeleteOnClose) {
    NFlyout* flyout = new NFlyout(parent);
    flyout->setAnimationType(animType);
    flyout->setAttribute(Qt::WA_DeleteOnClose, isDeleteOnClose);

    flyout->setContent(content);

    return flyout;
}

void NFlyout::showAt(QWidget* target) {
    if (!target)
        return;

    Q_D(NFlyout);
    d->_pTarget = target;

    emit opening();

    QRect  rect = d->calculatePlacement(target, d->_pPlacement);
    QPoint pos  = rect.topLeft();

    exec(pos);

    d->_isOpen = true;
    emit opened();
}

void NFlyout::exec(const QPoint& pos) {
    Q_D(NFlyout);

    show();

    d->animationManager()->exec(pos);
}

void NFlyout::hide() {
    Q_D(NFlyout);

    if (!d->_isOpen)
        return;

    emit closing();
    QWidget::hide();
    d->_isOpen = false;
    emit closed();
}

void NFlyout::fadeOut() {
    Q_D(NFlyout);

    if (!d->_fadeOutAnimation) {
        d->_fadeOutAnimation = new QPropertyAnimation(this, "windowOpacity", this);
        d->_fadeOutAnimation->setDuration(120);
        connect(d->_fadeOutAnimation, &QPropertyAnimation::finished, this, &NFlyout::hide);
    }

    d->_fadeOutAnimation->setStartValue(1.0);
    d->_fadeOutAnimation->setEndValue(0.0);
    d->_fadeOutAnimation->start();
}

void NFlyout::showEvent(QShowEvent* event) { QWidget::showEvent(event); }

void NFlyout::hideEvent(QHideEvent* event) { QWidget::hideEvent(event); }

void NFlyout::paintEvent(QPaintEvent* event) {
    Q_D(NFlyout);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    QColor backgroundColor = d->_isDark ? d->_pDarkBackgroundColor : d->_pLightBackgroundColor;
    QColor borderColor     = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundColor);

    QRect contentRect = rect().adjusted(1, 1, -1, -1);

    painter.drawRoundedRect(contentRect, d->_pBorderRadius, d->_pBorderRadius);

    painter.setPen(QPen(borderColor, d->_pBorderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(contentRect, d->_pBorderRadius, d->_pBorderRadius);

    QWidget::paintEvent(event);
}

void NFlyout::enterEvent(QEnterEvent* event) {
    Q_D(NFlyout);
    d->_isHovered = true;
    update();
    QWidget::enterEvent(event);
}

void NFlyout::leaveEvent(QEvent* event) {
    Q_D(NFlyout);
    d->_isHovered = false;
    update();
    QWidget::leaveEvent(event);
}

void NFlyout::mousePressEvent(QMouseEvent* event) {
    Q_D(NFlyout);
    d->_isPressed = true;
    update();
    QWidget::mousePressEvent(event);
}

void NFlyout::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NFlyout);
    d->_isPressed = false;
    update();
    QWidget::mouseReleaseEvent(event);
}

void NFlyout::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        hide();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

bool NFlyout::eventFilter(QObject* watched, QEvent* event) {
    Q_D(NFlyout);

    if (isVisible() && d->_pTarget && watched == d->_pTarget->window() && event->type() == QEvent::Move) {
        fadeOut();
        return false;
    }

    if (isVisible() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent    = static_cast<QMouseEvent*>(event);
        QWidget*     clickedWidget = QApplication::widgetAt(mouseEvent->globalPosition().toPoint());

        if (clickedWidget && clickedWidget != this && !isAncestorOf(clickedWidget) &&
            (!d->_pTarget || (clickedWidget != d->_pTarget && !d->_pTarget->isAncestorOf(clickedWidget)))) {
            if (d->_lightDismissMode == LightDismissMode::On) {
                fadeOut();
                return true;
            } else if (d->_lightDismissMode == LightDismissMode::Auto) {
                fadeOut();
                return true;
            } else if (d->_lightDismissMode == LightDismissMode::Off) {
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}

void NFlyout::setupEventFilter() {
    QApplication::instance()->removeEventFilter(this);

    QApplication::instance()->installEventFilter(this);
}

QPoint NFlyout::calculatePositionForTarget(QWidget* target) {
    Q_D(NFlyout);

    if (!target)
        return QPoint();

    return d->animationManager()->position(target);
}

void NFlyout::setContentsMargins(int left, int top, int right, int bottom) {
    Q_D(NFlyout);
    if (d->_mainLayout) {
        d->_mainLayout->setContentsMargins(left, top, right, bottom);

        if (d->_pContent && d->_pContent->layout()) {
            d->_pContent->layout()->setContentsMargins(0, 0, 0, 0);
        }

        updateGeometry();
        update();
    }
}

void NFlyout::setContentsMargins(const QMargins& margins) {
    setContentsMargins(margins.left(), margins.top(), margins.right(), margins.bottom());
}

QMargins NFlyout::contentsMargins() const {
    Q_D(const NFlyout);
    if (d->_mainLayout) {
        return d->_mainLayout->contentsMargins();
    }
    return QMargins();
}