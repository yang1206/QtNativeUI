# NTreeView 树形视图

### 介绍

树形视图用于显示具有层级关系的数据。NTreeView 基于 Qt 的 QTreeView，提供了现代化的外观、丰富的交互功能和自定义选项。

### 基本用法

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建数据模型
QStandardItemModel* model = new QStandardItemModel();

// 创建根项
QStandardItem* rootItem = new QStandardItem("根项");

// 创建子项
QStandardItem* child1 = new QStandardItem("子项1");
QStandardItem* child2 = new QStandardItem("子项2");
QStandardItem* child3 = new QStandardItem("子项3");

// 添加子项到根项
rootItem->appendRow(child1);
rootItem->appendRow(child2);
rootItem->appendRow(child3);

// 设置模型
model->appendRow(rootItem);
treeView->setModel(model);

// 展开所有项
treeView->expandAll();

// 监听项选择
connect(treeView, &QTreeView::clicked, [](const QModelIndex& index) {
    qDebug() << "选择了：" << index.data().toString();
});
```

### 多级树形结构

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建数据模型
QStandardItemModel* model = new QStandardItemModel();

// 创建根项
QStandardItem* rootItem = new QStandardItem("公司");

// 创建部门
QStandardItem* deptA = new QStandardItem("技术部");
QStandardItem* deptB = new QStandardItem("销售部");
QStandardItem* deptC = new QStandardItem("人力资源部");

// 创建员工
QStandardItem* emp1 = new QStandardItem("张三");
QStandardItem* emp2 = new QStandardItem("李四");
QStandardItem* emp3 = new QStandardItem("王五");
QStandardItem* emp4 = new QStandardItem("赵六");

// 构建树形结构
deptA->appendRow(emp1);
deptA->appendRow(emp2);
deptB->appendRow(emp3);
deptB->appendRow(emp4);

rootItem->appendRow(deptA);
rootItem->appendRow(deptB);
rootItem->appendRow(deptC);

// 设置模型
model->appendRow(rootItem);
treeView->setModel(model);

// 展开所有项
treeView->expandAll();
```

### 文件系统树

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 使用文件系统模型
QFileSystemModel* fileModel = new QFileSystemModel();
fileModel->setRootPath(QDir::homePath());

treeView->setModel(fileModel);
treeView->setRootIndex(fileModel->index(QDir::homePath()));

// 隐藏某些列
treeView->hideColumn(1);  // 隐藏大小列
treeView->hideColumn(2);  // 隐藏类型列
treeView->hideColumn(3);  // 隐藏修改日期列

// 监听项双击
connect(treeView, &QTreeView::doubleClicked, [=](const QModelIndex& index) {
    QString filePath = fileModel->filePath(index);
    if (fileModel->isDir(index)) {
        qDebug() << "打开文件夹：" << filePath;
    } else {
        qDebug() << "打开文件：" << filePath;
    }
});
```

### 项目树

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建数据模型
QStandardItemModel* model = new QStandardItemModel();

// 创建项目
QStandardItem* project = new QStandardItem("我的项目");

// 创建源代码文件夹
QStandardItem* srcFolder = new QStandardItem("src");
srcFolder->appendRow(new QStandardItem("main.cpp"));
srcFolder->appendRow(new QStandardItem("utils.cpp"));
srcFolder->appendRow(new QStandardItem("config.cpp"));

// 创建头文件文件夹
QStandardItem* headerFolder = new QStandardItem("include");
headerFolder->appendRow(new QStandardItem("main.h"));
headerFolder->appendRow(new QStandardItem("utils.h"));
headerFolder->appendRow(new QStandardItem("config.h"));

// 创建资源文件夹
QStandardItem* resourceFolder = new QStandardItem("resources");
resourceFolder->appendRow(new QStandardItem("icons.qrc"));
resourceFolder->appendRow(new QStandardItem("styles.qss"));

// 构建树形结构
project->appendRow(srcFolder);
project->appendRow(headerFolder);
project->appendRow(resourceFolder);

// 设置模型
model->appendRow(project);
treeView->setModel(model);

// 展开所有项
treeView->expandAll();
```

