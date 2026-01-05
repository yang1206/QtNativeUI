# NScrollArea 滚动区域

### 介绍

滚动区域用于显示超出可视范围的内容，提供水平和垂直滚动功能。NScrollArea 基于 Qt 的 QScrollArea，提供了自定义滚动条样式和透明背景支持。

### 基本用法

```cpp
// 创建滚动区域
NScrollArea* scrollArea = new NScrollArea();

// 创建内容控件
QWidget* contentWidget = new QWidget();
contentWidget->setFixedSize(800, 1200); // 大于滚动区域的尺寸

QVBoxLayout* layout = new QVBoxLayout(contentWidget);
for (int i = 0; i < 50; ++i) {
    layout->addWidget(new NPushButton(QString("按钮 %1").arg(i + 1)));
}

// 设置滚动内容
scrollArea->setWidget(contentWidget);
scrollArea->setWidgetResizable(true);
```

### 自定义滚动条

启用自定义滚动条样式，提供更现代的外观。

```cpp
NScrollArea* customScrollArea = new NScrollArea();

// 启用自定义滚动条
customScrollArea->setUseCustomScrollBars(true);

// 设置滚动策略
customScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
customScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
```

### 透明背景

适用于需要透明背景的场景，如叠加在其他控件上。

```cpp
NScrollArea* transparentArea = new NScrollArea();

// 启用透明背景
transparentArea->enableTransparentBackground();

// 设置内容
QWidget* content = new QWidget();
content->setStyleSheet("background: transparent;");
transparentArea->setWidget(content);
```

### 视口边距设置

自定义滚动区域的内容边距。

```cpp
NScrollArea* marginArea = new NScrollArea();

// 设置统一边距
marginArea->setViewportMargins(20);

// 或设置不同方向的边距
marginArea->setViewportMargins(10, 15, 10, 15); // 左、上、右、下
```

### 图片查看器示例

```cpp
// 创建图片查看器
NScrollArea* imageViewer = new NScrollArea();
imageViewer->setUseCustomScrollBars(true);

QLabel* imageLabel = new QLabel();
imageLabel->setAlignment(Qt::AlignCenter);
imageLabel->setScaledContents(false);

// 加载图片
QPixmap pixmap(":/images/large_image.jpg");
imageLabel->setPixmap(pixmap);
imageLabel->resize(pixmap.size());

imageViewer->setWidget(imageLabel);
imageViewer->setWidgetResizable(false); // 保持原始大小

// 添加缩放功能
imageViewer->setMinimumSize(400, 300);

// 鼠标滚轮缩放
imageViewer->installEventFilter(new class : public QObject {
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::Wheel) {
            QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
            if (wheelEvent->modifiers() & Qt::ControlModifier) {
                // Ctrl + 滚轮缩放
                double scaleFactor = wheelEvent->angleDelta().y() > 0 ? 1.1 : 0.9;
                QLabel* label = qobject_cast<QLabel*>(obj);
                if (label) {
                    QSize newSize = label->size() * scaleFactor;
                    label->resize(newSize);
                    return true;
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }
});
```

### 长列表显示

```cpp
// 创建长列表
NScrollArea* listArea = new NScrollArea();
listArea->setUseCustomScrollBars(true);

QWidget* listWidget = new QWidget();
QVBoxLayout* listLayout = new QVBoxLayout(listWidget);

// 添加大量项目
for (int i = 0; i < 1000; ++i) {
    QWidget* itemWidget = new QWidget();
    itemWidget->setFixedHeight(60);
    
    QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
    
    // 头像
    NLabel* avatar = new NLabel();
    avatar->setFixedSize(40, 40);
    avatar->setStyleSheet("background: #E0E0E0; border-radius: 20px;");
    
    // 信息
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->addWidget(new NLabel(QString("用户 %1").arg(i + 1), NLabelType::Body));
    infoLayout->addWidget(new NLabel("这是一条消息内容", NLabelType::Caption));
    
    itemLayout->addWidget(avatar);
    itemLayout->addLayout(infoLayout);
    itemLayout->addStretch();
    
    listLayout->addWidget(itemWidget);
}

listArea->setWidget(listWidget);
listArea->setWidgetResizable(true);
```

### 设置页面滚动

