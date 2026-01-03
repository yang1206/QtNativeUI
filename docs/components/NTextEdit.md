# NTextEdit 富文本编辑器

### 介绍

富文本编辑器用于编辑和显示格式化文本，支持字体样式、颜色、图片等富文本功能。NTextEdit 基于 Qt 的 QTextEdit，提供了现代化的外观和丰富的编辑功能。

### 基本用法

```cpp
// 创建富文本编辑器
NTextEdit* textEdit = new NTextEdit();
textEdit->setPlaceholderText("请输入内容...");

// 设置初始内容
textEdit->setHtml("<h1>标题</h1><p>这是一段<b>粗体</b>和<i>斜体</i>文本。</p>");

// 监听内容变化
connect(textEdit, &QTextEdit::textChanged, []() {
    qDebug() << "文本内容已改变";
});
```

### 文档编辑器

```cpp
// 创建完整的文档编辑器
QWidget* editorWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(editorWidget);

// 工具栏
QToolBar* toolBar = new QToolBar();

// 字体格式按钮
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

toolBar->addWidget(boldBtn);
toolBar->addWidget(italicBtn);
toolBar->addWidget(underlineBtn);
toolBar->addSeparator();

// 对齐按钮
NToolButton* alignLeftBtn = new NToolButton();
alignLeftBtn->setFluentIcon(NRegularIconType::TextAlignLeft24Regular, 16);
alignLeftBtn->setCheckable(true);

NToolButton* alignCenterBtn = new NToolButton();
alignCenterBtn->setFluentIcon(NRegularIconType::TextAlignCenter24Regular, 16);
alignCenterBtn->setCheckable(true);

NToolButton* alignRightBtn = new NToolButton();
alignRightBtn->setFluentIcon(NRegularIconType::TextAlignRight24Regular, 16);
alignRightBtn->setCheckable(true);

QButtonGroup* alignGroup = new QButtonGroup();
alignGroup->addButton(alignLeftBtn, 0);
alignGroup->addButton(alignCenterBtn, 1);
alignGroup->addButton(alignRightBtn, 2);
alignGroup->setExclusive(true);

toolBar->addWidget(alignLeftBtn);
toolBar->addWidget(alignCenterBtn);
toolBar->addWidget(alignRightBtn);

// 文本编辑器
NTextEdit* editor = new NTextEdit();
editor->setMinimumHeight(400);

layout->addWidget(toolBar);
layout->addWidget(editor);

// 连接格式化功能
connect(boldBtn, &QToolButton::toggled, [=](bool checked) {
    QTextCharFormat format;
    format.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    editor->mergeCurrentCharFormat(format);
});

connect(italicBtn, &QToolButton::toggled, [=](bool checked) {
    QTextCharFormat format;
    format.setFontItalic(checked);
    editor->mergeCurrentCharFormat(format);
});

connect(underlineBtn, &QToolButton::toggled, [=](bool checked) {
    QTextCharFormat format;
    format.setFontUnderline(checked);
    editor->mergeCurrentCharFormat(format);
});

connect(alignGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int id) {
    Qt::Alignment alignment;
    switch (id) {
        case 0: alignment = Qt::AlignLeft; break;
        case 1: alignment = Qt::AlignCenter; break;
        case 2: alignment = Qt::AlignRight; break;
    }
    editor->setAlignment(alignment);
});
```

### 代码编辑器

```cpp
// 创建代码编辑器
NTextEdit* codeEditor = new NTextEdit();
codeEditor->setPlaceholderText("输入代码...");

// 设置等宽字体
QFont codeFont("Consolas", 12);
if (!codeFont.exactMatch()) {
    codeFont.setFamily("Monaco");
    if (!codeFont.exactMatch()) {
        codeFont.setFamily("Courier New");
    }
}
codeEditor->setFont(codeFont);

// 设置代码样式
codeEditor->setLineWrapMode(QTextEdit::NoWrap);
codeEditor->setAcceptRichText(false); // 纯文本模式

// 语法高亮（简单示例）
connect(codeEditor, &QTextEdit::textChanged, [=]() {
    QString text = codeEditor->toPlainText();
    
    // 简单的关键字高亮
    QTextDocument* doc = codeEditor->document();
    QTextCursor cursor(doc);
    
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(0, 0, 255));
    keywordFormat.setFontWeight(QFont::Bold);
    
    QStringList keywords = {"class", "public", "private", "void", "int", "return"};
    for (const QString& keyword : keywords) {
        cursor.movePosition(QTextCursor::Start);
        while (!cursor.isNull() && !cursor.atEnd()) {
            cursor = doc->find(keyword, cursor, QTextDocument::FindWholeWords);
            if (!cursor.isNull()) {
                cursor.mergeCharFormat(keywordFormat);
            }
        }
    }
});
```

### 笔记应用

