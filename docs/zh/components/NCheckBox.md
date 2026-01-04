# NCheckBox 复选框

### 介绍

复选框用于在一组选项中进行多选操作。NCheckBox 基于 Qt 的 QCheckBox，提供了 Fluent Design 风格的现代化外观和动画效果。

### 基本用法

```cpp
// 基本复选框
NCheckBox* checkBox = new NCheckBox("同意用户协议");

// 默认选中状态
NCheckBox* checkedBox = new NCheckBox("记住密码");
checkedBox->setChecked(true);

// 三态复选框
NCheckBox* triStateBox = new NCheckBox("全选");
triStateBox->setTristate(true);
triStateBox->setCheckState(Qt::PartiallyChecked);
```

### 状态监听

```cpp
NCheckBox* checkBox = new NCheckBox("启用通知");

// 监听状态变化
connect(checkBox, &QCheckBox::toggled, [](bool checked) {
    qDebug() << "复选框状态：" << (checked ? "选中" : "未选中");
});

// 监听三态变化
connect(checkBox, QOverload<int>::of(&QCheckBox::stateChanged), 
        [](int state) {
    switch (state) {
        case Qt::Unchecked:
            qDebug() << "未选中";
            break;
        case Qt::PartiallyChecked:
            qDebug() << "部分选中";
            break;
        case Qt::Checked:
            qDebug() << "选中";
            break;
    }
});
```

### 禁用状态

```cpp
NCheckBox* disabledBox = new NCheckBox("禁用选项");
disabledBox->setEnabled(false);
disabledBox->setChecked(true); // 禁用但选中状态
```

### 自定义样式

支持自定义复选框的颜色和圆角等样式属性。

```cpp
NCheckBox* styledBox = new NCheckBox("自定义样式");

// 设置圆角
styledBox->setBorderRadius(6);

// 自定义明亮主题颜色
styledBox->setLightDefaultColor(QColor(255, 255, 255));
styledBox->setLightHoverColor(QColor(245, 245, 245));
styledBox->setLightPressColor(QColor(230, 230, 230));
styledBox->setLightTextColor(QColor(32, 32, 32));

// 自定义边框颜色
styledBox->setLightBorderColor(QColor(200, 200, 200));
styledBox->setLightBorderHoverColor(QColor(100, 150, 255));
styledBox->setLightBorderPressColor(QColor(80, 130, 235));
```

### 复选框组

```cpp
// 创建复选框组
QWidget* groupWidget = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(groupWidget);

QList<QString> options = {"选项1", "选项2", "选项3", "选项4"};
QList<NCheckBox*> checkBoxes;

for (const QString& option : options) {
    NCheckBox* checkBox = new NCheckBox(option);
    checkBoxes.append(checkBox);
    layout->addWidget(checkBox);
}

// 全选/取消全选功能
NCheckBox* selectAllBox = new NCheckBox("全选");
selectAllBox->setTristate(true);

connect(selectAllBox, &QCheckBox::clicked, [checkBoxes, selectAllBox]() {
    bool checked = selectAllBox->checkState() != Qt::Unchecked;
    for (NCheckBox* box : checkBoxes) {
        box->setChecked(checked);
    }
});

// 监听子选项变化，更新全选状态
for (NCheckBox* box : checkBoxes) {
    connect(box, &QCheckBox::toggled, [checkBoxes, selectAllBox]() {
        int checkedCount = 0;
        for (NCheckBox* b : checkBoxes) {
            if (b->isChecked()) checkedCount++;
        }
        
        if (checkedCount == 0) {
            selectAllBox->setCheckState(Qt::Unchecked);
        } else if (checkedCount == checkBoxes.size()) {
            selectAllBox->setCheckState(Qt::Checked);
        } else {
            selectAllBox->setCheckState(Qt::PartiallyChecked);
        }
    });
}
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NCheckBox(QWidget* parent = nullptr)` | 创建空的复选框 |
| `NCheckBox(const QString& text, QWidget* parent = nullptr)` | 创建带文本的复选框 |

### 方法

| 方法 | 说明 | 参数 |
|------|------|------|
| `setChecked(bool checked)` | 设置选中状态 | `true` / `false` |
| `nextCheckState()` | 切换到下一个状态 | - |
| `setBorderRadius(int radius)` | 设置圆角半径 | 圆角像素值 |
| `borderRadius()` | 获取圆角半径 | - |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightDefaultColor` | 明亮主题默认背景色 | `QColor` |
| `darkDefaultColor` | 暗黑主题默认背景色 | `QColor` |
| `lightHoverColor` | 明亮主题悬停背景色 | `QColor` |
| `darkHoverColor` | 暗黑主题悬停背景色 | `QColor` |
| `lightPressColor` | 明亮主题按下背景色 | `QColor` |
| `darkPressColor` | 暗黑主题按下背景色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |
| `lightBorderHoverColor` | 明亮主题悬停边框色 | `QColor` |
| `darkBorderHoverColor` | 暗黑主题悬停边框色 | `QColor` |
| `lightBorderPressColor` | 明亮主题按下边框色 | `QColor` |
| `darkBorderPressColor` | 暗黑主题按下边框色 | `QColor` |

### 继承的 API

NCheckBox 继承自 `QCheckBox`，支持所有标准 QCheckBox 的方法和信号：

- `setText()` / `text()` - 设置/获取复选框文本
- `setCheckState()` / `checkState()` - 设置/获取检查状态
- `setTristate()` / `isTristate()` - 设置/获取三态模式
- `isChecked()` - 获取是否选中
- `toggled(bool)` - 状态切换信号
- `stateChanged(int)` - 状态改变信号
- `clicked(bool)` - 点击信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 复选框大小 | `16px` | 复选框方块大小 |
| 圆角半径 | `2px` | 复选框圆角 |
| 文本间距 | `8px` | 复选框与文本间距 |
| 动画时长 | `150ms` | 状态切换动画时长 |

### 主题适配

```cpp
// 复选框会自动响应主题变化和强调色变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 复选框颜色会自动更新
});

connect(nTheme, &NTheme::accentColorChanged, [](const NThemeColor& color) {
    // 选中状态的强调色会自动更新
});
```