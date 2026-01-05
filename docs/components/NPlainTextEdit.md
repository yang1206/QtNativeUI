# NPlainTextEdit 纯文本编辑器

### 介绍

纯文本编辑器用于编辑和显示纯文本内容，性能优于富文本编辑器，适合代码编辑、日志查看等场景。NPlainTextEdit 基于 Qt 的 QPlainTextEdit，提供了现代化的外观和高效的文本处理。

### 基本用法

```cpp
// 创建纯文本编辑器
NPlainTextEdit* textEdit = new NPlainTextEdit();
textEdit->setPlaceholderText("请输入纯文本内容...");

// 设置初始内容
textEdit->setPlainText("这是一段纯文本内容。\n支持多行文本编辑。");

// 监听内容变化
connect(textEdit, &QPlainTextEdit::textChanged, []() {
    qDebug() << "文本内容已改变";
});
```

### 代码编辑器

```cpp
// 创建代码编辑器
NPlainTextEdit* codeEditor = new NPlainTextEdit();
codeEditor->setPlaceholderText("输入代码...");

// 设置等宽字体
QFont codeFont;
QStringList fontFamilies = {"Fira Code", "Consolas", "Monaco", "Courier New"};
for (const QString& family : fontFamilies) {
    codeFont.setFamily(family);
    if (QFontInfo(codeFont).family() == family) {
        break;
    }
}
codeFont.setPointSize(12);
codeEditor->setFont(codeFont);

// 设置制表符宽度
QFontMetrics metrics(codeFont);
codeEditor->setTabStopDistance(4 * metrics.horizontalAdvance(' '));

// 启用行号显示（需要自定义实现）
codeEditor->setLineWrapMode(QPlainTextEdit::NoWrap);

// 设置示例代码
QString sampleCode = R"(#include <iostream>
#include <string>

int main() {
    std::string message = "Hello, World!";
    std::cout << message << std::endl;
    return 0;
})";
codeEditor->setPlainText(sampleCode);
```

### 日志查看器

```cpp
// 创建日志查看器
NPlainTextEdit* logViewer = new NPlainTextEdit();
logViewer->setReadOnly(true);
logViewer->setMaximumBlockCount(1000); // 限制最大行数

// 设置等宽字体
QFont logFont("Consolas", 10);
logViewer->setFont(logFont);

// 模拟日志输出
QTimer* logTimer = new QTimer();
connect(logTimer, &QTimer::timeout, [=]() {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logEntry = QString("[%1] INFO: 系统运行正常\n").arg(timestamp);
    
    logViewer->appendPlainText(logEntry.trimmed());
    
    // 自动滚动到底部
    QTextCursor cursor = logViewer->textCursor();
    cursor.movePosition(QTextCursor::End);
    logViewer->setTextCursor(cursor);
});

logTimer->start(2000); // 每2秒添加一条日志

// 清空日志按钮
NPushButton* clearBtn = new NPushButton("清空日志");
connect(clearBtn, &QPushButton::clicked, [=]() {
    logViewer->clear();
});
```

### 配置文件编辑器

```cpp
// 创建配置文件编辑器
NPlainTextEdit* configEditor = new NPlainTextEdit();
configEditor->setPlaceholderText("编辑配置文件...");

// 设置配置文件内容
QString configContent = R"([database]
host = localhost
port = 3306
username = admin
password = secret

[server]
port = 8080
debug = true
max_connections = 100

[logging]
level = INFO
file = /var/log/app.log
)";
configEditor->setPlainText(configContent);

// 保存配置
NPushButton* saveConfigBtn = new NPushButton("保存配置");
connect(saveConfigBtn, &QPushButton::clicked, [=]() {
    QString config = configEditor->toPlainText();
    
    // 简单验证
    if (config.contains("[database]") && config.contains("[server]")) {
        NInfoBar::success(NInfoBarType::Top, "保存成功", "配置文件已保存", 2000);
    } else {
        NInfoBar::error(NInfoBarType::Top, "保存失败", "配置文件格式不正确", 3000);
    }
});
```

### 文本统计工具

```cpp
// 创建文本统计工具
QWidget* textAnalyzer = new QWidget();
QVBoxLayout* analyzerLayout = new QVBoxLayout(textAnalyzer);

NPlainTextEdit* textInput = new NPlainTextEdit();
textInput->setPlaceholderText("输入要分析的文本...");
textInput->setMaximumHeight(200);

// 统计信息显示
QWidget* statsWidget = new QWidget();
QGridLayout* statsLayout = new QGridLayout(statsWidget);

NLabel* charCountLabel = new NLabel("字符数: 0", NLabelType::Body);
NLabel* wordCountLabel = new NLabel("单词数: 0", NLabelType::Body);
NLabel* lineCountLabel = new NLabel("行数: 0", NLabelType::Body);
NLabel* paragraphCountLabel = new NLabel("段落数: 0", NLabelType::Body);

statsLayout->addWidget(charCountLabel, 0, 0);
statsLayout->addWidget(wordCountLabel, 0, 1);
statsLayout->addWidget(lineCountLabel, 1, 0);
statsLayout->addWidget(paragraphCountLabel, 1, 1);

analyzerLayout->addWidget(textInput);
analyzerLayout->addWidget(statsWidget);

// 实时统计
connect(textInput, &QPlainTextEdit::textChanged, [=]() {
    QString text = textInput->toPlainText();
    
    int charCount = text.length();
    int wordCount = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).size();
    int lineCount = text.split('\n').size();
    int paragraphCount = text.split(QRegularExpression("\n\\s*\n"), Qt::SkipEmptyParts).size();
    
    charCountLabel->setText(QString("字符数: %1").arg(charCount));
    wordCountLabel->setText(QString("单词数: %1").arg(wordCount));
    lineCountLabel->setText(QString("行数: %1").arg(lineCount));
    paragraphCountLabel->setText(QString("段落数: %1").arg(paragraphCount));
});
```

