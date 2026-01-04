# NToolTip 工具提示

### 介绍

工具提示是一个浮动的提示窗口，用于显示关于控件的简短说明。NToolTip 提供了现代化的外观、自动定位、动画效果和丰富的自定义选项。

### 基本用法

```cpp
// 创建工具提示
NToolTip* tooltip = new NToolTip("这是一个工具提示");

// 在指定控件处显示
NPushButton* button = new NPushButton("悬停查看提示");
tooltip->adjustPosition(button, NToolTipPosition::TOP);

// 显示工具提示
tooltip->show();
```

### 快速显示

```cpp
// 快速显示工具提示
NPushButton* button = new NPushButton("按钮");

NToolTip::showText("点击此按钮执行操作", button, NToolTipPosition::TOP);
```

### 自动显示

```cpp
// 创建按钮
NPushButton* button = new NPushButton("悬停查看提示");

// 创建工具提示过滤器
NToolTipFilter* filter = new NToolTipFilter(button, 300, NToolTipPosition::TOP);

// 设置工具提示文本
NToolTip* tooltip = NToolTip::createToolTip("这是一个自动显示的工具提示", button);

// 当鼠标进入按钮时自动显示工具提示
```

### 定位选项

```cpp
// 创建工具提示
NToolTip* tooltip = new NToolTip("工具提示文本");

// 在不同位置显示
NPushButton* button = new NPushButton("按钮");

// 在上方显示
tooltip->adjustPosition(button, NToolTipPosition::TOP);

// 在下方显示
// tooltip->adjustPosition(button, NToolTipPosition::BOTTOM);

// 在左方显示
// tooltip->adjustPosition(button, NToolTipPosition::LEFT);

// 在右方显示
// tooltip->adjustPosition(button, NToolTipPosition::RIGHT);

// 在左上角显示
// tooltip->adjustPosition(button, NToolTipPosition::TOP_LEFT);

// 在右上角显示
// tooltip->adjustPosition(button, NToolTipPosition::TOP_RIGHT);

// 在左下角显示
// tooltip->adjustPosition(button, NToolTipPosition::BOTTOM_LEFT);

// 在右下角显示
// tooltip->adjustPosition(button, NToolTipPosition::BOTTOM_RIGHT);

tooltip->show();
```

### 自定义文本

```cpp
// 创建工具提示
NToolTip* tooltip = new NToolTip();

// 设置文本
tooltip->setText("自定义工具提示文本");

// 获取文本
QString text = tooltip->text();
qDebug() << "工具提示文本：" << text;

// 显示
tooltip->show();
```

### 显示时长

```cpp
// 创建工具提示
NToolTip* tooltip = new NToolTip("3秒后自动隐藏");

// 设置显示时长（毫秒）
tooltip->setDuration(3000);

// 显示
tooltip->show();
```

### 自定义样式

```cpp
// 创建工具提示
NToolTip* tooltip = new NToolTip("自定义样式的工具提示");

// 设置圆角
tooltip->setBorderRadius(8);

// 设置内容边距
tooltip->setContentPadding(12);

// 设置阴影半径
tooltip->setShadowRadius(10);

// 自定义颜色
tooltip->setLightBackgroundColor(QColor(255, 255, 255));
tooltip->setLightTextColor(QColor(50, 50, 50));
tooltip->setLightBorderColor(QColor(200, 200, 200));

tooltip->setDarkBackgroundColor(QColor(45, 45, 45));
tooltip->setDarkTextColor(QColor(220, 220, 220));
tooltip->setDarkBorderColor(QColor(80, 80, 80));

// 显示
tooltip->show();
```

### 按钮工具提示

```cpp
// 创建按钮
NPushButton* saveBtn = new NPushButton("保存");
NPushButton* deleteBtn = new NPushButton("删除");
NPushButton* settingsBtn = new NPushButton("设置");

// 为按钮添加工具提示
NToolTip::showText("保存文件 (Ctrl+S)", saveBtn, NToolTipPosition::TOP);
NToolTip::showText("删除选中项 (Delete)", deleteBtn, NToolTipPosition::TOP);
NToolTip::showText("打开设置 (Ctrl+,)", settingsBtn, NToolTipPosition::TOP);
```

### 工具栏工具提示

```cpp
// 创建工具栏
QToolBar* toolBar = new QToolBar();

// 创建工具栏按钮
QAction* newAction = toolBar->addAction("新建");
QAction* openAction = toolBar->addAction("打开");
QAction* saveAction = toolBar->addAction("保存");

// 为工具栏按钮添加工具提示
newAction->setToolTip("创建新文件 (Ctrl+N)");
openAction->setToolTip("打开文件 (Ctrl+O)");
saveAction->setToolTip("保存文件 (Ctrl+S)");
```

### 表格单元格工具提示

