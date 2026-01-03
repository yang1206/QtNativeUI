# NCalendarDatePicker 日期选择器

### 介绍

日期选择器用于选择日期，提供了下拉日历界面。NCalendarDatePicker 支持单选、多选和范围选择模式，具有本地化支持和灵活的日期格式设置。

### 基本用法

```cpp
// 创建日期选择器
NCalendarDatePicker* datePicker = new NCalendarDatePicker();
datePicker->setSelectedDate(QDate::currentDate());
datePicker->setPlaceholderText("请选择日期");

// 监听日期选择
connect(datePicker, &NCalendarDatePicker::dateSelected, [](const QDate& date) {
    qDebug() << "选择的日期：" << date.toString("yyyy-MM-dd");
});
```

### 日期格式设置

```cpp
NCalendarDatePicker* formatPicker = new NCalendarDatePicker();

// 设置不同的日期格式
formatPicker->setDateFormat("yyyy年MM月dd日");  // 中文格式
// formatPicker->setDateFormat("MM/dd/yyyy");     // 美式格式
// formatPicker->setDateFormat("dd.MM.yyyy");     // 欧式格式
// formatPicker->setDateFormat("yyyy-MM-dd");     // ISO格式

formatPicker->setSelectedDate(QDate::currentDate());
```

### 日期范围限制

```cpp
NCalendarDatePicker* rangePicker = new NCalendarDatePicker();

// 设置可选日期范围（只能选择未来30天内的日期）
QDate today = QDate::currentDate();
rangePicker->setMinimumDate(today);
rangePicker->setMaximumDate(today.addDays(30));

rangePicker->setPlaceholderText("选择未来30天内的日期");
```

### 多选模式

```cpp
NCalendarDatePicker* multiPicker = new NCalendarDatePicker();
multiPicker->setDateSelectionMode(NCalendarWidget::MultiSelection);
multiPicker->setPlaceholderText("选择多个日期");

// 监听多选变化
connect(multiPicker, &NCalendarDatePicker::selectedDatesChanged, 
        [](const QList<QDate>& dates) {
    qDebug() << "选择的日期数量：" << dates.size();
    for (const QDate& date : dates) {
        qDebug() << "  " << date.toString("yyyy-MM-dd");
    }
});

// 预设选择的日期
QList<QDate> preselectedDates;
preselectedDates << QDate::currentDate();
preselectedDates << QDate::currentDate().addDays(1);
preselectedDates << QDate::currentDate().addDays(7);
multiPicker->setSelectedDates(preselectedDates);
```

### 范围选择模式

```cpp
NCalendarDatePicker* rangePicker = new NCalendarDatePicker();
rangePicker->setDateSelectionMode(NCalendarWidget::RangeSelection);
rangePicker->setPlaceholderText("选择日期范围");

// 监听范围选择变化
connect(rangePicker, &NCalendarDatePicker::selectedDateRangeChanged,
        [](const QPair<QDate, QDate>& range) {
    if (range.first.isValid() && range.second.isValid()) {
        qDebug() << "选择的日期范围：" 
                 << range.first.toString("yyyy-MM-dd") 
                 << " 到 " 
                 << range.second.toString("yyyy-MM-dd");
        
        int days = range.first.daysTo(range.second) + 1;
        qDebug() << "总天数：" << days;
    }
});

// 预设日期范围
QDate startDate = QDate::currentDate();
QDate endDate = startDate.addDays(6);
rangePicker->setDateRange(startDate, endDate);
```

### 本地化设置

```cpp
// 中文本地化
NCalendarDatePicker* chinesePicker = new NCalendarDatePicker();
chinesePicker->setLocale(QLocale(QLocale::Chinese, QLocale::China));
chinesePicker->setDateFormat("yyyy年MM月dd日");

// 英文本地化
NCalendarDatePicker* englishPicker = new NCalendarDatePicker();
englishPicker->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
englishPicker->setDateFormat("MM/dd/yyyy");

// 德文本地化
NCalendarDatePicker* germanPicker = new NCalendarDatePicker();
germanPicker->setLocale(QLocale(QLocale::German, QLocale::Germany));
germanPicker->setDateFormat("dd.MM.yyyy");
```

### 表单应用示例

```cpp
// 创建预订表单
QWidget* bookingForm = new QWidget();
QFormLayout* formLayout = new QFormLayout(bookingForm);

// 入住日期
NCalendarDatePicker* checkinPicker = new NCalendarDatePicker();
checkinPicker->setMinimumDate(QDate::currentDate());
checkinPicker->setPlaceholderText("选择入住日期");

// 退房日期
NCalendarDatePicker* checkoutPicker = new NCalendarDatePicker();
checkoutPicker->setMinimumDate(QDate::currentDate().addDays(1));
checkoutPicker->setPlaceholderText("选择退房日期");

formLayout->addRow("入住日期:", checkinPicker);
formLayout->addRow("退房日期:", checkoutPicker);

// 联动逻辑：入住日期变化时更新退房日期的最小值
connect(checkinPicker, &NCalendarDatePicker::dateSelected, [=](const QDate& date) {
    checkoutPicker->setMinimumDate(date.addDays(1));
    
    // 如果退房日期早于入住日期，清空退房日期
    if (checkoutPicker->selectedDate() <= date) {
        checkoutPicker->setSelectedDate(QDate());
    }
});

// 计算住宿天数
connect(checkoutPicker, &NCalendarDatePicker::dateSelected, [=](const QDate& date) {
    QDate checkinDate = checkinPicker->selectedDate();
    if (checkinDate.isValid() && date.isValid()) {
        int nights = checkinDate.daysTo(date);
        qDebug() << "住宿天数：" << nights << "晚";
    }
});
```

