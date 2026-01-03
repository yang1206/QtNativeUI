# NGroupBox 分组框

### 介绍

分组框用于将相关的控件组织在一起，提供视觉上的分组效果。NGroupBox 基于 Qt 的 QGroupBox，提供了多种样式、折叠功能和图标支持。

### 基本用法

```cpp
// 基本分组框
NGroupBox* groupBox = new NGroupBox("基本设置");

QVBoxLayout* layout = new QVBoxLayout(groupBox);
layout->addWidget(new NCheckBox("启用功能A"));
layout->addWidget(new NCheckBox("启用功能B"));
layout->addWidget(new NCheckBox("启用功能C"));
```

### 分组框样式

支持三种不同的视觉样式：标准、卡片和轮廓。

```cpp
// 标准样式（默认）
NGroupBox* standardBox = new NGroupBox("标准样式");
standardBox->setGroupBoxStyle(NGroupBox::Standard);

// 卡片样式
NGroupBox* cardBox = new NGroupBox("卡片样式");
cardBox->setGroupBoxStyle(NGroupBox::Card);

// 轮廓样式
NGroupBox* outlinedBox = new NGroupBox("轮廓样式");
outlinedBox->setGroupBoxStyle(NGroupBox::Outlined);
```

### 可折叠分组框

支持折叠功能，用户可以展开或收起分组内容。

```cpp
NGroupBox* collapsibleBox = new NGroupBox("高级设置");

// 启用折叠功能
collapsibleBox->setCollapsible(true);

// 设置初始状态为折叠
collapsibleBox->setCollapsed(true);

// 自定义折叠图标
collapsibleBox->setExpandedIcon(NRegularIconType::ChevronDown24Regular);
collapsibleBox->setCollapsedIcon(NRegularIconType::ChevronRight24Regular);

// 监听折叠状态变化
connect(collapsibleBox, &NGroupBox::collapsedChanged, [](bool collapsed) {
    qDebug() << "分组框" << (collapsed ? "已折叠" : "已展开");
});

// 添加内容
QVBoxLayout* layout = new QVBoxLayout(collapsibleBox);
layout->addWidget(new NSlider(Qt::Horizontal));
layout->addWidget(new NSpinBox());
layout->addWidget(new NComboBox());
```

### 带图标的标题

支持在标题前添加图标，增强视觉效果。

```cpp
NGroupBox* iconBox = new NGroupBox("网络设置");

// 设置标题图标
iconBox->setTitleIcon(NRegularIconType::Wifi24Regular, 20);

// 也可以使用 Filled 图标
NGroupBox* securityBox = new NGroupBox("安全设置");
securityBox->setTitleIcon(NFilledIconType::Shield24Filled, 18);

// 清除图标
iconBox->clearTitleIcon();
```

### 设置页面示例

```cpp
// 创建设置页面的分组布局
QWidget* settingsPage = new QWidget();
QVBoxLayout* mainLayout = new QVBoxLayout(settingsPage);

// 外观设置组
NGroupBox* appearanceGroup = new NGroupBox("外观");
appearanceGroup->setTitleIcon(NRegularIconType::Color24Regular);
appearanceGroup->setGroupBoxStyle(NGroupBox::Card);

QFormLayout* appearanceLayout = new QFormLayout(appearanceGroup);
appearanceLayout->addRow("主题", new NComboBox({"自动", "浅色", "深色"}));
appearanceLayout->addRow("字体大小", new NSpinBox());
appearanceLayout->addRow("启用动画", new NToggleSwitch());

// 通知设置组
NGroupBox* notificationGroup = new NGroupBox("通知");
notificationGroup->setTitleIcon(NRegularIconType::Alert24Regular);
notificationGroup->setCollapsible(true);

QVBoxLayout* notificationLayout = new QVBoxLayout(notificationGroup);
notificationLayout->addWidget(new NCheckBox("桌面通知"));
notificationLayout->addWidget(new NCheckBox("声音提醒"));
notificationLayout->addWidget(new NCheckBox("邮件通知"));

// 高级设置组
NGroupBox* advancedGroup = new NGroupBox("高级设置");
advancedGroup->setTitleIcon(NRegularIconType::Settings24Regular);
advancedGroup->setCollapsible(true);
advancedGroup->setCollapsed(true);  // 默认折叠

QFormLayout* advancedLayout = new QFormLayout(advancedGroup);
advancedLayout->addRow("调试模式", new NToggleSwitch());
advancedLayout->addRow("日志级别", new NComboBox({"错误", "警告", "信息", "调试"}));
advancedLayout->addRow("缓存大小", new NSpinBox());

mainLayout->addWidget(appearanceGroup);
mainLayout->addWidget(notificationGroup);
mainLayout->addWidget(advancedGroup);
mainLayout->addStretch();
```

### 自定义样式

```cpp
NGroupBox* styledBox = new NGroupBox("自定义样式");

// 设置圆角
styledBox->setBorderRadius(12);

// 设置边框显示
styledBox->setShowBorder(true);

// 设置内容边距
styledBox->setContentMargin(16);

// 设置标题高度和间距
styledBox->setTitleHeight(40);
styledBox->setTitleSpacing(12);

// 设置折叠指示器大小
styledBox->setCollapseIndicatorSize(16);

// 自定义颜色
styledBox->setLightBackgroundColor(QColor(248, 249, 250));
styledBox->setLightBorderColor(QColor(220, 220, 220));
styledBox->setLightTitleColor(QColor(32, 32, 32));

styledBox->setDarkBackgroundColor(QColor(45, 45, 45));
styledBox->setDarkBorderColor(QColor(100, 100, 100));
styledBox->setDarkTitleColor(QColor(255, 255, 255));
```