```cpp
// 创建表格
QTableWidget* table = new QTableWidget(3, 3);

// 为单元格添加工具提示
for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
        QTableWidgetItem* item = new QTableWidgetItem(QString("单元格 (%1, %2)").arg(row).arg(col));
        item->setToolTip(QString("这是第 %1 行第 %2 列的单元格").arg(row + 1).arg(col + 1));
        table->setItem(row, col, item);
    }
}
```

### 列表项工具提示

```cpp
// 创建列表
NListView* listView = new NListView();

// 创建模型
QStringListModel* model = new QStringListModel();
QStringList items = {"项目1", "项目2", "项目3"};
model->setStringList(items);

listView->setModel(model);

// 为列表项添加工具提示
for (int i = 0; i < items.size(); ++i) {
    QModelIndex index = model->index(i);
    model->setData(index, QString("这是 %1 的详细说明").arg(items[i]), Qt::ToolTipRole);
}
```

### 动态工具提示

```cpp
// 创建标签
QLabel* label = new QLabel("悬停查看动态提示");

// 创建工具提示过滤器
NToolTipFilter* filter = new NToolTipFilter(label, 500, NToolTipPosition::TOP);

// 动态更新工具提示
int counter = 0;
QTimer* timer = new QTimer();
connect(timer, &QTimer::timeout, [=]() mutable {
    counter++;
    QString tooltipText = QString("动态提示 - 计数：%1").arg(counter);
    filter->showToolTip();
});

timer->start(1000);
```

### 富文本工具提示

```cpp
// 创建按钮
NPushButton* button = new NPushButton("详细信息");

// 创建富文本工具提示
QString richText = "<b>标题</b><br/>"
                   "这是一个<i>富文本</i>工具提示<br/>"
                   "支持<u>格式化</u>文本";

NToolTip* tooltip = new NToolTip(richText);
tooltip->adjustPosition(button, NToolTipPosition::TOP);

// 显示
tooltip->show();
```

### 工具提示延迟

```cpp
// 创建按钮
NPushButton* button = new NPushButton("延迟提示");

// 创建工具提示过滤器，设置延迟时间
NToolTipFilter* filter = new NToolTipFilter(button, 1000, NToolTipPosition::TOP);

// 修改延迟时间
filter->setShowDelay(500);

// 创建工具提示
NToolTip* tooltip = NToolTip::createToolTip("这是一个延迟显示的工具提示", button);
```

### 隐藏工具提示

```cpp
// 创建工具提示过滤器
NToolTipFilter* filter = new NToolTipFilter(nullptr, 300, NToolTipPosition::TOP);

// 显示工具提示
filter->showToolTip();

// 隐藏工具提示
filter->hideToolTip();
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NToolTip(const QString& text = "", QWidget* parent = nullptr)` | 创建工具提示 |

### 文本管理

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setText(const QString& text)` | 设置工具提示文本 | 文本字符串 |
| `text()` | 获取工具提示文本 | 返回 `QString` |

### 显示时长

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setDuration(int duration)` | 设置显示时长 | 时长（毫秒） |
| `duration()` | 获取显示时长 | 返回 `int` |

### 定位

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `adjustPosition(QWidget* widget, NToolTipPosition position)` | 调整工具提示位置 | - |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `borderRadius` | 边框圆角半径 | `int` |
| `contentPadding` | 内容边距 | `int` |
| `shadowRadius` | 阴影半径 | `int` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightBackgroundColor` | 明亮主题背景色 | `QColor` |
| `darkBackgroundColor` | 暗黑主题背景色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |

### 静态方法

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `createToolTip(const QString& text, QWidget* parent)` | 快速创建工具提示 | 返回 `NToolTip*` |
| `showText(const QString& text, QWidget* parent, NToolTipPosition position)` | 快速显示工具提示 | - |

## NToolTipFilter API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NToolTipFilter(QWidget* parent, int showDelay, NToolTipPosition position)` | 创建工具提示过滤器 |

### 方法

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `hideToolTip()` | 隐藏工具提示 | - |
| `showToolTip()` | 显示工具提示 | - |
| `setShowDelay(int delay)` | 设置显示延迟 | 延迟时间（毫秒） |
| `setPosition(NToolTipPosition position)` | 设置显示位置 | 位置枚举值 |

## 定位选项

| 位置 | 说明 |
|------|------|
| `NToolTipPosition::TOP` | 在目标上方 |
| `NToolTipPosition::BOTTOM` | 在目标下方 |
| `NToolTipPosition::LEFT` | 在目标左方 |
| `NToolTipPosition::RIGHT` | 在目标右方 |
| `NToolTipPosition::TOP_LEFT` | 在目标左上角 |
| `NToolTipPosition::TOP_RIGHT` | 在目标右上角 |
| `NToolTipPosition::BOTTOM_LEFT` | 在目标左下角 |
| `NToolTipPosition::BOTTOM_RIGHT` | 在目标右下角 |

## 主题定制

工具提示会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
