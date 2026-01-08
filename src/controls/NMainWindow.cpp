#include "QtNativeUI/NMainWindow.h"

#include <QApplication>
#include <QPainter>
#include <QStyle>

#include "../private/nmainwindow_p.h"
#include "QtNativeUI/NTheme.h"
#include "QtNativeUI/NWindowBar.h"
#include "QtNativeUI/NWindowButton.h"

#include <QWKWidgets/widgetwindowagent.h>

NMainWindowPrivate::NMainWindowPrivate()  = default;
NMainWindowPrivate::~NMainWindowPrivate() = default;

void NMainWindowPrivate::updateBackgroundColor() {
    Q_Q(NMainWindow);
    backgroundColor = nTheme->isDarkMode()
                          ? nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Dark)
                          : nTheme->getColorForTheme(NFluentColorKey::SolidBackgroundFillColorBase, NThemeType::Light);
}

NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), d_ptr(new NMainWindowPrivate()) {
    d_ptr->q_ptr = this;
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_TranslucentBackground);

    setupWindowAgent();
    setupDefaultWindowBar();
    connectWindowBarSignals();
}

NMainWindow::~NMainWindow() { delete d_ptr; }

void NMainWindow::paintEvent(QPaintEvent* event) {
    Q_D(NMainWindow);

    if (d->backdropType == None) {
        QPainter painter(this);
        painter.fillRect(event->rect(), d->backgroundColor);
    }

    QMainWindow::paintEvent(event);
}

void NMainWindow::setupWindowAgent() {
    Q_D(NMainWindow);

    d->windowAgent = new QWK::WidgetWindowAgent(this);
    d->windowAgent->setup(this);

#ifdef Q_OS_WIN
    d->windowAgent->setWindowAttribute(QStringLiteral("dark-mode"), nTheme->isDarkMode());

    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        Q_D(NMainWindow);
        d->windowAgent->setWindowAttribute(QStringLiteral("dark-mode"), nTheme->isDarkMode());

        if (d->backdropType == None) {
            d->updateBackgroundColor();
            QPalette pal = palette();
            pal.setColor(QPalette::Window, d->backgroundColor);
            setPalette(pal);
            update();
        }

        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(this, &event);
    });
#endif
}

void NMainWindow::setupDefaultWindowBar() {
    Q_D(NMainWindow);

    d->windowBar = new NWindowBar(this);
    d->windowBar->setHostWidget(this);

    d->windowAgent->setTitleBar(d->windowBar);

#ifndef Q_OS_MAC
    d->windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, d->windowBar->systemButton(NWindowButton::Minimize));
    d->windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, d->windowBar->systemButton(NWindowButton::Maximize));
    d->windowAgent->setSystemButton(QWK::WindowAgentBase::Close, d->windowBar->systemButton(NWindowButton::Close));
    d->windowAgent->setHitTestVisible(d->windowBar->systemButton(NWindowButton::Theme), true);
    d->windowAgent->setHitTestVisible(d->windowBar->systemButton(NWindowButton::Pin), true);
#endif

    setMenuWidget(d->windowBar);
}

void NMainWindow::connectWindowBarSignals() {
    Q_D(NMainWindow);

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

    if (d->backdropType == type)
        return;

    applyBackdropType(type);
    d->backdropType = type;
    emit backdropTypeChanged(type);
}

void NMainWindow::applyBackdropType(BackdropType type) {
    Q_D(NMainWindow);

    bool needsRepaint = false;

#ifdef Q_OS_WIN
    static const QStringList effectKeys = {QString(),
                                           QStringLiteral("dwm-blur"),
                                           QStringLiteral("acrylic-material"),
                                           QStringLiteral("mica"),
                                           QStringLiteral("mica-alt")};

    for (int i = 1; i < effectKeys.size(); ++i) {
        d->windowAgent->setWindowAttribute(effectKeys[i], false);
    }

    if (type != None && type < effectKeys.size()) {
        d->windowAgent->setWindowAttribute(effectKeys[type], true);
        setPalette(QPalette());
        if (d->backdropType == None) {
            needsRepaint = true;
        }
    } else {
        d->updateBackgroundColor();
        QPalette pal = palette();
        pal.setColor(QPalette::Window, d->backgroundColor);
        setPalette(pal);
        needsRepaint = true;
    }

#elif defined(Q_OS_MAC)
    if (type == Blur) {
        d->windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), nTheme->isDarkMode() ? "dark" : "light");
        setPalette(QPalette());
        if (d->backdropType == None) {
            needsRepaint = true;
        }
    } else {
        d->windowAgent->setWindowAttribute(QStringLiteral("blur-effect"), "none");
        d->updateBackgroundColor();
        QPalette pal = palette();
        pal.setColor(QPalette::Window, d->backgroundColor);
        setPalette(pal);
        needsRepaint = true;
    }
