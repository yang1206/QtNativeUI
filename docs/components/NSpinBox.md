# NSpinBox 数字输入框

### 介绍

数字输入框用于输入和调整数值，提供了上下调节按钮。NSpinBox 基于 Qt 的 QSpinBox，提供了现代化的外观和丰富的自定义选项。

### 基本用法

```cpp
// 基本数字输入框
NSpinBox* spinBox = new NSpinBox();
spinBox->setRange(0, 100);
spinBox->setValue(50);
spinBox->setSuffix(" %");

// 监听值变化
connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [](int value) {
    qDebug() << "数值：" << value;
});
```

### 不同数值类型

```cpp
// 年龄输入
NSpinBox* ageBox = new NSpinBox();
ageBox->setRange(0, 150);
ageBox->setSuffix(" 岁");
ageBox->setSpecialValueText("未知");

// 百分比输入
NSpinBox* percentBox = new NSpinBox();
percentBox->setRange(0, 100);
percentBox->setSuffix("%");
percentBox->setSingleStep(5);

// 数量输入
NSpinBox* quantityBox = new NSpinBox();
quantityBox->setRange(1, 9999);
quantityBox->setPrefix("数量: ");
quantityBox->setValue(1);

// 温度输入
NSpinBox* tempBox = new NSpinBox();
tempBox->setRange(-50, 50);
tempBox->setSuffix("°C");
tempBox->setValue(25);
```

### 表单应用

```cpp
// 创建设置表单
QWidget* settingsForm = new QWidget();
QFormLayout* layout = new QFormLayout(settingsForm);

// 字体大小设置
NSpinBox* fontSizeBox = new NSpinBox();
fontSizeBox->setRange(8, 72);
fontSizeBox->setSuffix(" pt");
fontSizeBox->setValue(12);
layout->addRow("字体大小", fontSizeBox);

// 自动保存间隔
NSpinBox* autoSaveBox = new NSpinBox();
autoSaveBox->setRange(1, 60);
autoSaveBox->setSuffix(" 分钟");
autoSaveBox->setValue(5);
layout->addRow("自动保存", autoSaveBox);

// 最大历史记录
NSpinBox* historyBox = new NSpinBox();
historyBox->setRange(10, 1000);
historyBox->setSuffix(" 条");
historyBox->setValue(100);
layout->addRow("历史记录", historyBox);

// 连接字体大小变化
connect(fontSizeBox, QOverload<int>::of(&QSpinBox::valueChanged), [](int size) {
    QFont font = qApp->font();
    font.setPointSize(size);
    qApp->setFont(font);
});
```

### 步长和精度控制

```cpp
// 大步长调节
NSpinBox* bigStepBox = new NSpinBox();
bigStepBox->setRange(0, 10000);
bigStepBox->setSingleStep(100);  // 每次增减100
bigStepBox->setValue(1000);

// 键盘加速
NSpinBox* accelBox = new NSpinBox();
accelBox->setRange(0, 1000000);
accelBox->setSingleStep(1);
accelBox->setAccelerated(true);  // 长按时加速

// 循环模式
NSpinBox* wrapBox = new NSpinBox();
wrapBox->setRange(1, 12);
wrapBox->setWrapping(true);  // 超出范围时循环
wrapBox->setSuffix(" 月");
```

### 验证和格式化

```cpp
// 自定义验证
NSpinBox* validatedBox = new NSpinBox();
validatedBox->setRange(0, 255);

// 获取内部编辑器进行自定义
QLineEdit* lineEdit = validatedBox->getLineEdit();
if (lineEdit) {
    lineEdit->setPlaceholderText("请输入0-255的数值");
    
    // 添加输入验证
    QIntValidator* validator = new QIntValidator(0, 255, validatedBox);
    lineEdit->setValidator(validator);
}

// 特殊值文本
NSpinBox* specialBox = new NSpinBox();
specialBox->setRange(-1, 100);
specialBox->setSpecialValueText("自动");  // -1时显示"自动"
specialBox->setValue(-1);
```

