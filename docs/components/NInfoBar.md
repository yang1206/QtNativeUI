# NInfoBar 信息栏

### 介绍

信息栏用于向用户显示重要的状态信息、通知或反馈。支持四种信息类型（信息、成功、警告、错误）和多种显示位置，可以自动消失或手动关闭。

### 基本用法

```cpp
// 创建信息栏
NInfoBar* infoBar = new NInfoBar(
    NInfoBarType::Information,
    "提示",
    "这是一条信息提示",
    this,
    true  // 显示关闭按钮
);

// 添加到布局中
layout->addWidget(infoBar);
```

### 静态方法显示

使用静态方法可以快速显示临时信息栏，支持自动定位和消失。

```cpp
// 信息提示
NInfoBar::information(
    NInfoBarType::Top,
    "信息",
    "操作已完成",
    3000,  // 3秒后自动消失
    this
);

// 成功提示
NInfoBar::success(
    NInfoBarType::TopRight,
    "成功",
    "文件保存成功",
    2000,
    this
);

// 警告提示
NInfoBar::warning(
    NInfoBarType::Bottom,
    "警告",
    "磁盘空间不足",
    5000,
    this
);

// 错误提示
NInfoBar::error(
    NInfoBarType::BottomLeft,
    "错误",
    "网络连接失败",
    4000,
    this
);
```

### 不同位置显示

支持8个不同的显示位置，适应各种界面布局需求。

```cpp
// 顶部居中
NInfoBar::information(NInfoBarType::Top, "标题", "消息", 3000, this);

// 顶部左侧
NInfoBar::information(NInfoBarType::TopLeft, "标题", "消息", 3000, this);

// 顶部右侧
NInfoBar::information(NInfoBarType::TopRight, "标题", "消息", 3000, this);

// 底部居中
NInfoBar::information(NInfoBarType::Bottom, "标题", "消息", 3000, this);

// 底部左侧
NInfoBar::information(NInfoBarType::BottomLeft, "标题", "消息", 3000, this);

// 底部右侧
NInfoBar::information(NInfoBarType::BottomRight, "标题", "消息", 3000, this);

// 左侧居中
NInfoBar::information(NInfoBarType::Left, "标题", "消息", 3000, this);

// 右侧居中
NInfoBar::information(NInfoBarType::Right, "标题", "消息", 3000, this);
```

### 添加自定义控件

可以在信息栏中添加额外的控件，如按钮、链接等。

```cpp
NInfoBar* actionBar = new NInfoBar(
    NInfoBarType::Warning,
    "更新可用",
    "发现新版本，是否立即更新？",
    this,
    true
);

// 添加更新按钮
NPushButton* updateBtn = new NPushButton("立即更新");
updateBtn->setButtonType(NPushButton::Accent);
connect(updateBtn, &QPushButton::clicked, []() {
    // 执行更新逻辑
    qDebug() << "开始更新...";
});
actionBar->addWidget(updateBtn);

// 添加稍后按钮
NPushButton* laterBtn = new NPushButton("稍后提醒");
connect(laterBtn, &QPushButton::clicked, [actionBar]() {
    actionBar->hide();
});
actionBar->addWidget(laterBtn);
```

### 文件操作反馈

```cpp
// 文件保存成功
void saveFile() {
    // 执行保存逻辑...
    bool success = true; // 假设保存成功
    
    if (success) {
        NInfoBar::success(
            NInfoBarType::TopRight,
            "保存成功",
            "文件已保存到桌面",
            2000,
            this
        );
    } else {
        NInfoBar::error(
            NInfoBarType::TopRight,
            "保存失败",
            "磁盘空间不足或文件被占用",
            4000,
            this
        );
    }
}

// 网络操作反馈
void uploadFile() {
    NInfoBar::information(
        NInfoBarType::Top,
        "上传中",
        "正在上传文件，请稍候...",
        0,  // 不自动消失
        this,
        false  // 不显示关闭按钮
    );
    
    // 模拟异步上传
    QTimer::singleShot(3000, [this]() {
        NInfoBar::success(
            NInfoBarType::Top,
            "上传完成",
            "文件上传成功",
            3000,
            this
        );
    });
}
```

### 表单验证反馈

