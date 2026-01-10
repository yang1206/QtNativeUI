# NDialog 对话框

## 介绍

NDialog 是 QtNativeUI 的无边框对话框组件，基于 Qt 的 QDialog 扩展而来。它集成了 QWindowKit 库，提供了现代化的窗口效果和自定义标题栏支持，同时保持 QDialog 的所有原生行为（模态、exec()、accept()、reject() 等）。

## 基本用法

```cpp
#include <QtNativeUI/NDialog.h>

// 创建对话框
NDialog* dialog = new NDialog(this);
dialog->setWindowTitle("设置");
dialog->resize(400, 300);

// 设置背景效果
dialog->setBackdropType(NDialog::Mica);

// 设置内容
QWidget* content = new QWidget(dialog);
QVBoxLayout* layout = new QVBoxLayout(content);
layout->addWidget(new QLabel("对话框内容"));

NPushButton* okBtn = new NPushButton("确定", content);
connect(okBtn, &NPushButton::clicked, dialog, &QDialog::accept);
layout->addWidget(okBtn);

dialog->setContentWidget(content);

// 显示对话框
dialog->exec();
```

## 窗口背景效果

NDialog 支持与 NMainWindow 相同的背景效果：

```cpp
NDialog* dialog = new NDialog(this);

// 无效果（纯色背景）
dialog->setBackdropType(NDialog::None);

// 模糊效果
dialog->setBackdropType(NDialog::Blur);

// Acrylic 效果 (Windows 10/11)
dialog->setBackdropType(NDialog::Acrylic);

// Mica 效果 (Windows 11)
dialog->setBackdropType(NDialog::Mica);

// MicaAlt 效果 (Windows 11)
dialog->setBackdropType(NDialog::MicaAlt);
```

## 标题栏

NDialog 默认提供一个简化的标题栏，只包含标题和关闭按钮。

### 使用默认标题栏

```cpp
NDialog* dialog = new NDialog(this);
dialog->setWindowTitle("我的对话框");

// 获取默认标题栏
NWindowBar* bar = dialog->windowBar();

// 默认标题栏只显示关闭按钮
// 最小化、最大化、置顶、主题按钮默认隐藏
```

### 自定义标题栏

```cpp
NDialog* dialog = new NDialog(this);
dialog->setBackdropType(NDialog::Mica);
dialog->resize(450, 350);

// 创建自定义标题栏
QWidget* titleBar = new QWidget(dialog);
QHBoxLayout* titleLayout = new QHBoxLayout(titleBar);
titleLayout->setContentsMargins(10, 5, 5, 5);

QLabel* titleLabel = new QLabel("自定义标题栏", titleBar);
QFont titleFont = titleLabel->font();
titleFont.setBold(true);
titleLabel->setFont(titleFont);

NPushButton* closeBtn = new NPushButton("×", titleBar);
closeBtn->setFixedSize(30, 30);
connect(closeBtn, &NPushButton::clicked, dialog, &QDialog::reject);

titleLayout->addWidget(titleLabel);
titleLayout->addStretch();
titleLayout->addWidget(closeBtn);

titleBar->setFixedHeight(40);

// 设置自定义标题栏
dialog->setTitleBar(titleBar);

// 确保关闭按钮可以接收鼠标事件
dialog->setHitTestVisible(closeBtn, true);
```

### 隐藏默认标题栏

使用 `setWindowBarVisible(false)` 可以隐藏默认标题栏，适用于需要完全自定义布局的场景：

```cpp
NDialog* dialog = new NDialog(this);
dialog->setBackdropType(NDialog::Mica);
dialog->resize(400, 300);

// 隐藏默认标题栏
dialog->setWindowBarVisible(false);

// 创建内容
QWidget* content = new QWidget(dialog);
QVBoxLayout* layout = new QVBoxLayout(content);
layout->setContentsMargins(20, 20, 20, 20);

QLabel* label = new QLabel("无标题栏对话框", content);
layout->addWidget(label);
layout->addStretch();

QHBoxLayout* btnLayout = new QHBoxLayout();
NPushButton* cancelBtn = new NPushButton("取消", content);
NPushButton* okBtn = new NPushButton("确定", content);
connect(cancelBtn, &NPushButton::clicked, dialog, &QDialog::reject);
connect(okBtn, &NPushButton::clicked, dialog, &QDialog::accept);

btnLayout->addStretch();
btnLayout->addWidget(cancelBtn);
btnLayout->addWidget(okBtn);
layout->addLayout(btnLayout);

dialog->setContentWidget(content);
dialog->exec();
```

