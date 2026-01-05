# NHyperlinkButton 超链接按钮

### 介绍

超链接按钮提供了类似网页链接的交互体验，支持URL跳转和自定义点击行为。NHyperlinkButton 基于 Qt 的 QPushButton，具有链接样式的外观和悬停效果。

### 基本用法

```cpp
// 创建基本超链接按钮
NHyperlinkButton* linkBtn = new NHyperlinkButton("访问官网");
linkBtn->setUrl("https://www.qt.io");

// 监听链接激活
connect(linkBtn, &NHyperlinkButton::linkActivated, [](const QUrl& url) {
    qDebug() << "打开链接：" << url.toString();
    QDesktopServices::openUrl(url);
});
```

### 不同类型的链接

```cpp
// 网站链接
NHyperlinkButton* websiteBtn = new NHyperlinkButton("Qt官方网站");
websiteBtn->setUrl("https://www.qt.io");

// 邮件链接
NHyperlinkButton* emailBtn = new NHyperlinkButton("发送邮件");
emailBtn->setUrl("mailto:support@example.com");

// 文件链接
NHyperlinkButton* fileBtn = new NHyperlinkButton("打开文档");
fileBtn->setUrl("file:///path/to/document.pdf");

// 自定义协议
NHyperlinkButton* customBtn = new NHyperlinkButton("打开应用");
customBtn->setUrl("myapp://action/open");
```

### 帮助和文档链接

```cpp
// 创建帮助页面的链接组
QWidget* helpWidget = new QWidget();
QVBoxLayout* helpLayout = new QVBoxLayout(helpWidget);

NLabel* helpTitle = new NLabel("获取帮助", NLabelType::Subtitle);
helpLayout->addWidget(helpTitle);

// 用户手册
NHyperlinkButton* manualBtn = new NHyperlinkButton("用户手册");
manualBtn->setUrl("https://docs.example.com/manual");

// 常见问题
NHyperlinkButton* faqBtn = new NHyperlinkButton("常见问题");
faqBtn->setUrl("https://docs.example.com/faq");

// 联系支持
NHyperlinkButton* supportBtn = new NHyperlinkButton("联系技术支持");
supportBtn->setUrl("mailto:support@example.com?subject=技术支持请求");

// 社区论坛
NHyperlinkButton* forumBtn = new NHyperlinkButton("社区论坛");
forumBtn->setUrl("https://forum.example.com");

helpLayout->addWidget(manualBtn);
helpLayout->addWidget(faqBtn);
helpLayout->addWidget(supportBtn);
helpLayout->addWidget(forumBtn);

// 统一处理链接点击
QList<NHyperlinkButton*> linkButtons = {manualBtn, faqBtn, supportBtn, forumBtn};
for (NHyperlinkButton* btn : linkButtons) {
    connect(btn, &NHyperlinkButton::linkActivated, [](const QUrl& url) {
        QDesktopServices::openUrl(url);
    });
}
```

### 内部导航链接

```cpp
// 创建应用内导航链接
NHyperlinkButton* settingsBtn = new NHyperlinkButton("打开设置");
NHyperlinkButton* aboutBtn = new NHyperlinkButton("关于应用");
NHyperlinkButton* historyBtn = new NHyperlinkButton("查看历史");

// 不设置URL，使用自定义行为
connect(settingsBtn, &QPushButton::clicked, []() {
    // 打开设置页面
    openSettingsDialog();
});

connect(aboutBtn, &QPushButton::clicked, []() {
    // 显示关于对话框
    showAboutDialog();
});

connect(historyBtn, &QPushButton::clicked, []() {
    // 显示历史记录
    showHistoryWindow();
});
```

### 社交媒体链接

```cpp
// 创建社交媒体链接组
QWidget* socialWidget = new QWidget();
QHBoxLayout* socialLayout = new QHBoxLayout(socialWidget);

// GitHub
NHyperlinkButton* githubBtn = new NHyperlinkButton("GitHub");
githubBtn->setUrl("https://github.com/username/project");

// Twitter
NHyperlinkButton* twitterBtn = new NHyperlinkButton("Twitter");
twitterBtn->setUrl("https://twitter.com/username");

// LinkedIn
NHyperlinkButton* linkedinBtn = new NHyperlinkButton("LinkedIn");
linkedinBtn->setUrl("https://linkedin.com/in/username");

socialLayout->addWidget(githubBtn);
socialLayout->addWidget(twitterBtn);
socialLayout->addWidget(linkedinBtn);
socialLayout->addStretch();

// 统一处理社交链接
QList<NHyperlinkButton*> socialButtons = {githubBtn, twitterBtn, linkedinBtn};
for (NHyperlinkButton* btn : socialButtons) {
    connect(btn, &NHyperlinkButton::linkActivated, [](const QUrl& url) {
        // 可以添加统计或确认逻辑
        qDebug() << "访问社交媒体：" << url.toString();
        QDesktopServices::openUrl(url);
    });
}
```

### 下载链接

