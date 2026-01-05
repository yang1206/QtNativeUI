# NDoubleSpinBox 双精度数字输入框

### 介绍

双精度数字输入框用于输入和调整浮点数值，提供了上下调节按钮和精度控制。NDoubleSpinBox 基于 Qt 的 QDoubleSpinBox，提供了现代化的外观和丰富的自定义选项。

### 基本用法

```cpp
// 基本双精度数字输入框
NDoubleSpinBox* doubleSpinBox = new NDoubleSpinBox();
doubleSpinBox->setRange(0.0, 100.0);
doubleSpinBox->setValue(50.5);
doubleSpinBox->setDecimals(2);  // 保留2位小数
doubleSpinBox->setSuffix(" %");

// 监听值变化
connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
        [](double value) {
    qDebug() << "数值：" << value;
});
```

### 不同精度和单位

```cpp
// 价格输入（货币）
NDoubleSpinBox* priceBox = new NDoubleSpinBox();
priceBox->setRange(0.0, 99999.99);
priceBox->setDecimals(2);
priceBox->setPrefix("¥ ");
priceBox->setSingleStep(0.01);
priceBox->setValue(99.99);

// 重量输入
NDoubleSpinBox* weightBox = new NDoubleSpinBox();
weightBox->setRange(0.0, 1000.0);
weightBox->setDecimals(3);
weightBox->setSuffix(" kg");
weightBox->setSingleStep(0.1);
weightBox->setValue(75.5);

// 温度输入
NDoubleSpinBox* tempBox = new NDoubleSpinBox();
tempBox->setRange(-273.15, 1000.0);
tempBox->setDecimals(1);
tempBox->setSuffix("°C");
tempBox->setSingleStep(0.5);
tempBox->setValue(25.0);

// 百分比输入
NDoubleSpinBox* percentBox = new NDoubleSpinBox();
percentBox->setRange(0.0, 100.0);
percentBox->setDecimals(1);
percentBox->setSuffix("%");
percentBox->setSingleStep(0.1);
percentBox->setValue(85.5);
```

### 科学计算应用

```cpp
// 创建科学计算表单
QWidget* scientificForm = new QWidget();
QFormLayout* layout = new QFormLayout(scientificForm);

// 圆周率相关计算
NDoubleSpinBox* radiusBox = new NDoubleSpinBox();
radiusBox->setRange(0.001, 1000.0);
radiusBox->setDecimals(3);
radiusBox->setSuffix(" m");
radiusBox->setSingleStep(0.1);
radiusBox->setValue(1.0);

NDoubleSpinBox* areaBox = new NDoubleSpinBox();
areaBox->setRange(0.0, 1000000.0);
areaBox->setDecimals(6);
areaBox->setSuffix(" m²");
areaBox->setReadOnly(true);

NDoubleSpinBox* circumferenceBox = new NDoubleSpinBox();
circumferenceBox->setRange(0.0, 10000.0);
circumferenceBox->setDecimals(6);
circumferenceBox->setSuffix(" m");
circumferenceBox->setReadOnly(true);

layout->addRow("半径:", radiusBox);
layout->addRow("面积:", areaBox);
layout->addRow("周长:", circumferenceBox);

// 实时计算
connect(radiusBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
        [=](double radius) {
    double area = M_PI * radius * radius;
    double circumference = 2 * M_PI * radius;
    
    areaBox->setValue(area);
    circumferenceBox->setValue(circumference);
});

// 触发初始计算
radiusBox->setValue(1.0);
```

### 金融应用

```cpp
// 创建投资计算器
QWidget* investmentForm = new QWidget();
QFormLayout* investLayout = new QFormLayout(investmentForm);

// 本金
NDoubleSpinBox* principalBox = new NDoubleSpinBox();
principalBox->setRange(1.0, 10000000.0);
principalBox->setDecimals(2);
principalBox->setPrefix("¥ ");
principalBox->setValue(10000.0);

// 年利率
NDoubleSpinBox* rateBox = new NDoubleSpinBox();
rateBox->setRange(0.01, 50.0);
rateBox->setDecimals(2);
rateBox->setSuffix("%");
rateBox->setSingleStep(0.01);
rateBox->setValue(3.5);

// 投资年限
NDoubleSpinBox* yearsBox = new NDoubleSpinBox();
yearsBox->setRange(0.1, 50.0);
yearsBox->setDecimals(1);
yearsBox->setSuffix(" 年");
yearsBox->setSingleStep(0.5);
yearsBox->setValue(5.0);

// 复利结果
NDoubleSpinBox* resultBox = new NDoubleSpinBox();
resultBox->setRange(0.0, 100000000.0);
resultBox->setDecimals(2);
resultBox->setPrefix("¥ ");
resultBox->setReadOnly(true);

investLayout->addRow("本金:", principalBox);
investLayout->addRow("年利率:", rateBox);
investLayout->addRow("投资年限:", yearsBox);
investLayout->addRow("复利结果:", resultBox);

// 复利计算
auto calculateCompoundInterest = [=]() {
    double principal = principalBox->value();
    double rate = rateBox->value() / 100.0;
    double years = yearsBox->value();
    
    double result = principal * pow(1 + rate, years);
    resultBox->setValue(result);
};

connect(principalBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), calculateCompoundInterest);
connect(rateBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), calculateCompoundInterest);
connect(yearsBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), calculateCompoundInterest);

calculateCompoundInterest(); // 初始计算
```

