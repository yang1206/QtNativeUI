# 示例程序重构 - 需求文档

## 1. 项目概述

将 QtNativeUI 的示例程序从旧的单页面展示方式重构为使用 NNavigationView 的现代化导航结构，每个组件独立成页，提供更好的用户体验和代码组织。

## 2. 用户故事

### 2.1 作为开发者
我希望能够快速找到并查看特定组件的示例，而不是在一个长页面中滚动查找。

**验收标准：**
- 左侧导航栏按功能分组显示所有组件
- 点击导航项可以快速切换到对应组件的示例页面
- 每个组件页面独立，互不干扰

### 2.2 作为开发者
我希望每个组件的示例都完整展示其所有功能和状态，方便我理解如何使用。

**验收标准：**
- 每个组件示例展示基本用法
- 展示组件的所有主要功能（如图标、清除按钮等）
- 展示组件的不同状态（禁用、只读、错误等）
- 包含交互示例，展示信号的使用
- 所有控件设置合适的尺寸，避免被压缩变形

### 2.3 作为开发者
我希望示例代码风格统一，易于理解和参考。

**验收标准：**
- 所有示例页面继承自统一的 BasePage 基类
- 使用统一的布局方式（QHBoxLayout 横向排列）
- 使用统一的命名规范
- 代码简洁，没有无意义的注释

## 3. 功能需求

### 3.1 组件分组

按功能将组件分为以下分组：

#### 3.1.1 按钮类 (Buttons) - 5个 ✅
- NPushButton - 标准按钮
- NToolButton - 工具按钮
- NToggleButton - 切换按钮
- NHyperlinkButton - 超链接按钮
- NDropDownButton - 下拉按钮

#### 3.1.2 输入类 (Inputs) - 9个 ✅
- NLabel - 标签
- NLineEdit - 单行输入
- NTextEdit - 富文本编辑器
- NPlainTextEdit - 纯文本编辑器
- NAutoSuggestBox - 建议框
- NComboBox - 下拉框
- NSpinBox - 整数输入框
- NDoubleSpinBox - 浮点数输入框
- NPicker - 滚轮选择器

#### 3.1.3 选择类 (Selections) - 4个 ✅
- NCheckBox - 复选框
- NRadioButton - 单选按钮
- NToggleSwitch - 开关
- NSlider - 滑块（包含 NRangeSlider）

#### 3.1.4 日期时间类 (Date & Time) - 3个 ✅
- NCalendarWidget - 日历控件
- NCalendarDatePicker - 日期选择器
- NTimePicker - 时间选择器

#### 3.1.5 进度指示类 (Progress) - 2个 ✅
- NProgressBar - 进度条
- NProgressRing - 进度环

#### 3.1.6 容器类 (Containers) - 4个 ✅
- NGroupBox - 分组框
- NScrollArea - 滚动区域
- NStackedWidget - 堆叠控件
- NOverlay - 遮罩层

#### 3.1.7 导航类 (Navigation) - 5个
- NNavigationView - 导航视图
- NTabWidget - 标签页控件
- NTabBar - 标签栏
- NPivot - 枢轴控件
- NMenu - 菜单

#### 3.1.8 列表视图类 (Lists & Trees) - 2个
- NListView - 列表视图
- NTreeView - 树形视图

#### 3.1.9 消息反馈类 (Feedback) - 5个
- NDialog - 基础对话框
- NContentDialog - 内容对话框
- NFlyout - 浮出控件
- NInfoBar - 信息栏
- NToolTip - 工具提示

#### 3.1.10 窗口类 (Windows) - 3个
- NMainWindow - 主窗口
- NWindowBar - 窗口标题栏
- NScrollBar - 滚动条

#### 3.1.11 工具展示类 (Tools) - 2个独立页面
- Colors - 颜色展示（Fluent Colors + Accent Colors）
- Icons - 图标展示（Regular + Filled，带搜索和分页）

### 3.2 BasePage 基类

**功能：**
- 提供统一的页面标题和描述
- 提供 `addSection(title, widget/layout)` 方法添加内容区块
- 自动处理滚动区域
- 统一的样式和间距

**API：**
```cpp
BasePage(const QString& title, const QString& description, QWidget* parent = nullptr);
void addSection(const QString& sectionTitle, QWidget* widget);
void addSection(const QString& sectionTitle, QLayout* layout);
```

### 3.3 示例页面规范

每个组件示例页面必须：

1. **继承 BasePage**
2. **展示内容：**
   - 基本用法
   - 主要功能（如图标、清除按钮等）
   - 不同状态（禁用、只读、错误等）
   - 交互示例（展示信号的使用）
3. **布局规范：**
   - 使用 QHBoxLayout 横向排列控件
   - 设置合适的 spacing（通常 16px）
   - 每个控件设置合适的最小尺寸
   - 使用 addStretch() 避免控件被拉伸变形
4. **代码规范：**
   - 不添加无意义的注释
   - 变量命名清晰
   - 代码简洁

### 3.4 主窗口结构

**功能：**
- 使用 NNavigationView 作为主布局
- 左侧导航栏显示分组和组件列表
- 右侧内容区域显示选中的组件示例页面
- 支持搜索和过滤（可选）

### 3.5 目录结构