```cpp
// 创建下载页面
QWidget* downloadWidget = new QWidget();
QVBoxLayout* downloadLayout = new QVBoxLayout(downloadWidget);

NLabel* downloadTitle = new NLabel("下载资源", NLabelType::Title);
downloadLayout->addWidget(downloadTitle);

// 软件下载
NHyperlinkButton* appDownloadBtn = new NHyperlinkButton("下载应用程序 (Windows)");
appDownloadBtn->setUrl("https://releases.example.com/app-windows.exe");

NHyperlinkButton* macDownloadBtn = new NHyperlinkButton("下载应用程序 (macOS)");
macDownloadBtn->setUrl("https://releases.example.com/app-macos.dmg");

// 文档下载
NHyperlinkButton* pdfDownloadBtn = new NHyperlinkButton("下载PDF手册");
pdfDownloadBtn->setUrl("https://docs.example.com/manual.pdf");

downloadLayout->addWidget(appDownloadBtn);
downloadLayout->addWidget(macDownloadBtn);
downloadLayout->addWidget(pdfDownloadBtn);

// 处理下载链接
QList<NHyperlinkButton*> downloadButtons = {appDownloadBtn, macDownloadBtn, pdfDownloadBtn};
for (NHyperlinkButton* btn : downloadButtons) {
    connect(btn, &NHyperlinkButton::linkActivated, [](const QUrl& url) {
        // 可以显示下载确认对话框
        QMessageBox::StandardButton reply = QMessageBox::question(
            nullptr,
            "确认下载",
            QString("确定要下载文件吗？\n%1").arg(url.toString()),
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply == QMessageBox::Yes) {
            QDesktopServices::openUrl(url);
        }
    });
}
```

### 链接验证和错误处理

```cpp
NHyperlinkButton* validatedBtn = new NHyperlinkButton("验证链接");
validatedBtn->setUrl("https://example.com/api/check");

connect(validatedBtn, &NHyperlinkButton::linkActivated, [](const QUrl& url) {
    // 验证链接是否有效
    if (!url.isValid()) {
        NInfoBar::error(
            NInfoBarType::Top,
            "链接错误",
            "链接格式不正确",
            3000
        );
        return;
    }
    
    // 检查网络连接
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "QtNativeUI App");
    
    QNetworkReply* reply = manager->head(request);
    
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 链接有效，打开
            QDesktopServices::openUrl(reply->url());
        } else {
            // 链接无效或网络错误
            NInfoBar::error(
                NInfoBarType::Top,
                "无法访问",
                "链接无法访问，请检查网络连接",
                4000
            );
        }
        reply->deleteLater();
    });
});
```

### 自定义样式

```cpp
NHyperlinkButton* styledBtn = new NHyperlinkButton("自定义样式链接");

// 设置禁用状态颜色
styledBtn->setDisabledColor(QColor(150, 150, 150));

// 设置悬停背景色
styledBtn->setBackgroundHoverColor(QColor(240, 248, 255));

// 设置按下背景色
styledBtn->setBackgroundPressColor(QColor(220, 235, 255));
```

### 链接状态管理

```cpp
// 创建可切换状态的链接
NHyperlinkButton* toggleBtn = new NHyperlinkButton("启用功能");
bool featureEnabled = false;

connect(toggleBtn, &QPushButton::clicked, [=]() mutable {
    featureEnabled = !featureEnabled;
    
    if (featureEnabled) {
        toggleBtn->setText("禁用功能");
        // 执行启用逻辑
    } else {
        toggleBtn->setText("启用功能");
        // 执行禁用逻辑
    }
});

// 根据条件禁用链接
auto updateLinkState = [=]() {
    bool canAccess = checkUserPermission();
    toggleBtn->setEnabled(canAccess);
    
    if (!canAccess) {
        toggleBtn->setToolTip("您没有权限访问此功能");
    }
};
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NHyperlinkButton(QWidget* parent = nullptr)` | 创建空的超链接按钮 |
| `NHyperlinkButton(QString text, QWidget* parent = nullptr)` | 创建带文本的超链接按钮 |

### URL 设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setUrl(const QString& url)` | 设置链接URL（字符串） | URL字符串 |
| `setUrl(const QUrl& url)` | 设置链接URL（对象） | URL对象 |
| `url()` | 获取链接URL | - |

### 颜色属性

| 方法 | 说明 | 参数 |
|------|------|------|
| `setDisabledColor(const QColor& color)` | 设置禁用状态颜色 | 颜色值 |
| `disabledColor()` | 获取禁用状态颜色 | - |
| `setBackgroundHoverColor(const QColor& color)` | 设置悬停背景色 | 颜色值 |
| `backgroundHoverColor()` | 获取悬停背景色 | - |
| `setBackgroundPressColor(const QColor& color)` | 设置按下背景色 | 颜色值 |
| `backgroundPressColor()` | 获取按下背景色 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `linkActivated(const QUrl& url)` | 链接被激活时触发 | 链接URL |

### 继承的 API

NHyperlinkButton 继承自 `QPushButton`，支持所有标准 QPushButton 的方法和信号：

- `setText()` / `text()` - 设置/获取按钮文本
- `setEnabled()` / `isEnabled()` - 启用/禁用状态
- `setVisible()` / `isVisible()` - 显示/隐藏状态
- `setToolTip()` / `toolTip()` - 设置/获取工具提示
- `clicked()` - 点击信号
- `pressed()` / `released()` - 按下/释放信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认颜色 | 强调色 | 链接文本颜色 |
| 悬停颜色 | 强调色（深色） | 悬停时文本颜色 |
| 禁用颜色 | 灰色 | 禁用状态文本颜色 |
| 下划线 | 悬停时显示 | 链接下划线样式 |

### 主题适配

```cpp
// 超链接按钮会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 链接颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 链接颜色会自动更新为新的强调色
});
```