### 生日选择器

```cpp
NCalendarDatePicker* birthdayPicker = new NCalendarDatePicker();

// 设置合理的生日范围（100年前到今天）
QDate today = QDate::currentDate();
birthdayPicker->setMinimumDate(today.addYears(-100));
birthdayPicker->setMaximumDate(today);

// 设置默认日期为30年前
birthdayPicker->setSelectedDate(today.addYears(-30));
birthdayPicker->setPlaceholderText("选择出生日期");
birthdayPicker->setDateFormat("yyyy年MM月dd日");

// 计算年龄
connect(birthdayPicker, &NCalendarDatePicker::dateSelected, [=](const QDate& birthday) {
    if (birthday.isValid()) {
        int age = birthday.daysTo(QDate::currentDate()) / 365;
        qDebug() << "年龄：" << age << "岁";
    }
});
```

### 工作日选择器

```cpp
// 创建只能选择工作日的日期选择器
NCalendarDatePicker* workdayPicker = new NCalendarDatePicker();
workdayPicker->setPlaceholderText("选择工作日");

// 监听弹出窗口打开事件，设置工作日限制
connect(workdayPicker, &NCalendarDatePicker::popupOpened, [=]() {
    // 这里可以通过访问内部日历控件来设置工作日限制
    // 具体实现取决于NCalendarWidget的API
});

// 验证选择的日期是否为工作日
connect(workdayPicker, &NCalendarDatePicker::dateSelected, [=](const QDate& date) {
    if (date.dayOfWeek() >= 6) { // 周六或周日
        NInfoBar::warning(
            NInfoBarType::Top,
            "提示",
            "请选择工作日（周一至周五）",
            3000,
            workdayPicker
        );
        workdayPicker->setSelectedDate(QDate()); // 清空选择
    }
});
```

### 自定义样式

```cpp
NCalendarDatePicker* styledPicker = new NCalendarDatePicker();

// 设置边框圆角
styledPicker->setBorderRadius(8);

// 设置占位符文本
styledPicker->setPlaceholderText("点击选择日期");

// 设置日期格式
styledPicker->setDateFormat("yyyy/MM/dd");
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NCalendarDatePicker(QWidget* parent = nullptr)` | 创建日期选择器 |

### 日期设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setSelectedDate(const QDate& date)` | 设置选中日期 | 日期对象 |
| `selectedDate()` | 获取选中日期 | - |
| `setMinimumDate(const QDate& date)` | 设置最小日期 | 最小日期 |
| `minimumDate()` | 获取最小日期 | - |
| `setMaximumDate(const QDate& date)` | 设置最大日期 | 最大日期 |
| `maximumDate()` | 获取最大日期 | - |

### 多选支持

| 方法 | 说明 | 参数 |
|------|------|------|
| `setDateSelectionMode(NCalendarWidget::DateSelectionMode mode)` | 设置选择模式 | 选择模式枚举 |
| `dateSelectionMode()` | 获取选择模式 | - |
| `setSelectedDates(const QList<QDate>& dates)` | 设置多选日期 | 日期列表 |
| `selectedDates()` | 获取多选日期 | - |
| `setDateRange(const QDate& startDate, const QDate& endDate)` | 设置日期范围 | 开始日期、结束日期 |
| `selectedDateRange()` | 获取选择的日期范围 | - |

### 格式和本地化

| 方法 | 说明 | 参数 |
|------|------|------|
| `setDateFormat(const QString& format)` | 设置日期格式 | 格式字符串 |
| `dateFormat()` | 获取日期格式 | - |
| `setLocale(const QLocale& locale)` | 设置本地化 | 本地化对象 |
| `locale()` | 获取本地化 | - |
| `setPlaceholderText(const QString& text)` | 设置占位符文本 | 占位符文本 |
| `placeholderText()` | 获取占位符文本 | - |

### 样式设置

| 方法 | 说明 | 参数 |
|------|------|------|
| `setBorderRadius(int radius)` | 设置边框圆角 | 圆角半径 |
| `borderRadius()` | 获取边框圆角 | - |

### 信号

| 信号 | 说明 | 参数 |
|------|------|------|
| `dateSelected(QDate date)` | 日期被选择时触发 | 选择的日期 |
| `dateSelectionModeChanged(NCalendarWidget::DateSelectionMode mode)` | 选择模式改变时触发 | 新的选择模式 |
| `selectedDatesChanged(const QList<QDate>& dates)` | 多选日期改变时触发 | 选择的日期列表 |
| `selectedDateRangeChanged(const QPair<QDate, QDate>& range)` | 日期范围改变时触发 | 选择的日期范围 |
| `popupOpened()` | 弹出日历时触发 | - |
| `popupClosed()` | 关闭日历时触发 | - |

### 枚举

#### NCalendarWidget::DateSelectionMode

| 值 | 说明 |
|----|------|
| `SingleSelection` | 单选模式（默认） |
| `MultiSelection` | 多选模式 |
| `RangeSelection` | 范围选择模式 |

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认高度 | `32px` | 日期选择器高度 |
| 边框圆角 | `4px` | 边框圆角半径 |
| 内边距 | `8px 12px` | 内容内边距 |
| 日历宽度 | `280px` | 弹出日历宽度 |
| 日历高度 | `320px` | 弹出日历高度 |

### 主题适配

```cpp
// 日期选择器会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 日期选择器颜色会自动更新
});
```