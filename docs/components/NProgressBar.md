# NProgressBar 进度条

### 介绍

进度条用于显示任务的完成进度或加载状态。NProgressBar 基于 Qt 的 QProgressBar，提供了现代化的外观、流畅的动画效果和多种状态支持。

### 基本用法

```cpp
// 基本进度条
NProgressBar* progressBar = new NProgressBar();
progressBar->setRange(0, 100);
progressBar->setValue(50);

// 水平进度条（默认）
NProgressBar* horizontalBar = new NProgressBar(Qt::Horizontal);

// 垂直进度条
NProgressBar* verticalBar = new NProgressBar(Qt::Vertical);
```

### 不确定进度

用于显示正在进行但无法确定具体进度的任务。

```cpp
NProgressBar* indeterminateBar = new NProgressBar();
indeterminateBar->setIndeterminate(true);

// 也可以通过设置范围来实现
NProgressBar* loadingBar = new NProgressBar();
loadingBar->setRange(0, 0); // 范围为0时自动变为不确定进度
```

### 进度状态

支持正常、暂停、错误三种状态，每种状态有不同的颜色表示。

```cpp
NProgressBar* statusBar = new NProgressBar();
statusBar->setRange(0, 100);
statusBar->setValue(30);

// 暂停状态（黄色）
statusBar->setPaused(true);
// 或者
statusBar->pause();

// 恢复正常状态
statusBar->resume();

// 错误状态（红色）
statusBar->setError(true);
// 或者
statusBar->error();

// 重置到正常状态
statusBar->reset();
```

### 文件下载示例

```cpp
// 模拟文件下载进度
NProgressBar* downloadBar = new NProgressBar();
downloadBar->setRange(0, 100);
downloadBar->setFormat("下载中... %p%");

QTimer* downloadTimer = new QTimer();
int progress = 0;

connect(downloadTimer, &QTimer::timeout, [=]() mutable {
    progress += qrand() % 5 + 1; // 随机增加1-5
    
    if (progress >= 100) {
        progress = 100;
        downloadTimer->stop();
        downloadBar->setFormat("下载完成");
    }
    
    downloadBar->setValue(progress);
});

downloadTimer->start(100); // 每100ms更新一次
```

### 批量操作进度

```cpp
// 批量文件处理进度
NProgressBar* batchBar = new NProgressBar();
batchBar->setRange(0, 1000); // 1000个文件

QLabel* statusLabel = new NLabel("准备处理...", NLabelType::Body);

// 模拟批量处理
QTimer* batchTimer = new QTimer();
int processed = 0;

connect(batchTimer, &QTimer::timeout, [=]() mutable {
    processed += qrand() % 10 + 1;
    
    if (processed >= 1000) {
        processed = 1000;
        batchTimer->stop();
        statusLabel->setText("处理完成");
    } else {
        statusLabel->setText(QString("正在处理第 %1 个文件...").arg(processed));
    }
    
    batchBar->setValue(processed);
});

batchTimer->start(50);
```

### 自定义样式

支持自定义进度条的颜色、圆角、厚度等样式属性。

```cpp
NProgressBar* styledBar = new NProgressBar();

// 设置圆角和厚度
styledBar->setBorderRadius(8);
styledBar->setProgressThickness(12);

// 自定义明亮主题颜色
styledBar->setLightTrackColor(QColor(240, 240, 240));
styledBar->setLightProgressColor(QColor(0, 120, 215));
styledBar->setLightTextColor(QColor(32, 32, 32));

// 自定义状态颜色
styledBar->setLightPausedColor(QColor(255, 193, 7));  // 暂停状态为黄色
styledBar->setLightErrorColor(QColor(220, 53, 69));   // 错误状态为红色

// 启用动画
styledBar->setAnimationEnabled(true);
styledBar->setAnimationDuration(300);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NProgressBar(QWidget* parent = nullptr)` | 创建水平进度条 |
| `NProgressBar(Qt::Orientation orientation, QWidget* parent = nullptr)` | 创建指定方向的进度条 |

### 进度控制

| 方法 | 说明 | 参数 |
|------|------|------|
| `setValue(int value)` | 设置进度值 | 进度值 |
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

### 样式设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置圆角半径 | 圆角像素值 |
| `getBorderRadius()` | 获取圆角半径 | - |
| `setProgressThickness(int thickness)` | 设置进度条厚度 | 厚度像素值 |
| `getProgressThickness()` | 获取进度条厚度 | - |
| `setAnimationEnabled(bool enabled)` | 启用/禁用动画 | `true` / `false` |
| `setAnimationDuration(int duration)` | 设置动画时长 | 毫秒数 |

### 信号

| 信号 | 说明 |
|------|------|
| `trackThicknessChanged()` | 进度条厚度改变时触发 |
| `borderRadiusChanged()` | 圆角半径改变时触发 |

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

### 继承的 API

NProgressBar 继承自 `QProgressBar`，支持所有标准 QProgressBar 的方法和信号：

- `setRange(int min, int max)` / `minimum()` / `maximum()` - 设置/获取范围
- `value()` - 获取当前值
- `setFormat(const QString& format)` / `format()` - 设置/获取文本格式
- `setTextVisible(bool visible)` / `isTextVisible()` - 设置/获取文本可见性
- `setOrientation()` / `orientation()` - 设置/获取方向
- `valueChanged(int value)` - 值改变信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认高度 | `6px` | 水平进度条高度 |
| 默认宽度 | `6px` | 垂直进度条宽度 |
| 圆角半径 | `3px` | 进度条圆角 |
| 动画时长 | `200ms` | 进度变化动画时长 |
| 不确定动画时长 | `2000ms` | 不确定进度动画周期 |

### 状态颜色

| 状态 | 明亮主题 | 暗黑主题 | 说明 |
|------|----------|----------|------|
| 正常 | 强调色 | 强调色 | 使用系统强调色 |
| 暂停 | `#FFC107` | `#FFD54F` | 黄色系 |
| 错误 | `#DC3545` | `#F44336` | 红色系 |

### 主题适配

```cpp
// 进度条会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 进度条颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 进度色会自动更新为新的强调色
});
```