### 自定义样式

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 设置项目高度
treeView->setItemHeight(32);

// 设置项目圆角
treeView->setItemBorderRadius(4);

// 设置边框圆角
treeView->setBorderRadius(8);

// 设置缩进
treeView->setIndentation(20);

// 自定义颜色
treeView->setLightBackgroundColor(QColor(255, 255, 255));
treeView->setLightItemHoverColor(QColor(245, 245, 245));
treeView->setLightItemSelectedColor(QColor(0, 120, 215));
treeView->setLightItemPressedColor(QColor(0, 100, 195));
treeView->setLightTextColor(QColor(32, 32, 32));
treeView->setLightBorderColor(QColor(200, 200, 200));

treeView->setDarkBackgroundColor(QColor(45, 45, 45));
treeView->setDarkItemHoverColor(QColor(60, 60, 60));
treeView->setDarkItemSelectedColor(QColor(0, 150, 255));
treeView->setDarkItemPressedColor(QColor(0, 130, 235));
treeView->setDarkTextColor(QColor(220, 220, 220));
treeView->setDarkBorderColor(QColor(80, 80, 80));

// 创建模型
QStandardItemModel* model = new QStandardItemModel();
QStandardItem* rootItem = new QStandardItem("根项");
rootItem->appendRow(new QStandardItem("子项1"));
rootItem->appendRow(new QStandardItem("子项2"));
model->appendRow(rootItem);

treeView->setModel(model);
treeView->expandAll();
```

### 项目展开/折叠

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建模型
QStandardItemModel* model = new QStandardItemModel();
QStandardItem* rootItem = new QStandardItem("根项");
QStandardItem* child1 = new QStandardItem("子项1");
QStandardItem* child2 = new QStandardItem("子项2");
rootItem->appendRow(child1);
rootItem->appendRow(child2);
model->appendRow(rootItem);

treeView->setModel(model);

// 创建按钮
NPushButton* expandBtn = new NPushButton("展开所有");
NPushButton* collapseBtn = new NPushButton("折叠所有");

// 处理按钮点击
connect(expandBtn, &NPushButton::clicked, [=]() {
    treeView->expandAll();
});

connect(collapseBtn, &NPushButton::clicked, [=]() {
    treeView->collapseAll();
});

// 展开/折叠特定项
QModelIndex rootIndex = model->index(0, 0);
treeView->expand(rootIndex);
// treeView->collapse(rootIndex);
```

### 项目选择

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建模型
QStandardItemModel* model = new QStandardItemModel();
QStandardItem* rootItem = new QStandardItem("根项");
rootItem->appendRow(new QStandardItem("子项1"));
rootItem->appendRow(new QStandardItem("子项2"));
rootItem->appendRow(new QStandardItem("子项3"));
model->appendRow(rootItem);

treeView->setModel(model);

// 设置选择模式
treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

// 监听选择变化
connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
        [=](const QItemSelection& selected, const QItemSelection& deselected) {
    qDebug() << "选择了" << selected.indexes().size() << "项";
    for (const QModelIndex& index : selected.indexes()) {
        qDebug() << "选中：" << index.data().toString();
    }
});
```

### 项目编辑

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建模型
QStandardItemModel* model = new QStandardItemModel();
QStandardItem* rootItem = new QStandardItem("根项");
rootItem->appendRow(new QStandardItem("子项1"));
rootItem->appendRow(new QStandardItem("子项2"));
model->appendRow(rootItem);

treeView->setModel(model);

// 启用编辑
treeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

// 监听项编辑完成
connect(model, &QStandardItemModel::itemChanged, [](QStandardItem* item) {
    qDebug() << "项已编辑：" << item->text();
});
```

