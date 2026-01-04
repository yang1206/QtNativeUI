# NRadioButton 单选按钮

### 介绍

单选按钮用于在一组互斥选项中选择一个。NRadioButton 基于 Qt 的 QRadioButton，提供了现代化的圆形外观和流畅的选择动画。

### 基本用法

```cpp
// 创建单选按钮组
QWidget* radioGroup = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(radioGroup);

NRadioButton* option1 = new NRadioButton("选项1");
NRadioButton* option2 = new NRadioButton("选项2");
NRadioButton* option3 = new NRadioButton("选项3");

// 设置默认选中
option1->setChecked(true);

layout->addWidget(option1);
layout->addWidget(option2);
layout->addWidget(option3);

// 监听选择变化
connect(option1, &QRadioButton::toggled, [](bool checked) {
    if (checked) qDebug() << "选择了选项1";
});
```

### 使用 QButtonGroup 管理

推荐使用 QButtonGroup 来管理单选按钮组，确保互斥性。

```cpp
// 创建按钮组
QButtonGroup* buttonGroup = new QButtonGroup();

// 主题选择
NRadioButton* autoTheme = new NRadioButton("跟随系统");
NRadioButton* lightTheme = new NRadioButton("浅色主题");
NRadioButton* darkTheme = new NRadioButton("深色主题");

// 添加到按钮组
buttonGroup->addButton(autoTheme, 0);
buttonGroup->addButton(lightTheme, 1);
buttonGroup->addButton(darkTheme, 2);

// 设置默认选择
autoTheme->setChecked(true);

// 监听组选择变化
connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
        [](int id) {
    switch (id) {
        case 0:
            nTheme->setThemeMode(NThemeType::System);
            break;
        case 1:
            nTheme->setThemeMode(NThemeType::Light);
            break;
        case 2:
            nTheme->setThemeMode(NThemeType::Dark);
            break;
    }
});
```

### 水平布局

```cpp
// 创建水平单选按钮组
QWidget* horizontalGroup = new QWidget();
QHBoxLayout* hLayout = new QHBoxLayout(horizontalGroup);

QButtonGroup* sizeGroup = new QButtonGroup();

NRadioButton* smallSize = new NRadioButton("小");
NRadioButton* mediumSize = new NRadioButton("中");
NRadioButton* largeSize = new NRadioButton("大");

sizeGroup->addButton(smallSize, 0);
sizeGroup->addButton(mediumSize, 1);
sizeGroup->addButton(largeSize, 2);

mediumSize->setChecked(true); // 默认选中中等大小

hLayout->addWidget(smallSize);
hLayout->addWidget(mediumSize);
hLayout->addWidget(largeSize);
hLayout->addStretch();
```

### 设置页面示例

```cpp
// 创建设置页面
QWidget* settingsPage = new QWidget();
QVBoxLayout* mainLayout = new QVBoxLayout(settingsPage);

// 语言设置
NGroupBox* languageGroup = new NGroupBox("界面语言");
QVBoxLayout* langLayout = new QVBoxLayout(languageGroup);

QButtonGroup* langButtonGroup = new QButtonGroup();
NRadioButton* chineseRadio = new NRadioButton("简体中文");
NRadioButton* englishRadio = new NRadioButton("English");
NRadioButton* japaneseRadio = new NRadioButton("日本語");

langButtonGroup->addButton(chineseRadio, 0);
langButtonGroup->addButton(englishRadio, 1);
langButtonGroup->addButton(japaneseRadio, 2);

chineseRadio->setChecked(true);

langLayout->addWidget(chineseRadio);
langLayout->addWidget(englishRadio);
langLayout->addWidget(japaneseRadio);

// 启动设置
NGroupBox* startupGroup = new NGroupBox("启动选项");
QVBoxLayout* startupLayout = new QVBoxLayout(startupGroup);

QButtonGroup* startupButtonGroup = new QButtonGroup();
NRadioButton* normalStart = new NRadioButton("正常启动");
NRadioButton* minimizedStart = new NRadioButton("最小化启动");
NRadioButton* trayStart = new NRadioButton("启动到系统托盘");

startupButtonGroup->addButton(normalStart, 0);
startupButtonGroup->addButton(minimizedStart, 1);
startupButtonGroup->addButton(trayStart, 2);

normalStart->setChecked(true);

startupLayout->addWidget(normalStart);
startupLayout->addWidget(minimizedStart);
startupLayout->addWidget(trayStart);

mainLayout->addWidget(languageGroup);
mainLayout->addWidget(startupGroup);
mainLayout->addStretch();
```

### 禁用状态

```cpp
NRadioButton* disabledRadio = new NRadioButton("禁用选项");
disabledRadio->setEnabled(false);
disabledRadio->setChecked(true); // 可以设置为选中但禁用状态
```

### 自定义样式

