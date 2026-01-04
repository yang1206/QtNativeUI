# NComboBox 下拉选择框

### 介绍

下拉选择框用于从预定义的选项列表中选择一个值。NComboBox 基于 Qt 的 QComboBox，提供了现代化的外观和丰富的自定义选项。

### 基本用法

```cpp
// 基本下拉框
NComboBox* comboBox = new NComboBox();
comboBox->addItem("选项1");
comboBox->addItem("选项2");
comboBox->addItem("选项3");

// 使用字符串列表创建
QStringList items = {"苹果", "香蕉", "橙子", "葡萄"};
NComboBox* fruitBox = new NComboBox(items);

// 设置当前选中项
fruitBox->setCurrentIndex(1); // 选中"香蕉"
fruitBox->setCurrentText("橙子"); // 通过文本选中
```

### 可编辑下拉框

支持用户输入自定义值，不仅限于预定义选项。

```cpp
NComboBox* editableBox = new NComboBox();
editableBox->setEditable(true);
editableBox->addItems({"北京", "上海", "广州", "深圳"});

// 获取编辑器
QLineEdit* lineEdit = editableBox->getLineEdit();
if (lineEdit) {
    lineEdit->setPlaceholderText("请选择或输入城市");
}

// 监听文本变化
connect(editableBox, &QComboBox::currentTextChanged, [](const QString& text) {
    qDebug() << "当前文本：" << text;
});
```

### 带图标的选项

```cpp
NComboBox* iconBox = new NComboBox();

// 添加带图标的选项
iconBox->addItem(QIcon(":/icons/file.png"), "文件");
iconBox->addItem(QIcon(":/icons/folder.png"), "文件夹");
iconBox->addItem(QIcon(":/icons/image.png"), "图片");

// 或者使用 Fluent 图标
for (int i = 0; i < iconBox->count(); ++i) {
    // 可以通过自定义委托来显示 Fluent 图标
}
```

### 分组选项

```cpp
// 创建带分组的下拉框
NComboBox* groupBox = new NComboBox();

// 添加分组标题（通常设为不可选择）
groupBox->addItem("--- 水果 ---");
groupBox->setItemData(0, 0, Qt::UserRole - 1); // 设为分隔符

groupBox->addItem("苹果");
groupBox->addItem("香蕉");

groupBox->addItem("--- 蔬菜 ---");
groupBox->setItemData(3, 0, Qt::UserRole - 1);

groupBox->addItem("胡萝卜");
groupBox->addItem("白菜");
```

### 设置页面示例

```cpp
// 创建设置页面的下拉选择
QWidget* settingsWidget = new QWidget();
QFormLayout* layout = new QFormLayout(settingsWidget);

// 主题选择
NComboBox* themeBox = new NComboBox();
themeBox->addItems({"跟随系统", "浅色主题", "深色主题"});
layout->addRow("主题模式", themeBox);

// 语言选择
NComboBox* langBox = new NComboBox();
langBox->addItems({"简体中文", "English", "日本語"});
layout->addRow("界面语言", langBox);

// 字体大小
NComboBox* fontSizeBox = new NComboBox();
fontSizeBox->addItems({"小", "中", "大", "特大"});
layout->addRow("字体大小", fontSizeBox);

// 连接主题切换
connect(themeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), 
        [](int index) {
    switch (index) {
        case 0: nTheme->setThemeMode(NThemeType::System); break;
        case 1: nTheme->setThemeMode(NThemeType::Light); break;
        case 2: nTheme->setThemeMode(NThemeType::Dark); break;
    }
});
```

### 自定义样式

```cpp
NComboBox* styledBox = new NComboBox();

// 设置边框样式
styledBox->setBorderRadius(8);
styledBox->setBorderWidth(1);

// 自定义明亮主题颜色
styledBox->setLightBackgroundColor(QColor(255, 255, 255));
styledBox->setLightBackgroundHoverColor(QColor(245, 245, 245));
styledBox->setLightBackgroundFocusColor(QColor(240, 248, 255));
styledBox->setLightTextColor(QColor(32, 32, 32));
styledBox->setLightBorderColor(QColor(200, 200, 200));

// 下拉按钮颜色
styledBox->setLightDropdownButtonBgColor(QColor(240, 240, 240));
styledBox->setLightDropdownButtonHoverColor(QColor(220, 220, 220));

// 选中项颜色
styledBox->setLightSelectionBackgroundColor(QColor(0, 120, 215));
styledBox->setLightSelectionTextColor(QColor(255, 255, 255));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NComboBox(QWidget* parent = nullptr)` | 创建空的下拉框 |
| `NComboBox(const QStringList& items, QWidget* parent = nullptr)` | 创建带选项列表的下拉框 |

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
| `lightDropdownButtonBgColor` | 明亮主题下拉按钮背景色 | `QColor` |
| `darkDropdownButtonBgColor` | 暗黑主题下拉按钮背景色 | `QColor` |
| `lightDropdownButtonHoverColor` | 明亮主题下拉按钮悬停色 | `QColor` |
| `darkDropdownButtonHoverColor` | 暗黑主题下拉按钮悬停色 | `QColor` |
| `lightDropdownButtonPressColor` | 明亮主题下拉按钮按下色 | `QColor` |
| `darkDropdownButtonPressColor` | 暗黑主题下拉按钮按下色 | `QColor` |
| `lightDropdownButtonDisabledColor` | 明亮主题下拉按钮禁用色 | `QColor` |
| `darkDropdownButtonDisabledColor` | 暗黑主题下拉按钮禁用色 | `QColor` |
| `lightSelectionBackgroundColor` | 明亮主题选中背景色 | `QColor` |
| `darkSelectionBackgroundColor` | 暗黑主题选中背景色 | `QColor` |
| `lightSelectionTextColor` | 明亮主题选中文本色 | `QColor` |
| `darkSelectionTextColor` | 暗黑主题选中文本色 | `QColor` |

### 继承的 API

NComboBox 继承自 `QComboBox`，支持所有标准 QComboBox 的方法和信号：

- `addItem()` / `addItems()` - 添加选项
- `removeItem()` / `clear()` - 移除选项
- `setCurrentIndex()` / `currentIndex()` - 设置/获取当前索引
- `setCurrentText()` / `currentText()` - 设置/获取当前文本
- `setEditable()` / `isEditable()` - 设置/获取可编辑状态
- `count()` / `itemText()` - 获取选项数量和文本
- `currentIndexChanged()` - 索引改变信号
- `currentTextChanged()` - 文本改变信号
- `activated()` - 选项激活信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 默认高度 | `32px` | 下拉框高度 |
| 边框圆角 | `4px` | 边框圆角半径 |
| 边框宽度 | `1px` | 边框宽度 |
| 内边距 | `8px 12px` | 内容内边距 |
| 下拉按钮宽度 | `32px` | 右侧下拉按钮宽度 |

### 主题适配

```cpp
// 下拉框会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 下拉框颜色会自动更新
});
```