### 嵌套分组

```cpp
// 创建嵌套的分组结构
NGroupBox* mainGroup = new NGroupBox("主要设置");
QVBoxLayout* mainLayout = new QVBoxLayout(mainGroup);

// 子分组1
NGroupBox* subGroup1 = new NGroupBox("子设置1");
subGroup1->setGroupBoxStyle(NGroupBox::Outlined);
QVBoxLayout* sub1Layout = new QVBoxLayout(subGroup1);
sub1Layout->addWidget(new NCheckBox("选项1"));
sub1Layout->addWidget(new NCheckBox("选项2"));

// 子分组2
NGroupBox* subGroup2 = new NGroupBox("子设置2");
subGroup2->setGroupBoxStyle(NGroupBox::Outlined);
subGroup2->setCollapsible(true);
QVBoxLayout* sub2Layout = new QVBoxLayout(subGroup2);
sub2Layout->addWidget(new NSlider(Qt::Horizontal));
sub2Layout->addWidget(new NSpinBox());

mainLayout->addWidget(subGroup1);
mainLayout->addWidget(subGroup2);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NGroupBox(QWidget* parent = nullptr)` | 创建无标题分组框 |
| `NGroupBox(const QString& title, QWidget* parent = nullptr)` | 创建带标题分组框 |

### 样式设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setGroupBoxStyle(GroupBoxStyle style)` | 设置分组框样式 | 样式枚举 |
| `groupBoxStyle()` | 获取分组框样式 | - |

### 图标设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setTitleIcon(NRegularIconType::Icon icon, int size)` | 设置标题 Regular 图标 | 图标类型、大小 |
| `setTitleIcon(NFilledIconType::Icon icon, int size)` | 设置标题 Filled 图标 | 图标类型、大小 |
| `clearTitleIcon()` | 清除标题图标 | - |
| `setExpandedIcon(NRegularIconType::Icon icon)` | 设置展开状态图标 | 图标类型 |
| `setCollapsedIcon(NRegularIconType::Icon icon)` | 设置折叠状态图标 | 图标类型 |

### 折叠功能

| 方法 | 说明 | 参数 |
|------|------|------|
| `setCollapsible(bool collapsible)` | 设置是否可折叠 | `true` / `false` |
| `isCollapsible()` | 获取是否可折叠 | - |
| `setCollapsed(bool collapsed)` | 设置折叠状态 | `true` / `false` |
| `isCollapsed()` | 获取折叠状态 | - |

### 样式属性

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |
| `setShowBorder(bool show)` | 设置是否显示边框 | `true` / `false` |
| `showBorder()` | 获取是否显示边框 | - |
| `setContentMargin(int margin)` | 设置内容边距 | 边距像素值 |
| `contentMargin()` | 获取内容边距 | - |
| `setTitleHeight(int height)` | 设置标题高度 | 高度像素值 |
| `titleHeight()` | 获取标题高度 | - |
| `setTitleSpacing(int spacing)` | 设置标题间距 | 间距像素值 |
| `titleSpacing()` | 获取标题间距 | - |
| `setCollapseIndicatorSize(int size)` | 设置折叠指示器大小 | 大小像素值 |
| `collapseIndicatorSize()` | 获取折叠指示器大小 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `collapsedChanged(bool collapsed)` | 折叠状态改变时触发 | 新的折叠状态 |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightBackgroundColor` | 明亮主题背景色 | `QColor` |
| `darkBackgroundColor` | 暗黑主题背景色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |
| `lightTitleColor` | 明亮主题标题色 | `QColor` |
| `darkTitleColor` | 暗黑主题标题色 | `QColor` |

### 枚举

#### GroupBoxStyle

| 值 | 说明 |
|----|------|
| `Standard` | 标准样式（默认） |
| `Card` | 卡片样式 |
| `Outlined` | 轮廓样式 |

### 继承的 API

NGroupBox 继承自 `QGroupBox`，支持所有标准 QGroupBox 的方法和属性：

- `setTitle()` / `title()` - 设置/获取标题文本
- `setAlignment()` / `alignment()` - 设置/获取标题对齐方式
- `setCheckable()` / `isCheckable()` - 设置/获取是否可选中
- `setChecked()` / `isChecked()` - 设置/获取选中状态
- `setFlat()` / `isFlat()` - 设置/获取平面样式
- `clicked(bool)` - 点击信号（可选中时）
- `toggled(bool)` - 切换信号（可选中时）

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 标题高度 | `32px` | 分组框标题区域高度 |
| 边框圆角 | `6px` | 分组框圆角半径 |
| 内容边距 | `12px` | 内容区域边距 |
| 标题间距 | `8px` | 标题与内容间距 |
| 折叠指示器大小 | `12px` | 折叠箭头大小 |

### 主题适配

```cpp
// 分组框会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 分组框颜色会自动更新
});
```