### 自定义样式

```cpp
NSpinBox* styledBox = new NSpinBox();

// 设置边框样式
styledBox->setBorderRadius(8);
styledBox->setBorderWidth(1);

// 自定义颜色
styledBox->setLightBackgroundColor(QColor(255, 255, 255));
styledBox->setLightBackgroundHoverColor(QColor(245, 245, 245));
styledBox->setLightBackgroundFocusColor(QColor(240, 248, 255));
styledBox->setLightTextColor(QColor(32, 32, 32));
styledBox->setLightBorderColor(QColor(200, 200, 200));

// 按钮颜色
styledBox->setLightButtonBgColor(QColor(240, 240, 240));
styledBox->setLightButtonHoverColor(QColor(220, 220, 220));
styledBox->setLightButtonPressColor(QColor(200, 200, 200));

// 选中文本颜色
styledBox->setLightSelectionBackgroundColor(QColor(0, 120, 215));
styledBox->setLightSelectionTextColor(QColor(255, 255, 255));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NSpinBox(QWidget* parent = nullptr)` | 创建数字输入框 |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `getLineEdit()` | 获取内部编辑器 | - |
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
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightTextDisabledColor` | 明亮主题禁用文本色 | `QColor` |
| `darkTextDisabledColor` | 暗黑主题禁用文本色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |
| `lightBorderDisabledColor` | 明亮主题禁用边框色 | `QColor` |
| `darkBorderDisabledColor` | 暗黑主题禁用边框色 | `QColor` |
| `lightBottomLineColor` | 明亮主题底部线条色 | `QColor` |
| `darkBottomLineColor` | 暗黑主题底部线条色 | `QColor` |
| `lightButtonBgColor` | 明亮主题按钮背景色 | `QColor` |
| `darkButtonBgColor` | 暗黑主题按钮背景色 | `QColor` |
| `lightButtonHoverColor` | 明亮主题按钮悬停色 | `QColor` |
| `darkButtonHoverColor` | 暗黑主题按钮悬停色 | `QColor` |
| `lightButtonPressColor` | 明亮主题按钮按下色 | `QColor` |
| `darkButtonPressColor` | 暗黑主题按钮按下色 | `QColor` |
| `lightButtonDisabledColor` | 明亮主题按钮禁用色 | `QColor` |
| `darkButtonDisabledColor` | 暗黑主题按钮禁用色 | `QColor` |
| `lightSelectionBackgroundColor` | 明亮主题选中背景色 | `QColor` |
| `darkSelectionBackgroundColor` | 暗黑主题选中背景色 | `QColor` |
| `lightSelectionTextColor` | 明亮主题选中文本色 | `QColor` |
| `darkSelectionTextColor` | 暗黑主题选中文本色 | `QColor` |

### 继承的 API

NSpinBox 继承自 `QSpinBox`，支持所有标准 QSpinBox 的方法和信号：

- `setRange()` / `minimum()` / `maximum()` - 设置/获取范围
- `setValue()` / `value()` - 设置/获取当前值
- `setSingleStep()` / `singleStep()` - 设置/获取步长
- `setPrefix()` / `prefix()` - 设置/获取前缀
- `setSuffix()` / `suffix()` - 设置/获取后缀
- `setSpecialValueText()` / `specialValueText()` - 设置/获取特殊值文本
- `setWrapping()` / `wrapping()` - 设置/获取循环模式
- `setAccelerated()` / `isAccelerated()` - 设置/获取加速模式
- `valueChanged(int)` - 值改变信号
- `textChanged(const QString&)` - 文本改变信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认高度 | `32px` | 输入框高度 |
| 边框圆角 | `4px` | 边框圆角半径 |
| 边框宽度 | `1px` | 边框宽度 |
| 内边距 | `8px 12px` | 内容内边距 |
| 按钮宽度 | `20px` | 上下调节按钮宽度 |

### 主题适配

```cpp
// 数字输入框会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 输入框颜色会自动更新
});
```