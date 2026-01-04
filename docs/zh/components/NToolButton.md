# NToolButton 工具按钮

### 介绍

工具按钮是一种紧凑的按钮控件，通常用于工具栏、菜单栏和操作面板。NToolButton 基于 Qt 的 QToolButton，提供了现代化的外观、Fluent 图标支持和灵活的显示模式。

### 基本用法

```cpp
// 创建基本工具按钮
NToolButton* toolBtn = new NToolButton();
toolBtn->setFluentIcon(NRegularIconType::Save24Regular, 20);
toolBtn->setToolTip("保存文件");

// 带文本的工具按钮
NToolButton* textBtn = new NToolButton("保存");
textBtn->setFluentIcon(NRegularIconType::Save24Regular, 16);

// 监听点击事件
connect(toolBtn, &QToolButton::clicked, []() {
    qDebug() << "保存文件";
});
```

### 按钮类型

```cpp
// 标准工具按钮
NToolButton* standardBtn = new NToolButton();
standardBtn->setButtonType(NToolButton::Standard);
standardBtn->setFluentIcon(NRegularIconType::Edit24Regular, 20);

// 强调色工具按钮
NToolButton* accentBtn = new NToolButton();
accentBtn->setButtonType(NToolButton::Accent);
accentBtn->setFluentIcon(NRegularIconType::Add24Regular, 20);
```

### 工具栏示例

```cpp
// 创建工具栏
QToolBar* toolBar = new QToolBar();

// 文件操作按钮
NToolButton* newBtn = new NToolButton();
newBtn->setFluentIcon(NRegularIconType::DocumentAdd24Regular, 20);
newBtn->setToolTip("新建文件 (Ctrl+N)");

NToolButton* openBtn = new NToolButton();
openBtn->setFluentIcon(NRegularIconType::FolderOpen24Regular, 20);
openBtn->setToolTip("打开文件 (Ctrl+O)");

NToolButton* saveBtn = new NToolButton();
saveBtn->setFluentIcon(NRegularIconType::Save24Regular, 20);
saveBtn->setToolTip("保存文件 (Ctrl+S)");

// 编辑操作按钮
NToolButton* undoBtn = new NToolButton();
undoBtn->setFluentIcon(NRegularIconType::ArrowUndo24Regular, 20);
undoBtn->setToolTip("撤销 (Ctrl+Z)");

NToolButton* redoBtn = new NToolButton();
redoBtn->setFluentIcon(NRegularIconType::ArrowRedo24Regular, 20);
redoBtn->setToolTip("重做 (Ctrl+Y)");

// 添加到工具栏
toolBar->addWidget(newBtn);
toolBar->addWidget(openBtn);
toolBar->addWidget(saveBtn);
toolBar->addSeparator();
toolBar->addWidget(undoBtn);
toolBar->addWidget(redoBtn);

// 连接操作
connect(newBtn, &QToolButton::clicked, []() { /* 新建文件 */ });
connect(openBtn, &QToolButton::clicked, []() { /* 打开文件 */ });
connect(saveBtn, &QToolButton::clicked, []() { /* 保存文件 */ });
connect(undoBtn, &QToolButton::clicked, []() { /* 撤销操作 */ });
connect(redoBtn, &QToolButton::clicked, []() { /* 重做操作 */ });
```

### 不同显示模式

```cpp
// 仅图标模式
NToolButton* iconOnlyBtn = new NToolButton();
iconOnlyBtn->setFluentIcon(NRegularIconType::Settings24Regular, 20);
iconOnlyBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

// 仅文本模式
NToolButton* textOnlyBtn = new NToolButton();
textOnlyBtn->setText("设置");
textOnlyBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);

// 图标在文本上方
NToolButton* iconAboveBtn = new NToolButton();
iconAboveBtn->setText("设置");
iconAboveBtn->setFluentIcon(NRegularIconType::Settings24Regular, 24);
iconAboveBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

// 图标在文本旁边
NToolButton* iconBesideBtn = new NToolButton();
iconBesideBtn->setText("设置");
iconBesideBtn->setFluentIcon(NRegularIconType::Settings24Regular, 16);
iconBesideBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
```

### 媒体控制按钮