## 内容区域

使用 `setContentWidget()` 设置对话框的内容区域：

```cpp
NDialog* dialog = new NDialog(this);
dialog->setBackdropType(NDialog::Mica);
dialog->resize(400, 300);

// 创建内容控件
QWidget* content = new QWidget(dialog);
QVBoxLayout* layout = new QVBoxLayout(content);
layout->setContentsMargins(20, 10, 20, 20);

QLabel* label = new QLabel("这是对话框的内容区域", content);
layout->addWidget(label);
layout->addStretch();

QHBoxLayout* btnLayout = new QHBoxLayout();
NPushButton* cancelBtn = new NPushButton("取消", content);
NPushButton* okBtn = new NPushButton("确定", content);
okBtn->setButtonType(NPushButton::Accent);

connect(cancelBtn, &NPushButton::clicked, dialog, &QDialog::reject);
connect(okBtn, &NPushButton::clicked, dialog, &QDialog::accept);

btnLayout->addStretch();
btnLayout->addWidget(cancelBtn);
btnLayout->addWidget(okBtn);
layout->addLayout(btnLayout);

// 设置内容
dialog->setContentWidget(content);

dialog->exec();
```

## 系统按钮

可以注册自定义按钮为系统按钮，获得原生窗口行为（如 Windows 11 Snap Layout）：

```cpp
NDialog* dialog = new NDialog(this);

// 创建自定义关闭按钮
NPushButton* closeBtn = new NPushButton("关闭", dialog);

// 注册为系统关闭按钮
dialog->setSystemButton(NDialog::Close, closeBtn);
```

## 完整示例

```cpp
#include <QApplication>
#include <QtNativeUI/NDialog.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NLineEdit.h>

void showSettingsDialog(QWidget* parent)
{
    NDialog* dialog = new NDialog(parent);
    dialog->setWindowTitle("设置");
    dialog->setBackdropType(NDialog::Mica);
    dialog->resize(450, 350);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // 创建内容
    QWidget* content = new QWidget(dialog);
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(20, 10, 20, 20);

    // 表单
    QFormLayout* form = new QFormLayout();
    NLineEdit* nameEdit = new NLineEdit(content);
    NLineEdit* emailEdit = new NLineEdit(content);
    form->addRow("用户名:", nameEdit);
    form->addRow("邮箱:", emailEdit);
    layout->addLayout(form);

    layout->addStretch();

    // 按钮
    QHBoxLayout* btnLayout = new QHBoxLayout();
    NPushButton* cancelBtn = new NPushButton("取消", content);
    NPushButton* saveBtn = new NPushButton("保存", content);
    saveBtn->setButtonType(NPushButton::Accent);

    connect(cancelBtn, &NPushButton::clicked, dialog, &QDialog::reject);
    connect(saveBtn, &NPushButton::clicked, dialog, [=]() {
        qDebug() << "保存设置:" << nameEdit->text() << emailEdit->text();
        dialog->accept();
    });

    btnLayout->addStretch();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    layout->addLayout(btnLayout);

    dialog->setContentWidget(content);
    dialog->exec();
}
```

## API 参考

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NDialog(QWidget* parent = nullptr)` | 创建对话框实例 |

### 背景效果

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setBackdropType(BackdropType type)` | 设置窗口背景效果 | void |
| `backdropType() const` | 获取当前背景效果类型 | BackdropType |

### 标题栏

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `windowBar() const` | 获取默认标题栏组件 | NWindowBar* |
| `setTitleBar(QWidget* titleBar)` | 设置自定义标题栏 | void |
| `titleBar() const` | 获取当前标题栏 | QWidget* |
| `setWindowBarVisible(bool visible)` | 设置默认标题栏可见性 | void |
| `windowBarVisible() const` | 获取默认标题栏可见性 | bool |

