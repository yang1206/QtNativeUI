#include <QOperatingSystemVersion>
#include <QtNativeUI/NMainWindow.h>
#include "../private/nmainwindow_p.h"
#include "../private/nwindoweffect_win.h"

Q_PROPERTY_CREATE_Q_CPP(NMainWindow, int, BackdropEffect)

NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), d_ptr(new NMainWindowPrivate()) {
    d_ptr->q_ptr = this;
    initWindow();
}

NMainWindow::~NMainWindow() {}

void NMainWindow::initWindow() {
    // 初始化平台特定效果
    initPlatformEffect();

    // 设置默认效果
    applyBackdropEffect(getDefaultEffect());
}

void NMainWindow::initPlatformEffect() {
#ifdef Q_OS_WIN
    NWindowEffectWin::initWindowEffect(this);
#endif
    // 其他平台不需要特殊初始化
}

bool NMainWindow::applyBackdropEffect(BackdropType type) {
    Q_D(NMainWindow);
    bool success = setPlatformEffect(type);

    if (success) {
        d->setBackdropEffect(static_cast<int>(type));
    }

    return success;
}

NMainWindow::BackdropType NMainWindow::backdropEffect() const {
    Q_D(const NMainWindow);
    return static_cast<BackdropType>(d->getBackdropEffect());
}

bool NMainWindow::setPlatformEffect(BackdropType type) {
#ifdef Q_OS_WIN
    // Windows平台使用特殊效果
    return NWindowEffectWin::setWindowBackdropEffect(this, static_cast<int>(type));
#else
    // 其他平台直接忽略效果设置，返回成功
    Q_UNUSED(type);
    return true;
#endif
}

NMainWindow::BackdropType NMainWindow::getDefaultEffect() {
#ifdef Q_OS_WIN
    bool isWin11 =
        QOperatingSystemVersion::current() >= QOperatingSystemVersion(QOperatingSystemVersion::Windows, 10, 0, 22000);
    return isWin11 ? BackdropType::Mica : BackdropType::Acrylic;
#else
    // 其他平台默认无效果
    return BackdropType::None;
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
    // 其他平台不进行特殊处理
}