```cpp
// 创建媒体控制面板
QWidget* mediaPanel = new QWidget();
QHBoxLayout* mediaLayout = new QHBoxLayout(mediaPanel);

// 播放/暂停按钮
NToolButton* playPauseBtn = new NToolButton();
playPauseBtn->setFluentIcon(NFilledIconType::Play24Filled, 24);
playPauseBtn->setButtonType(NToolButton::Accent);

// 停止按钮
NToolButton* stopBtn = new NToolButton();
stopBtn->setFluentIcon(NRegularIconType::Stop24Regular, 20);

// 上一首/下一首
NToolButton* prevBtn = new NToolButton();
prevBtn->setFluentIcon(NRegularIconType::Previous24Regular, 20);

NToolButton* nextBtn = new NToolButton();
nextBtn->setFluentIcon(NRegularIconType::Next24Regular, 20);

// 音量按钮
NToolButton* volumeBtn = new NToolButton();
volumeBtn->setFluentIcon(NRegularIconType::Speaker224Regular, 20);

mediaLayout->addWidget(prevBtn);
mediaLayout->addWidget(playPauseBtn);
mediaLayout->addWidget(stopBtn);
mediaLayout->addWidget(nextBtn);
mediaLayout->addSeparator();
mediaLayout->addWidget(volumeBtn);

// 播放状态切换
bool isPlaying = false;
connect(playPauseBtn, &QToolButton::clicked, [=]() mutable {
    isPlaying = !isPlaying;
    if (isPlaying) {
        playPauseBtn->setFluentIcon(NFilledIconType::Pause24Filled, 24);
        playPauseBtn->setToolTip("暂停");
    } else {
        playPauseBtn->setFluentIcon(NFilledIconType::Play24Filled, 24);
        playPauseBtn->setToolTip("播放");
    }
});
```

### 格式化工具栏

```cpp
// 创建文本格式化工具栏
QWidget* formatPanel = new QWidget();
QHBoxLayout* formatLayout = new QHBoxLayout(formatPanel);

// 字体样式按钮
NToolButton* boldBtn = new NToolButton();
boldBtn->setFluentIcon(NRegularIconType::TextBold24Regular, 16);
boldBtn->setCheckable(true);
boldBtn->setToolTip("粗体 (Ctrl+B)");

NToolButton* italicBtn = new NToolButton();
italicBtn->setFluentIcon(NRegularIconType::TextItalic24Regular, 16);
italicBtn->setCheckable(true);
italicBtn->setToolTip("斜体 (Ctrl+I)");

NToolButton* underlineBtn = new NToolButton();
underlineBtn->setFluentIcon(NRegularIconType::TextUnderline24Regular, 16);
underlineBtn->setCheckable(true);
underlineBtn->setToolTip("下划线 (Ctrl+U)");

// 对齐按钮
NToolButton* alignLeftBtn = new NToolButton();
alignLeftBtn->setFluentIcon(NRegularIconType::TextAlignLeft24Regular, 16);
alignLeftBtn->setCheckable(true);
alignLeftBtn->setChecked(true);

NToolButton* alignCenterBtn = new NToolButton();
alignCenterBtn->setFluentIcon(NRegularIconType::TextAlignCenter24Regular, 16);
alignCenterBtn->setCheckable(true);

NToolButton* alignRightBtn = new NToolButton();
alignRightBtn->setFluentIcon(NRegularIconType::TextAlignRight24Regular, 16);
alignRightBtn->setCheckable(true);

// 创建对齐按钮组
QButtonGroup* alignGroup = new QButtonGroup();
alignGroup->addButton(alignLeftBtn, 0);
alignGroup->addButton(alignCenterBtn, 1);
alignGroup->addButton(alignRightBtn, 2);
alignGroup->setExclusive(true);

formatLayout->addWidget(boldBtn);
formatLayout->addWidget(italicBtn);
formatLayout->addWidget(underlineBtn);
formatLayout->addSeparator();
formatLayout->addWidget(alignLeftBtn);
formatLayout->addWidget(alignCenterBtn);
formatLayout->addWidget(alignRightBtn);

// 连接格式化操作
connect(boldBtn, &QToolButton::toggled, [](bool checked) {
    qDebug() << "粗体：" << (checked ? "开启" : "关闭");
});

connect(alignGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), [](int id) {
    QStringList alignments = {"左对齐", "居中对齐", "右对齐"};
    qDebug() << "对齐方式：" << alignments[id];
});
```

### 透明背景按钮

```cpp
// 创建透明背景的工具按钮（适用于标题栏等场景）
NToolButton* transparentBtn = new NToolButton();
transparentBtn->setFluentIcon(NRegularIconType::Dismiss24Regular, 16);
transparentBtn->setTransparentBackground(true);
transparentBtn->setToolTip("关闭");

// 最小化按钮
NToolButton* minimizeBtn = new NToolButton();
minimizeBtn->setFluentIcon(NRegularIconType::Subtract24Regular, 16);
minimizeBtn->setTransparentBackground(true);
minimizeBtn->setToolTip("最小化");

// 最大化按钮
NToolButton* maximizeBtn = new NToolButton();
maximizeBtn->setFluentIcon(NRegularIconType::Square24Regular, 16);
maximizeBtn->setTransparentBackground(true);
maximizeBtn->setToolTip("最大化");
```

### 自定义样式

