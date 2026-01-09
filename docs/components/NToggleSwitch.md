# NToggleSwitch 开关

### 介绍

开关用于控制单个设置的开启或关闭状态。NToggleSwitch 基于 Qt 的 QAbstractButton，提供了流畅的切换动画和现代化的外观。

### 基本用法

```cpp
// 基本开关
NToggleSwitch* toggleSwitch = new NToggleSwitch();

// 带标签的开关
NToggleSwitch* labeledSwitch = new NToggleSwitch("启用通知");

// 设置初始状态
toggleSwitch->setChecked(true);

// 监听状态变化
connect(toggleSwitch, &NToggleSwitch::checkedChanged, [](bool checked) {
    qDebug() << "开关状态：" << (checked ? "开启" : "关闭");
});
```

### 设置项示例

开关常用于设置页面中的各种选项控制。

```cpp
// 创建设置页面
QWidget* settingsWidget = new QWidget();
QFormLayout* layout = new QFormLayout(settingsWidget);

// 通知设置
NToggleSwitch* notificationSwitch = new NToggleSwitch();
layout->addRow("推送通知", notificationSwitch);

// 自动更新
NToggleSwitch* autoUpdateSwitch = new NToggleSwitch();
autoUpdateSwitch->setChecked(true);
layout->addRow("自动更新", autoUpdateSwitch);

// 深色模式
NToggleSwitch* darkModeSwitch = new NToggleSwitch();
layout->addRow("深色模式", darkModeSwitch);

// 连接深色模式切换
connect(darkModeSwitch, &NToggleSwitch::checkedChanged, [](bool checked) {
    if (checked) {
        nTheme->setThemeMode(NThemeType::Dark);
    } else {
        nTheme->setThemeMode(NThemeType::Light);
    }
});
```

### 禁用状态

```cpp
NToggleSwitch* disabledSwitch = new NToggleSwitch("禁用选项");
disabledSwitch->setEnabled(false);
disabledSwitch->setChecked(true); // 禁用但保持开启状态
```

### 自定义样式

支持自定义轨道和滑块的颜色、圆角等样式属性。

```cpp
NToggleSwitch* styledSwitch = new NToggleSwitch("自定义样式");

// 设置轨道样式
styledSwitch->setTrackBorderRadius(12);
styledSwitch->setTrackBorderWidth(1);

// 自定义明亮主题颜色
styledSwitch->setLightTrackDefaultColor(QColor(230, 230, 230));
styledSwitch->setLightTrackBorderColor(QColor(200, 200, 200));
styledSwitch->setLightThumbDefaultColor(QColor(255, 255, 255));
styledSwitch->setLightThumbCheckedColor(QColor(0, 120, 215));
styledSwitch->setLightTextColor(QColor(32, 32, 32));

// 自定义暗黑主题颜色
styledSwitch->setDarkTrackDefaultColor(QColor(60, 60, 60));
styledSwitch->setDarkTrackBorderColor(QColor(100, 100, 100));
styledSwitch->setDarkThumbDefaultColor(QColor(200, 200, 200));
styledSwitch->setDarkThumbCheckedColor(QColor(100, 180, 255));
styledSwitch->setDarkTextColor(QColor(255, 255, 255));
```

### 批量控制

```cpp
// 创建开关组，实现批量控制
QList<NToggleSwitch*> switches;
switches << new NToggleSwitch("功能A");
switches << new NToggleSwitch("功能B");
switches << new NToggleSwitch("功能C");

// 全部开启按钮
NPushButton* enableAllBtn = new NPushButton("全部开启");
connect(enableAllBtn, &QPushButton::clicked, [switches]() {
    for (NToggleSwitch* sw : switches) {
        sw->setChecked(true);
    }
});

// 全部关闭按钮
NPushButton* disableAllBtn = new NPushButton("全部关闭");
connect(disableAllBtn, &QPushButton::clicked, [switches]() {
    for (NToggleSwitch* sw : switches) {
        sw->setChecked(false);
    }
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NToggleSwitch(QWidget* parent = nullptr)` | 创建无标签开关 |
| `NToggleSwitch(const QString& text, QWidget* parent = nullptr)` | 创建带标签开关 |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `setChecked(bool checked)` | 设置开关状态 | `true` / `false` |
| `isChecked()` | 获取开关状态 | - |
| `setTrackBorderRadius(int radius)` | 设置轨道圆角 | 圆角半径 |
| `trackBorderRadius()` | 获取轨道圆角 | - |
| `setTrackBorderWidth(int width)` | 设置轨道边框宽度 | 边框宽度 |
| `trackBorderWidth()` | 获取轨道边框宽度 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `checkedChanged(bool checked)` | 开关状态改变时触发 | 新的状态 |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightTrackDefaultColor` | 明亮主题轨道默认色 | `QColor` |
| `darkTrackDefaultColor` | 暗黑主题轨道默认色 | `QColor` |
| `lightTrackBorderColor` | 明亮主题轨道边框色 | `QColor` |
| `darkTrackBorderColor` | 暗黑主题轨道边框色 | `QColor` |
| `lightThumbDefaultColor` | 明亮主题滑块默认色 | `QColor` |
| `darkThumbDefaultColor` | 暗黑主题滑块默认色 | `QColor` |
| `lightThumbCheckedColor` | 明亮主题滑块选中色 | `QColor` |
| `darkThumbCheckedColor` | 暗黑主题滑块选中色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |

### 继承的 API

NToggleSwitch 继承自 `QAbstractButton`，支持相关的方法和信号：

- `setText()` / `text()` - 设置/获取标签文本
- `setEnabled()` / `isEnabled()` - 启用/禁用状态
- `setVisible()` / `isVisible()` - 显示/隐藏状态
- `clicked()` - 点击信号
- `pressed()` / `released()` - 按下/释放信号
- `toggled(bool)` - 切换信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 轨道宽度 | `44px` | 开关轨道宽度 |
| 轨道高度 | `20px` | 开关轨道高度 |
| 滑块大小 | `16px` | 滑块圆形直径 |
| 轨道圆角 | `10px` | 轨道圆角半径 |
| 动画时长 | `150ms` | 滑块滑动动画时长 |
| 文本间距 | `8px` | 开关与文本间距 |

### 主题适配

```cpp
// 开关会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 开关颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 选中状态的强调色会自动更新
});
```