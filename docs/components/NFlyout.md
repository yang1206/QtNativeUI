# NFlyout 浮出菜单

### 介绍

浮出菜单是一个浮动的弹出窗口，用于显示临时内容、菜单或提示信息。NFlyout 支持多种动画效果、自定义内容、自动定位和外部点击关闭等功能。

### 基本用法

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 创建内容
QWidget* content = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(content);
layout->addWidget(new QLabel("这是浮出菜单内容"));
layout->addWidget(new NPushButton("确定"));

// 设置内容
flyout->setContent(content);

// 在按钮处显示
NPushButton* button = new NPushButton("显示菜单");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});
```

### 带菜单的浮出

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 创建菜单内容
NMenu* menu = new NMenu();
menu->addItem("编辑");
menu->addItem("删除");
menu->addItem("分享");

// 设置菜单为内容
flyout->setContent(menu);

// 在指定位置显示
NPushButton* moreButton = new NPushButton("更多");
connect(moreButton, &NPushButton::clicked, [=]() {
    flyout->showAt(moreButton);
});
```

### 自定义内容

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 创建自定义内容
QWidget* customContent = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(customContent);

// 添加标题
QLabel* title = new QLabel("选择操作");
title->setStyleSheet("font-weight: bold; font-size: 14px;");
layout->addWidget(title);

// 添加选项
QStringList options = {"选项1", "选项2", "选项3"};
for (const QString& option : options) {
    NPushButton* btn = new NPushButton(option);
    connect(btn, &NPushButton::clicked, [=]() {
        qDebug() << "选择了：" << option;
        flyout->hide();
    });
    layout->addWidget(btn);
}

flyout->setContent(customContent);

// 显示浮出菜单
NPushButton* showBtn = new NPushButton("显示");
connect(showBtn, &NPushButton::clicked, [=]() {
    flyout->showAt(showBtn);
});
```

### 动画效果

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 设置动画类型
flyout->setAnimationType(NFlyoutAnimationType::PULL_UP);  // 向上弹出
// 或其他动画类型：
// NFlyoutAnimationType::DROP_DOWN    // 向下弹出
// NFlyoutAnimationType::SLIDE_LEFT   // 从右向左滑入
// NFlyoutAnimationType::SLIDE_RIGHT  // 从左向右滑入
// NFlyoutAnimationType::FADE_IN      // 淡入
// NFlyoutAnimationType::NONE         // 无动画

// 创建内容
QLabel* content = new QLabel("带动画的浮出菜单");
flyout->setContent(content);

// 显示
NPushButton* button = new NPushButton("显示");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});
```

### 定位控制

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 设置放置位置
flyout->setPlacement(Qt::TopEdge);  // 在目标上方
// 或其他位置：
// Qt::BottomEdge  // 在目标下方
// Qt::LeftEdge    // 在目标左方
// Qt::RightEdge   // 在目标右方

// 创建内容
QLabel* content = new QLabel("定位的浮出菜单");
flyout->setContent(content);

// 在指定控件处显示
NPushButton* button = new NPushButton("显示");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});
```

### 外部点击关闭

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 设置外部点击关闭模式
flyout->setLightDismissMode(NFlyout::On);  // 启用外部点击关闭
// 或：
// NFlyout::Off   // 禁用外部点击关闭
// NFlyout::Auto  // 按平台决定

// 创建内容
QLabel* content = new QLabel("点击外部关闭");
flyout->setContent(content);

// 显示
NPushButton* button = new NPushButton("显示");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});

// 监听关闭事件
connect(flyout, &NFlyout::closed, [=]() {
    qDebug() << "浮出菜单已关闭";
});
```

### 阴影效果

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 设置阴影效果
flyout->setShadowEffect(35, QPoint(0, 8));  // 模糊半径35，偏移(0, 8)

// 创建内容
QLabel* content = new QLabel("带阴影的浮出菜单");
flyout->setContent(content);

// 显示
NPushButton* button = new NPushButton("显示");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});
```

### 内容边距

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 设置内容边距
flyout->setContentsMargins(16, 12, 16, 12);

// 创建内容
QWidget* content = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(content);
layout->addWidget(new QLabel("内容1"));
layout->addWidget(new QLabel("内容2"));

flyout->setContent(content);

// 显示
NPushButton* button = new NPushButton("显示");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});
```

### 快速创建