```cpp
void validateForm() {
    QStringList errors;
    
    // 验证用户名
    if (usernameEdit->text().isEmpty()) {
        errors << "用户名不能为空";
    }
    
    // 验证密码
    if (passwordEdit->text().length() < 6) {
        errors << "密码长度至少6位";
    }
    
    // 验证邮箱
    if (!emailEdit->text().contains("@")) {
        errors << "邮箱格式不正确";
    }
    
    if (!errors.isEmpty()) {
        NInfoBar::warning(
            NInfoBarType::Top,
            "表单验证失败",
            errors.join("；"),
            5000,
            this
        );
    } else {
        NInfoBar::success(
            NInfoBarType::Top,
            "验证通过",
            "表单信息填写正确",
            2000,
            this
        );
    }
}
```

### 系统状态通知

```cpp
// 网络状态监控
void onNetworkStatusChanged(bool connected) {
    if (connected) {
        NInfoBar::success(
            NInfoBarType::BottomRight,
            "网络已连接",
            "网络连接已恢复",
            3000,
            this
        );
    } else {
        NInfoBar::error(
            NInfoBarType::BottomRight,
            "网络断开",
            "请检查网络连接",
            0,  // 持续显示直到手动关闭
            this
        );
    }
}

// 电池状态提醒
void onBatteryLow() {
    NInfoBar* batteryBar = new NInfoBar(
        NInfoBarType::Warning,
        "电量不足",
        "电池电量低于20%，请及时充电",
        this,
        true
    );
    
    // 添加省电模式按钮
    NPushButton* powerSaveBtn = new NPushButton("启用省电模式");
    connect(powerSaveBtn, &QPushButton::clicked, []() {
        // 启用省电模式
        qDebug() << "已启用省电模式";
    });
    batteryBar->addWidget(powerSaveBtn);
}
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NInfoBar(NInfoBarType::InfoMode severity, const QString& title, const QString& message, QWidget* parent, bool showCloseButton)` | 创建信息栏 |

### 静态方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `information(NInfoBarType::PositionPolicy position, QString title, QString message, int displayDuration, QWidget* parent, bool showCloseButton)` | 显示信息提示 | 位置、标题、消息、显示时长、父控件、关闭按钮 |
| `success(NInfoBarType::PositionPolicy position, QString title, QString message, int displayDuration, QWidget* parent, bool showCloseButton)` | 显示成功提示 | 位置、标题、消息、显示时长、父控件、关闭按钮 |
| `warning(NInfoBarType::PositionPolicy position, QString title, QString message, int displayDuration, QWidget* parent, bool showCloseButton)` | 显示警告提示 | 位置、标题、消息、显示时长、父控件、关闭按钮 |
| `error(NInfoBarType::PositionPolicy position, QString title, QString message, int displayDuration, QWidget* parent, bool showCloseButton)` | 显示错误提示 | 位置、标题、消息、显示时长、父控件、关闭按钮 |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `addWidget(QWidget* widget)` | 添加自定义控件 | 控件指针 |

### 枚举

#### NInfoBarType::InfoMode

| 值 | 说明 | 颜色 |
|----|------|------|
| `Information` | 信息提示 | 蓝色系 |
| `Success` | 成功提示 | 绿色系 |
| `Warning` | 警告提示 | 黄色系 |
| `Error` | 错误提示 | 红色系 |

#### NInfoBarType::PositionPolicy

| 值 | 说明 |
|----|------|
| `Top` | 顶部居中 |
| `TopLeft` | 左上角 |
| `TopRight` | 右上角 |
| `Bottom` | 底部居中 |
| `BottomLeft` | 左下角 |
| `BottomRight` | 右下角 |
| `Left` | 左侧居中 |
| `Right` | 右侧居中 |

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 最小高度 | `48px` | 信息栏最小高度 |
| 圆角半径 | `6px` | 信息栏圆角 |
| 内边距 | `12px 16px` | 信息栏内边距 |
| 图标大小 | `20px` | 状态图标大小 |
| 动画时长 | `300ms` | 显示/隐藏动画时长 |

### 状态颜色

| 状态 | 明亮主题 | 暗黑主题 | 说明 |
|------|----------|----------|------|
| 信息 | `#0078D4` | `#4FC3F7` | 蓝色系 |
| 成功 | `#107C10` | `#4CAF50` | 绿色系 |
| 警告 | `#FF8C00` | `#FF9800` | 橙色系 |
| 错误 | `#D13438` | `#F44336` | 红色系 |

### 主题适配

```cpp
// 信息栏会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 信息栏颜色会自动更新
});
```