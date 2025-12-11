#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QHideEvent>
#include <QLabel>
#include <QShowEvent>
#include <QtNativeUI/NToolTip.h>
#include "../private/ntooltip_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NToolTip, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, QColor, LightTextColor)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, QColor, DarkTextColor)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, int, ContentPadding)
Q_PROPERTY_CREATE_Q_CPP(NToolTip, int, ShadowRadius)

NToolTip::NToolTip(const QString& text, QWidget* parent) : QFrame(parent), d_ptr(new NToolTipPrivate()) {
    Q_D(NToolTip);
    d->q_ptr = this;
    d->text  = text;

    init();
}

NToolTip::~NToolTip() {}

void NToolTip::init() {
    Q_D(NToolTip);

    d->container = createContainer();
    d->timer     = new QTimer(this);

    setLayout(new QHBoxLayout());
    d->containerLayout = new QHBoxLayout(d->container);
    d->label           = new QLabel(d->text, this);

    d->themeMode = nTheme->themeMode();
    d->isDark    = nTheme->isDarkMode();

    d->_pBorderRadius   = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pContentPadding = 4;
    d->_pShadowRadius   = 25;

    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkBackgroundColor  = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);

    d->_pLightTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkTextColor  = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);

    d->_pLightBorderColor = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkBorderColor  = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);

    layout()->setContentsMargins(12, 8, 12, 12);
    layout()->addWidget(d->container);
    d->containerLayout->addWidget(d->label);
    d->containerLayout->setContentsMargins(6, 4, 6, 4);

    d->opacityAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    d->opacityAnimation->setDuration(150);

#ifndef Q_OS_MACOS
    d->shadowEffect = new QGraphicsDropShadowEffect(this);
    d->shadowEffect->setBlurRadius(25);
    d->shadowEffect->setColor(QColor(0, 0, 0, 50));
    d->shadowEffect->setOffset(0, 5);
    d->container->setGraphicsEffect(d->shadowEffect);
#endif

    d->timer->setSingleShot(true);
    connect(d->timer, &QTimer::timeout, this, &NToolTip::hide);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);

    d->container->setObjectName("container");
    d->label->setObjectName("contentLabel");

    d->updateStyle();

    d->label->adjustSize();
    adjustSize();

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NToolTip);
        d->themeMode = mode;
        d->isDark    = nTheme->isDarkMode();
        d->updateStyle();
    });
}

QWidget* NToolTip::createContainer() { return new QWidget(this); }

QString NToolTip::text() const {
    Q_D(const NToolTip);
    return d->text;
}

void NToolTip::setText(const QString& text) {
    Q_D(NToolTip);
    d->text = text;
    d->label->setText(text);
    d->container->adjustSize();
    adjustSize();
}

int NToolTip::duration() const {
    Q_D(const NToolTip);
    return d->duration;
}

void NToolTip::setDuration(int duration) {
    Q_D(NToolTip);
    d->duration = duration;
}

void NToolTip::adjustPosition(QWidget* widget, NToolTipPosition position) {
    NToolTipPositionManager* manager = NToolTipPositionManager::create(position);
    move(manager->position(this, widget));
    delete manager;
}

void NToolTip::showEvent(QShowEvent* event) {
    Q_D(NToolTip);

    d->opacityAnimation->setStartValue(0.0);
    d->opacityAnimation->setEndValue(1.0);
    d->opacityAnimation->start();

    d->timer->stop();
    if (d->duration > 0) {
        d->timer->start(d->duration + d->opacityAnimation->duration());
    }

    QFrame::showEvent(event);
}

void NToolTip::hideEvent(QHideEvent* event) {
    Q_D(NToolTip);
    d->timer->stop();
    QFrame::hideEvent(event);
}

NToolTip* NToolTip::createToolTip(const QString& text, QWidget* parent) {
    QWidget* window = parent ? parent->window() : nullptr;
    return new NToolTip(text, window);
}

void NToolTip::showText(const QString& text, QWidget* parent, NToolTipPosition position) {
    if (!parent)
        return;

    static NToolTip* tooltip = nullptr;

    if (!tooltip || !tooltip->isVisible()) {
        if (tooltip)
            delete tooltip;
        tooltip = createToolTip(text, parent);
    } else {
        tooltip->setText(text);
    }

    tooltip->adjustPosition(parent, position);
    tooltip->show();
}

NToolTipFilter::NToolTipFilter(QWidget* parent, int showDelay, NToolTipPosition position)
    : QObject(parent), tooltipDelay(showDelay), position(position) {
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &NToolTipFilter::showToolTip);

    if (parent) {
        parent->installEventFilter(this);
    }
}

NToolTipFilter::~NToolTipFilter() {
    if (tooltip) {
        tooltip->deleteLater();
        tooltip = nullptr;
    }
}

bool NToolTipFilter::eventFilter(QObject* watched, QEvent* event) {
    if (watched == parent()) {
        if (event->type() == QEvent::ToolTip) {
            return true;
        } else if (event->type() == QEvent::Hide || event->type() == QEvent::Leave) {
            hideToolTip();
        } else if (event->type() == QEvent::Enter) {
            isEnter               = true;
            QWidget* parentWidget = qobject_cast<QWidget*>(parent());

            if (canShowToolTip()) {
                if (!tooltip) {
                    tooltip = createToolTip();
                }

                int t = parentWidget->toolTipDuration() > 0 ? parentWidget->toolTipDuration() : -1;
                tooltip->setDuration(t);

                timer->start(tooltipDelay);
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            hideToolTip();
        }
    }

    return QObject::eventFilter(watched, event);
}

NToolTip* NToolTipFilter::createToolTip() {
    QWidget* parentWidget = qobject_cast<QWidget*>(parent());
    return NToolTip::createToolTip(parentWidget->toolTip(), parentWidget);
}

void NToolTipFilter::hideToolTip() {
    isEnter = false;
    timer->stop();
    if (tooltip) {
        tooltip->hide();
    }
}

void NToolTipFilter::showToolTip() {
    if (!isEnter)
        return;

    QWidget* parentWidget = qobject_cast<QWidget*>(parent());
    tooltip->setText(parentWidget->toolTip());
    tooltip->adjustPosition(parentWidget, position);
    tooltip->show();
}

void NToolTipFilter::setShowDelay(int delay) { tooltipDelay = delay; }

void NToolTipFilter::setPosition(NToolTipPosition pos) { position = pos; }

bool NToolTipFilter::canShowToolTip() const {
    QWidget* parentWidget = qobject_cast<QWidget*>(parent());
    return parentWidget && !parentWidget->toolTip().isEmpty() && parentWidget->isEnabled();
}