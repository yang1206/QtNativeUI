# NToggleButton 切换按钮

### 介绍

切换按钮是一个可以在两种状态之间切换的按钮。NToggleButton 提供了现代化的外观、图标支持和丰富的自定义选项。

### 基本用法

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton("启用");

// 设置初始状态
toggleBtn->setChecked(true);

// 监听状态变化
connect(toggleBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "按钮状态：" << (checked ? "启用" : "禁用");
});

// 监听点击
connect(toggleBtn, QOverload<bool>::of(&NToggleButton::clicked), [](bool checked) {
    qDebug() << "按钮被点击，当前状态：" << checked;
});
```

### 带图标的切换按钮

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton("通知");

// 设置图标
toggleBtn->setFluentIcon(NRegularIconType::Bell24Regular, 20);

// 设置初始状态
toggleBtn->setChecked(true);

// 监听状态变化
connect(toggleBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "通知已" << (checked ? "启用" : "禁用");
});
```

### 功能开关

```cpp
// 创建功能开关
NToggleButton* darkModeBtn = new NToggleButton("暗黑模式");
darkModeBtn->setFluentIcon(NRegularIconType::WeatherMoon24Regular, 18);

NToggleButton* soundBtn = new NToggleButton("声音");
soundBtn->setFluentIcon(NRegularIconType::Speaker224Regular, 18);

NToggleButton* vibrationBtn = new NToggleButton("振动");
vibrationBtn->setFluentIcon(NRegularIconType::PhoneVibrate24Regular, 18);

// 处理状态变化
connect(darkModeBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "暗黑模式：" << (checked ? "开启" : "关闭");
});

connect(soundBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "声音：" << (checked ? "开启" : "关闭");
});

connect(vibrationBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "振动：" << (checked ? "开启" : "关闭");
});
```

### 切换状态

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton("切换");

// 获取当前状态
bool isChecked = toggleBtn->isChecked();
qDebug() << "当前状态：" << isChecked;

// 设置状态
toggleBtn->setChecked(!isChecked);

// 切换状态
toggleBtn->toggle();
```

### 自定义文本

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton();

// 设置文本
toggleBtn->setText("自定义文本");

// 获取文本
QString text = toggleBtn->text();
qDebug() << "按钮文本：" << text;

// 监听状态变化并更新文本
connect(toggleBtn, QOverload<bool>::of(&NToggleButton::toggled), [=](bool checked) {
    toggleBtn->setText(checked ? "已启用" : "已禁用");
});
```

### 自定义样式

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton("样式");

// 设置圆角
toggleBtn->setBorderRadius(6);

// 自定义颜色
toggleBtn->setLightDefaultColor(QColor(240, 240, 240));
toggleBtn->setLightHoverColor(QColor(230, 230, 230));
toggleBtn->setLightPressColor(QColor(200, 200, 200));

toggleBtn->setLightTextDefaultColor(QColor(50, 50, 50));
toggleBtn->setLightTextPressColor(QColor(0, 0, 0));

toggleBtn->setLightBorderColor(QColor(200, 200, 200));

// 强调色
toggleBtn->setAccentDefaultColor(QColor(0, 120, 215));
toggleBtn->setAccentHoverColor(QColor(0, 140, 235));
toggleBtn->setAccentPressColor(QColor(0, 100, 195));
toggleBtn->setAccentTextColor(QColor(255, 255, 255));

// 暗黑主题
toggleBtn->setDarkDefaultColor(QColor(50, 50, 50));
toggleBtn->setDarkHoverColor(QColor(70, 70, 70));
toggleBtn->setDarkPressColor(QColor(100, 100, 100));

toggleBtn->setDarkTextDefaultColor(QColor(200, 200, 200));
toggleBtn->setDarkTextPressColor(QColor(255, 255, 255));

toggleBtn->setDarkBorderColor(QColor(80, 80, 80));
```

### 禁用状态

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton("禁用示例");

// 禁用按钮
toggleBtn->setEnabled(false);

// 自定义禁用状态颜色
toggleBtn->setAccentDisabledColor(QColor(200, 200, 200));
toggleBtn->setAccentDisabledTextColor(QColor(150, 150, 150));
```

### 图标大小

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton("图标");

// 设置图标
toggleBtn->setFluentIcon(NRegularIconType::Star24Regular);

// 设置图标大小
toggleBtn->setIconSize(QSize(24, 24));

// 获取图标大小
QSize iconSize = toggleBtn->iconSize();
qDebug() << "图标大小：" << iconSize.width() << "x" << iconSize.height();
```

### 按钮大小

```cpp
// 创建切换按钮
NToggleButton* toggleBtn = new NToggleButton("大按钮");

// 设置最小大小
toggleBtn->setMinimumWidth(120);
toggleBtn->setMinimumHeight(40);