### 内容区域

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setContentWidget(QWidget* widget)` | 设置内容控件 | void |
| `contentWidget() const` | 获取当前内容控件 | QWidget* |

### 窗口属性

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setWindowAttribute(const QString& key, const QVariant& value)` | 设置窗口属性 | bool |
| `windowAttribute(const QString& key) const` | 获取窗口属性 | QVariant |
| `borderThickness() const` | 获取窗口边框厚度 | int |
| `titleBarHeight() const` | 获取标题栏高度 | int |

### 系统按钮

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setSystemButton(SystemButtonType type, QAbstractButton* button)` | 注册系统按钮 | void |
| `setHitTestVisible(QWidget* widget, bool visible)` | 设置控件鼠标事件可见性 | void |

### macOS 特定方法

#### 隐藏原生系统按钮（红绿灯）

在 macOS 上使用自定义标题栏或隐藏默认标题栏时，原生的红绿灯按钮默认仍然可见。可以使用以下方法隐藏：

```cpp
#ifdef Q_OS_MAC
class MyDialog : public NDialog {
public:
    MyDialog(QWidget* parent = nullptr) : NDialog(parent) {}

protected:
    void showEvent(QShowEvent* event) override {
        NDialog::showEvent(event);
        // 隐藏 macOS 红绿灯按钮
        setNativeSystemButtonsVisible(false);
    }
};

MyDialog* dialog = new MyDialog(this);
dialog->setTitleBar(customTitleBar);  // 或 setWindowBarVisible(false)
dialog->exec();
#endif
```

#### 自定义系统按钮位置

```cpp
#ifdef Q_OS_MAC
NDialog* dialog = new NDialog(this);

// 完全隐藏红绿灯（推荐方式）
dialog->setNativeSystemButtonsVisible(false);

// 或者使用回调自定义位置
dialog->setSystemButtonAreaCallback([](const QSize& size) {
    constexpr int width = -75;
    return QRect(QPoint(size.width() - width, 0), QSize(width, size.height()));
});

dialog->exec();
#endif
```

**重要提示：**
- 必须在窗口显示后调用，推荐在 `showEvent()` 中调用
- 当使用 `setTitleBar()` 或 `setWindowBarVisible(false)` 时建议隐藏红绿灯
- 对于简单隐藏需求，推荐使用 `setNativeSystemButtonsVisible(false)`

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `setNativeSystemButtonsVisible(bool visible)` | 设置原生系统按钮可见性 | void |
| `setSystemButtonAreaCallback(...)` | 设置系统按钮区域回调 | void |

### 高级功能

| 方法 | 说明 | 返回值 |
|------|------|--------|
| `windowAgent() const` | 获取 QWindowKit 代理对象 | QWK::WidgetWindowAgent* |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `backdropTypeChanged(BackdropType type)` | 背景效果类型已更改 | 新的效果类型 |

### 枚举类型

#### BackdropType

| 值 | 说明 | 平台支持 |
|----|------|----------|
| `None` | 无特殊效果 | 所有平台 |
| `Blur` | 模糊效果 | Windows (DWM), macOS |
| `Acrylic` | 亚克力材质效果 | Windows 10/11 |
| `Mica` | Mica 材质效果 | Windows 11 |
| `MicaAlt` | Mica 替代材质效果 | Windows 11 |

#### SystemButtonType

| 值 | 说明 |
|----|------|
| `Minimize` | 最小化按钮 |
| `Maximize` | 最大化按钮 |
| `Close` | 关闭按钮 |

## 与 NMainWindow 的区别

| 特性 | NMainWindow | NDialog |
|------|-------------|---------|
| 基类 | QMainWindow | QDialog |
| 默认标题栏按钮 | 全部显示 | 只有关闭按钮 |
| 模态支持 | 否 | 是 (exec()) |
| 菜单栏支持 | 是 | 否 |
| 中心控件 | setCentralWidget() | setContentWidget() |

## 平台差异

与 NMainWindow 相同，参见 [NMainWindow 文档](./NMainWindow.md#平台差异)。
