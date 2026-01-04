# NProgressRing 环形进度条

### 介绍

环形进度条以圆环形式显示任务进度，适用于需要紧凑显示进度的场景。NProgressRing 提供了确定和不确定两种进度模式，支持多种状态和自定义样式。

### 基本用法

```cpp
// 创建环形进度条
NProgressRing* progressRing = new NProgressRing();
progressRing->setRange(0, 100);
progressRing->setValue(50);
progressRing->setFixedSize(80, 80);

// 监听进度变化
connect(progressRing, &NProgressRing::valueChanged, [](int value) {
    qDebug() << "进度：" << value << "%";
});
```

### 不确定进度

用于显示正在进行但无法确定具体进度的任务。

```cpp
NProgressRing* indeterminateRing = new NProgressRing();
indeterminateRing->setIndeterminate(true);
indeterminateRing->setFixedSize(60, 60);

// 也可以通过设置范围来实现
NProgressRing* loadingRing = new NProgressRing();
loadingRing->setRange(0, 0); // 范围为0时自动变为不确定进度
```

### 进度状态

支持正常、暂停、错误三种状态，每种状态有不同的颜色表示。

```cpp
NProgressRing* statusRing = new NProgressRing();
statusRing->setRange(0, 100);
statusRing->setValue(30);

// 暂停状态（黄色）
statusRing->setPaused(true);
// 或者
statusRing->pause();

// 恢复正常状态
statusRing->resume();

// 错误状态（红色）
statusRing->setError(true);
// 或者
statusRing->error();

// 重置到正常状态
statusRing->reset();
```

### 文本显示

支持在环形进度条中央显示进度文本。

```cpp
NProgressRing* textRing = new NProgressRing();
textRing->setRange(0, 100);
textRing->setValue(75);
textRing->setTextVisible(true);

// 自定义文本格式
textRing->setFormat("%p%");  // 显示百分比
// textRing->setFormat("%v/%m");  // 显示 当前值/最大值
// textRing->setFormat("完成");    // 显示自定义文本
```

### 文件上传进度

```cpp
// 创建文件上传进度显示
QWidget* uploadWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(uploadWidget);

NProgressRing* uploadRing = new NProgressRing();
uploadRing->setFixedSize(100, 100);
uploadRing->setTextVisible(true);
uploadRing->setFormat("%p%");

NLabel* statusLabel = new NLabel("准备上传...", NLabelType::Body);
statusLabel->setAlignment(Qt::AlignCenter);

layout->addWidget(uploadRing, 0, Qt::AlignCenter);
layout->addWidget(statusLabel);

// 模拟上传进度
QTimer* uploadTimer = new QTimer();
int progress = 0;

connect(uploadTimer, &QTimer::timeout, [=]() mutable {
    progress += qrand() % 5 + 1;
    
    if (progress >= 100) {
        progress = 100;
        uploadTimer->stop();
        statusLabel->setText("上传完成");
        uploadRing->setValue(progress);
    } else {
        statusLabel->setText(QString("正在上传... %1%").arg(progress));
        uploadRing->setValue(progress);
    }
});

uploadTimer->start(100);
```

### 系统资源监控

```cpp
// CPU 使用率监控
NProgressRing* cpuRing = new NProgressRing();
cpuRing->setFixedSize(80, 80);
cpuRing->setTextVisible(true);
cpuRing->setFormat("CPU\n%p%");
cpuRing->setRange(0, 100);

// 内存使用率监控
NProgressRing* memoryRing = new NProgressRing();
memoryRing->setFixedSize(80, 80);
memoryRing->setTextVisible(true);
memoryRing->setFormat("内存\n%p%");
memoryRing->setRange(0, 100);

// 磁盘使用率监控
NProgressRing* diskRing = new NProgressRing();
diskRing->setFixedSize(80, 80);
diskRing->setTextVisible(true);
diskRing->setFormat("磁盘\n%p%");
diskRing->setRange(0, 100);

// 模拟系统监控更新
QTimer* monitorTimer = new QTimer();
connect(monitorTimer, &QTimer::timeout, [=]() {
    // 模拟获取系统资源使用率
    int cpuUsage = qrand() % 100;
    int memoryUsage = qrand() % 100;
    int diskUsage = qrand() % 100;
    
    cpuRing->setValue(cpuUsage);
    memoryRing->setValue(memoryUsage);
    diskRing->setValue(diskUsage);
    
    // 根据使用率设置状态
    if (cpuUsage > 80) cpuRing->setError(true);
    else if (cpuUsage > 60) cpuRing->setPaused(true);
    else cpuRing->resume();
});

monitorTimer->start(1000); // 每秒更新
```

### 多任务进度显示

