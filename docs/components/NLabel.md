# NLabel 标签

### 介绍

标签用于显示文本内容，支持 Fluent Design 的排版系统。NLabel 基于 Qt 的 QLabel，提供了预定义的文本样式类型，可以轻松实现一致的排版效果。

### 文本类型

NLabel 提供了多种预定义的文本类型，每种类型都有对应的字体大小和字重。

```cpp
// 说明文本 (12px 常规)
NLabel* captionLabel = new NLabel("说明文本", NLabelType::Caption);

// 正文文本 (14px 常规)
NLabel* bodyLabel = new NLabel("这是正文内容", NLabelType::Body);

// 强调正文 (14px 半粗体)
NLabel* bodyStrongLabel = new NLabel("重要内容", NLabelType::BodyStrong);

// 大号正文 (16px 常规)
NLabel* bodyLargeLabel = new NLabel("大号正文", NLabelType::BodyLarge);

// 副标题 (20px 半粗体)
NLabel* subtitleLabel = new NLabel("副标题", NLabelType::Subtitle);

// 标题 (28px 半粗体)
NLabel* titleLabel = new NLabel("页面标题", NLabelType::Title);

// 大号标题 (40px 半粗体)
NLabel* titleLargeLabel = new NLabel("大标题", NLabelType::TitleLarge);

// 展示型标题 (68px 半粗体)
NLabel* displayLabel = new NLabel("DISPLAY", NLabelType::Display);
```

### 动态类型切换

```cpp
NLabel* dynamicLabel = new NLabel("动态文本");

// 运行时切换类型
dynamicLabel->setType(NLabelType::Title);

// 监听类型变化
connect(dynamicLabel, &NLabel::typeChanged, []() {
    qDebug() << "标签类型已改变";
});
```

### 自定义颜色

支持为明暗主题分别设置文本颜色。

```cpp
NLabel* colorLabel = new NLabel("自定义颜色文本", NLabelType::Body);

// 设置明亮主题文本色
colorLabel->setLightTextColor(QColor(0, 120, 215));

// 设置暗黑主题文本色
colorLabel->setDarkTextColor(QColor(100, 180, 255));

// 设置禁用状态颜色
colorLabel->setLightTextDisabledColor(QColor(150, 150, 150));
colorLabel->setDarkTextDisabledColor(QColor(100, 100, 100));
```

### 排版示例

```cpp
// 创建典型的内容页面排版
QWidget* contentWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(contentWidget);

// 页面标题
NLabel* pageTitle = new NLabel("设置", NLabelType::Title);
layout->addWidget(pageTitle);

// 分组标题
NLabel* groupTitle = new NLabel("外观", NLabelType::Subtitle);
layout->addWidget(groupTitle);

// 选项说明
NLabel* optionDesc = new NLabel("选择应用程序的主题模式", NLabelType::Body);
layout->addWidget(optionDesc);

// 详细说明
NLabel* detailDesc = new NLabel("深色主题可以减少眼部疲劳", NLabelType::Caption);
detailDesc->setLightTextColor(QColor(100, 100, 100));
layout->addWidget(detailDesc);
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NLabel(QWidget* parent = nullptr)` | 创建空标签 |
| `NLabel(const QString& text, QWidget* parent = nullptr)` | 创建带文本的标签 |
| `NLabel(const QString& text, NLabelType::Type type, QWidget* parent = nullptr)` | 创建指定类型的标签 |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `setType(NLabelType::Type type)` | 设置标签类型 | 标签类型枚举 |
| `type()` | 获取当前标签类型 | - |

### 信号

| 信号 | 说明 |
|------|------|
| `typeChanged()` | 标签类型改变时触发 |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightTextDisabledColor` | 明亮主题禁用文本色 | `QColor` |
| `darkTextDisabledColor` | 暗黑主题禁用文本色 | `QColor` |

### 枚举

#### NLabelType::Type

| 值 | 说明 | 字体规格 |
|----|------|----------|
| `Caption` | 说明文本 | 12px 常规 |
| `Body` | 正文文本 | 14px 常规 |
| `BodyStrong` | 强调正文 | 14px 半粗体 |
| `BodyLarge` | 大号正文 | 16px 常规 |
| `BodyStrongLarge` | 大号强调正文 | 16px 半粗体 |
| `Subtitle` | 副标题 | 20px 半粗体 |
| `Title` | 标题 | 28px 半粗体 |
| `TitleLarge` | 大号标题 | 40px 半粗体 |
| `Display` | 展示型标题 | 68px 半粗体 |
| `Default` | 默认样式 | 使用系统默认 |

### 继承的 API

NLabel 继承自 `QLabel`，支持所有标准 QLabel 的方法和属性：

- `setText()` / `text()` - 设置/获取文本内容
- `setAlignment()` / `alignment()` - 设置/获取文本对齐方式
- `setWordWrap()` / `wordWrap()` - 设置/获取自动换行
- `setPixmap()` / `pixmap()` - 设置/获取图片内容
- `setOpenExternalLinks()` - 设置是否打开外部链接
- `linkActivated()` - 链接激活信号
- `linkHovered()` - 链接悬停信号

## 主题定制

### 排版规范

| 类型 | 字体大小 | 字重 | 行高 | 使用场景 |
|------|----------|------|------|----------|
| Display | 68px | 半粗体 | 1.2 | 大型展示标题 |
| TitleLarge | 40px | 半粗体 | 1.3 | 页面主标题 |
| Title | 28px | 半粗体 | 1.3 | 区域标题 |
| Subtitle | 20px | 半粗体 | 1.4 | 分组标题 |
| BodyLarge | 16px | 常规 | 1.5 | 重要正文 |
| BodyStrongLarge | 16px | 半粗体 | 1.5 | 强调正文 |
| Body | 14px | 常规 | 1.5 | 标准正文 |
| BodyStrong | 14px | 半粗体 | 1.5 | 强调内容 |
| Caption | 12px | 常规 | 1.4 | 辅助说明 |

### 主题适配

```cpp
// 标签会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 文本颜色会自动更新
});
```