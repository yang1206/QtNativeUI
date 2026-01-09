#include "QtNativeUI/NMainWindow.h"

#include <QPainter>
#include <functional>

#include "../private/nmainwindow_p.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NWindowBar.h"
#include "QtNativeUI/NWindowButton.h"

#include <QWKWidgets/widgetwindowagent.h>

NMainWindow::NMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , d_ptr(new NMainWindowPrivate())
{
    d_ptr->q_ptr = this;
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_TranslucentBackground);

    Q_D(NMainWindow);
    d->setupWindowAgent();
    d->setupThemeConnection();
    setupDefaultWindowBar();
    connectWindowBarSignals();
}

NMainWindow::~NMainWindow()
{
    delete d_ptr;
}

void NMainWindow::paintEvent(QPaintEvent* event)
{
    Q_D(NMainWindow);

    if (d->backdropType == None) {
        QPainter painter(this);
        painter.fillRect(event->rect(), d->backgroundColor);
    }

    QMainWindow::paintEvent(event);
}

void NMainWindow::setupDefaultWindowBar()
{
    Q_D(NMainWindow);

    d->windowBar = new NWindowBar(this);
    d->windowBar->setHostWidget(this);
    d->windowAgent->setTitleBar(d->windowBar);
    d->registerSystemButtons(d->windowBar);
    setMenuWidget(d->windowBar);
}

void NMainWindow::connectWindowBarSignals()
{
    Q_D(NMainWindow);

    if (!d->windowBar) return;

    connect(d->windowBar, &NWindowBar::minimizeRequested, this, &QWidget::showMinimized);

    connect(d->windowBar, &NWindowBar::pinRequested, this, [this](bool pinned) {
        setWindowFlag(Qt::WindowStaysOnTopHint, pinned);
        show();
        emit pinButtonToggled(pinned);
    });

    connect(d->windowBar, &NWindowBar::themeRequested, this, [this]() {
        auto currentMode = nTheme->themeMode();
        bool willBeDark;
        if (currentMode == NThemeType::ThemeMode::Light) {
            nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
            willBeDark = true;
        } else {
            nTheme->setThemeMode(NThemeType::ThemeMode::Light);
            willBeDark = false;
        }
        emit themeToggled(willBeDark);
    });

    connect(d->windowBar, &NWindowBar::maximizeRequested, this, [this]() {
        bool willMaximize = !isMaximized();
        if (willMaximize) {
            showMaximized();
        } else {
            showNormal();
        }
        emit maximizeButtonToggled(willMaximize);
    });

    connect(d->windowBar, &NWindowBar::closeRequested, this, [this]() {
        close();
        emit windowClosed();
    });
}

void NMainWindow::setBackdropType(BackdropType type)
{
    Q_D(NMainWindow);

    if (d->backdropType == type)
        return;

    d->applyBackdropEffect(type);
    d->backdropType = type;
    emit backdropTypeChanged(type);
}

NMainWindow::BackdropType NMainWindow::backdropType() const
{
    Q_D(const NMainWindow);
    return d->backdropType;
}

int NMainWindow::borderThickness() const
{
    Q_D(const NMainWindow);
    QVariant val = d->windowAgent->windowAttribute(QStringLiteral("border-thickness"));
    return val.isValid() ? val.toInt() : 0;
}

int NMainWindow::titleBarHeight() const
{
    Q_D(const NMainWindow);
    QVariant val = d->windowAgent->windowAttribute(QStringLiteral("title-bar-height"));
    return val.isValid() ? val.toInt() : 32;
}

#ifdef Q_OS_MAC
void NMainWindow::setNativeSystemButtonsVisible(bool visible)
{
    Q_D(NMainWindow);
    d->windowAgent->setWindowAttribute(QStringLiteral("no-system-buttons"), !visible);
}

bool NMainWindow::nativeSystemButtonsVisible() const
{
    Q_D(const NMainWindow);
    QVariant val = d->windowAgent->windowAttribute(QStringLiteral("no-system-buttons"));
    return val.isValid() ? !val.toBool() : true;
}