### 物理单位转换

```cpp
// 创建单位转换器
QWidget* converterWidget = new QWidget();
QVBoxLayout* converterLayout = new QVBoxLayout(converterWidget);

// 长度转换
NGroupBox* lengthGroup = new NGroupBox("长度转换");
QFormLayout* lengthLayout = new QFormLayout(lengthGroup);

NDoubleSpinBox* metersBox = new NDoubleSpinBox();
metersBox->setRange(0.0, 1000000.0);
metersBox->setDecimals(6);
metersBox->setSuffix(" m");

NDoubleSpinBox* feetBox = new NDoubleSpinBox();
feetBox->setRange(0.0, 3280840.0);
feetBox->setDecimals(6);
feetBox->setSuffix(" ft");

NDoubleSpinBox* inchesBox = new NDoubleSpinBox();
inchesBox->setRange(0.0, 39370080.0);
inchesBox->setDecimals(6);
inchesBox->setSuffix(" in");

lengthLayout->addRow("米:", metersBox);
lengthLayout->addRow("英尺:", feetBox);
lengthLayout->addRow("英寸:", inchesBox);

// 长度转换逻辑
connect(metersBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
        [=](double meters) {
    feetBox->blockSignals(true);
    inchesBox->blockSignals(true);
    
    feetBox->setValue(meters * 3.28084);
    inchesBox->setValue(meters * 39.3701);
    
    feetBox->blockSignals(false);
    inchesBox->blockSignals(false);
});

connect(feetBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), 
        [=](double feet) {
    metersBox->blockSignals(true);
    inchesBox->blockSignals(true);
    
    metersBox->setValue(feet / 3.28084);
    inchesBox->setValue(feet * 12.0);
    
    metersBox->blockSignals(false);
    inchesBox->blockSignals(false);
});

converterLayout->addWidget(lengthGroup);
```

### 精度控制和验证

```cpp
NDoubleSpinBox* precisionBox = new NDoubleSpinBox();

// 设置高精度
precisionBox->setDecimals(8);
precisionBox->setRange(-999999.99999999, 999999.99999999);
precisionBox->setSingleStep(0.00000001);

// 获取内部编辑器进行自定义
QLineEdit* lineEdit = precisionBox->getLineEdit();
if (lineEdit) {
    lineEdit->setPlaceholderText("请输入高精度数值");
    
    // 添加输入验证
    QDoubleValidator* validator = new QDoubleValidator(-999999.99999999, 999999.99999999, 8, precisionBox);
    validator->setNotation(QDoubleValidator::StandardNotation);
    lineEdit->setValidator(validator);
}

// 特殊值处理
precisionBox->setSpecialValueText("未设置");
precisionBox->setMinimum(-1.0);
precisionBox->setValue(-1.0); // 显示"未设置"
```

### 自定义样式

```cpp
NDoubleSpinBox* styledBox = new NDoubleSpinBox();

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
| `NDoubleSpinBox(QWidget* parent = nullptr)` | 创建双精度数字输入框 |

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

NDoubleSpinBox 继承自 `QDoubleSpinBox`，支持所有标准 QDoubleSpinBox 的方法和信号：

- `setRange()` / `minimum()` / `maximum()` - 设置/获取范围
- `setValue()` / `value()` - 设置/获取当前值
- `setDecimals()` / `decimals()` - 设置/获取小数位数
- `setSingleStep()` / `singleStep()` - 设置/获取步长
- `setPrefix()` / `prefix()` - 设置/获取前缀
- `setSuffix()` / `suffix()` - 设置/获取后缀
- `setSpecialValueText()` / `specialValueText()` - 设置/获取特殊值文本
- `setWrapping()` / `wrapping()` - 设置/获取循环模式
- `setAccelerated()` / `isAccelerated()` - 设置/获取加速模式
- `valueChanged(double)` - 值改变信号
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
// 双精度数字输入框会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 输入框颜色会自动更新
});
```