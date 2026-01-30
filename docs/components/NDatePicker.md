# NDatePicker 日期选择器

### 介绍

日期选择器用于选择日期，提供滚轮式的交互体验。支持年/月/日选择，具有完整的国际化支持，可自动适配不同语言的月份名称显示。

### 基本用法

创建一个基本的日期选择器，默认使用系统 locale。

```cpp
NDatePicker* datePicker = new NDatePicker();
datePicker->setDate(QDate::currentDate());

connect(datePicker, &NDatePicker::dateChanged, [](const QDate& date) {
    qDebug() << "选择的日期:" << date.toString("yyyy-MM-dd");
});
```

### 日期范围限制

通过 `setMinimumDate()` 和 `setMaximumDate()` 限制可选日期范围。

```cpp
NDatePicker* rangePicker = new NDatePicker();

// 只能选择未来 30 天内的日期
QDate today = QDate::currentDate();
rangePicker->setMinimumDate(today);
rangePicker->setMaximumDate(today.addDays(30));
rangePicker->setDate(today);
```

### 国际化支持

NDatePicker 完整支持国际化，月份名称会根据 locale 自动显示为对应语言。

```cpp
// 中文（简体）- 显示 "1月, 2月, 3月..."
NDatePicker* chinesePicker = new NDatePicker();
chinesePicker->setLocale(QLocale(QLocale::Chinese, QLocale::China));
chinesePicker->setDate(QDate::currentDate());

// 英文（美国）- 显示 "Jan, Feb, Mar..."
NDatePicker* englishPicker = new NDatePicker();
englishPicker->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
englishPicker->setDate(QDate::currentDate());

// 法语 - 显示 "janv., févr., mars..."
NDatePicker* frenchPicker = new NDatePicker();
frenchPicker->setLocale(QLocale(QLocale::French, QLocale::France));
frenchPicker->setDate(QDate::currentDate());

// 德语 - 显示 "Jan., Feb., März..."
NDatePicker* germanPicker = new NDatePicker();
germanPicker->setLocale(QLocale(QLocale::German, QLocale::Germany));
germanPicker->setDate(QDate::currentDate());

// 日语 - 显示 "1月, 2月, 3月..."
NDatePicker* japanesePicker = new NDatePicker();
japanesePicker->setLocale(QLocale(QLocale::Japanese, QLocale::Japan));
japanesePicker->setDate(QDate::currentDate());
```

### 设置和获取日期

使用 `setDate()` 和 `getDate()` 方法操作日期。

```cpp
NDatePicker* datePicker = new NDatePicker();

// 设置日期
datePicker->setDate(QDate(2024, 6, 15));

// 获取日期
QDate currentDate = datePicker->getDate();
qDebug() << "当前日期:" << currentDate.toString("yyyy-MM-dd");
```

### 圆角设置

通过 `setBorderRadius()` 设置按钮圆角。

```cpp
NDatePicker* datePicker = new NDatePicker();
datePicker->setBorderRadius(8);
```

## API

### 构造函数

| 构造函数                                 | 说明           |
| ---------------------------------------- | -------------- |
| `NDatePicker(QWidget* parent = nullptr)` | 创建日期选择器 |

### 属性

| 属性           | 类型      | 说明           | 默认值                |
| -------------- | --------- | -------------- | --------------------- |
| `date`         | `QDate`   | 当前选中的日期 | 当前系统日期          |
| `minimumDate`  | `QDate`   | 最小可选日期   | `QDate(1900, 1, 1)`   |
| `maximumDate`  | `QDate`   | 最大可选日期   | `QDate(9999, 12, 31)` |
| `locale`       | `QLocale` | 本地化设置     | 系统 locale           |
| `borderRadius` | `int`     | 按钮圆角半径   | `4`                   |

### 方法

| 方法                                | 说明             | 参数        |
| ----------------------------------- | ---------------- | ----------- |
| `setDate(const QDate& date)`        | 设置日期         | 日期对象    |
| `getDate()`                         | 获取当前日期     | -           |
| `setMinimumDate(const QDate& date)` | 设置最小可选日期 | 最小日期    |
| `getMinimumDate()`                  | 获取最小可选日期 | -           |
| `setMaximumDate(const QDate& date)` | 设置最大可选日期 | 最大日期    |
| `getMaximumDate()`                  | 获取最大可选日期 | -           |
| `setLocale(const QLocale& locale)`  | 设置本地化       | locale 对象 |
| `getLocale()`                       | 获取本地化       | -           |
| `setBorderRadius(int radius)`       | 设置圆角半径     | 半径像素值  |
| `getBorderRadius()`                 | 获取圆角半径     | -           |

