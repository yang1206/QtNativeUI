# NMainWindow 主窗口

## 介绍

NMainWindow 是 QtNativeUI 的核心窗口组件，基于 Qt 的 QMainWindow 扩展而来。它集成了 QWindowKit 库，提供了现代化的窗口效果、自定义标题栏以及跨平台的原生窗口体验。

## 基本用法

```cpp
#include <QtNativeUI/NMainWindow.h>

// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("我的应用");
mainWindow->resize(800, 600);

// 创建中心控件
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);
layout->addWidget(new QLabel("欢迎使用 QtNativeUI"));

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

## 窗口背景效果

### Mica 效果 (Windows 11)

```cpp
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("Mica 效果");

// 应用 Mica 背景效果
mainWindow->setWindowEffect(NMainWindow::Mica);

// 检查当前效果
if (mainWindow->windowEffect() == NMainWindow::Mica) {
    qDebug() << "Mica 效果已应用";
}

mainWindow->show();
```

### Acrylic 效果 (Windows 10/11)

```cpp
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("Acrylic 效果");

// 应用 Acrylic 背景效果
mainWindow->setWindowEffect(NMainWindow::Acrylic);

mainWindow->show();
```

### 模糊效果 (跨平台)

```cpp
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("模糊效果");

// 应用模糊效果 (macOS 原生支持，Windows 使用 DWM 模糊)
mainWindow->setWindowEffect(NMainWindow::Blur);

mainWindow->show();
```

## 标题栏定制

### 添加菜单栏

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 创建菜单栏
QMenuBar* menuBar = new QMenuBar();
QMenu* fileMenu = menuBar->addMenu("文件");
fileMenu->addAction("新建");
fileMenu->addAction("打开");
fileMenu->addAction("保存");
fileMenu->addSeparator();
fileMenu->addAction("退出");

// 设置菜单栏 (会自动集成到标题栏中)
mainWindow->setMenuBar(menuBar);

mainWindow->show();
```

### 获取系统按钮

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 获取主题切换按钮
NWindowButton* themeBtn = mainWindow->systemButton(NMainWindow::Theme);
if (themeBtn) {
    // 自定义主题按钮的工具提示
    themeBtn->setToolTip("切换亮色/暗色主题");
}

// 获取置顶按钮
NWindowButton* pinBtn = mainWindow->systemButton(NMainWindow::Pin);
if (pinBtn) {
    pinBtn->setToolTip("窗口置顶");
}

mainWindow->show();
```

### 自定义标题栏控件

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 创建自定义标题栏控件
QWidget* customTitleBar = new QWidget();
QHBoxLayout* layout = new QHBoxLayout(customTitleBar);

QLabel* titleLabel = new QLabel("自定义标题");
QPushButton* customBtn = new QPushButton("自定义按钮");

layout->addWidget(titleLabel);
layout->addStretch();
layout->addWidget(customBtn);

// 设置自定义标题栏
mainWindow->setTitleBarWidget(customTitleBar);

// 确保自定义按钮可以接收鼠标事件
mainWindow->setHitTestVisible(customBtn, true);

mainWindow->show();
```

## 信号处理

### 监听主题切换

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 监听主题按钮点击
connect(mainWindow, &NMainWindow::themeToggled, [](bool isDark) {
    qDebug() << "主题已切换为：" << (isDark ? "暗色" : "亮色");
});

mainWindow->show();
```

### 监听窗口操作

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 监听置顶状态变化
connect(mainWindow, &NMainWindow::pinButtonToggled, [](bool pinned) {
    qDebug() << "窗口置顶状态：" << (pinned ? "已置顶" : "取消置顶");
});

// 监听最大化状态变化
connect(mainWindow, &NMainWindow::maximizeButtonToggled, [](bool maximized) {
    qDebug() << "窗口状态：" << (maximized ? "已最大化" : "已还原");
});

// 监听窗口关闭
connect(mainWindow, &NMainWindow::windowClosed, []() {
    qDebug() << "窗口即将关闭";
});

mainWindow->show();
```

### 监听背景效果变化

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 监听背景效果变化
connect(mainWindow, &NMainWindow::windowEffectChanged, [](NMainWindow::WindowEffectType type) {
    switch (type) {
        case NMainWindow::None:
            qDebug() << "背景效果：无";
            break;
        case NMainWindow::Mica:
            qDebug() << "背景效果：Mica";
            break;
        case NMainWindow::Acrylic:
            qDebug() << "背景效果：Acrylic";
            break;
        case NMainWindow::Blur:
            qDebug() << "背景效果：模糊";
            break;
        case NMainWindow::MicaAlt:
            qDebug() << "背景效果：Mica Alt";
            break;
    }
});

