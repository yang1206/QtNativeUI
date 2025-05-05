#include <QApplication>
#include <QEvent>
#include <QOperatingSystemVersion>
#include <QtNativeUI/NMainWindow.h>
#include "../private/nmainwindow_p.h"
#include "../private/nwindoweffect_win.h"
#include "QtNativeUI/NTheme.h"

NMainWindow::NMainWindow(QWidget* parent) : QMainWindow(parent), d_ptr(new NMainWindowPrivate()) {
    d_ptr->q_ptr = this;
    initWindow();
}

NMainWindow::~NMainWindow() {}

void NMainWindow::initWindow() {
    // 初始化平台特定效果前，添加一个标志表明这是NMainWindow
    setProperty("nMainWindow", true);

    // 初始化平台特定效果
    initPlatformEffect();

    // 设置默认效果
    applyBackdropEffect(getDefaultEffect());

    // 设置事件过滤器，处理所有子控件的样式保持
    installEventFilter(this);

    // 连接主题变化信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this]() {
        QEvent event(QEvent::StyleChange);
        QApplication::sendEvent(this, &event);
    });
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

        // 如果是暗黑模式效果，确保通知NTheme
        if (type == BackdropType::Mica || (type == BackdropType::Default && getDefaultEffect() == BackdropType::Mica)) {
            if (nTheme->themeMode() != NThemeType::ThemeMode::Dark) {
                // 只在需要时更新主题模式
                nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
            }
        }
    }

    return success;
}

void NMainWindow::setBackdropEffect(BackdropType type) {
    d_ptr->_pBackdropEffect = type;
    applyBackdropEffect(type);
}

int NMainWindow::getBackdropEffect() {
    Q_D(const NMainWindow);
    return d->_pBackdropEffect;
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

// 添加一个事件过滤器，统一处理所有子控件的样式保持
bool NMainWindow::eventFilter(QObject* watched, QEvent* event) {
    // 当子控件添加到窗口时
    if (event->type() == QEvent::ChildAdded) {
        QChildEvent* childEvent = static_cast<QChildEvent*>(event);
        QObject*     child      = childEvent->child();

        // 递归处理所有子控件
        protectWidgetStyle(qobject_cast<QWidget*>(child));
    } else if (event->type() == QEvent::StyleChange) {
        // 样式变化时，确保所有子控件更新样式
        updateChildrenStyles(this);
    }

    return QMainWindow::eventFilter(watched, event);
}

// 保护控件样式的通用方法
void NMainWindow::protectWidgetStyle(QWidget* widget) {
    if (!widget)
        return;

    // 排除NMainWindow自身
    if (widget->property("nMainWindow").toBool())
        return;

    // 对任何以"N"开头的自定义控件应用样式保护
    QString className = widget->metaObject()->className();
    if (className.startsWith('N') && className != "NMainWindow") {
        // 确保自定义控件保持其样式
        widget->setAttribute(Qt::WA_StyledBackground, true);

        // 确保控件的自动填充背景属性正确设置
        if (widget->testAttribute(Qt::WA_TranslucentBackground)) {
            // 对于透明背景的控件，不需要自动填充背景
            widget->setAutoFillBackground(false);
        } else {
            // 对于非透明背景的控件，需要自动填充背景
            widget->setAutoFillBackground(true);
        }
    }

    // 递归处理所有子控件
    for (QObject* child : widget->children()) {
        protectWidgetStyle(qobject_cast<QWidget*>(child));
    }
}

// 更新所有子控件样式的方法
void NMainWindow::updateChildrenStyles(QWidget* parent) {
    if (!parent)
        return;

    // 对所有子控件发送样式变化事件
    for (QObject* child : parent->children()) {
        QWidget* childWidget = qobject_cast<QWidget*>(child);
        if (childWidget) {
            // 确保自定义控件保持其样式
            QString className = childWidget->metaObject()->className();
            if (className.startsWith('N') && className != "NMainWindow") {
                // 发送样式更新事件
                QEvent styleEvent(QEvent::StyleChange);
                QApplication::sendEvent(childWidget, &styleEvent);
            }

            // 递归处理子控件的子控件
            updateChildrenStyles(childWidget);
        }
    }
}
