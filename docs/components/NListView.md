# NListView 列表视图

### 介绍

列表视图用于显示数据项的列表，支持选择、排序和自定义显示。NListView 基于 Qt 的 QListView，提供了现代化的外观和丰富的交互功能。

### 基本用法

```cpp
// 创建列表视图
NListView* listView = new NListView();

// 创建数据模型
QStringListModel* model = new QStringListModel();
QStringList items = {"项目1", "项目2", "项目3", "项目4", "项目5"};
model->setStringList(items);

// 设置模型
listView->setModel(model);

// 监听选择变化
connect(listView->selectionModel(), &QItemSelectionModel::currentChanged,
        [](const QModelIndex& current, const QModelIndex& previous) {
    if (current.isValid()) {
        qDebug() << "选择了：" << current.data().toString();
    }
});
```

### 文件列表

```cpp
// 创建文件列表
NListView* fileList = new NListView();

// 使用标准文件系统模型
QFileSystemModel* fileModel = new QFileSystemModel();
fileModel->setRootPath(QDir::homePath());
fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);

fileList->setModel(fileModel);
fileList->setRootIndex(fileModel->index(QDir::homePath()));

// 双击打开文件
connect(fileList, &QListView::doubleClicked, [](const QModelIndex& index) {
    QFileSystemModel* model = qobject_cast<QFileSystemModel*>(fileList->model());
    if (model) {
        QString filePath = model->filePath(index);
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
});
```

### 联系人列表

```cpp
// 创建联系人数据模型
class ContactModel : public QAbstractListModel {
public:
    struct Contact {
        QString name;
        QString phone;
        QString email;
        QPixmap avatar;
    };

    void addContact(const Contact& contact) {
        beginInsertRows(QModelIndex(), contacts.size(), contacts.size());
        contacts.append(contact);
        endInsertRows();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return contacts.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= contacts.size()) {
            return QVariant();
        }

        const Contact& contact = contacts[index.row()];
        switch (role) {
            case Qt::DisplayRole:
                return contact.name;
            case Qt::UserRole:
                return contact.phone;
            case Qt::UserRole + 1:
                return contact.email;
            case Qt::DecorationRole:
                return contact.avatar;
        }
        return QVariant();
    }

private:
    QList<Contact> contacts;
};

// 使用联系人模型
NListView* contactList = new NListView();
ContactModel* contactModel = new ContactModel();

// 添加联系人
ContactModel::Contact contact1{"张三", "138-0000-0000", "zhang@example.com", QPixmap()};
ContactModel::Contact contact2{"李四", "139-0000-0000", "li@example.com", QPixmap()};
contactModel->addContact(contact1);
contactModel->addContact(contact2);

contactList->setModel(contactModel);
```

### 任务列表

```cpp
// 创建任务列表
class TaskModel : public QAbstractListModel {
public:
    struct Task {
        QString title;
        QString description;
        bool completed;
        QDateTime dueDate;
        int priority; // 1-高, 2-中, 3-低
    };

    void addTask(const Task& task) {
        beginInsertRows(QModelIndex(), tasks.size(), tasks.size());
        tasks.append(task);
        endInsertRows();
    }

    void toggleTask(int index) {
        if (index >= 0 && index < tasks.size()) {
            tasks[index].completed = !tasks[index].completed;
            emit dataChanged(createIndex(index, 0), createIndex(index, 0));
        }
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return tasks.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= tasks.size()) {
            return QVariant();
        }

        const Task& task = tasks[index.row()];
        switch (role) {
            case Qt::DisplayRole:
                return task.title;
            case Qt::ToolTipRole:
                return task.description;
            case Qt::CheckStateRole:
                return task.completed ? Qt::Checked : Qt::Unchecked;
            case Qt::UserRole:
                return task.priority;
        }
        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }

private:
    QList<Task> tasks;
};

NListView* taskList = new NListView();
TaskModel* taskModel = new TaskModel();

// 添加任务
TaskModel::Task task1{"完成项目文档", "编写用户手册和API文档", false, QDateTime::currentDateTime().addDays(3), 1};
TaskModel::Task task2{"代码审查", "审查新功能的代码实现", false, QDateTime::currentDateTime().addDays(1), 2};
taskModel->addTask(task1);
taskModel->addTask(task2);

taskList->setModel(taskModel);

// 双击切换任务状态
connect(taskList, &QListView::doubleClicked, [=](const QModelIndex& index) {
    taskModel->toggleTask(index.row());
});
```

### 图片画廊