```cpp
NRadioButton* styledRadio = new NRadioButton("自定义样式");

// 设置圆圈大小
styledRadio->setOuterCircleSize(20);
styledRadio->setInnerCircleSize(10);

// 设置边框宽度
styledRadio->setOuterCircleBorderWidth(2);

// 设置文本间距
styledRadio->setTextPadding(12);

// 自定义颜色
styledRadio->setLightOuterCircleDefaultColor(QColor(255, 255, 255));
styledRadio->setLightOuterCircleCheckedColor(QColor(0, 120, 215));
styledRadio->setLightOuterCircleBorderColor(QColor(200, 200, 200));
styledRadio->setLightInnerCircleColor(QColor(255, 255, 255));
styledRadio->setLightTextColor(QColor(32, 32, 32));

// 禁用状态颜色
styledRadio->setLightDisabledColor(QColor(150, 150, 150));
```

### 表单验证

```cpp
// 创建必选的单选按钮组
QWidget* requiredGroup = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(requiredGroup);

NLabel* questionLabel = new NLabel("请选择您的性别：", NLabelType::Body);
layout->addWidget(questionLabel);

QButtonGroup* genderGroup = new QButtonGroup();
NRadioButton* maleRadio = new NRadioButton("男");
NRadioButton* femaleRadio = new NRadioButton("女");
NRadioButton* otherRadio = new NRadioButton("其他");

genderGroup->addButton(maleRadio, 0);
genderGroup->addButton(femaleRadio, 1);
genderGroup->addButton(otherRadio, 2);

layout->addWidget(maleRadio);
layout->addWidget(femaleRadio);
layout->addWidget(otherRadio);

// 验证函数
auto validateGender = [genderGroup]() -> bool {
    return genderGroup->checkedButton() != nullptr;
};

// 提交按钮
NPushButton* submitBtn = new NPushButton("提交");
connect(submitBtn, &QPushButton::clicked, [validateGender]() {
    if (!validateGender()) {
        NInfoBar::warning(
            NInfoBarType::Top,
            "验证失败",
            "请选择您的性别",
            3000
        );
    } else {
        NInfoBar::success(
            NInfoBarType::Top,
            "提交成功",
            "信息已保存",
            2000
        );
    }
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NRadioButton(QWidget* parent = nullptr)` | 创建无标签单选按钮 |
| `NRadioButton(const QString& text, QWidget* parent = nullptr)` | 创建带标签单选按钮 |

### 样式属性

| 方法 | 说明 | 参数 |
|------|------|------|
| `setOuterCircleSize(int size)` | 设置外圆大小 | 大小像素值 |
| `outerCircleSize()` | 获取外圆大小 | - |
| `setOuterCircleBorderWidth(int width)` | 设置外圆边框宽度 | 宽度像素值 |
| `outerCircleBorderWidth()` | 获取外圆边框宽度 | - |
| `setInnerCircleSize(int size)` | 设置内圆大小 | 大小像素值 |
| `innerCircleSize()` | 获取内圆大小 | - |
| `setTextPadding(int padding)` | 设置文本间距 | 间距像素值 |
| `textPadding()` | 获取文本间距 | - |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightOuterCircleDefaultColor` | 明亮主题外圆默认色 | `QColor` |
| `darkOuterCircleDefaultColor` | 暗黑主题外圆默认色 | `QColor` |
| `lightOuterCircleCheckedColor` | 明亮主题外圆选中色 | `QColor` |
| `darkOuterCircleCheckedColor` | 暗黑主题外圆选中色 | `QColor` |
| `lightOuterCircleBorderColor` | 明亮主题外圆边框色 | `QColor` |
| `darkOuterCircleBorderColor` | 暗黑主题外圆边框色 | `QColor` |
| `lightInnerCircleColor` | 明亮主题内圆色 | `QColor` |
| `darkInnerCircleColor` | 暗黑主题内圆色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightDisabledColor` | 明亮主题禁用色 | `QColor` |
| `darkDisabledColor` | 暗黑主题禁用色 | `QColor` |

### 继承的 API

NRadioButton 继承自 `QRadioButton`，支持所有标准 QRadioButton 的方法和信号：

- `setText()` / `text()` - 设置/获取按钮文本
- `setChecked()` / `isChecked()` - 设置/获取选中状态
- `setAutoExclusive()` / `autoExclusive()` - 设置/获取自动互斥
- `clicked()` - 点击信号
- `pressed()` / `released()` - 按下/释放信号
- `toggled(bool)` - 切换信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 外圆大小 | `16px` | 单选按钮外圆直径 |
| 内圆大小 | `8px` | 选中状态内圆直径 |
| 边框宽度 | `2px` | 外圆边框宽度 |
| 文本间距 | `8px` | 按钮与文本间距 |
| 动画时长 | `150ms` | 选中状态切换动画时长 |

### 主题适配

```cpp
// 单选按钮会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 单选按钮颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 选中状态的强调色会自动更新
});
```