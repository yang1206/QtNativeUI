# NOverlay 遮罩层

## 介绍

NOverlay 是 QtNativeUI 的遮罩层组件，用于在界面上显示半透明遮罩，常用于加载状态、模态背景等场景。支持自定义颜色、动画效果和内容显示。

## 基本用法

```cpp
#include <QtNativeUI/NOverlay.h>

// 创建遮罩层
NOverlay* overlay = new NOverlay(this);
overlay->setAttribute(Qt::WA_DeleteOnClose);

// 显示遮罩
overlay->showOverlay();

// 点击关闭（默认启用）
// overlay 会在点击时自动关闭并删除
```

## 加载状态遮罩

NOverlay 内置了加载状态显示功能，会自动创建 ProgressRing 和文字：

```cpp
// 在当前父控件上显示加载
NOverlay* overlay = new NOverlay(this);
overlay->setAttribute(Qt::WA_DeleteOnClose);
overlay->showLoading("正在加载数据...");

// 3秒后关闭
QTimer::singleShot(3000, overlay, &NOverlay::close);
```

### 局部加载遮罩

```cpp
// 在指定控件上显示加载遮罩
NPushButton* button = new NPushButton("提交", this);

NOverlay* overlay = new NOverlay();
overlay->setAttribute(Qt::WA_DeleteOnClose);
overlay->showLoading(button, "提交中...");

// 或者构造时指定父控件
NOverlay* overlay2 = new NOverlay(button);
overlay2->showLoading("处理中...");
```

## 自定义内容

可以设置自定义的内容控件：

```cpp
NOverlay* overlay = new NOverlay(this);
overlay->setAttribute(Qt::WA_DeleteOnClose);

// 创建自定义内容
QWidget* content = new QWidget(overlay);
QVBoxLayout* layout = new QVBoxLayout(content);

QLabel* label = new QLabel("自定义提示内容", content);
label->setAlignment(Qt::AlignCenter);

NPushButton* btn = new NPushButton("确定", content);
connect(btn, &NPushButton::clicked, overlay, &NOverlay::close);

layout->addWidget(label);
layout->addWidget(btn);

overlay->setContentWidget(content);
overlay->showOverlay();
```

## 颜色和样式

### 自定义遮罩颜色

```cpp
NOverlay* overlay = new NOverlay(this);

// 设置亮色和暗色主题下的遮罩颜色
overlay->setLightOverlayColor(QColor(0, 100, 200));  // 蓝色
overlay->setDarkOverlayColor(QColor(0, 80, 160));

// 设置透明度 (0-255)
overlay->setOpacity(120);

overlay->showOverlay();
```

### 动画设置

```cpp
NOverlay* overlay = new NOverlay(this);

// 设置动画时长（毫秒）
overlay->setAnimationDuration(500);  // 500ms 动画

// 禁用动画
overlay->setAnimationDuration(0);    // 无动画

overlay->showOverlay();
```

## 交互控制

### 点击关闭

```cpp
NOverlay* overlay = new NOverlay(this);

// 启用点击关闭（默认）
overlay->setClickToClose(true);

// 禁用点击关闭（如加载状态）
overlay->setClickToClose(false);

overlay->showOverlay();
```

### 手动关闭

```cpp
NOverlay* overlay = new NOverlay(this);
overlay->showOverlay();

// 带动画关闭（默认）
overlay->close();

// 立即隐藏（不关闭）
overlay->hideOverlay();

// 隐藏加载状态
overlay->hideLoading();
```

## 完整示例