### 上下文菜单

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建模型
QStandardItemModel* model = new QStandardItemModel();
QStandardItem* rootItem = new QStandardItem("根项");
rootItem->appendRow(new QStandardItem("子项1"));
rootItem->appendRow(new QStandardItem("子项2"));
model->appendRow(rootItem);

treeView->setModel(model);

// 创建上下文菜单
NMenu* contextMenu = new NMenu();
contextMenu->addItem("编辑");
contextMenu->addItem("删除");
contextMenu->addItem("复制");
contextMenu->addSeparator();
contextMenu->addItem("属性");

// 显示上下文菜单
connect(treeView, &QWidget::customContextMenuRequested, [=](const QPoint& pos) {
    contextMenu->exec(treeView->mapToGlobal(pos));
});

treeView->setContextMenuPolicy(Qt::CustomContextMenu);
```

### 搜索和过滤

```cpp
// 创建树形视图
NTreeView* treeView = new NTreeView();

// 创建模型
QStandardItemModel* model = new QStandardItemModel();
QStandardItem* rootItem = new QStandardItem("根项");
rootItem->appendRow(new QStandardItem("苹果"));
rootItem->appendRow(new QStandardItem("香蕉"));
rootItem->appendRow(new QStandardItem("橙子"));
rootItem->appendRow(new QStandardItem("葡萄"));
model->appendRow(rootItem);

// 创建过滤代理模型
QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel();
proxyModel->setSourceModel(model);
proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
proxyModel->setFilterKeyColumn(-1);  // 搜索所有列

treeView->setModel(proxyModel);

// 创建搜索框
NLineEdit* searchEdit = new NLineEdit();
searchEdit->setPlaceholderText("搜索...");

// 连接搜索
connect(searchEdit, &QLineEdit::textChanged, [=](const QString& text) {
    proxyModel->setFilterFixedString(text);
});
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NTreeView(QWidget* parent = nullptr)` | 创建树形视图 |

### 样式属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `itemHeight` | 项目高度 | `int` |
| `itemBorderRadius` | 项目圆角半径 | `int` |
| `borderRadius` | 边框圆角半径 | `int` |
| `indentation` | 缩进宽度 | `int` |

### 颜色属性

| 属性 | 说明 | 类型 |
|------|------|------|
| `lightBackgroundColor` | 明亮主题背景色 | `QColor` |
| `darkBackgroundColor` | 暗黑主题背景色 | `QColor` |
| `lightItemHoverColor` | 明亮主题项目悬停色 | `QColor` |
| `darkItemHoverColor` | 暗黑主题项目悬停色 | `QColor` |
| `lightItemSelectedColor` | 明亮主题项目选中色 | `QColor` |
| `darkItemSelectedColor` | 暗黑主题项目选中色 | `QColor` |
| `lightItemPressedColor` | 明亮主题项目按下色 | `QColor` |
| `darkItemPressedColor` | 暗黑主题项目按下色 | `QColor` |
| `lightTextColor` | 明亮主题文本色 | `QColor` |
| `darkTextColor` | 暗黑主题文本色 | `QColor` |
| `lightBorderColor` | 明亮主题边框色 | `QColor` |
| `darkBorderColor` | 暗黑主题边框色 | `QColor` |

### 继承的 API

NTreeView 继承自 `QTreeView`，支持所有标准 QTreeView 的方法：

- `setModel()` / `model()` - 设置/获取数据模型
- `setRootIndex()` / `rootIndex()` - 设置/获取根项索引
- `expand()` / `collapse()` - 展开/折叠项
- `expandAll()` / `collapseAll()` - 展开/折叠所有项
- `setSelectionMode()` / `selectionMode()` - 设置/获取选择模式
- `clicked()` / `doubleClicked()` - 点击/双击信号
- `expanded()` / `collapsed()` - 展开/折叠信号

## 主题定制

树形视图会自动响应应用主题变化，支持明亮和暗黑主题的自动切换。