#else
    Q_UNUSED(type)
#endif

    if (needsRepaint) {
        update();
    }
}

NMainWindow::BackdropType NMainWindow::backdropType() const {
    Q_D(const NMainWindow);
    return d->backdropType;
}

int NMainWindow::borderThickness() const {
    Q_D(const NMainWindow);
    QVariant val = d->windowAgent->windowAttribute(QStringLiteral("border-thickness"));
    return val.isValid() ? val.toInt() : 0;
}

int NMainWindow::titleBarHeight() const {
    Q_D(const NMainWindow);
    QVariant val = d->windowAgent->windowAttribute(QStringLiteral("title-bar-height"));
    return val.isValid() ? val.toInt() : 32;
}

#ifdef Q_OS_MAC
void NMainWindow::setNativeSystemButtonsVisible(bool visible) {
    Q_D(NMainWindow);
    d->windowAgent->setWindowAttribute(QStringLiteral("no-system-buttons"), !visible);
}

bool NMainWindow::nativeSystemButtonsVisible() const {
    Q_D(const NMainWindow);
    QVariant val = d->windowAgent->windowAttribute(QStringLiteral("no-system-buttons"));
    return val.isValid() ? !val.toBool() : true;
}
#endif

bool NMainWindow::setWindowAttribute(const QString& key, const QVariant& value) {
    Q_D(NMainWindow);
    return d->windowAgent->setWindowAttribute(key, value);
}

QVariant NMainWindow::windowAttribute(const QString& key) const {
    Q_D(const NMainWindow);
    return d->windowAgent->windowAttribute(key);
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
        d->windowAgent->setTitleBar(bar);

#ifndef Q_OS_MAC
        d->windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, bar->systemButton(NWindowButton::Minimize));
        d->windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, bar->systemButton(NWindowButton::Maximize));
        d->windowAgent->setSystemButton(QWK::WindowAgentBase::Close, bar->systemButton(NWindowButton::Close));
        d->windowAgent->setHitTestVisible(bar->systemButton(NWindowButton::Theme), true);
        d->windowAgent->setHitTestVisible(bar->systemButton(NWindowButton::Pin), true);
#endif

        setMenuWidget(bar);
        connectWindowBarSignals();
    }
}

void NMainWindow::setSystemButton(SystemButtonType type, QAbstractButton* button) {
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

void NMainWindow::setSystemButtonVisible(SystemButtonType type, bool visible) {
    Q_D(NMainWindow);
    if (d->windowBar) {
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
                return;
        }
        d->windowBar->setSystemButtonVisible(buttonType, visible);
    }
}

bool NMainWindow::systemButtonVisible(SystemButtonType type) const {
    Q_D(const NMainWindow);
    if (!d->windowBar) return false;
    
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
            return false;
    }
    return d->windowBar->systemButtonVisible(buttonType);
}

NWindowButton* NMainWindow::systemButton(SystemButtonType type) const {
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

void NMainWindow::setHitTestVisible(QWidget* widget, bool visible) {
    Q_D(NMainWindow);
    d->windowAgent->setHitTestVisible(widget, visible);
}

void NMainWindow::setTitleBarWidget(QWidget* widget) {
    Q_D(NMainWindow);
    d->windowAgent->setTitleBar(widget);
    setMenuWidget(widget);
}

QWidget* NMainWindow::titleBarWidget() const { return menuWidget(); }

QWK::WidgetWindowAgent* NMainWindow::windowAgent() const {
    Q_D(const NMainWindow);
    return d->windowAgent;
}

void NMainWindow::setMenuBar(QMenuBar* menuBar) {
    Q_D(NMainWindow);
    
    if (auto oldMenuBar = this->menuBar()) {
        setHitTestVisible(oldMenuBar, false);
    }
    
    d->windowBar->setMenuBar(menuBar);
    
    if (menuBar) {
        setHitTestVisible(menuBar, true);
    }
}

QMenuBar* NMainWindow::menuBar() const {
    Q_D(const NMainWindow);
    return d->windowBar->menuBar();
}