```cpp
// 创建多任务进度面板
QWidget* taskPanel = new QWidget();
QGridLayout* gridLayout = new QGridLayout(taskPanel);

QStringList taskNames = {"下载任务1", "下载任务2", "安装程序", "更新数据"};
QList<NProgressRing*> taskRings;

for (int i = 0; i < taskNames.size(); ++i) {
    // 创建任务进度环
    NProgressRing* taskRing = new NProgressRing();
    taskRing->setFixedSize(60, 60);
    taskRing->setTextVisible(true);
    taskRing->setFormat("%p%");
    taskRing->setRange(0, 100);
    taskRings.append(taskRing);
    
    // 创建任务标签
    NLabel* taskLabel = new NLabel(taskNames[i], NLabelType::Caption);
    taskLabel->setAlignment(Qt::AlignCenter);
    
    // 添加到网格布局
    int row = i / 2;
    int col = i % 2;
    
    QVBoxLayout* taskLayout = new QVBoxLayout();
    taskLayout->addWidget(taskRing, 0, Qt::AlignCenter);
    taskLayout->addWidget(taskLabel);
    
    QWidget* taskWidget = new QWidget();
    taskWidget->setLayout(taskLayout);
    
    gridLayout->addWidget(taskWidget, row, col);
}

// 模拟任务进度更新
for (int i = 0; i < taskRings.size(); ++i) {
    QTimer* taskTimer = new QTimer();
    taskTimer->setProperty("taskIndex", i);
    
    connect(taskTimer, &QTimer::timeout, [taskRings, taskTimer]() {
        int index = taskTimer->property("taskIndex").toInt();
        NProgressRing* ring = taskRings[index];
        
        int currentValue = ring->value();
        int increment = qrand() % 3 + 1;
        
        if (currentValue + increment >= 100) {
            ring->setValue(100);
            taskTimer->stop();
        } else {
            ring->setValue(currentValue + increment);
        }
    });
    
    taskTimer->start(200 + i * 50); // 不同任务不同更新频率
}
```

### 自定义样式

```cpp
NProgressRing* styledRing = new NProgressRing();

// 设置线条宽度
styledRing->setStrokeWidth(8);

// 启用动画
styledRing->setAnimationEnabled(true);
styledRing->setAnimationDuration(300);

// 自定义颜色
styledRing->setLightTrackColor(QColor(230, 230, 230));
styledRing->setLightProgressColor(QColor(0, 120, 215));
styledRing->setLightTextColor(QColor(32, 32, 32));

// 状态颜色
styledRing->setLightPausedColor(QColor(255, 193, 7));
styledRing->setLightErrorColor(QColor(220, 53, 69));

// 禁用状态颜色
styledRing->setLightDisabledTrackColor(QColor(200, 200, 200));
styledRing->setLightDisabledProgressColor(QColor(150, 150, 150));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NProgressRing(QWidget* parent = nullptr)` | 创建环形进度条 |

### 进度控制

| 方法 | 说明 | 参数 |
|------|------|------|
| `setMinimum(int minimum)` | 设置最小值 | 最小值 |
| `minimum()` | 获取最小值 | - |
| `setMaximum(int maximum)` | 设置最大值 | 最大值 |
| `maximum()` | 获取最大值 | - |
| `setRange(int minimum, int maximum)` | 设置范围 | 最小值、最大值 |
| `setValue(int value)` | 设置当前值 | 当前值 |
| `value()` | 获取当前值 | - |
| `setProgressPosition(qreal position)` | 设置进度位置（0.0-1.0） | 位置比例 |
| `progressPosition()` | 获取进度位置 | - |
| `setIndeterminate(bool indeterminate)` | 设置不确定进度模式 | `true` / `false` |
| `isIndeterminate()` | 获取是否为不确定进度 | - |

### 状态控制

| 方法 | 说明 | 参数 |
|------|------|------|
| `pause()` | 暂停进度（设为暂停状态） | - |
| `resume()` | 恢复进度（恢复正常状态） | - |
| `setPaused(bool paused)` | 设置暂停状态 | `true` / `false` |
| `isPaused()` | 获取是否为暂停状态 | - |
| `error()` | 设为错误状态 | - |
| `setError(bool error)` | 设置错误状态 | `true` / `false` |
| `isError()` | 获取是否为错误状态 | - |
| `reset()` | 重置到正常状态 | - |

### 文本显示

| 方法 | 说明 | 参数 |
|------|------|------|
| `setFormat(const QString& format)` | 设置文本格式 | 格式字符串 |
| `format()` | 获取文本格式 | - |
| `setTextVisible(bool visible)` | 设置文本可见性 | `true` / `false` |
| `isTextVisible()` | 获取文本可见性 | - |

### 样式设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setStrokeWidth(int width)` | 设置线条宽度 | 宽度像素值 |
| `getStrokeWidth()` | 获取线条宽度 | - |
| `setAnimationEnabled(bool enabled)` | 启用/禁用动画 | `true` / `false` |
| `setAnimationDuration(int duration)` | 设置动画时长 | 毫秒数 |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `valueChanged(int value)` | 值改变时触发 | 新的值 |
| `strokeWidthChanged()` | 线条宽度改变时触发 | - |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightTrackColor` | 明亮主题轨道色 | `QColor` |
| `darkTrackColor` | 暗黑主题轨道色 | `QColor` |
| `lightProgressColor` | 明亮主题进度色 | `QColor` |
| `darkProgressColor` | 暗黑主题进度色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightDisabledTrackColor` | 明亮主题禁用轨道色 | `QColor` |
| `darkDisabledTrackColor` | 暗黑主题禁用轨道色 | `QColor` |
| `lightDisabledProgressColor` | 明亮主题禁用进度色 | `QColor` |
| `darkDisabledProgressColor` | 暗黑主题禁用进度色 | `QColor` |
| `lightPausedColor` | 明亮主题暂停状态色 | `QColor` |
| `darkPausedColor` | 暗黑主题暂停状态色 | `QColor` |
| `lightErrorColor` | 明亮主题错误状态色 | `QColor` |
| `darkErrorColor` | 暗黑主题错误状态色 | `QColor` |

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认大小 | `48px` | 环形进度条默认直径 |
| 线条宽度 | `4px` | 进度环线条宽度 |
| 动画时长 | `200ms` | 进度变化动画时长 |
| 不确定动画时长 | `2000ms` | 不确定进度动画周期 |

### 主题适配

```cpp
// 环形进度条会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 进度条颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 进度色会自动更新为新的强调色
});
```