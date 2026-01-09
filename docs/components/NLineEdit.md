# NLineEdit 文本输入框

### 介绍

文本输入框用于接收用户的单行文本输入。NLineEdit 基于 Qt 的 QLineEdit，提供了 Fluent Design 风格的现代化外观和增强功能。

### 基本用法

```cpp
// 基本文本输入框
NLineEdit* lineEdit = new NLineEdit();
lineEdit->setPlaceholderText("请输入内容");

// 带默认文本的输入框
NLineEdit* textEdit = new NLineEdit("默认文本");
```

### 图标操作

支持在输入框左侧或右侧添加图标按钮，常用于搜索、清除等功能。

```cpp
NLineEdit* searchEdit = new NLineEdit();
searchEdit->setPlaceholderText("搜索...");

// 添加搜索图标到左侧
QAction* searchAction = searchEdit->addAction(NRegularIconType::Search24Regular, 
                                             QLineEdit::LeadingPosition);

// 添加清除图标到右侧
QAction* clearAction = searchEdit->addAction(NRegularIconType::Dismiss24Regular, 
                                           QLineEdit::TrailingPosition);

// 连接清除功能
connect(clearAction, &QAction::triggered, searchEdit, &QLineEdit::clear);
```

### 焦点事件

NLineEdit 提供了增强的焦点事件信号，方便处理输入框的焦点状态。

```cpp
NLineEdit* focusEdit = new NLineEdit();

// 监听焦点进入事件
connect(focusEdit, &NLineEdit::focusIn, [](const QString& text) {
    qDebug() << "焦点进入，当前文本：" << text;
});

// 监听焦点离开事件
connect(focusEdit, &NLineEdit::focusOut, [](const QString& text) {
    qDebug() << "焦点离开，当前文本：" << text;
});
```

### 自定义样式

支持自定义边框、背景色、文本色等样式属性。

```cpp
NLineEdit* styledEdit = new NLineEdit();

// 设置圆角
styledEdit->setBorderRadius(8);

// 设置边框宽度
styledEdit->setBorderWidth(2);

// 自定义明亮主题颜色
styledEdit->setLightBackgroundColor(QColor(248, 249, 250));
styledEdit->setLightBorderColor(QColor(200, 200, 200));
styledEdit->setLightTextColor(QColor(32, 32, 32));

// 自定义暗黑主题颜色
styledEdit->setDarkBackgroundColor(QColor(45, 45, 45));
styledEdit->setDarkBorderColor(QColor(100, 100, 100));
styledEdit->setDarkTextColor(QColor(255, 255, 255));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NLineEdit(QWidget* parent = nullptr)` | 创建空的文本输入框 |
| `NLineEdit(const QString& text, QWidget* parent = nullptr)` | 创建带默认文本的输入框 |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `addAction(QAction* action, ActionPosition position)` | 添加操作按钮 | 操作对象、位置 |
| `addAction(NRegularIconType::Icon icon, ActionPosition position)` | 添加 Regular 图标按钮 | 图标类型、位置 |
| `addAction(NFilledIconType::Icon icon, ActionPosition position)` | 添加 Filled 图标按钮 | 图标类型、位置 |
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |
| `setBorderWidth(int width)` | 设置边框宽度 | 边框宽度 |
| `borderWidth()` | 获取边框宽度 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `focusIn(QString text)` | 焦点进入时触发 | 当前文本内容 |
| `focusOut(QString text)` | 焦点离开时触发 | 当前文本内容 |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightBackgroundColor` | 明亮主题背景色 | `QColor` |
| `darkBackgroundColor` | 暗黑主题背景色 | `QColor` |
| `lightBackgroundHoverColor` | 明亮主题悬停背景色 | `QColor` |
| `darkBackgroundHoverColor` | 暗黑主题悬停背景色 | `QColor` |
| `lightBackgroundFocusColor` | 明亮主题焦点背景色 | `QColor` |
| `darkBackgroundFocusColor` | 暗黑主题焦点背景色 | `QColor` |
| `lightBackgroundDisabledColor` | 明亮主题禁用背景色 | `QColor` |
| `darkBackgroundDisabledColor` | 暗黑主题禁用背景色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |
| `lightBorderDisabledColor` | 明亮主题禁用边框色 | `QColor` |
| `darkBorderDisabledColor` | 暗黑主题禁用边框色 | `QColor` |
| `lightBottomLineColor` | 明亮主题底部线条色 | `QColor` |
| `darkBottomLineColor` | 暗黑主题底部线条色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightTextDisabledColor` | 明亮主题禁用文本色 | `QColor` |
| `darkTextDisabledColor` | 暗黑主题禁用文本色 | `QColor` |
| `lightSelectionBackgroundColor` | 明亮主题选中背景色 | `QColor` |
| `darkSelectionBackgroundColor` | 暗黑主题选中背景色 | `QColor` |
| `lightSelectionTextColor` | 明亮主题选中文本色 | `QColor` |
| `darkSelectionTextColor` | 暗黑主题选中文本色 | `QColor` |

### 继承的 API

NLineEdit 继承自 `QLineEdit`，支持所有标准 QLineEdit 的方法和信号：

- `setText()` / `text()` - 设置/获取文本内容
- `setPlaceholderText()` / `placeholderText()` - 设置/获取占位符文本
- `setMaxLength()` / `maxLength()` - 设置/获取最大长度
- `setReadOnly()` / `isReadOnly()` - 设置/获取只读状态
- `setEchoMode()` / `echoMode()` - 设置/获取回显模式（密码输入等）
- `textChanged()` - 文本改变信号
- `editingFinished()` - 编辑完成信号
- `returnPressed()` - 回车键按下信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 边框圆角 | `4px` | 输入框圆角 |
| 边框宽度 | `1px` | 输入框边框宽度 |
| 默认高度 | `32px` | 标准输入框高度 |
| 内边距 | `8px 12px` | 输入框内边距 |

### 主题适配

```cpp
// 输入框会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 输入框颜色会自动更新
});
```