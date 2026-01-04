# NAutoSuggestBox 自动建议框

### 介绍

自动建议框提供输入时的智能提示功能，支持多种过滤模式和自定义建议项。NAutoSuggestBox 适用于搜索框、地址输入、标签选择等场景。

### 基本用法

```cpp
// 创建自动建议框
NAutoSuggestBox* suggestBox = new NAutoSuggestBox();
suggestBox->setPlaceholderText("输入搜索内容...");

// 添加建议项
suggestBox->addSuggestion("苹果");
suggestBox->addSuggestion("香蕉");
suggestBox->addSuggestion("橙子");
suggestBox->addSuggestion("葡萄");

// 监听建议项点击
connect(suggestBox, &NAutoSuggestBox::suggestionClicked, 
        [](const QString& text, const QVariantMap& data) {
    qDebug() << "选择了：" << text;
});

// 监听查询提交（回车键）
connect(suggestBox, &NAutoSuggestBox::querySubmitted, [](const QString& query) {
    qDebug() << "搜索：" << query;
});
```

### 带图标的建议项

```cpp
NAutoSuggestBox* iconSuggestBox = new NAutoSuggestBox();
iconSuggestBox->setPlaceholderText("搜索文件...");

// 添加带图标的建议项
iconSuggestBox->addSuggestion(NRegularIconType::Document24Regular, "文档.docx");
iconSuggestBox->addSuggestion(NRegularIconType::Image24Regular, "图片.jpg");
iconSuggestBox->addSuggestion(NRegularIconType::Video24Regular, "视频.mp4");
iconSuggestBox->addSuggestion(NFilledIconType::Folder24Filled, "我的文件夹");
```

### 过滤模式设置

```cpp
NAutoSuggestBox* filterBox = new NAutoSuggestBox();

// 设置不同的过滤模式
filterBox->setFilterMode(NAutoSuggestBox::StartsWith);  // 以输入开头
// filterBox->setFilterMode(NAutoSuggestBox::Contains);    // 包含输入（默认）
// filterBox->setFilterMode(NAutoSuggestBox::EndsWith);    // 以输入结尾
// filterBox->setFilterMode(NAutoSuggestBox::Equals);      // 完全匹配

// 设置大小写敏感性
filterBox->setCaseSensitivity(Qt::CaseInsensitive);  // 不区分大小写
```

### 带数据的建议项

```cpp
NAutoSuggestBox* dataSuggestBox = new NAutoSuggestBox();
dataSuggestBox->setPlaceholderText("搜索用户...");

// 添加带自定义数据的建议项
QVariantMap userData1;
userData1["id"] = 1001;
userData1["email"] = "zhang@example.com";
userData1["department"] = "技术部";
dataSuggestBox->addSuggestion(NRegularIconType::Person24Regular, "张三", userData1);

QVariantMap userData2;
userData2["id"] = 1002;
userData2["email"] = "li@example.com";
userData2["department"] = "市场部";
dataSuggestBox->addSuggestion(NRegularIconType::Person24Regular, "李四", userData2);

// 处理选择事件
connect(dataSuggestBox, &NAutoSuggestBox::suggestionClicked,
        [](const QString& text, const QVariantMap& data) {
    qDebug() << "选择用户：" << text;
    qDebug() << "用户ID：" << data["id"].toInt();
    qDebug() << "邮箱：" << data["email"].toString();
    qDebug() << "部门：" << data["department"].toString();
});
```

### 搜索引擎示例

```cpp
// 创建搜索引擎建议框
NAutoSuggestBox* searchBox = new NAutoSuggestBox();
searchBox->setPlaceholderText("搜索或输入网址");

// 添加搜索建议
QStringList searchSuggestions = {
    "Qt开发教程",
    "C++编程指南",
    "JavaScript基础",
    "Python数据分析",
    "机器学习入门",
    "Web前端开发",
    "数据库设计",
    "算法与数据结构"
};

for (const QString& suggestion : searchSuggestions) {
    QVariantMap data;
    data["type"] = "search";
    data["url"] = QString("https://search.example.com?q=%1").arg(suggestion);
    searchBox->addSuggestion(NRegularIconType::Search24Regular, suggestion, data);
}

// 添加历史记录
QStringList historyItems = {
    "https://www.qt.io",
    "https://github.com",
    "https://stackoverflow.com"
};

for (const QString& url : historyItems) {
    QVariantMap data;
    data["type"] = "history";
    data["url"] = url;
    searchBox->addSuggestion(NRegularIconType::History24Regular, url, data);
}

// 处理搜索
connect(searchBox, &NAutoSuggestBox::suggestionClicked,
        [](const QString& text, const QVariantMap& data) {
    QString type = data["type"].toString();
    QString url = data["url"].toString();
    
    if (type == "search") {
        qDebug() << "执行搜索：" << text;
        // 打开搜索结果页面
    } else if (type == "history") {
        qDebug() << "访问历史页面：" << url;
        // 打开历史页面
    }
});

connect(searchBox, &NAutoSuggestBox::querySubmitted, [](const QString& query) {
    qDebug() << "直接搜索：" << query;
    // 执行搜索
});
```

### 地址输入示例

