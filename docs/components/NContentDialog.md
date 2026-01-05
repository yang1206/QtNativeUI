# NContentDialog 内容对话框

### 介绍

内容对话框用于显示重要信息并获取用户确认或输入。NContentDialog 基于 Qt 的 QDialog，提供了现代化的外观和灵活的按钮配置。

### 基本用法

```cpp
// 创建简单确认对话框
NContentDialog* dialog = new NContentDialog(this);
dialog->setTitle("确认删除");
dialog->setContent("确定要删除这个文件吗？此操作无法撤销。");
dialog->setPrimaryButtonText("删除");
dialog->setCloseButtonText("取消");

// 显示对话框并获取结果
NContentDialog::DialogResult result = dialog->showDialog();
if (result == NContentDialog::ResultPrimary) {
    // 用户点击了删除按钮
    qDebug() << "执行删除操作";
} else {
    qDebug() << "取消删除";
}
```

### 三按钮对话框

```cpp
NContentDialog* saveDialog = new NContentDialog(this);
saveDialog->setTitle("保存文档");
saveDialog->setContent("文档已修改，是否保存更改？");
saveDialog->setPrimaryButtonText("保存");
saveDialog->setSecondaryButtonText("不保存");
saveDialog->setCloseButtonText("取消");

NContentDialog::DialogResult result = saveDialog->showDialog();
switch (result) {
    case NContentDialog::ResultPrimary:
        // 保存文档
        saveDocument();
        break;
    case NContentDialog::ResultSecondary:
        // 不保存，直接关闭
        closeDocument();
        break;
    case NContentDialog::ResultClose:
        // 取消操作
        break;
}
```

### 自定义内容控件

```cpp
// 创建登录对话框
NContentDialog* loginDialog = new NContentDialog(this);
loginDialog->setTitle("用户登录");
loginDialog->setPrimaryButtonText("登录");
loginDialog->setCloseButtonText("取消");

// 创建自定义内容
QWidget* loginWidget = new QWidget();
QFormLayout* formLayout = new QFormLayout(loginWidget);

NLineEdit* usernameEdit = new NLineEdit();
usernameEdit->setPlaceholderText("请输入用户名");

NLineEdit* passwordEdit = new NLineEdit();
passwordEdit->setEchoMode(QLineEdit::Password);
passwordEdit->setPlaceholderText("请输入密码");

NCheckBox* rememberBox = new NCheckBox("记住密码");

formLayout->addRow("用户名:", usernameEdit);
formLayout->addRow("密码:", passwordEdit);
formLayout->addRow("", rememberBox);

loginDialog->setContentWidget(loginWidget);

// 连接按钮事件
connect(loginDialog, &NContentDialog::primaryButtonClicked, [=]() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    bool remember = rememberBox->isChecked();
    
    if (username.isEmpty() || password.isEmpty()) {
        NInfoBar::warning(NInfoBarType::Top, "提示", "请填写完整信息", 3000, loginDialog);
        return;
    }
    
    // 执行登录逻辑
    if (performLogin(username, password)) {
        if (remember) {
            saveCredentials(username, password);
        }
        loginDialog->accept();
    } else {
        NInfoBar::error(NInfoBarType::Top, "登录失败", "用户名或密码错误", 3000, loginDialog);
    }
});
```

### 设置对话框