### 搜索和替换

```cpp
// 创建带搜索功能的文本编辑器
QWidget* searchableEditor = new QWidget();
QVBoxLayout* editorLayout = new QVBoxLayout(searchableEditor);

// 搜索栏
QWidget* searchBar = new QWidget();
QHBoxLayout* searchLayout = new QHBoxLayout(searchBar);

NLineEdit* searchEdit = new NLineEdit();
searchEdit->setPlaceholderText("搜索...");

NLineEdit* replaceEdit = new NLineEdit();
replaceEdit->setPlaceholderText("替换为...");

NPushButton* findBtn = new NPushButton("查找");
NPushButton* replaceBtn = new NPushButton("替换");
NPushButton* replaceAllBtn = new NPushButton("全部替换");

searchLayout->addWidget(searchEdit);
searchLayout->addWidget(replaceEdit);
searchLayout->addWidget(findBtn);
searchLayout->addWidget(replaceBtn);
searchLayout->addWidget(replaceAllBtn);

// 文本编辑器
NPlainTextEdit* editor = new NPlainTextEdit();
editor->setPlainText("这是一段示例文本。\n可以在这里测试搜索和替换功能。\n示例文本包含多个示例。");

editorLayout->addWidget(searchBar);
editorLayout->addWidget(editor);

// 查找功能
connect(findBtn, &QPushButton::clicked, [=]() {
    QString searchText = searchEdit->text();
    if (!searchText.isEmpty()) {
        if (!editor->find(searchText)) {
            // 如果没找到，从头开始搜索
            QTextCursor cursor = editor->textCursor();
            cursor.movePosition(QTextCursor::Start);
            editor->setTextCursor(cursor);
            editor->find(searchText);
        }
    }
});

// 替换功能
connect(replaceBtn, &QPushButton::clicked, [=]() {
    QString searchText = searchEdit->text();
    QString replaceText = replaceEdit->text();
    
    QTextCursor cursor = editor->textCursor();
    if (cursor.hasSelection() && cursor.selectedText() == searchText) {
        cursor.insertText(replaceText);
    }
});

// 全部替换
connect(replaceAllBtn, &QPushButton::clicked, [=]() {
    QString searchText = searchEdit->text();
    QString replaceText = replaceEdit->text();
    
    if (!searchText.isEmpty()) {
        QString content = editor->toPlainText();
        content.replace(searchText, replaceText);
        editor->setPlainText(content);
    }
});
```

### 自定义样式

```cpp
NPlainTextEdit* styledEditor = new NPlainTextEdit();

// 设置边框样式
styledEditor->setBorderRadius(8);
styledEditor->setBorderWidth(1);

// 自定义颜色
styledEditor->setLightBackgroundColor(QColor(248, 249, 250));
styledEditor->setLightBackgroundHoverColor(QColor(240, 240, 240));
styledEditor->setLightBackgroundFocusColor(QColor(255, 255, 255));
styledEditor->setLightTextColor(QColor(32, 32, 32));
styledEditor->setLightBorderColor(QColor(200, 200, 200));

// 选中文本颜色
styledEditor->setLightSelectionBackgroundColor(QColor(0, 120, 215));
styledEditor->setLightSelectionTextColor(QColor(255, 255, 255));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NPlainTextEdit(QWidget* parent = nullptr)` | 创建空的纯文本编辑器 |
| `NPlainTextEdit(const QString& text, QWidget* parent = nullptr)` | 创建带初始文本的编辑器 |

### 样式设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |
| `setBorderWidth(int width)` | 设置边框宽度 | 边框宽度 |
| `borderWidth()` | 获取边框宽度 | - |

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

NPlainTextEdit 继承自 `QPlainTextEdit`，支持所有标准 QPlainTextEdit 的方法和信号：

- `setPlainText()` / `toPlainText()` - 设置/获取纯文本内容
- `insertPlainText()` / `appendPlainText()` - 插入/追加纯文本
- `setReadOnly()` / `isReadOnly()` - 设置/获取只读状态
- `setLineWrapMode()` / `lineWrapMode()` - 设置/获取换行模式
- `setMaximumBlockCount()` / `maximumBlockCount()` - 设置/获取最大块数
- `setTabStopDistance()` / `tabStopDistance()` - 设置/获取制表符距离
- `find()` - 查找文本
- `textChanged()` - 文本改变信号
- `selectionChanged()` - 选择改变信号
- `cursorPositionChanged()` - 光标位置改变信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 边框圆角 | `4px` | 编辑器边框圆角 |
| 边框宽度 | `1px` | 编辑器边框宽度 |
| 内边距 | `8px` | 编辑器内边距 |
| 最小高度 | `100px` | 编辑器最小高度 |

### 主题适配

```cpp
// 纯文本编辑器会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 编辑器颜色会自动更新
});
```