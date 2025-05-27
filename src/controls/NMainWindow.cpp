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
    // 初始化窗口属性
    setProperty("nMainWindow", true);
    setAttribute(Qt::WA_TranslucentBackground);

    // 初始化平台特定效果
    initPlatformEffect();

    // 设置默认效果
    applyBackdropEffect(getDefaultEffect());

    // 设置事件过滤器，处理所有子控件的样式保持
    installEventFilter(this);

    // 连接主题变化信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        Q_D(NMainWindow);
        d->isDarkMode = nTheme->isDarkMode();

#ifdef Q_OS_WIN
        // 更新窗口暗色模式
        NWindowEffectWin::getInstance()->setDarkMode(this, d->isDarkMode);

        // 如果是None模式，更新背景颜色
        if (d->backdropEffect == None) {
            d->updateBackgroundColor();
            QPalette pal = palette();
            pal.setColor(QPalette::Window, d->backgroundColor);
            setPalette(pal);
            update();
        }
#endif

        // 发送样式更新事件
        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(this, &event);

        emit darkModeChanged(d->isDarkMode);
    });
}

void NMainWindow::initPlatformEffect() {
#ifdef Q_OS_WIN
    // 初始化效果类
    NWindowEffectWin::getInstance()->initialize();

    // 初始化窗口效果
    NWindowEffectWin::getInstance()->extendFrameIntoClientArea(this);

    // 根据当前主题设置窗口暗色模式
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

    // 转换枚举类型
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

    // 获取旧效果类型
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
        // 确保窗口更新
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

        // Windows 11特有的圆角控制
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
        // 启用窗口阴影
        margins.cxLeftWidth = 1;
    }

    // 设置阴影效果
    NWindowEffectWin::getInstance()->_dwmExtendFrameIntoClientArea(hwnd, &margins);
#else
    Q_UNUSED(enable);
#endif
}

bool NMainWindow::eventFilter(QObject* watched, QEvent* event) { return QMainWindow::eventFilter(watched, event); }

void NMainWindow::changeEvent(QEvent* event) {
    if (event->type() == QEvent::WindowStateChange) {
        // 窗口状态变化时可能需要重新应用效果
        applyBackdropEffect(backdropEffect());
    }

    QMainWindow::changeEvent(event);
}

bool NMainWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) {
    return QMainWindow::nativeEvent(eventType, message, result);
}

void NMainWindow::paintEvent(QPaintEvent* event) {
    Q_D(NMainWindow);

    // 如果是None模式，确保背景颜色正确
    if (d->backdropEffect == None) {
        QPainter painter(this);
        painter.fillRect(rect(), d->backgroundColor);
    }

    // 处理子控件绘制
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QMainWindow::paintEvent(event);
}