```
examples/
├── pages/
│   ├── BasePage.h/cpp           # 基类
│   ├── buttons/                 # 按钮类示例
│   │   ├── PushButtonPage.h/cpp
│   │   ├── ToolButtonPage.h/cpp
│   │   ├── ToggleButtonPage.h/cpp
│   │   ├── HyperlinkButtonPage.h/cpp
│   │   └── DropDownButtonPage.h/cpp
│   ├── inputs/                  # 输入类示例
│   │   ├── LabelPage.h/cpp
│   │   ├── LineEditPage.h/cpp
│   │   ├── TextEditPage.h/cpp
│   │   ├── PlainTextEditPage.h/cpp
│   │   ├── AutoSuggestBoxPage.h/cpp
│   │   ├── ComboBoxPage.h/cpp
│   │   ├── SpinBoxPage.h/cpp
│   │   ├── DoubleSpinBoxPage.h/cpp
│   │   └── PickerPage.h/cpp
│   ├── selections/              # 选择类示例
│   │   ├── CheckBoxPage.h/cpp
│   │   ├── RadioButtonPage.h/cpp
│   │   ├── ToggleSwitchPage.h/cpp
│   │   └── SliderPage.h/cpp
│   ├── datetime/                # 日期时间类示例
│   │   ├── CalendarWidgetPage.h/cpp
│   │   ├── CalendarDatePickerPage.h/cpp
│   │   └── TimePickerPage.h/cpp
│   ├── progress/                # 进度指示类示例
│   │   ├── ProgressBarPage.h/cpp
│   │   └── ProgressRingPage.h/cpp
│   ├── containers/              # 容器类示例
│   │   ├── GroupBoxPage.h/cpp
│   │   ├── ScrollAreaPage.h/cpp
│   │   ├── StackedWidgetPage.h/cpp
│   │   └── OverlayPage.h/cpp
│   ├── navigation/              # 导航类示例
│   │   ├── NavigationViewPage.h/cpp
│   │   ├── TabWidgetPage.h/cpp
│   │   ├── TabBarPage.h/cpp
│   │   ├── PivotPage.h/cpp
│   │   └── MenuPage.h/cpp
│   ├── lists/                   # 列表视图类示例
│   │   ├── ListViewPage.h/cpp
│   │   └── TreeViewPage.h/cpp
│   ├── feedback/                # 消息反馈类示例
│   │   ├── DialogPage.h/cpp
│   │   ├── ContentDialogPage.h/cpp
│   │   ├── FlyoutPage.h/cpp
│   │   ├── InfoBarPage.h/cpp
│   │   └── ToolTipPage.h/cpp
│   ├── windows/                 # 窗口类示例
│   │   ├── MainWindowPage.h/cpp
│   │   ├── WindowBarPage.h/cpp
│   │   └── ScrollBarPage.h/cpp
│   └── tools/                   # 工具展示类（不用 BasePage）
│       ├── ColorsPage.h/cpp
│       └── IconsPage.h/cpp
├── mainwindow.h/cpp             # 主窗口
└── main.cpp
```

## 4. 非功能需求

### 4.1 性能
- 页面切换应流畅，无明显延迟
- 图标页面应使用分页，避免一次加载过多图标

### 4.2 可维护性
- 代码结构清晰，易于添加新组件示例
- 统一的代码风格和命名规范
- 适当的代码注释（仅在必要时）

### 4.3 用户体验
- 导航清晰，易于查找组件
- 示例完整，易于理解
- 控件尺寸合适，不变形

## 5. 约束条件

### 5.1 技术约束
- 使用 Qt5/Qt6 Widgets
- 使用 C++17/C++20
- 遵循项目现有的代码风格

### 5.2 兼容性约束
- 支持 Windows、macOS、Linux
- 支持亮色/暗色主题切换

## 6. 验收标准

### 6.1 完成标准
- [ ] 所有 11 个分组的组件示例页面已创建
- [ ] 所有示例页面继承自 BasePage
- [ ] 所有示例展示完整功能和状态
- [ ] 主窗口使用 NNavigationView 布局
- [ ] 导航栏正确显示所有分组和组件
- [ ] 页面切换正常工作
- [ ] 代码风格统一
- [ ] 旧的 components/ 目录已删除

### 6.2 质量标准
- 所有控件尺寸合适，不变形
- 代码简洁，无冗余
- 无编译警告
- 示例程序可正常运行

## 7. 里程碑

### 阶段 1：基础设施 ✅
- [x] 创建 BasePage 基类
- [x] 创建目录结构

### 阶段 2：按钮和输入类 ✅
- [x] 完成 5 个按钮类示例
- [x] 完成 9 个输入类示例
- [x] 完成 4 个选择类示例
- [x] 完成 3 个日期时间类示例
- [x] 完成 2 个进度指示类示例

### 阶段 3：其他组件类
- [ ] 完成导航类示例（5个）
- [ ] 完成列表视图类示例（2个）
- [ ] 完成消息反馈类示例（5个）
- [ ] 完成窗口类示例（3个）

### 阶段 4：工具展示类
- [ ] 迁移 Colors 页面
- [ ] 迁移 Icons 页面

### 阶段 5：清理和优化
- [ ] 更新主窗口导航
- [ ] 删除旧的 components/ 目录
- [ ] 测试所有功能
- [ ] 代码审查和优化
