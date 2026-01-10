#include "QtNativeUI/NMainWindow.h"

#include <QPainter>
#include <qevent.h>

#include "../private/nframelesshelper_p.h"
#include "../private/nmainwindow_p.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NWindowBar.h"
#include "QtNativeUI/NWindowButton.h"

NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), d_ptr(new NMainWindowPrivate()) {
    d_ptr->q_ptr = this;

    Q_D(NMainWindow);
    d->setupFrameless();
    setupDefaultWindowBar();
    connectWindowBarSignals();
}

NMainWindow::~NMainWindow() { delete d_ptr; }

void NMainWindow::paintEvent(QPaintEvent* event) {
    Q_D(NMainWindow);

    if (d->frameless->backdropType() == NFramelessHelper::None) {
        QPainter painter(this);
        painter.fillRect(event->rect(), d->frameless->backgroundColor());
    }

    QMainWindow::paintEvent(event);
}

void NMainWindow::setupDefaultWindowBar() {
    Q_D(NMainWindow);

    d->windowBar = new NWindowBar(this);
    d->windowBar->setHostWidget(this);
    d->frameless->setTitleBar(d->windowBar);
    d->registerSystemButtons(d->windowBar);
    setMenuWidget(d->windowBar);
}

void NMainWindow::connectWindowBarSignals() {
    Q_D(NMainWindow);

    if (!d->windowBar)
        return;

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

void NMainWindow::setBackdropType(BackdropType type) {
    Q_D(NMainWindow);
    d->frameless->setBackdropType(static_cast<NFramelessHelper::BackdropType>(type));
    if (d->frameless->backdropType() == static_cast<NFramelessHelper::BackdropType>(type)) {
        emit backdropTypeChanged(type);
    }
}

NMainWindow::BackdropType NMainWindow::backdropType() const {
    Q_D(const NMainWindow);
    return static_cast<BackdropType>(d->frameless->backdropType());
}

int NMainWindow::borderThickness() const {
    Q_D(const NMainWindow);
    return d->frameless->borderThickness();
}

int NMainWindow::titleBarHeight() const {
    Q_D(const NMainWindow);
    return d->frameless->titleBarHeight();
}

#ifdef Q_OS_MAC
void NMainWindow::setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback) {
    Q_D(NMainWindow);
    d->frameless->setSystemButtonAreaCallback(callback);
}

void NMainWindow::setNativeSystemButtonsVisible(bool visible) {
    Q_D(NMainWindow);
    d->frameless->setNativeSystemButtonsVisible(visible);
}
#endif

bool NMainWindow::setWindowAttribute(const QString& key, const QVariant& value) {
    Q_D(NMainWindow);
    return d->frameless->setWindowAttribute(key, value);
}

QVariant NMainWindow::windowAttribute(const QString& key) const {
    Q_D(const NMainWindow);
    return d->frameless->windowAttribute(key);
}

NWindowBar* NMainWindow::windowBar() const {
    Q_D(const NMainWindow);
    return d->windowBar;
}

void NMainWindow::setWindowBar(NWindowBar* bar) {
    Q_D(NMainWindow);

    if (d->windowBar) {
        disconnect(d->windowBar, nullptr, this, nullptr);
        d->windowBar->deleteLater();
    }

    d->windowBar = bar;

    if (bar) {
        bar->setHostWidget(this);
        d->frameless->setTitleBar(bar);
        d->registerSystemButtons(bar);
        setMenuWidget(bar);
        connectWindowBarSignals();
    }
}

void NMainWindow::setSystemButton(SystemButtonType type, QAbstractButton* button) {
    Q_D(NMainWindow);

    if (type == Pin || type == Theme) {
        d->frameless->setHitTestVisible(button, true);
        return;
    }

    NFramelessHelper::SystemButtonType helperType;
    switch (type) {
        case WindowIcon:
            helperType = NFramelessHelper::WindowIcon;
            break;
        case Minimize:
            helperType = NFramelessHelper::Minimize;
            break;
        case Maximize:
            helperType = NFramelessHelper::Maximize;
            break;
        case Close:
            helperType = NFramelessHelper::Close;
            break;
        default:
            return;
    }

    d->frameless->setSystemButton(helperType, button);
}

NWindowButton* NMainWindow::systemButton(SystemButtonType type) const {
    Q_D(const NMainWindow);
    if (!d->windowBar)
        return nullptr;

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

void NMainWindow::setHitTestVisible(QWidget* widget, bool visible) {
    Q_D(NMainWindow);
    d->frameless->setHitTestVisible(widget, visible);
}

void NMainWindow::setTitleBarWidget(QWidget* widget) {
    Q_D(NMainWindow);
    d->frameless->setTitleBar(widget);
    setMenuWidget(widget);
}

QWidget* NMainWindow::titleBarWidget() const { return menuWidget(); }

QWK::WidgetWindowAgent* NMainWindow::windowAgent() const {
    Q_D(const NMainWindow);
    return d->frameless->windowAgent();
}

void NMainWindow::setMenuBar(QMenuBar* menuBar) {
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

QMenuBar* NMainWindow::menuBar() const {
    Q_D(const NMainWindow);
    return d->windowBar ? d->windowBar->menuBar() : nullptr;
}