// 获取推荐大小
QSize sizeHint = toggleBtn->sizeHint();
qDebug() << "推荐大小：" << sizeHint.width() << "x" << sizeHint.height();
```

### 按钮组

```cpp
// 创建按钮组
QWidget* buttonGroup = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(buttonGroup);

// 创建多个切换按钮
NToggleButton* btn1 = new NToggleButton("选项1");
NToggleButton* btn2 = new NToggleButton("选项2");
NToggleButton* btn3 = new NToggleButton("选项3");

layout->addWidget(btn1);
layout->addWidget(btn2);
layout->addWidget(btn3);

// 互斥选择
connect(btn1, QOverload<bool>::of(&NToggleButton::toggled), [=](bool checked) {
    if (checked) {
        btn2->setChecked(false);
        btn3->setChecked(false);
    }
});

connect(btn2, QOverload<bool>::of(&NToggleButton::toggled), [=](bool checked) {
    if (checked) {
        btn1->setChecked(false);
        btn3->setChecked(false);
    }
});

connect(btn3, QOverload<bool>::of(&NToggleButton::toggled), [=](bool checked) {
    if (checked) {
        btn1->setChecked(false);
        btn2->setChecked(false);
    }
});
```

### 带图标的切换按钮组

```cpp
// 创建按钮组
QWidget* buttonGroup = new QWidget();
QHBoxLayout* layout = new QHBoxLayout(buttonGroup);

// 创建带图标的切换按钮
NToggleButton* boldBtn = new NToggleButton("B");
boldBtn->setFluentIcon(NFilledIconType::TextBold24Filled, 16);

NToggleButton* italicBtn = new NToggleButton("I");
italicBtn->setFluentIcon(NFilledIconType::TextItalic24Filled, 16);

NToggleButton* underlineBtn = new NToggleButton("U");
underlineBtn->setFluentIcon(NFilledIconType::TextUnderline24Filled, 16);

layout->addWidget(boldBtn);
layout->addWidget(italicBtn);
layout->addWidget(underlineBtn);

// 处理状态变化
connect(boldBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "加粗：" << (checked ? "启用" : "禁用");
});

connect(italicBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "斜体：" << (checked ? "启用" : "禁用");
});

connect(underlineBtn, QOverload<bool>::of(&NToggleButton::toggled), [](bool checked) {
    qDebug() << "下划线：" << (checked ? "启用" : "禁用");
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NToggleButton(QWidget* parent = nullptr)` | 创建切换按钮 |
| `NToggleButton(const QString& text, QWidget* parent = nullptr)` | 创建带文本的切换按钮 |

### 状态管理

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setChecked(bool checked)` | 设置选中状态 | 布尔值 |
| `isChecked()` | 获取选中状态 | 返回 `bool` |
| `toggle()` | 切换状态 | - |

### 文本

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setText(const QString& text)` | 设置按钮文本 | 文本字符串 |
| `text()` | 获取按钮文本 | 返回 `QString` |

### 图标

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setFluentIcon(NRegularIconType::Icon icon, int size, const QColor& color)` | 设置 Fluent 图标 | - |
| `setFluentIcon(NFilledIconType::Icon icon, int size, const QColor& color)` | 设置填充 Fluent 图标 | - |
| `icon()` | 获取图标 | 返回 `QIcon` |
| `setIconSize(const QSize& size)` | 设置图标大小 | 大小对象 |
| `iconSize()` | 获取图标大小 | 返回 `QSize` |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `borderRadius` | 边框圆角半径 | `int` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightDefaultColor` | 明亮主题默认色 | `QColor` |
| `darkDefaultColor` | 暗黑主题默认色 | `QColor` |
| `lightHoverColor` | 明亮主题悬停色 | `QColor` |
| `darkHoverColor` | 暗黑主题悬停色 | `QColor` |
| `lightPressColor` | 明亮主题按下色 | `QColor` |
| `darkPressColor` | 暗黑主题按下色 | `QColor` |
| `lightTextDefaultColor` | 明亮主题文本色 | `QColor` |
| `darkTextDefaultColor` | 暗黑主题文本色 | `QColor` |
| `lightTextPressColor` | 明亮主题按下文本色 | `QColor` |
| `darkTextPressColor` | 暗黑主题按下文本色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |
| `accentDefaultColor` | 强调色默认值 | `QColor` |
| `accentHoverColor` | 强调色悬停值 | `QColor` |
| `accentPressColor` | 强调色按下值 | `QColor` |
| `accentDisabledColor` | 强调色禁用值 | `QColor` |
| `accentTextColor` | 强调色文本色 | `QColor` |
| `accentDisabledTextColor` | 强调色禁用文本色 | `QColor` |

### 大小

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `sizeHint()` | 获取推荐大小 | 返回 `QSize` |
| `minimumSizeHint()` | 获取最小推荐大小 | 返回 `QSize` |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `clicked(bool checked)` | 按钮被点击 | 当前选中状态 |
| `toggled(bool checked)` | 状态已改变 | 新的选中状态 |

## 主题定制

切换按钮会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