```cpp
#include <QApplication>
#include <QTimer>
#include <QtNativeUI/NOverlay.h>
#include <QtNativeUI/NPushButton.h>

void showLoadingExample(QWidget* parent)
{
    // 创建加载遮罩
    NOverlay* overlay = new NOverlay(parent);
    overlay->setAttribute(Qt::WA_DeleteOnClose);
    
    // 自定义样式
    overlay->setLightOverlayColor(QColor(0, 0, 0));
    overlay->setDarkOverlayColor(QColor(0, 0, 0));
    overlay->setOpacity(150);
    overlay->setAnimationDuration(300);
    
    // 显示加载状态
    overlay->showLoading("正在处理请求...");
    
    // 模拟异步操作
    QTimer::singleShot(3000, overlay, [overlay]() {
        overlay->close();  // 自动带淡出动画
    });
}

void showCustomOverlay(QWidget* parent)
{
    NOverlay* overlay = new NOverlay(parent);
    overlay->setAttribute(Qt::WA_DeleteOnClose);
    
    // 创建自定义内容
    QWidget* content = new QWidget(overlay);
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setAlignment(Qt::AlignCenter);
    
    QLabel* title = new QLabel("确认操作", content);
    QFont font = title->font();
    font.setBold(true);
    font.setPointSize(14);
    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);
    
    QLabel* message = new QLabel("此操作不可撤销，确定继续吗？", content);
    message->setAlignment(Qt::AlignCenter);
    message->setWordWrap(true);
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    NPushButton* cancelBtn = new NPushButton("取消", content);
    NPushButton* okBtn = new NPushButton("确定", content);
    
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(okBtn);
    
    layout->addWidget(title);
    layout->addWidget(message);
    layout->addLayout(btnLayout);
    
    connect(cancelBtn, &NPushButton::clicked, overlay, &NOverlay::close);
    connect(okBtn, &NPushButton::clicked, overlay, [overlay]() {
        qDebug() << "用户确认操作";
        overlay->close();
    });
    
    overlay->setContentWidget(content);
    overlay->setClickToClose(false);  // 必须通过按钮关闭
    overlay->showOverlay();
}
```

## API 参考

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NOverlay(QWidget* parent = nullptr)` | 创建遮罩层实例 |

### 显示和隐藏

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `showOverlay()` | 在当前父控件上显示遮罩 | void |
| `showOverlay(QWidget* parent)` | 在指定控件上显示遮罩 | void |
| `hideOverlay()` | 隐藏遮罩（不关闭） | void |
| `close()` | 关闭遮罩（带动画） | void |

### 加载状态

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `showLoading(const QString& text)` | 在当前父控件上显示加载状态 | void |
| `showLoading(QWidget* parent, const QString& text)` | 在指定控件上显示加载状态 | void |
| `hideLoading()` | 隐藏加载状态 | void |

### 内容设置

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setContentWidget(QWidget* widget)` | 设置自定义内容控件 | void |
| `contentWidget() const` | 获取当前内容控件 | QWidget* |

### 样式属性

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setLightOverlayColor(const QColor& color)` | 设置亮色主题遮罩颜色 | void |
| `getLightOverlayColor() const` | 获取亮色主题遮罩颜色 | QColor |
| `setDarkOverlayColor(const QColor& color)` | 设置暗色主题遮罩颜色 | void |
| `getDarkOverlayColor() const` | 获取暗色主题遮罩颜色 | QColor |
| `setOpacity(int opacity)` | 设置透明度 (0-255) | void |
| `getOpacity() const` | 获取透明度 | int |

### 交互属性

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setClickToClose(bool enable)` | 设置点击是否关闭 | void |
| `getClickToClose() const` | 获取点击关闭状态 | bool |
| `setAnimationDuration(int ms)` | 设置动画时长（毫秒） | void |
| `getAnimationDuration() const` | 获取动画时长 | int |

### 状态查询

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `isAnimating() const` | 是否正在播放动画 | bool |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `clicked()` | 遮罩被点击时发射 | 无 |

## 使用场景

### 1. 全屏加载状态

```cpp
// 应用启动时的全屏加载
NOverlay* overlay = new NOverlay(mainWindow);
overlay->showLoading("正在初始化应用...");
```

### 2. 局部操作反馈

```cpp
// 按钮点击后的局部加载
connect(submitBtn, &NPushButton::clicked, this, [=]() {
    NOverlay* overlay = new NOverlay(submitBtn);
    overlay->showLoading("提交中...");
    
    // 异步提交完成后关闭
    submitData().then([overlay]() {
        overlay->close();
    });
});
```

### 3. 模态确认对话框

```cpp
// 替代 QMessageBox 的现代化确认框
NOverlay* overlay = new NOverlay(this);
overlay->setContentWidget(createConfirmWidget());
overlay->setClickToClose(false);
overlay->showOverlay();
```

### 4. 引导遮罩

```cpp
// 功能引导时的半透明遮罩
NOverlay* overlay = new NOverlay(this);
overlay->setOpacity(100);  // 较低透明度
overlay->setContentWidget(createGuideWidget());
overlay->showOverlay();
```

## 注意事项

1. **内存管理**：建议使用 `setAttribute(Qt::WA_DeleteOnClose)` 自动管理内存
2. **动画性能**：在低性能设备上可以设置 `setAnimationDuration(0)` 禁用动画
3. **层级关系**：遮罩会自动调用 `raise()` 确保显示在最上层
4. **主题适配**：遮罩颜色会根据当前主题自动切换
5. **响应式布局**：遮罩会自动适应父控件的大小变化