```cpp
// 创建地址输入框
NAutoSuggestBox* addressBox = new NAutoSuggestBox();
addressBox->setPlaceholderText("输入地址...");

// 添加常用地址
QStringList addresses = {
    "北京市朝阳区",
    "上海市浦东新区",
    "广州市天河区",
    "深圳市南山区",
    "杭州市西湖区",
    "成都市高新区"
};

for (const QString& address : addresses) {
    QVariantMap data;
    data["type"] = "city";
    data["coordinates"] = QString("lat,lng"); // 实际应用中存储真实坐标
    addressBox->addSuggestion(NRegularIconType::Location24Regular, address, data);
}

// 处理地址选择
connect(addressBox, &NAutoSuggestBox::suggestionClicked,
        [](const QString& text, const QVariantMap& data) {
    qDebug() << "选择地址：" << text;
    // 可以在这里调用地图API获取详细信息
});
```

### 标签输入示例

```cpp
// 创建标签输入框
NAutoSuggestBox* tagBox = new NAutoSuggestBox();
tagBox->setPlaceholderText("添加标签...");

// 预定义标签
QStringList predefinedTags = {
    "重要", "紧急", "工作", "个人", "学习",
    "项目", "会议", "待办", "已完成", "进行中"
};

for (const QString& tag : predefinedTags) {
    QVariantMap data;
    data["color"] = QColor::fromHsv(qrand() % 360, 100, 200); // 随机颜色
    tagBox->addSuggestion(NRegularIconType::Tag24Regular, tag, data);
}

// 处理标签选择
QStringList selectedTags;
connect(tagBox, &NAutoSuggestBox::suggestionClicked,
        [&selectedTags, tagBox](const QString& text, const QVariantMap& data) {
    if (!selectedTags.contains(text)) {
        selectedTags.append(text);
        qDebug() << "添加标签：" << text;
        qDebug() << "当前标签：" << selectedTags;
        
        // 清空输入框
        tagBox->clear();
    }
});
```

### 动态建议更新

```cpp
NAutoSuggestBox* dynamicBox = new NAutoSuggestBox();
dynamicBox->setPlaceholderText("搜索产品...");

// 模拟从服务器获取建议
QTimer* searchTimer = new QTimer();
searchTimer->setSingleShot(true);
searchTimer->setInterval(300); // 300ms延迟

connect(dynamicBox, &NAutoSuggestBox::querySubmitted, [=](const QString& query) {
    searchTimer->stop();
    searchTimer->start();
});

connect(searchTimer, &QTimer::timeout, [=]() {
    QString query = dynamicBox->text();
    if (query.length() >= 2) { // 至少2个字符才搜索
        // 清空现有建议
        dynamicBox->clearSuggestions();
        
        // 模拟API调用
        QStringList results = simulateApiSearch(query);
        for (const QString& result : results) {
            QVariantMap data;
            data["score"] = qrand() % 100; // 相关度评分
            dynamicBox->addSuggestion(NRegularIconType::Box24Regular, result, data);
        }
    }
});
```

### 自定义样式

```cpp
NAutoSuggestBox* styledBox = new NAutoSuggestBox();

// 设置边框圆角
styledBox->setBorderRadius(8);

// 设置大小写敏感性
styledBox->setCaseSensitivity(Qt::CaseInsensitive);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NAutoSuggestBox(QWidget* parent = nullptr)` | 创建自动建议框 |

### 文本操作

| 方法 | 说明 | 参数 |
|------|------|------|
| `setText(const QString& text)` | 设置输入文本 | 文本内容 |
| `text()` | 获取输入文本 | - |
| `clear()` | 清空输入文本 | - |
| `setPlaceholderText(const QString& text)` | 设置占位符文本 | 占位符文本 |
| `placeholderText()` | 获取占位符文本 | - |
| `setFocus()` | 设置焦点到输入框 | - |

### 建议项管理

| 方法 | 说明 | 参数 |
|------|------|------|
| `addSuggestion(const QString& text, const QVariantMap& data)` | 添加文本建议项 | 文本、自定义数据 |
| `addSuggestion(NRegularIconType::Icon icon, const QString& text, const QVariantMap& data)` | 添加带 Regular 图标的建议项 | 图标、文本、数据 |
| `addSuggestion(NFilledIconType::Icon icon, const QString& text, const QVariantMap& data)` | 添加带 Filled 图标的建议项 | 图标、文本、数据 |
| `removeSuggestion(const QString& key)` | 根据键值移除建议项 | 键值 |
| `removeSuggestion(int index)` | 根据索引移除建议项 | 索引 |
| `clearSuggestions()` | 清空所有建议项 | - |

### 过滤设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setFilterMode(FilterMode mode)` | 设置过滤模式 | 过滤模式枚举 |
| `filterMode()` | 获取过滤模式 | - |
| `setCaseSensitivity(Qt::CaseSensitivity sensitivity)` | 设置大小写敏感性 | 敏感性枚举 |
| `caseSensitivity()` | 获取大小写敏感性 | - |

### 样式设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `suggestionClicked(QString text, QVariantMap data)` | 建议项被点击时触发 | 建议文本、自定义数据 |
| `querySubmitted(const QString& query)` | 查询提交时触发（回车键） | 查询文本 |

### 枚举

#### FilterMode

| 值 | 说明 |
|----|------|
| `Contains` | 包含搜索文本（默认） |
| `StartsWith` | 以搜索文本开头 |
| `EndsWith` | 以搜索文本结尾 |
| `Equals` | 完全匹配 |

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认高度 | `32px` | 输入框高度 |
| 边框圆角 | `4px` | 边框圆角半径 |
| 建议项高度 | `36px` | 建议项标准高度 |
| 最大建议数 | `8` | 最多显示的建议项数量 |
| 弹出延迟 | `150ms` | 建议列表弹出延迟 |

### 主题适配

```cpp
// 自动建议框会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 建议框颜色会自动更新
});
```