### 信号

| 信号                                   | 说明                           | 参数        |
| -------------------------------------- | ------------------------------ | ----------- |
| `dateChanged(const QDate& date)`       | 日期改变时触发（点击确认按钮） | 新的日期值  |
| `localeChanged(const QLocale& locale)` | locale 改变时触发              | 新的 locale |

### 继承的 API

NDatePicker 继承自 `QPushButton`，支持所有标准 QPushButton 的方法和信号：

- `setEnabled()` / `isEnabled()` - 启用/禁用状态
- `setVisible()` / `isVisible()` - 显示/隐藏状态
- `setToolTip()` - 设置工具提示

## 交互说明

### 弹出选择器

点击按钮后，会弹出一个包含滚轮选择器的浮动窗口：

- **年份滚轮**：选择年份（根据 minimumDate 和 maximumDate 范围）
- **月份滚轮**：选择月份（显示为月份名称，根据 locale 自动本地化）
- **日期滚轮**：选择日期（1-31，根据当月天数自动调整）

### 操作方式

- **滚轮滚动**：使用鼠标滚轮滚动选择
- **点击跳转**：点击非中心项快速跳转
- **确认/取消**：点击底部的确认或取消按钮

### 确认和取消

- **确认按钮**（✓）：应用选择的日期并触发 `dateChanged` 信号
- **取消按钮**（✕）：放弃修改，恢复到打开前的日期
- **点击外部**：等同于取消操作

### 智能日期处理

- **月份天数自动调整**：切换月份时，如果当前选择的日期超过该月天数，会自动调整到该月最后一天
- **闰年处理**：2 月份会根据年份自动处理 28 天或 29 天
- **范围限制**：选择的日期会自动限制在 minimumDate 和 maximumDate 范围内

## 使用示例

### 完整示例

```cpp
#include <QtNativeUI/NDatePicker.h>
#include <QVBoxLayout>
#include <QLabel>

// 创建日期选择器
NDatePicker* datePicker = new NDatePicker();
datePicker->setDate(QDate::currentDate());

// 显示结果的标签
QLabel* resultLabel = new QLabel("选择的日期: " + datePicker->getDate().toString("yyyy-MM-dd"));

// 连接信号
connect(datePicker, &NDatePicker::dateChanged, [resultLabel](const QDate& date) {
    resultLabel->setText("选择的日期: " + date.toString("yyyy-MM-dd"));
});

// 添加到布局
QVBoxLayout* layout = new QVBoxLayout();
layout->addWidget(datePicker);
layout->addWidget(resultLabel);
```

### 表单中使用

```cpp
// 创建表单
QFormLayout* formLayout = new QFormLayout();

// 开始日期
NDatePicker* startDatePicker = new NDatePicker();
startDatePicker->setDate(QDate::currentDate());
formLayout->addRow("开始日期:", startDatePicker);

// 结束日期
NDatePicker* endDatePicker = new NDatePicker();
endDatePicker->setDate(QDate::currentDate().addDays(7));
formLayout->addRow("结束日期:", endDatePicker);

// 验证日期范围
connect(endDatePicker, &NDatePicker::dateChanged, [=](const QDate& endDate) {
    QDate startDate = startDatePicker->getDate();
    if (endDate <= startDate) {
        qWarning() << "结束日期必须晚于开始日期";
    }
});
```

### 生日选择器

```cpp
NDatePicker* birthdayPicker = new NDatePicker();

// 设置合理的生日范围（100 年前到今天）
QDate today = QDate::currentDate();
birthdayPicker->setMinimumDate(today.addYears(-100));
birthdayPicker->setMaximumDate(today);

// 设置默认日期为 30 年前
birthdayPicker->setDate(today.addYears(-30));

// 计算年龄
connect(birthdayPicker, &NDatePicker::dateChanged, [=](const QDate& birthday) {
    if (birthday.isValid()) {
        int age = birthday.daysTo(QDate::currentDate()) / 365;
        qDebug() << "年龄：" << age << "岁";
    }
});
```