```cpp
NToolButton* styledBtn = new NToolButton();

// 设置圆角
styledBtn->setBorderRadius(8);

// 设置透明背景
styledBtn->setTransparentBackground(true);

// 自定义颜色
styledBtn->setLightDefaultColor(QColor(240, 240, 240));
styledBtn->setLightHoverColor(QColor(220, 220, 220));
styledBtn->setLightPressColor(QColor(200, 200, 200));
styledBtn->setLightTextDefaultColor(QColor(32, 32, 32));
styledBtn->setLightBorderColor(QColor(180, 180, 180));

// 设置强调色
styledBtn->setAccentColor(QColor(0, 120, 215));
```

### 状态管理

```cpp
// 创建可切换状态的工具按钮
NToolButton* toggleBtn = new NToolButton();
toggleBtn->setCheckable(true);
toggleBtn->setFluentIcon(NRegularIconType::Eye24Regular, 20);
toggleBtn->setToolTip("显示/隐藏");

connect(toggleBtn, &QToolButton::toggled, [=](bool checked) {
    if (checked) {
        toggleBtn->setFluentIcon(NFilledIconType::EyeOff24Filled, 20);
        toggleBtn->setToolTip("隐藏");
    } else {
        toggleBtn->setFluentIcon(NRegularIconType::Eye24Regular, 20);
        toggleBtn->setToolTip("显示");
    }
});

// 根据条件启用/禁用按钮
auto updateButtonState = [=]() {
    bool hasContent = checkHasContent();
    toggleBtn->setEnabled(hasContent);
};
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NToolButton(QWidget* parent = nullptr)` | 创建空的工具按钮 |
| `NToolButton(QString text, QWidget* parent = nullptr)` | 创建带文本的工具按钮 |

### 按钮类型

| 方法 | 说明 | 参数 |
|------|------|------|
| `setButtonType(ButtonType type)` | 设置按钮类型 | 按钮类型枚举 |
| `buttonType()` | 获取按钮类型 | - |

### 图标设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setFluentIcon(NRegularIconType::Icon icon, int size, const QColor& color)` | 设置 Regular 图标 | 图标类型、大小、颜色 |
| `setFluentIcon(NFilledIconType::Icon icon, int size, const QColor& color)` | 设置 Filled 图标 | 图标类型、大小、颜色 |
| `setIcon(const QIcon& icon)` | 设置自定义图标 | 图标对象 |

### 文本和样式

| 方法 | 说明 | 参数 |
|------|------|------|
| `setText(const QString& text)` | 设置按钮文本 | 文本内容 |
| `setToolButtonStyle(Qt::ToolButtonStyle style)` | 设置显示样式 | 样式枚举 |

### 颜色设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setAccentColor(const QColor& color)` | 设置强调色 | 颜色值 |
| `setAccentColor(const NAccentColor& color)` | 设置主题强调色 | 主题颜色对象 |
| `resetAccentColor()` | 重置为默认强调色 | - |

### 样式属性

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |
| `setTransparentBackground(bool transparent)` | 设置透明背景 | `true` / `false` |
| `transparentBackground()` | 获取是否透明背景 | - |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightDefaultColor` | 明亮主题默认背景色 | `QColor` |
| `darkDefaultColor` | 暗黑主题默认背景色 | `QColor` |
| `lightHoverColor` | 明亮主题悬停背景色 | `QColor` |
| `darkHoverColor` | 暗黑主题悬停背景色 | `QColor` |
| `lightPressColor` | 明亮主题按下背景色 | `QColor` |
| `darkPressColor` | 暗黑主题按下背景色 | `QColor` |
| `lightTextDefaultColor` | 明亮主题默认文本色 | `QColor` |
| `darkTextDefaultColor` | 暗黑主题默认文本色 | `QColor` |
| `lightTextPressColor` | 明亮主题按下文本色 | `QColor` |
| `darkTextPressColor` | 暗黑主题按下文本色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |

### 枚举

#### ButtonType

| 值 | 说明 |
|----|------|
| `Standard` | 标准按钮（默认） |
| `Accent` | 强调色按钮 |

### 继承的 API

NToolButton 继承自 `QToolButton`，支持所有标准 QToolButton 的方法和信号：

- `setCheckable()` / `isCheckable()` - 设置/获取可选中状态
- `setChecked()` / `isChecked()` - 设置/获取选中状态
- `setAutoRaise()` / `autoRaise()` - 设置/获取自动凸起
- `setPopupMode()` / `popupMode()` - 设置/获取弹出模式
- `setMenu()` / `menu()` - 设置/获取关联菜单
- `clicked()` - 点击信号
- `pressed()` / `released()` - 按下/释放信号
- `toggled(bool)` - 切换信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认大小 | `32px` | 工具按钮默认大小 |
| 边框圆角 | `4px` | 按钮圆角半径 |
| 图标大小 | `16px` | 默认图标大小 |
| 内边距 | `6px` | 按钮内边距 |

### 主题适配

```cpp
// 工具按钮会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 按钮颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 强调色按钮会自动更新为新的强调色
});
```