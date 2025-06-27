// NMainWindow.cpp
#include <QApplication>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QtNativeUI/NMainWindow.h>
#include "../private/nmainwindow_p.h"
#include "../private/nwindoweffect_win.h"
#include "QtNativeUI/NTheme.h"

NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), d_ptr(new NMainWindowPrivate()) {
    d_ptr->q_ptr = this;
    initWindow();
}

NMainWindow::~NMainWindow() { delete d_ptr; }

void NMainWindow::initWindow() {
    setProperty("nMainWindow", true);
    setAttribute(Qt::WA_TranslucentBackground);
    initPlatformEffect();
    applyBackdropEffect(getDefaultEffect());

    installEventFilter(this);

    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        Q_D(NMainWindow);
        d->isDarkMode = nTheme->isDarkMode();

#ifdef Q_OS_WIN

        NWindowEffectWin::getInstance()->setDarkMode(this, d->isDarkMode);

        if (d->backdropEffect == None) {
            d->updateBackgroundColor();
            QPalette pal = palette();
            pal.setColor(QPalette::Window, d->backgroundColor);
            setPalette(pal);
            update();
        }
#endif
        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(this, &event);

        emit darkModeChanged(d->isDarkMode);
    });
}

void NMainWindow::initPlatformEffect() {
#ifdef Q_OS_WIN
    NWindowEffectWin::getInstance()->initialize();

    NWindowEffectWin::getInstance()->extendFrameIntoClientArea(this);

    NWindowEffectWin::getInstance()->setDarkMode(this, nTheme->isDarkMode());
#endif
}

bool NMainWindow::applyBackdropEffect(BackdropType type) {
    Q_D(NMainWindow);
    bool success = setPlatformEffect(type);

    if (success) {
        BackdropType oldType = d->backdropEffect;
        d->setBackdropEffect(type);

        if (oldType != type) {
            emit backdropEffectChanged(type);
        }
    }

    return success;
}

void NMainWindow::setBackdropEffect(BackdropType type) { applyBackdropEffect(type); }

NMainWindow::BackdropType NMainWindow::backdropEffect() const {
    Q_D(const NMainWindow);
    return d->backdropEffect;
}

bool NMainWindow::isDarkMode() const {
    Q_D(const NMainWindow);
    return d->isDarkMode;
}

bool NMainWindow::setPlatformEffect(BackdropType type) {
#ifdef Q_OS_WIN
    Q_D(NMainWindow);
    NWindowEffectWin::WindowBackdropType winType;
    switch (type) {
        case None:
            winType = NWindowEffectWin::None;
            break;
        case Mica:
            winType = NWindowEffectWin::Mica;
            break;
        case Acrylic:
            winType = NWindowEffectWin::Acrylic;
            break;
        case MicaAlt:
            winType = NWindowEffectWin::MicaAlt;
            break;
        case DWMBlur:
            winType = NWindowEffectWin::DWMBlur;
            break;
        default:
            winType = NWindowEffectWin::Auto;
    }
    NWindowEffectWin::WindowBackdropType oldWinType;
    switch (d->backdropEffect) {
        case None:
            oldWinType = NWindowEffectWin::None;
            break;
        case Mica:
            oldWinType = NWindowEffectWin::Mica;
            break;
        case Acrylic:
            oldWinType = NWindowEffectWin::Acrylic;
            break;
        case MicaAlt:
            oldWinType = NWindowEffectWin::MicaAlt;
            break;
        case DWMBlur:
            oldWinType = NWindowEffectWin::DWMBlur;
            break;
        default:
            oldWinType = NWindowEffectWin::Auto;
    }

    bool result = NWindowEffectWin::getInstance()->setWindowEffect(this, winType, oldWinType);

    if (type == None) {
        update();
    }

    return result;
#else
    Q_UNUSED(type);
    return true;
#endif
}

NMainWindow::BackdropType NMainWindow::getDefaultEffect() {
#ifdef Q_OS_WIN
    if (NWindowEffectWin::getInstance()->isWindows11_22H2OrGreater()) {
        return MicaAlt;
    } else if (NWindowEffectWin::getInstance()->isWindows11OrGreater()) {
        return Mica;
    } else if (NWindowEffectWin::getInstance()->isWindows10OrGreater()) {
        return Acrylic;
    } else {
        return None;
    }
#else
    return None;
#endif
}

void NMainWindow::enableWindowAnimation(bool enable) {
#ifdef Q_OS_WIN
    HWND  hwnd  = reinterpret_cast<HWND>(this->winId());
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);

    if (!enable) {
        style &= ~WS_CAPTION;
        style &= ~WS_THICKFRAME;
    } else {
        style |= WS_CAPTION;
        style |= WS_THICKFRAME;
    }

    SetWindowLong(hwnd, GWL_STYLE, style);
#else
    Q_UNUSED(enable);
#endif
}

void NMainWindow::setRoundedCorners(bool enable) {
#ifdef Q_OS_WIN
    if (NWindowEffectWin::getInstance()->isWindows11OrGreater()) {
        HWND  hwnd  = reinterpret_cast<HWND>(this->winId());
        DWORD value = enable ? 1 : 0;

        NWindowEffectWin::getInstance()->_dwmSetWindowAttribute(
            hwnd, NWindowEffectWin::DWMWA_WINDOW_CORNER_PREFERENCE, &value, sizeof(value));
    }
#else
    Q_UNUSED(enable);
#endif
}

void NMainWindow::setShadowEffect(bool enable) {
#ifdef Q_OS_WIN
    HWND    hwnd    = reinterpret_cast<HWND>(this->winId());
    MARGINS margins = {0};

    if (enable) {
        margins.cxLeftWidth = 1;
    }

    NWindowEffectWin::getInstance()->_dwmExtendFrameIntoClientArea(hwnd, &margins);
#else
    Q_UNUSED(enable);
#endif
}

bool NMainWindow::eventFilter(QObject* watched, QEvent* event) { return QMainWindow::eventFilter(watched, event); }

void NMainWindow::changeEvent(QEvent* event) {
    if (event->type() == QEvent::WindowStateChange) {
        applyBackdropEffect(backdropEffect());
    }

    QMainWindow::changeEvent(event);
}

bool NMainWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) {
    return QMainWindow::nativeEvent(eventType, message, result);
}

void NMainWindow::paintEvent(QPaintEvent* event) {
#ifdef Q_OS_WIN
    Q_D(NMainWindow);
    if (d->backdropEffect == None) {
        QPainter painter(this);
        painter.fillRect(rect(), d->backgroundColor);
    }

    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

#endif
    QMainWindow::paintEvent(event);
}