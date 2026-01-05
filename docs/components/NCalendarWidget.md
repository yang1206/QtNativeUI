# NCalendarWidget 日历控件

### 介绍

日历控件用于显示日期和支持日期选择。NCalendarWidget 提供了多种日期选择模式，包括单日期选择、多日期选择和日期范围选择，支持自定义日期范围和本地化显示。

### 基本用法

```cpp
// 创建日历控件
NCalendarWidget* calendar = new NCalendarWidget();

// 设置当前选中日期
calendar->setSelectedDate(QDate::currentDate());

// 监听日期选择
connect(calendar, &NCalendarWidget::clicked, [](const QDate& date) {
    qDebug() << "选择日期：" << date.toString("yyyy-MM-dd");
});
```

### 单日期选择

```cpp
// 创建日历并设置为单日期选择模式
NCalendarWidget* calendar = new NCalendarWidget();
calendar->setDateSelectionMode(NCalendarWidget::SingleDate);

// 设置初始日期
calendar->setSelectedDate(QDate(2025, 1, 15));

// 获取选中的日期
QDate selectedDate = calendar->selectedDate();
qDebug() << "选中日期：" << selectedDate.toString("yyyy-MM-dd");

// 监听日期变化
connect(calendar, &NCalendarWidget::clicked, [](const QDate& date) {
    qDebug() << "用户选择了：" << date.toString("yyyy-MM-dd");
});
```

### 多日期选择

```cpp
// 创建日历并设置为多日期选择模式
NCalendarWidget* calendar = new NCalendarWidget();
calendar->setDateSelectionMode(NCalendarWidget::MultipleDate);

// 设置多个选中日期
QList<QDate> dates = {
    QDate(2025, 1, 10),
    QDate(2025, 1, 15),
    QDate(2025, 1, 20),
    QDate(2025, 1, 25)
};
calendar->setSelectedDates(dates);

// 获取所有选中的日期
QList<QDate> selectedDates = calendar->selectedDates();
for (const QDate& date : selectedDates) {
    qDebug() << "选中日期：" << date.toString("yyyy-MM-dd");
}

// 监听多日期变化
connect(calendar, &NCalendarWidget::selectedDatesChanged, 
        [](const QList<QDate>& dates) {
    qDebug() << "选中了" << dates.size() << "个日期";
});
```

### 日期范围选择

```cpp
// 创建日历并设置为日期范围选择模式
NCalendarWidget* calendar = new NCalendarWidget();
calendar->setDateSelectionMode(NCalendarWidget::DateRange);

// 设置日期范围
QDate startDate(2025, 1, 10);
QDate endDate(2025, 1, 25);
calendar->setDateRange(startDate, endDate);

// 获取选中的日期范围
QPair<QDate, QDate> range = calendar->selectedDateRange();
qDebug() << "范围开始：" << range.first.toString("yyyy-MM-dd");
qDebug() << "范围结束：" << range.second.toString("yyyy-MM-dd");

// 监听范围变化
connect(calendar, &NCalendarWidget::selectedDateRangeChanged,
        [](const QPair<QDate, QDate>& range) {
    qDebug() << "选择范围：" 
             << range.first.toString("yyyy-MM-dd") 
             << "至" 
             << range.second.toString("yyyy-MM-dd");
});
```

### 日期范围限制

```cpp
// 创建日历
NCalendarWidget* calendar = new NCalendarWidget();

// 设置可选择的日期范围
QDate minDate(2025, 1, 1);
QDate maxDate(2025, 12, 31);
calendar->setMinimumDate(minDate);
calendar->setMaximumDate(maxDate);

// 只能选择2025年的日期
calendar->setSelectedDate(QDate(2025, 6, 15));
```

### 本地化显示

```cpp
// 创建日历
NCalendarWidget* calendar = new NCalendarWidget();

// 设置中文本地化
calendar->setLocale(QLocale(QLocale::Chinese, QLocale::China));

// 设置英文本地化
calendar->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

// 监听本地化变化
connect(calendar, &NCalendarWidget::localeChanged, [](const QLocale& locale) {
    qDebug() << "本地化已更改为：" << locale.name();
});
```

### 日期选择对话框

