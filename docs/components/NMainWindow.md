# NMainWindow 主窗口

### 介绍

主窗口是应用程序的顶级窗口，提供了现代化的窗口效果和主题支持。NMainWindow 基于 Qt 的 QMainWindow，集成了 Fluent Design 系统的窗口效果，支持 Mica、Acrylic 等现代背景效果。

### 基本用法

```cpp
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

### Mica 效果（Windows 11）

```cpp
// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("Mica 效果");

// 应用 Mica 背景效果
bool success = mainWindow->applyBackdropEffect(NMainWindow::Mica);
if (success) {
    qDebug() << "Mica 效果已应用";
} else {
    qDebug() << "Mica 效果不支持，使用默认效果";
}

// 创建内容
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);
layout->addWidget(new QLabel("这是 Mica 效果的窗口"));

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

### Acrylic 效果（Windows 10/11）

```cpp
// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("Acrylic 效果");

// 应用 Acrylic 背景效果
mainWindow->applyBackdropEffect(NMainWindow::Acrylic);

// 创建内容
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);
layout->addWidget(new QLabel("这是 Acrylic 效果的窗口"));

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

### 窗口动画

```cpp
// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("带动画的窗口");

// 启用窗口动画
mainWindow->enableWindowAnimation(true);

// 创建内容
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);
layout->addWidget(new QLabel("窗口打开和关闭时有动画效果"));

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

### 圆角窗口

```cpp
// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("圆角窗口");

// 启用圆角
mainWindow->setRoundedCorners(true);

// 创建内容
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);
layout->addWidget(new QLabel("窗口具有圆角"));

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

### 窗口阴影

```cpp
// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("带阴影的窗口");

// 启用阴影效果
mainWindow->setShadowEffect(true);

// 创建内容
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);
layout->addWidget(new QLabel("窗口具有阴影效果"));

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

### 主题响应

```cpp
// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("主题响应");

// 监听主题变化
connect(mainWindow, &NMainWindow::darkModeChanged, [](bool isDark) {
    qDebug() << "主题已切换为：" << (isDark ? "暗黑" : "明亮");
});

// 检查当前主题
if (mainWindow->isDarkMode()) {
    qDebug() << "当前使用暗黑主题";
} else {
    qDebug() << "当前使用明亮主题";
}

// 创建内容
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);
layout->addWidget(new QLabel("窗口会自动响应系统主题变化"));

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

### 完整应用示例

```cpp
// 创建主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("完整应用示例");
mainWindow->resize(1000, 700);

// 应用现代背景效果
mainWindow->applyBackdropEffect(NMainWindow::Mica);
mainWindow->enableWindowAnimation(true);
mainWindow->setRoundedCorners(true);
mainWindow->setShadowEffect(true);

// 创建菜单栏
QMenuBar* menuBar = mainWindow->menuBar();
QMenu* fileMenu = menuBar->addMenu("文件");
fileMenu->addAction("新建");
fileMenu->addAction("打开");
fileMenu->addAction("保存");
fileMenu->addSeparator();
fileMenu->addAction("退出");

// 创建工具栏
QToolBar* toolBar = mainWindow->addToolBar("工具栏");
toolBar->addAction("新建");
toolBar->addAction("打开");
toolBar->addAction("保存");

// 创建中心控件
QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);

// 添加标题
QLabel* titleLabel = new QLabel("欢迎使用应用");
titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
layout->addWidget(titleLabel);

// 添加内容
layout->addWidget(new QLabel("这是一个使用 NMainWindow 的完整应用示例"));
layout->addStretch();

mainWindow->setCentralWidget(centralWidget);

// 创建状态栏
QStatusBar* statusBar = mainWindow->statusBar();
statusBar->showMessage("就绪");

mainWindow->show();
```

### 多窗口应用

```cpp
// 主窗口
NMainWindow* mainWindow = new NMainWindow();
mainWindow->setWindowTitle("主窗口");
mainWindow->resize(800, 600);
mainWindow->applyBackdropEffect(NMainWindow::Mica);

QWidget* centralWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(centralWidget);

// 打开新窗口按钮
NPushButton* newWindowBtn = new NPushButton("打开新窗口");
connect(newWindowBtn, &NPushButton::clicked, [=]() {
    NMainWindow* childWindow = new NMainWindow();
    childWindow->setWindowTitle("子窗口");
    childWindow->resize(600, 400);
    childWindow->applyBackdropEffect(NMainWindow::Acrylic);
    
    QWidget* childCentral = new QWidget();
    QVBoxLayout* childLayout = new QVBoxLayout(childCentral);
    childLayout->addWidget(new QLabel("这是一个子窗口"));
    
    childWindow->setCentralWidget(childCentral);
    childWindow->show();
});

layout->addWidget(newWindowBtn);
layout->addStretch();

mainWindow->setCentralWidget(centralWidget);
mainWindow->show();
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NMainWindow(QWidget* parent = nullptr)` | 创建主窗口 |

### 背景效果

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `applyBackdropEffect(BackdropType type)` | 应用背景效果 | `None`、`Mica`、`Acrylic`、`MicaAlt`、`DWMBlur`，返回 `bool` |
| `setBackdropEffect(BackdropType type)` | 设置背景效果 | 效果类型 |
| `backdropEffect()` | 获取当前背景效果 | 返回 `BackdropType` |

### 窗口效果

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `enableWindowAnimation(bool enable)` | 启用/禁用窗口动画 | 布尔值 |
| `setRoundedCorners(bool enable)` | 启用/禁用圆角 | 布尔值 |
| `setShadowEffect(bool enable)` | 启用/禁用阴影效果 | 布尔值 |

### 主题

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `isDarkMode()` | 判断是否为暗黑主题 | 返回 `bool` |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `backdropEffectChanged(BackdropType type)` | 背景效果已更改 | 新的效果类型 |
| `darkModeChanged(bool isDark)` | 主题已更改 | 是否为暗黑主题 |

### 继承的 API

NMainWindow 继承自 `QMainWindow`，支持所有标准 QMainWindow 的方法：

- `setCentralWidget()` / `centralWidget()` - 设置/获取中心控件
- `menuBar()` - 获取菜单栏
- `addToolBar()` - 添加工具栏
- `statusBar()` - 获取状态栏
- `setWindowTitle()` / `windowTitle()` - 设置/获取窗口标题

## 背景效果说明

| 效果 | 平台 | 说明 |
|------|------|------|
| `Mica` | Windows 11+ | 现代 Mica 材质效果 |
| `Acrylic` | Windows 10/11 | 亚克力毛玻璃效果 |
| `MicaAlt` | Windows 11+ | Mica 替代效果 |
| `DWMBlur` | Windows Vista+ | DWM 模糊效果 |
| `None` | 所有平台 | 无特殊效果 |

## 主题定制

主窗口会自动响应系统主题变化，支持明亮和暗黑主题的自动切换。