mainWindow->show();
```

## 高级功能

### 直接操作 QWindowKit

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 获取 QWindowKit 代理对象
QWK::WidgetWindowAgent* agent = mainWindow->windowAgent();

// 设置自定义窗口属性
mainWindow->setWindowAttribute("dark-mode", true);

// 获取窗口属性
QVariant borderThickness = mainWindow->windowAttribute("border-thickness");
qDebug() << "窗口边框厚度：" << borderThickness.toInt();

mainWindow->show();
```

### macOS 特定功能

#### 隐藏原生系统按钮（红绿灯）

在 macOS 上，当使用自定义标题栏时，原生的红绿灯按钮默认仍然可见。QtNativeUI 提供了简单的 API 来隐藏这些按钮：

```cpp
#ifdef Q_OS_MAC
class MyMainWindow : public NMainWindow {
public:
    MyMainWindow(QWidget* parent = nullptr) : NMainWindow(parent) {}

protected:
    void showEvent(QShowEvent* event) override {
        NMainWindow::showEvent(event);
        // 隐藏 macOS 红绿灯按钮
        setNativeSystemButtonsVisible(false);
    }
};

MyMainWindow* mainWindow = new MyMainWindow();
mainWindow->show();
#endif
```

#### 自定义系统按钮位置

如果需要更精细的控制，可以使用回调函数自定义红绿灯按钮的位置：

```cpp
#ifdef Q_OS_MAC
NMainWindow* mainWindow = new NMainWindow();

// 将红绿灯移动到窗口右侧
mainWindow->setSystemButtonAreaCallback([](const QSize& size) {
    constexpr int width = 75;
    return QRect(QPoint(size.width() - width, 0), QSize(width, 32));
});

// 完全隐藏红绿灯（移到屏幕外）
mainWindow->setSystemButtonAreaCallback([](const QSize& size) {
    constexpr int width = -75;
    return QRect(QPoint(size.width() - width, 0), QSize(width, size.height()));
});

mainWindow->show();
#endif
```

**重要提示：**
- `setNativeSystemButtonsVisible()` 和 `setSystemButtonAreaCallback()` 必须在窗口显示后调用
- 推荐在 `showEvent()` 中调用，或者在 `show()` 之后立即调用
- 对于简单的隐藏需求，推荐使用 `setNativeSystemButtonsVisible(false)`

### 获取系统信息

```cpp
NMainWindow* mainWindow = new NMainWindow();

// 获取窗口边框厚度
int borderThickness = mainWindow->borderThickness();
qDebug() << "边框厚度：" << borderThickness << "像素";

// 获取标题栏高度
int titleBarHeight = mainWindow->titleBarHeight();
qDebug() << "标题栏高度：" << titleBarHeight << "像素";

mainWindow->show();
```

## 完整应用示例

```cpp
#include <QApplication>
#include <QtNativeUI/NMainWindow.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTheme.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建主窗口
    NMainWindow* mainWindow = new NMainWindow();
    mainWindow->setWindowTitle("QtNativeUI 完整示例");
    mainWindow->resize(1000, 700);

    // 应用现代背景效果
    mainWindow->setWindowEffect(NMainWindow::Mica);

    // 创建菜单栏
    QMenuBar* menuBar = new QMenuBar();
    QMenu* fileMenu = menuBar->addMenu("文件");
    fileMenu->addAction("新建", []() { qDebug() << "新建文件"; });
    fileMenu->addAction("打开", []() { qDebug() << "打开文件"; });
    fileMenu->addSeparator();
    fileMenu->addAction("退出", &app, &QApplication::quit);
    
    QMenu* viewMenu = menuBar->addMenu("视图");
    viewMenu->addAction("切换主题", []() {
        bool isDark = nTheme->isDarkMode();
        nTheme->setThemeMode(isDark ? NThemeType::ThemeMode::Light : NThemeType::ThemeMode::Dark);
    });
    
    mainWindow->setMenuBar(menuBar);

    // 创建中心控件
    QWidget* centralWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 添加标题
    QLabel* titleLabel = new QLabel("欢迎使用 QtNativeUI");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // 添加按钮
    NPushButton* testBtn = new NPushButton("测试按钮");
    testBtn->setToolTip("这是一个 Fluent Design 风格的按钮");
    layout->addWidget(testBtn, 0, Qt::AlignCenter);

    layout->addStretch();
    mainWindow->setCentralWidget(centralWidget);

    // 连接信号
    QObject::connect(mainWindow, &NMainWindow::themeToggled, [](bool isDark) {
        qDebug() << "主题已切换为：" << (isDark ? "暗色" : "亮色");
    });

    QObject::connect(mainWindow, &NMainWindow::pinButtonToggled, [](bool pinned) {
        qDebug() << "窗口置顶：" << (pinned ? "开启" : "关闭");
    });

    // 显示窗口
    mainWindow->show();

    return app.exec();
}
```