void NMainWindow::setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback)
{
    Q_D(NMainWindow);
    d->windowAgent->setSystemButtonAreaCallback(callback);
}
#endif

bool NMainWindow::setWindowAttribute(const QString& key, const QVariant& value)
{
    Q_D(NMainWindow);
    return d->windowAgent->setWindowAttribute(key, value);
}

QVariant NMainWindow::windowAttribute(const QString& key) const
{
    Q_D(const NMainWindow);
    return d->windowAgent->windowAttribute(key);
}

NWindowBar* NMainWindow::windowBar() const
{
    Q_D(const NMainWindow);
    return d->windowBar;
}

void NMainWindow::setWindowBar(NWindowBar* bar)
{
    Q_D(NMainWindow);

    if (d->windowBar) {
        disconnect(d->windowBar, nullptr, this, nullptr);
        d->windowBar->deleteLater();
    }

    d->windowBar = bar;

    if (bar) {
        bar->setHostWidget(this);
        d->windowAgent->setTitleBar(bar);
        d->registerSystemButtons(bar);
        setMenuWidget(bar);
        connectWindowBarSignals();
    }
}

void NMainWindow::setSystemButton(SystemButtonType type, QAbstractButton* button)
{
    Q_D(NMainWindow);

    if (type == Pin || type == Theme) {
        d->windowAgent->setHitTestVisible(button, true);
        return;
    }

    QWK::WindowAgentBase::SystemButton qwkType;
    switch (type) {
        case WindowIcon:
            qwkType = QWK::WindowAgentBase::WindowIcon;
            break;
        case Minimize:
            qwkType = QWK::WindowAgentBase::Minimize;
            break;
        case Maximize:
            qwkType = QWK::WindowAgentBase::Maximize;
            break;
        case Close:
            qwkType = QWK::WindowAgentBase::Close;
            break;
        default:
            return;
    }

    d->windowAgent->setSystemButton(qwkType, button);
}

NWindowButton* NMainWindow::systemButton(SystemButtonType type) const
{
    Q_D(const NMainWindow);
    if (!d->windowBar) return nullptr;

    NWindowButton::SystemButtonType buttonType;
    switch (type) {
        case Theme:
            buttonType = NWindowButton::Theme;
            break;
        case Pin:
            buttonType = NWindowButton::Pin;
            break;
        case Minimize:
            buttonType = NWindowButton::Minimize;
            break;
        case Maximize:
            buttonType = NWindowButton::Maximize;
            break;
        case Close:
            buttonType = NWindowButton::Close;
            break;
        default:
            return nullptr;
    }
    return d->windowBar->systemButton(buttonType);
}

void NMainWindow::setHitTestVisible(QWidget* widget, bool visible)
{
    Q_D(NMainWindow);
    d->windowAgent->setHitTestVisible(widget, visible);
}

void NMainWindow::setTitleBarWidget(QWidget* widget)
{
    Q_D(NMainWindow);
    d->windowAgent->setTitleBar(widget);
    setMenuWidget(widget);
}

QWidget* NMainWindow::titleBarWidget() const
{
    return menuWidget();
}

QWK::WidgetWindowAgent* NMainWindow::windowAgent() const
{
    Q_D(const NMainWindow);
    return d->windowAgent;
}

void NMainWindow::setMenuBar(QMenuBar* menuBar)
{
    Q_D(NMainWindow);

    if (auto oldMenuBar = this->menuBar()) {
        setHitTestVisible(oldMenuBar, false);
    }

    if (d->windowBar) {
        d->windowBar->setMenuBar(menuBar);
    }

    if (menuBar) {
        setHitTestVisible(menuBar, true);
    }
}

QMenuBar* NMainWindow::menuBar() const
{
    Q_D(const NMainWindow);
    return d->windowBar ? d->windowBar->menuBar() : nullptr;
}