### 多语言应用

```cpp
// 创建多语言日期选择器
QHBoxLayout* multiLangLayout = new QHBoxLayout();

// 中文
NDatePicker* cnPicker = new NDatePicker();
cnPicker->setLocale(QLocale(QLocale::Chinese, QLocale::China));
cnPicker->setDate(QDate::currentDate());
multiLangLayout->addWidget(new QLabel("中文:"));
multiLangLayout->addWidget(cnPicker);

// 英文
NDatePicker* enPicker = new NDatePicker();
enPicker->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
enPicker->setDate(QDate::currentDate());
multiLangLayout->addWidget(new QLabel("English:"));
multiLangLayout->addWidget(enPicker);

// 法语
NDatePicker* frPicker = new NDatePicker();
frPicker->setLocale(QLocale(QLocale::French, QLocale::France));
frPicker->setDate(QDate::currentDate());
multiLangLayout->addWidget(new QLabel("Français:"));
multiLangLayout->addWidget(frPicker);
```

### 动态切换 Locale

```cpp
NDatePicker* datePicker = new NDatePicker();
datePicker->setDate(QDate::currentDate());

// 创建语言切换按钮
QPushButton* switchButton = new QPushButton("切换到英文");

connect(switchButton, &QPushButton::clicked, [=]() {
    static bool isEnglish = false;
    if (isEnglish) {
        datePicker->setLocale(QLocale(QLocale::Chinese, QLocale::China));
        switchButton->setText("切换到英文");
    } else {
        datePicker->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        switchButton->setText("切换到中文");
    }
    isEnglish = !isEnglish;
});
```

## 主题定制

### 样式变量

组件会自动适配系统主题：

| 变量     | 默认值  | 说明                 |
| -------- | ------- | -------------------- |
| 按钮高度 | `32px`  | 日期选择器按钮高度   |
| 按钮宽度 | `240px` | 日期选择器按钮宽度   |
| 圆角半径 | `4px`   | 按钮圆角             |
| 滚轮高度 | `245px` | 弹出选择器的滚轮高度 |
| 可见项数 | `7`     | 滚轮可见项数量       |
| 项高度   | `35px`  | 滚轮每项的高度       |

### 主题适配

```cpp
// 日期选择器会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 组件颜色会自动更新
});
```

## 国际化说明

### 支持的语言

NDatePicker 通过 `QLocale` 支持所有 Qt 支持的语言，包括但不限于：

- **中文**（简体/繁体）：1月, 2月, 3月...
- **英语**（美国/英国）：Jan, Feb, Mar...
- **法语**：janv., févr., mars...
- **德语**：Jan., Feb., März...
- **西班牙语**：ene., feb., mar...
- **意大利语**：gen, feb, mar...
- **日语**：1月, 2月, 3月...
- **韩语**：1월, 2월, 3월...
- **俄语**：янв., февр., март...

### 自动宽度调整

月份滚轮的宽度会根据月份名称的实际长度自动调整：

- 使用 `QFontMetrics` 计算最长月份名称的宽度
- 自动添加适当的 padding
- 宽度范围：60px - 100px

### Locale 设置

```cpp
// 使用系统 locale（默认）
NDatePicker* systemPicker = new NDatePicker();

// 使用特定 locale
NDatePicker* customPicker = new NDatePicker();
customPicker->setLocale(QLocale(QLocale::French, QLocale::France));

// 获取当前 locale
QLocale currentLocale = customPicker->getLocale();
qDebug() << "当前语言:" << currentLocale.name();
```

## 注意事项

1. **日期格式**：使用 `QDate` 对象，确保日期有效性
2. **日期范围**：支持 1900-01-01 到 9999-12-31 的日期范围
3. **月份显示**：月份始终显示为名称（根据 locale），不显示数字
4. **日期顺序**：固定为 Year/Month/Day 顺序（ISO 8601 标准）
5. **信号触发**：只有点击确认按钮才会触发 `dateChanged` 信号
6. **取消操作**：点击取消或外部区域会恢复到打开前的日期
7. **闰年处理**：自动处理闰年的 2 月 29 日
8. **月份天数**：切换月份时自动调整日期到有效范围