```cpp
// 创建设置对话框
NContentDialog* settingsDialog = new NContentDialog(this);
settingsDialog->setTitle("应用设置");
settingsDialog->setPrimaryButtonText("保存");
settingsDialog->setSecondaryButtonText("重置");
settingsDialog->setCloseButtonText("取消");

// 创建设置内容
QWidget* settingsWidget = new QWidget();
QVBoxLayout* settingsLayout = new QVBoxLayout(settingsWidget);

// 主题设置
NGroupBox* themeGroup = new NGroupBox("外观");
QVBoxLayout* themeLayout = new QVBoxLayout(themeGroup);

QButtonGroup* themeButtonGroup = new QButtonGroup();
NRadioButton* autoTheme = new NRadioButton("跟随系统");
NRadioButton* lightTheme = new NRadioButton("浅色主题");
NRadioButton* darkTheme = new NRadioButton("深色主题");

themeButtonGroup->addButton(autoTheme, 0);
themeButtonGroup->addButton(lightTheme, 1);
themeButtonGroup->addButton(darkTheme, 2);

themeLayout->addWidget(autoTheme);
themeLayout->addWidget(lightTheme);
themeLayout->addWidget(darkTheme);

// 通知设置
NGroupBox* notificationGroup = new NGroupBox("通知");
QVBoxLayout* notificationLayout = new QVBoxLayout(notificationGroup);

NToggleSwitch* desktopNotification = new NToggleSwitch("桌面通知");
NToggleSwitch* soundNotification = new NToggleSwitch("声音提醒");

notificationLayout->addWidget(desktopNotification);
notificationLayout->addWidget(soundNotification);

settingsLayout->addWidget(themeGroup);
settingsLayout->addWidget(notificationGroup);

settingsDialog->setContentWidget(settingsWidget);

// 处理按钮事件
connect(settingsDialog, &NContentDialog::primaryButtonClicked, [=]() {
    // 保存设置
    saveSettings();
    settingsDialog->accept();
});

connect(settingsDialog, &NContentDialog::secondaryButtonClicked, [=]() {
    // 重置设置
    resetSettings();
    // 更新UI状态
    autoTheme->setChecked(true);
    desktopNotification->setChecked(true);
    soundNotification->setChecked(false);
});
```

### 异步操作对话框

```cpp
// 创建进度对话框
NContentDialog* progressDialog = new NContentDialog(this);
progressDialog->setTitle("正在处理");
progressDialog->setCloseButtonText("取消");
progressDialog->setIsPrimaryButtonEnabled(false);
progressDialog->setIsSecondaryButtonEnabled(false);

// 创建进度内容
QWidget* progressWidget = new QWidget();
QVBoxLayout* progressLayout = new QVBoxLayout(progressWidget);

NLabel* statusLabel = new NLabel("正在初始化...", NLabelType::Body);
NProgressBar* progressBar = new NProgressBar();
progressBar->setRange(0, 100);

progressLayout->addWidget(statusLabel);
progressLayout->addWidget(progressBar);

progressDialog->setContentWidget(progressWidget);

// 模拟异步任务
QTimer* taskTimer = new QTimer();
int progress = 0;

connect(taskTimer, &QTimer::timeout, [=]() mutable {
    progress += qrand() % 10 + 1;
    
    if (progress >= 100) {
        progress = 100;
        taskTimer->stop();
        statusLabel->setText("处理完成");
        progressBar->setValue(progress);
        
        // 启用确定按钮
        progressDialog->setPrimaryButtonText("完成");
        progressDialog->setIsPrimaryButtonEnabled(true);
    } else {
        statusLabel->setText(QString("正在处理... %1%").arg(progress));
        progressBar->setValue(progress);
    }
});

// 处理取消操作
connect(progressDialog, &NContentDialog::closeButtonClicked, [=]() {
    taskTimer->stop();
    // 取消任务逻辑
    progressDialog->reject();
});

taskTimer->start(200);
progressDialog->showDialog();
```

### 按钮状态控制

```cpp
NContentDialog* validationDialog = new NContentDialog(this);
validationDialog->setTitle("输入验证");
validationDialog->setPrimaryButtonText("提交");
validationDialog->setCloseButtonText("取消");

// 初始状态禁用提交按钮
validationDialog->setIsPrimaryButtonEnabled(false);

QWidget* inputWidget = new QWidget();
QFormLayout* inputLayout = new QFormLayout(inputWidget);

NLineEdit* nameEdit = new NLineEdit();
NLineEdit* emailEdit = new NLineEdit();

inputLayout->addRow("姓名:", nameEdit);
inputLayout->addRow("邮箱:", emailEdit);

validationDialog->setContentWidget(inputWidget);

// 实时验证输入
auto validateInput = [=]() {
    bool isValid = !nameEdit->text().isEmpty() && 
                   emailEdit->text().contains("@");
    validationDialog->setIsPrimaryButtonEnabled(isValid);
};

connect(nameEdit, &QLineEdit::textChanged, validateInput);
connect(emailEdit, &QLineEdit::textChanged, validateInput);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NContentDialog(QWidget* parent = nullptr)` | 创建内容对话框 |