## API 参考

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NMainWindow(QWidget* parent = nullptr)` | 创建主窗口实例 |

### 背景效果

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setWindowEffect(WindowEffectType type)` | 设置窗口背景效果 | void |
| `windowEffect() const` | 获取当前背景效果类型 | WindowEffectType |

### 标题栏

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `windowBar() const` | 获取窗口标题栏组件 | NWindowBar* |
| `setWindowBar(NWindowBar* bar)` | 设置自定义标题栏组件 | void |
| `setTitleBarWidget(QWidget* widget)` | 设置自定义标题栏控件 | void |
| `titleBarWidget() const` | 获取当前标题栏控件 | QWidget* |

### 系统按钮

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `systemButton(SystemButtonType type) const` | 获取指定类型的系统按钮 | NWindowButton* |
| `setSystemButton(SystemButtonType type, QAbstractButton* button)` | 设置自定义系统按钮 | void |

### 菜单栏

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setMenuBar(QMenuBar* menuBar)` | 设置菜单栏 | void |
| `menuBar() const` | 获取当前菜单栏 | QMenuBar* |

### 窗口属性

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setWindowAttribute(const QString& key, const QVariant& value)` | 设置窗口属性 | bool |
| `windowAttribute(const QString& key) const` | 获取窗口属性 | QVariant |
| `borderThickness() const` | 获取窗口边框厚度 | int |
| `titleBarHeight() const` | 获取标题栏高度 | int |

### macOS 特定方法

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setNativeSystemButtonsVisible(bool visible)` | 设置原生系统按钮可见性 | void |
| `nativeSystemButtonsVisible() const` | 获取原生系统按钮可见性 | bool |
| `setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback)` | 设置系统按钮区域回调 | void |

### 高级功能

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `windowAgent() const` | 获取 QWindowKit 代理对象 | QWK::WidgetWindowAgent* |
| `setHitTestVisible(QWidget* widget, bool visible)` | 设置控件鼠标事件可见性 | void |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `windowEffectChanged(WindowEffectType type)` | 背景效果类型已更改 | 新的效果类型 |
| `themeToggled(bool isDark)` | 主题已切换 | 是否为暗色主题 |
| `pinButtonToggled(bool pinned)` | 置顶状态已切换 | 是否已置顶 |
| `maximizeButtonToggled(bool maximized)` | 最大化状态已切换 | 是否已最大化 |
| `windowClosed()` | 窗口即将关闭 | 无 |

### 枚举类型

#### WindowEffectType

| 值 | 说明 | 平台支持 |
|----|------|----------|
| `None` | 无特殊效果 | 所有平台 |
| `Blur` | 模糊效果 | Windows (DWM), macOS |
| `Acrylic` | 亚克力材质效果 | Windows 10/11 |
| `Mica` | Mica 材质效果 | Windows 11 |
| `MicaAlt` | Mica 替代材质效果 | Windows 11 |

#### SystemButtonType

| 值 | 说明 |
|----|------|
| `WindowIcon` | 窗口图标按钮 |
| `Theme` | 主题切换按钮 |
| `Pin` | 窗口置顶按钮 |
| `Minimize` | 最小化按钮 |
| `Maximize` | 最大化按钮 |
| `Close` | 关闭按钮 |

## 平台差异

### Windows
- 支持 Mica (Windows 11)、Acrylic (Windows 10/11)、Blur (Vista+) 效果
- 自动集成 Windows 11 Snap Layout 功能
- 支持 DWM 窗口效果

### macOS
- 支持 NSVisualEffectView 模糊效果
- 自动适配亮色/暗色主题
- 支持原生交通灯按钮定制

### Linux
- 所有背景效果回退到 None
- 基本的窗口功能正常工作
- 依赖 X11/Wayland 窗口管理器