```cpp
// 创建包含日历的对话框
QDialog* dateDialog = new QDialog();
QVBoxLayout* layout = new QVBoxLayout(dateDialog);

NCalendarWidget* calendar = new NCalendarWidget();
layout->addWidget(calendar);

// 添加确认和取消按钮
QHBoxLayout* buttonLayout = new QHBoxLayout();
NPushButton* okButton = new NPushButton("确定");
NPushButton* cancelButton = new NPushButton("取消");

buttonLayout->addStretch();
buttonLayout->addWidget(okButton);
buttonLayout->addWidget(cancelButton);
layout->addLayout(buttonLayout);

// 处理按钮点击
connect(okButton, &NPushButton::clicked, [=]() {
    QDate selectedDate = calendar->selectedDate();
    qDebug() << "确认选择：" << selectedDate.toString("yyyy-MM-dd");
    dateDialog->accept();
});

connect(cancelButton, &NPushButton::clicked, dateDialog, &QDialog::reject);

dateDialog->exec();
```

### 日期范围选择器

```cpp
// 创建日期范围选择器
QWidget* rangeSelector = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(rangeSelector);

// 标签
QLabel* label = new QLabel("选择日期范围：");
layout->addWidget(label);

// 日历
NCalendarWidget* calendar = new NCalendarWidget();
calendar->setDateSelectionMode(NCalendarWidget::DateRange);
layout->addWidget(calendar);

// 显示选中范围
QLabel* rangeLabel = new QLabel();
layout->addWidget(rangeLabel);

// 更新范围显示
auto updateRangeDisplay = [=]() {
    QPair<QDate, QDate> range = calendar->selectedDateRange();
    QString rangeText = QString("选择范围：%1 至 %2")
        .arg(range.first.toString("yyyy-MM-dd"))
        .arg(range.second.toString("yyyy-MM-dd"));
    rangeLabel->setText(rangeText);
};

connect(calendar, &NCalendarWidget::selectedDateRangeChanged, updateRangeDisplay);
updateRangeDisplay();
```

### 自定义样式

```cpp
NCalendarWidget* calendar = new NCalendarWidget();

// 设置圆角
calendar->setBorderRadius(8);

// 自定义颜色（通过主题系统）
// 日历会自动响应主题变化
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NCalendarWidget(QWidget* parent = nullptr)` | 创建日历控件 |

### 日期选择

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setSelectedDate(const QDate& date)` | 设置单个选中日期 | 日期对象 |
| `selectedDate()` | 获取单个选中日期 | 返回 `QDate` |
| `setSelectedDates(const QList<QDate>& dates)` | 设置多个选中日期 | 日期列表 |
| `selectedDates()` | 获取多个选中日期 | 返回 `QList<QDate>` |
| `setDateRange(const QDate& start, const QDate& end)` | 设置日期范围 | 开始日期、结束日期 |
| `selectedDateRange()` | 获取日期范围 | 返回 `QPair<QDate, QDate>` |

### 日期范围限制

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setMinimumDate(const QDate& date)` | 设置最小可选日期 | 日期对象 |
| `minimumDate()` | 获取最小可选日期 | 返回 `QDate` |
| `setMaximumDate(const QDate& date)` | 设置最大可选日期 | 日期对象 |
| `maximumDate()` | 获取最大可选日期 | 返回 `QDate` |

### 选择模式

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setDateSelectionMode(DateSelectionMode mode)` | 设置日期选择模式 | `SingleDate`、`MultipleDate`、`DateRange` |
| `dateSelectionMode()` | 获取日期选择模式 | 返回 `DateSelectionMode` |

### 本地化

| 方法 | 说明 | 参数/返回值 |
|------|------|-----------|
| `setLocale(const QLocale& locale)` | 设置本地化 | `QLocale` 对象 |
| `locale()` | 获取本地化 | 返回 `QLocale` |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `borderRadius` | 边框圆角半径 | `int` |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `clicked(QDate date)` | 日期被点击 | 点击的日期 |
| `localeChanged(const QLocale& locale)` | 本地化已更改 | 新的本地化 |
| `dateSelectionModeChanged(DateSelectionMode mode)` | 选择模式已更改 | 新的选择模式 |
| `selectedDatesChanged(const QList<QDate>& dates)` | 多个选中日期已更改 | 新的日期列表 |
| `selectedDateRangeChanged(const QPair<QDate, QDate>& range)` | 日期范围已更改 | 新的日期范围 |

## 主题定制

日历控件会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