### 内容设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setTitle(const QString& title)` | 设置对话框标题 | 标题文本 |
| `title()` | 获取对话框标题 | - |
| `setContent(const QString& content)` | 设置内容文本 | 内容文本 |
| `content()` | 获取内容文本 | - |
| `setContentWidget(QWidget* widget)` | 设置自定义内容控件 | 控件指针 |

### 按钮设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setPrimaryButtonText(const QString& text)` | 设置主按钮文本 | 按钮文本 |
| `primaryButtonText()` | 获取主按钮文本 | - |
| `setSecondaryButtonText(const QString& text)` | 设置次按钮文本 | 按钮文本 |
| `secondaryButtonText()` | 获取次按钮文本 | - |
| `setCloseButtonText(const QString& text)` | 设置关闭按钮文本 | 按钮文本 |
| `closeButtonText()` | 获取关闭按钮文本 | - |

### 按钮状态

| 方法 | 说明 | 参数 |
|------|------|------|
| `setIsPrimaryButtonEnabled(bool enabled)` | 设置主按钮启用状态 | `true` / `false` |
| `isPrimaryButtonEnabled()` | 获取主按钮启用状态 | - |
| `setIsSecondaryButtonEnabled(bool enabled)` | 设置次按钮启用状态 | `true` / `false` |
| `isSecondaryButtonEnabled()` | 获取次按钮启用状态 | - |
| `setIsCloseButtonEnabled(bool enabled)` | 设置关闭按钮启用状态 | `true` / `false` |
| `isCloseButtonEnabled()` | 获取关闭按钮启用状态 | - |

### 对话框控制

| 方法 | 说明 | 参数 |
|------|------|------|
| `showDialog()` | 显示对话框并返回结果 | - |
| `closeDialog()` | 关闭对话框 | - |
| `result()` | 获取对话框结果 | - |

### 样式设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |
| `setDefaultButton(bool isDefault)` | 设置默认按钮 | `true` / `false` |
| `defaultButton()` | 获取默认按钮状态 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `primaryButtonClicked()` | 主按钮被点击时触发 | - |
| `secondaryButtonClicked()` | 次按钮被点击时触发 | - |
| `closeButtonClicked()` | 关闭按钮被点击时触发 | - |
| `dialogOpened()` | 对话框打开时触发 | - |
| `dialogClosed(DialogResult result)` | 对话框关闭时触发 | 关闭结果 |

### 枚举

#### DialogButton

| 值 | 说明 |
|----|------|
| `None` | 无按钮 |
| `Primary` | 主按钮 |
| `Secondary` | 次按钮 |
| `Close` | 关闭按钮 |

#### DialogResult

| 值 | 说明 |
|----|------|
| `ResultNone` | 无结果 |
| `ResultPrimary` | 主按钮结果 |
| `ResultSecondary` | 次按钮结果 |
| `ResultClose` | 关闭按钮结果 |

### 继承的 API

NContentDialog 继承自 `QDialog`，支持相关的方法和信号：

- `accept()` / `reject()` - 接受/拒绝对话框
- `exec()` - 模态显示对话框
- `show()` / `hide()` - 显示/隐藏对话框
- `setModal()` / `isModal()` - 设置/获取模态状态
- `accepted()` / `rejected()` - 接受/拒绝信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 最小宽度 | `320px` | 对话框最小宽度 |
| 最大宽度 | `600px` | 对话框最大宽度 |
| 边框圆角 | `8px` | 对话框圆角半径 |
| 内边距 | `24px` | 对话框内边距 |
| 按钮高度 | `32px` | 按钮标准高度 |

### 主题适配

```cpp
// 内容对话框会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 对话框颜色会自动更新
});
```