```cpp
// 创建笔记应用
QWidget* noteApp = new QWidget();
QHBoxLayout* noteLayout = new QHBoxLayout(noteApp);

// 笔记列表
QListWidget* noteList = new QListWidget();
noteList->setMaximumWidth(200);
noteList->addItem("会议记录");
noteList->addItem("项目计划");
noteList->addItem("学习笔记");

// 笔记编辑器
NTextEdit* noteEditor = new NTextEdit();
noteEditor->setPlaceholderText("开始记录你的想法...");

// 笔记标题
NLineEdit* titleEdit = new NLineEdit();
titleEdit->setPlaceholderText("笔记标题");

QVBoxLayout* editorLayout = new QVBoxLayout();
editorLayout->addWidget(titleEdit);
editorLayout->addWidget(noteEditor);

QWidget* editorWidget = new QWidget();
editorWidget->setLayout(editorLayout);

noteLayout->addWidget(noteList);
noteLayout->addWidget(editorWidget);

// 笔记切换
connect(noteList, &QListWidget::currentTextChanged, [=](const QString& title) {
    titleEdit->setText(title);
    // 这里可以加载对应笔记的内容
    if (title == "会议记录") {
        noteEditor->setHtml("<h2>项目会议</h2><p><b>时间：</b>2024年1月15日</p><p><b>参与者：</b>张三、李四、王五</p>");
    } else if (title == "项目计划") {
        noteEditor->setHtml("<h2>Q1项目计划</h2><ul><li>需求分析</li><li>原型设计</li><li>开发实现</li></ul>");
    }
});

// 自动保存
QTimer* autoSaveTimer = new QTimer();
autoSaveTimer->setInterval(30000); // 30秒自动保存
connect(autoSaveTimer, &QTimer::timeout, [=]() {
    // 保存当前笔记
    qDebug() << "自动保存笔记：" << titleEdit->text();
});
autoSaveTimer->start();
```

### 邮件编辑器

```cpp
// 创建邮件编辑器
QWidget* mailEditor = new QWidget();
QVBoxLayout* mailLayout = new QVBoxLayout(mailEditor);

// 邮件头部
QFormLayout* headerLayout = new QFormLayout();
NLineEdit* toEdit = new NLineEdit();
toEdit->setPlaceholderText("收件人邮箱");

NLineEdit* ccEdit = new NLineEdit();
ccEdit->setPlaceholderText("抄送（可选）");

NLineEdit* subjectEdit = new NLineEdit();
subjectEdit->setPlaceholderText("邮件主题");

headerLayout->addRow("收件人:", toEdit);
headerLayout->addRow("抄送:", ccEdit);
headerLayout->addRow("主题:", subjectEdit);

// 邮件内容
NTextEdit* mailContent = new NTextEdit();
mailContent->setPlaceholderText("请输入邮件内容...");
mailContent->setMinimumHeight(300);

// 设置默认邮件模板
QString emailTemplate = R"(
<p>您好，</p>
<p></p>
<p>此致<br/>
敬礼！</p>
<p><br/>
<b>发件人姓名</b><br/>
职位<br/>
公司名称<br/>
电话：xxx-xxxx-xxxx<br/>
邮箱：xxx@example.com</p>
)";
mailContent->setHtml(emailTemplate);

// 发送按钮
NPushButton* sendBtn = new NPushButton("发送邮件");
sendBtn->setButtonType(NPushButton::Accent);

mailLayout->addLayout(headerLayout);
mailLayout->addWidget(mailContent);
mailLayout->addWidget(sendBtn, 0, Qt::AlignRight);

// 发送邮件
connect(sendBtn, &QPushButton::clicked, [=]() {
    if (toEdit->text().isEmpty() || subjectEdit->text().isEmpty()) {
        NInfoBar::warning(NInfoBarType::Top, "提示", "请填写收件人和主题", 3000, mailEditor);
        return;
    }
    
    // 模拟发送邮件
    NInfoBar::success(NInfoBarType::Top, "发送成功", "邮件已发送", 2000, mailEditor);
});
```

### 自定义样式

```cpp
NTextEdit* styledEditor = new NTextEdit();

// 设置边框样式
styledEditor->setBorderRadius(8);
styledEditor->setBorderWidth(1);

// 自定义颜色
styledEditor->setLightBackgroundColor(QColor(255, 255, 255));
styledEditor->setLightBackgroundHoverColor(QColor(248, 248, 248));
styledEditor->setLightBackgroundFocusColor(QColor(240, 248, 255));
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
| `NTextEdit(QWidget* parent = nullptr)` | 创建空的富文本编辑器 |
| `NTextEdit(const QString& text, QWidget* parent = nullptr)` | 创建带初始文本的编辑器 |

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

NTextEdit 继承自 `QTextEdit`，支持所有标准 QTextEdit 的方法和信号：

- `setHtml()` / `toHtml()` - 设置/获取HTML内容
- `setPlainText()` / `toPlainText()` - 设置/获取纯文本
- `setMarkdown()` / `toMarkdown()` - 设置/获取Markdown内容
- `insertHtml()` / `insertPlainText()` - 插入HTML/纯文本
- `setReadOnly()` / `isReadOnly()` - 设置/获取只读状态
- `setLineWrapMode()` / `lineWrapMode()` - 设置/获取换行模式
- `setAcceptRichText()` / `acceptRichText()` - 设置/获取富文本支持
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
// 富文本编辑器会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 编辑器颜色会自动更新
});
```