```cpp
// 创建设置页面
NScrollArea* settingsArea = new NScrollArea();
settingsArea->setUseCustomScrollBars(true);
settingsArea->setViewportMargins(20);

QWidget* settingsWidget = new QWidget();
QVBoxLayout* settingsLayout = new QVBoxLayout(settingsWidget);

// 外观设置组
NGroupBox* appearanceGroup = new NGroupBox("外观设置");
QFormLayout* appearanceLayout = new QFormLayout(appearanceGroup);
appearanceLayout->addRow("主题", new NComboBox({"自动", "浅色", "深色"}));
appearanceLayout->addRow("字体大小", new NSpinBox());
appearanceLayout->addRow("启用动画", new NToggleSwitch());

// 通知设置组
NGroupBox* notificationGroup = new NGroupBox("通知设置");
QVBoxLayout* notificationLayout = new QVBoxLayout(notificationGroup);
notificationLayout->addWidget(new NCheckBox("桌面通知"));
notificationLayout->addWidget(new NCheckBox("声音提醒"));
notificationLayout->addWidget(new NCheckBox("邮件通知"));

// 高级设置组
NGroupBox* advancedGroup = new NGroupBox("高级设置");
QFormLayout* advancedLayout = new QFormLayout(advancedGroup);
advancedLayout->addRow("调试模式", new NToggleSwitch());
advancedLayout->addRow("日志级别", new NComboBox({"错误", "警告", "信息", "调试"}));
advancedLayout->addRow("缓存大小", new NSpinBox());

settingsLayout->addWidget(appearanceGroup);
settingsLayout->addWidget(notificationGroup);
settingsLayout->addWidget(advancedGroup);
settingsLayout->addStretch();

settingsArea->setWidget(settingsWidget);
settingsArea->setWidgetResizable(true);
```

### 内容区域信息

```cpp
NScrollArea* infoArea = new NScrollArea();

// 获取内容区域信息
QRect contentRect = infoArea->contentRect();
qDebug() << "内容区域：" << contentRect;

// 监听滚动位置变化
connect(infoArea->verticalScrollBar(), &QScrollBar::valueChanged, [](int value) {
    qDebug() << "垂直滚动位置：" << value;
});

connect(infoArea->horizontalScrollBar(), &QScrollBar::valueChanged, [](int value) {
    qDebug() << "水平滚动位置：" << value;
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NScrollArea(QWidget* parent = nullptr)` | 创建滚动区域 |

### 自定义滚动条

| 方法 | 说明 | 参数 |
|------|------|------|
| `setUseCustomScrollBars(bool use)` | 设置是否使用自定义滚动条 | `true` / `false` |
| `useCustomScrollBars()` | 获取是否使用自定义滚动条 | - |

### 背景设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `enableTransparentBackground()` | 启用透明背景 | - |

### 边距设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setViewportMargins(int left, int top, int right, int bottom)` | 设置视口边距 | 左、上、右、下边距 |
| `setViewportMargins(int margins)` | 设置统一视口边距 | 边距值 |

### 信息获取

| 方法 | 说明 | 参数 |
|------|------|------|
| `contentRect()` | 获取内容区域矩形 | - |

### 继承的 API

NScrollArea 继承自 `QScrollArea`，支持所有标准 QScrollArea 的方法和属性：

- `setWidget()` / `widget()` - 设置/获取滚动内容控件
- `setWidgetResizable()` / `widgetResizable()` - 设置/获取控件可调整大小
- `takeWidget()` - 移除并返回内容控件
- `setHorizontalScrollBarPolicy()` / `horizontalScrollBarPolicy()` - 设置/获取水平滚动条策略
- `setVerticalScrollBarPolicy()` / `verticalScrollBarPolicy()` - 设置/获取垂直滚动条策略
- `horizontalScrollBar()` / `verticalScrollBar()` - 获取水平/垂直滚动条
- `ensureVisible()` - 确保指定区域可见
- `ensureWidgetVisible()` - 确保指定控件可见
- `setAlignment()` / `alignment()` - 设置/获取内容对齐方式

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 滚动条宽度 | `12px` | 自定义滚动条宽度 |
| 滚动条圆角 | `6px` | 滚动条圆角半径 |
| 滚动条边距 | `2px` | 滚动条与边缘的距离 |
| 滑块最小长度 | `20px` | 滚动条滑块最小长度 |

### 主题适配

```cpp
// 滚动区域会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 滚动条颜色会自动更新
});
```