```cpp
// 使用静态方法快速创建和显示浮出菜单
QWidget* content = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(content);
layout->addWidget(new QLabel("快速创建的浮出菜单"));

NPushButton* button = new NPushButton("显示");

NFlyout* flyout = NFlyout::createWithContent(
    content,
    button,
    nullptr,
    NFlyoutAnimationType::PULL_UP,
    true  // 关闭时自动删除
);

connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});
```

### 自定义样式

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 设置圆角
flyout->setBorderRadius(8);

// 设置边框
flyout->setBorderWidth(1);

// 自定义颜色
flyout->setLightBackgroundColor(QColor(255, 255, 255));
flyout->setLightBorderColor(QColor(200, 200, 200));
flyout->setDarkBackgroundColor(QColor(45, 45, 45));
flyout->setDarkBorderColor(QColor(80, 80, 80));

// 创建内容
QLabel* content = new QLabel("自定义样式的浮出菜单");
flyout->setContent(content);

// 显示
NPushButton* button = new NPushButton("显示");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});
```

### 淡出关闭

```cpp
// 创建浮出菜单
NFlyout* flyout = new NFlyout();

// 创建内容
QLabel* content = new QLabel("浮出菜单");
flyout->setContent(content);

// 显示
NPushButton* button = new NPushButton("显示");
connect(button, &NPushButton::clicked, [=]() {
    flyout->showAt(button);
});

// 淡出关闭
NPushButton* closeBtn = new NPushButton("淡出关闭");
connect(closeBtn, &NPushButton::clicked, [=]() {
    flyout->fadeOut();
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NFlyout(QWidget* parent = nullptr)` | 创建浮出菜单 |

### 内容管理

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setContent(QWidget* content)` | 设置浮出菜单内容 | 内容控件 |
| `content()` | 获取浮出菜单内容 | 返回 `QWidget*` |

### 目标和定位

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setTarget(QWidget* target)` | 设置目标控件 | 目标控件 |
| `target()` | 获取目标控件 | 返回 `QWidget*` |
| `setPlacement(Qt::Edge placement)` | 设置放置位置 | `Qt::TopEdge`、`Qt::BottomEdge` 等 |
| `placement()` | 获取放置位置 | 返回 `Qt::Edge` |

### 动画

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setAnimationType(NFlyoutAnimationType type)` | 设置动画类型 | `PULL_UP`、`DROP_DOWN`、`SLIDE_LEFT`、`SLIDE_RIGHT`、`FADE_IN`、`NONE` |
| `animationType()` | 获取动画类型 | 返回 `NFlyoutAnimationType` |

### 外观控制

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setBorderRadius(int radius)` | 设置圆角半径 | 半径像素值 |
| `borderRadius()` | 获取圆角半径 | 返回 `int` |
| `setBorderWidth(int width)` | 设置边框宽度 | 宽度像素值 |
| `borderWidth()` | 获取边框宽度 | 返回 `int` |
| `setContentsMargins(const QMargins& margins)` | 设置内容边距 | 边距对象 |
| `contentsMargins()` | 获取内容边距 | 返回 `QMargins` |

### 关闭模式

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setLightDismissMode(LightDismissMode mode)` | 设置外部点击关闭模式 | `Auto`、`On`、`Off` |
| `lightDismissMode()` | 获取外部点击关闭模式 | 返回 `LightDismissMode` |

### 效果

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setShadowEffect(int blurRadius, const QPoint& offset)` | 设置阴影效果 | 模糊半径、偏移量 |

### 显示/隐藏

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `showAt(QWidget* target)` | 在指定控件处显示 | 目标控件 |
| `exec(const QPoint& pos)` | 在指定坐标显示 | 全局坐标 |
| `hide()` | 隐藏浮出菜单 | - |
| `fadeOut()` | 淡出并关闭 | - |
| `isOpen()` | 判断是否已打开 | 返回 `bool` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightBackgroundColor` | 明亮主题背景色 | `QColor` |
| `darkBackgroundColor` | 暗黑主题背景色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `opened()` | 浮出菜单已打开 | - |
| `opening()` | 浮出菜单正在打开 | - |
| `closed()` | 浮出菜单已关闭 | - |
| `closing()` | 浮出菜单正在关闭 | - |

### 静态方法

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `createWithContent(QWidget* content, QWidget* target, QWidget* parent, NFlyoutAnimationType animType, bool isDeleteOnClose)` | 快速创建浮出菜单 | 返回 `NFlyout*` |

## 主题定制

浮出菜单会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