```cpp
// 创建图片画廊
NListView* gallery = new NListView();
gallery->setViewMode(QListView::IconMode);
gallery->setResizeMode(QListView::Adjust);
gallery->setGridSize(QSize(150, 150));

// 图片模型
QStandardItemModel* imageModel = new QStandardItemModel();

// 添加图片项
QStringList imagePaths = {":/images/photo1.jpg", ":/images/photo2.jpg", ":/images/photo3.jpg"};
for (const QString& path : imagePaths) {
    QStandardItem* item = new QStandardItem();
    
    QPixmap pixmap(path);
    QPixmap thumbnail = pixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    item->setIcon(QIcon(thumbnail));
    item->setText(QFileInfo(path).baseName());
    item->setData(path, Qt::UserRole);
    
    imageModel->appendRow(item);
}

gallery->setModel(imageModel);

// 双击查看大图
connect(gallery, &QListView::doubleClicked, [](const QModelIndex& index) {
    QString imagePath = index.data(Qt::UserRole).toString();
    // 打开图片查看器
    QDesktopServices::openUrl(QUrl::fromLocalFile(imagePath));
});
```

### 搜索过滤

```cpp
// 创建可搜索的列表
QWidget* searchableList = new QWidget();
QVBoxLayout* layout = new QVBoxLayout(searchableList);

// 搜索框
NLineEdit* searchEdit = new NLineEdit();
searchEdit->setPlaceholderText("搜索...");
searchEdit->addAction(NRegularIconType::Search24Regular, QLineEdit::LeadingPosition);

// 列表视图
NListView* filteredList = new NListView();

// 数据模型
QStringListModel* dataModel = new QStringListModel();
QStringList allItems = {
    "苹果", "香蕉", "橙子", "葡萄", "草莓",
    "西瓜", "芒果", "菠萝", "柠檬", "樱桃"
};
dataModel->setStringList(allItems);

// 过滤代理模型
QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel();
proxyModel->setSourceModel(dataModel);
proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

filteredList->setModel(proxyModel);

layout->addWidget(searchEdit);
layout->addWidget(filteredList);

// 搜索过滤
connect(searchEdit, &QLineEdit::textChanged, [=](const QString& text) {
    proxyModel->setFilterFixedString(text);
});
```

### 自定义样式

```cpp
NListView* styledList = new NListView();

// 设置项目高度和圆角
styledList->setItemHeight(48);
styledList->setItemBorderRadius(6);
styledList->setBorderRadius(8);

// 自定义颜色
styledList->setLightBackgroundColor(QColor(255, 255, 255));
styledList->setLightItemHoverColor(QColor(245, 245, 245));
styledList->setLightItemSelectedColor(QColor(0, 120, 215));
styledList->setLightItemPressedColor(QColor(0, 100, 195));
styledList->setLightTextColor(QColor(32, 32, 32));
styledList->setLightBorderColor(QColor(200, 200, 200));
```

## API

### 构造函数

| 构造函数 | 说明 |
|---------|------|
| `NListView(QWidget* parent = nullptr)` | 创建列表视图 |

### 样式属性

| 方法 | 说明 | 参数 |
|------|------|------|
| `setItemHeight(int height)` | 设置项目高度 | 高度像素值 |
| `itemHeight()` | 获取项目高度 | - |
| `setItemBorderRadius(int radius)` | 设置项目圆角 | 圆角半径 |
| `itemBorderRadius()` | 获取项目圆角 | - |
| `setBorderRadius(int radius)` | 设置列表边框圆角 | 圆角半径 |
| `borderRadius()` | 获取列表边框圆角 | - |

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

NListView 继承自 `QListView`，支持所有标准 QListView 的方法和信号：

- `setModel()` / `model()` - 设置/获取数据模型
- `setSelectionMode()` / `selectionMode()` - 设置/获取选择模式
- `setViewMode()` / `viewMode()` - 设置/获取视图模式
- `setGridSize()` / `gridSize()` - 设置/获取网格大小
- `setResizeMode()` / `resizeMode()` - 设置/获取调整模式
- `clicked()` / `doubleClicked()` - 点击/双击信号
- `activated()` - 激活信号

## 主题定制

### 样式变量

| 变量 | 默认值 | 说明 |
|------|--------|------|
| 项目高度 | `32px` | 列表项目默认高度 |
| 项目圆角 | `4px` | 列表项目圆角半径 |
| 边框圆角 | `6px` | 列表边框圆角半径 |
| 项目间距 | `2px` | 列表项目间距 |

### 主题适配

```cpp
// 列表视图会自动响应主题变化
connect(nTheme, &NTheme::themeModeChanged, [](NThemeType::ThemeMode mode) {
    // 列表颜色会自动更新
});
```