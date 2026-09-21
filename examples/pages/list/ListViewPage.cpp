#include "ListViewPage.h"
#include <QColor>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NTheme.h>

#include "QtNativeUI/NLabel.h"

ListViewPage::ListViewPage(QWidget* parent)
    : BasePage("ListView 列表视图", "ListView 提供列表视图控件，支持单选、多选和自定义样式。", parent) {
    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NListView* basicListView = new NListView(this);
    basicListView->setMinimumSize(200, 250);

    QStandardItemModel* basicModel = new QStandardItemModel(basicListView);
    for (int i = 1; i <= 10; ++i) {
        QStandardItem* item = new QStandardItem(QString("列表项 %1").arg(i));
        basicModel->appendRow(item);
    }
    basicListView->setModel(basicModel);

    basicLayout->addWidget(basicListView);
    basicLayout->addStretch();

    addSection("基本列表视图", basicLayout);

    QHBoxLayout* selectionLayout = new QHBoxLayout();
    selectionLayout->setSpacing(32);

    NListView* singleListView = new NListView(this);
    singleListView->setMinimumSize(200, 250);
    singleListView->setSelectionMode(QAbstractItemView::SingleSelection);

    QStandardItemModel* singleModel = new QStandardItemModel(singleListView);
    for (int i = 1; i <= 8; ++i) {
        QStandardItem* item = new QStandardItem(QString("单选项 %1").arg(i));
        singleModel->appendRow(item);
    }
    singleListView->setModel(singleModel);

    NListView* multiListView = new NListView(this);
    multiListView->setMinimumSize(200, 250);
    multiListView->setSelectionMode(QAbstractItemView::MultiSelection);

    QStandardItemModel* multiModel = new QStandardItemModel(multiListView);
    for (int i = 1; i <= 8; ++i) {
        QStandardItem* item = new QStandardItem(QString("多选项 %1").arg(i));
        multiModel->appendRow(item);
    }
    multiListView->setModel(multiModel);

    QVBoxLayout* singleLayout = new QVBoxLayout();
    singleLayout->addWidget(new QLabel("单选模式:", this));
    singleLayout->addWidget(singleListView);

    QVBoxLayout* multiLayout = new QVBoxLayout();
    multiLayout->addWidget(new QLabel("多选模式:", this));
    multiLayout->addWidget(multiListView);

    selectionLayout->addLayout(singleLayout);
    selectionLayout->addLayout(multiLayout);
    selectionLayout->addStretch();

    addSection("不同选择模式", selectionLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();
    iconLayout->setSpacing(16);

    NListView* iconListView = new NListView(this);
    iconListView->setMinimumSize(250, 250);

    QStandardItemModel* iconModel = new QStandardItemModel(iconListView);
    QStringList         items     = {"首页", "文档", "设置", "帮助", "关于"};
    for (const QString& text : items) {
        QStandardItem* item = new QStandardItem(text);
        item->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
        iconModel->appendRow(item);
    }
    iconListView->setModel(iconModel);

    iconLayout->addWidget(iconListView);
    iconLayout->addStretch();

    addSection("带图标的列表", iconLayout);

    QHBoxLayout* iconModeLayout = new QHBoxLayout();
    iconModeLayout->setSpacing(16);
    NListView* iconModeView = new NListView(this);
    iconModeView->setMinimumSize(440, 280);
    iconModeView->setViewMode(QListView::IconMode);
    iconModeView->setWrapping(true);
    iconModeView->setResizeMode(QListView::Adjust);
    iconModeView->setMovement(QListView::Static);
    iconModeView->setFlow(QListView::LeftToRight);
    iconModeView->setUniformItemSizes(true);
    iconModeView->setWordWrap(true);
    iconModeView->setSpacing(8);
    iconModeView->setIconSize(QSize(48, 48));
    iconModeView->setGridSize(QSize(104, 112));
    iconModeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    iconModeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    auto* iconModeModel = new QStandardItemModel(iconModeView);
    const struct IconTile {
        QString           title;
        QStyle::StandardPixmap pixmap;
    } tiles[] = {
        {QStringLiteral("文档"), QStyle::SP_FileIcon},
        {QStringLiteral("文件夹"), QStyle::SP_DirIcon},
        {QStringLiteral("本机"), QStyle::SP_ComputerIcon},
        {QStringLiteral("磁盘"), QStyle::SP_DriveHDIcon},
        {QStringLiteral("桌面"), QStyle::SP_DesktopIcon},
        {QStringLiteral("回收站"), QStyle::SP_TrashIcon},
        {QStringLiteral("网络"), QStyle::SP_DriveNetIcon},
        {QStringLiteral("帮助"), QStyle::SP_DialogHelpButton},
    };
    for (const IconTile& tile : tiles) {
        auto* item = new QStandardItem(tile.title);
        item->setIcon(style()->standardIcon(tile.pixmap));
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        iconModeModel->appendRow(item);
    }
    iconModeView->setModel(iconModeModel);
    iconModeLayout->addWidget(iconModeView);
    iconModeLayout->addStretch();
    addSection(QStringLiteral("图标网格 (IconMode)"), iconModeLayout);

    QHBoxLayout* interactiveLayout = new QHBoxLayout();
    interactiveLayout->setSpacing(16);

    NListView* interactiveListView = new NListView(this);
    interactiveListView->setMinimumSize(250, 250);

    QStandardItemModel* interactiveModel = new QStandardItemModel(interactiveListView);
    for (int i = 1; i <= 5; ++i) {
        QStandardItem* item = new QStandardItem(QString("项目 %1").arg(i));
        interactiveModel->appendRow(item);
    }
    interactiveListView->setModel(interactiveModel);

    QLabel* selectionLabel = new QLabel("选中项: 无", this);
    connect(interactiveListView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            [selectionLabel](const QModelIndex& current, const QModelIndex&) {
                if (current.isValid()) {
                    selectionLabel->setText("选中项: " + current.data().toString());
                } else {
                    selectionLabel->setText("选中项: 无");
                }
            });

    QVBoxLayout* interactiveInfoLayout = new QVBoxLayout();
    interactiveInfoLayout->addWidget(selectionLabel);
    interactiveInfoLayout->addStretch();

    interactiveLayout->addWidget(interactiveListView);
    interactiveLayout->addLayout(interactiveInfoLayout);
    interactiveLayout->addStretch();

    addSection("交互示例", interactiveLayout);

    QHBoxLayout* fluentLayout = new QHBoxLayout();
    fluentLayout->setSpacing(16);

    NListView* emptyListView = new NListView(this);
    emptyListView->setMinimumSize(220, 200);
    emptyListView->setPlaceholderText(QStringLiteral("暂无数据"));
    emptyListView->setModel(new QStandardItemModel(emptyListView));

    NListView* richListView = new NListView(this);
    richListView->setMinimumSize(280, 260);
    QStandardItemModel* richModel  = new QStandardItemModel(richListView);
    auto                addRichRow = [&](const QString& title, const QString& subtitle, bool chevron, bool checkable) {
        QStandardItem* item = new QStandardItem(title);
        item->setData(subtitle, NListViewType::SubtitleRole);
        if (!subtitle.isEmpty())
            item->setData(subtitle, Qt::AccessibleDescriptionRole);
        if (chevron)
            item->setData(true, NListViewType::ShowChevronRole);
        if (checkable) {
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setData(Qt::Unchecked, Qt::CheckStateRole);
        }
        item->setIcon(richListView->style()->standardIcon(QStyle::SP_DirIcon));
        richModel->appendRow(item);
    };
    addRichRow(QStringLiteral("账户"), QStringLiteral("管理登录与安全选项"), true, false);
    addRichRow(QStringLiteral("通知"), QStringLiteral("选择提醒方式"), true, false);
    addRichRow(QStringLiteral("同步任务"), QStringLiteral("后台自动同步"), false, true);
    richListView->setModel(richModel);

    QVBoxLayout* emptyCol = new QVBoxLayout();
    emptyCol->addWidget(new QLabel(QStringLiteral("空状态占位:"), this));
    emptyCol->addWidget(emptyListView);
    QVBoxLayout* richCol = new QVBoxLayout();
    richCol->addWidget(new QLabel(QStringLiteral("副标题 / 箭头 / 复选:"), this));
    richCol->addWidget(richListView);
    fluentLayout->addLayout(emptyCol);
    fluentLayout->addLayout(richCol);
    fluentLayout->addStretch();
    addSection("Fluent 行样式", fluentLayout);

    QHBoxLayout* editLayout = new QHBoxLayout();
    NListView*   editList   = new NListView(this);
    editList->setMinimumSize(300, 200);
    editList->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
    auto* editModel = new QStandardItemModel(editList);
    auto* textItem  = new QStandardItem(QStringLiteral("双击编辑文本"));
    textItem->setFlags(textItem->flags() | Qt::ItemIsEditable);
    editModel->appendRow(textItem);
    auto* comboItem = new QStandardItem(QStringLiteral("选项 A"));
    comboItem->setFlags(comboItem->flags() | Qt::ItemIsEditable);
    comboItem->setData(QStringList{QStringLiteral("选项 A"), QStringLiteral("选项 B"), QStringLiteral("选项 C")},
                       NListViewType::ComboChoicesRole);
    editModel->appendRow(comboItem);
    editList->setModel(editModel);
    editLayout->addWidget(editList);
    editLayout->addStretch();
    addSection(QStringLiteral("内联编辑 (NLineEdit / NComboBox)"), editLayout);

    QHBoxLayout* groupLayout = new QHBoxLayout();
    groupLayout->setSpacing(24);

    auto fillGroupedModel = [](QStandardItemModel* model, const QColor& workColor, const QColor& doneColor) {
        const struct GroupRow {
            QString section;
            QString title;
            QString subtitle;
            QColor  headerColor;
        } rows[] = {
            {QStringLiteral("工作"), QStringLiteral("写周报"), QStringLiteral("周五 18:00"), workColor},
            {QStringLiteral("工作"), QStringLiteral("代码评审"), QStringLiteral("PR #128"), workColor},
            {QStringLiteral("工作"), QStringLiteral("联调接口"), QStringLiteral("周三"), workColor},
            {QStringLiteral("工作"), QStringLiteral("站会纪要"), QStringLiteral("每天"), workColor},
            {QStringLiteral("个人"), QStringLiteral("买菜"), QString(), QColor()},
            {QStringLiteral("个人"), QStringLiteral("看电影"), QStringLiteral("周六"), QColor()},
            {QStringLiteral("个人"), QStringLiteral("健身"), QStringLiteral("晚上"), QColor()},
            {QStringLiteral("已完成"), QStringLiteral("交税"), QStringLiteral("上周"), doneColor},
            {QStringLiteral("已完成"), QStringLiteral("更新证书"), QString(), doneColor},
            {QStringLiteral("已完成"), QStringLiteral("备份配置"), QString(), doneColor},
        };
        for (const GroupRow& row : rows) {
            auto* item = new QStandardItem(row.title);
            item->setData(row.section, NListViewType::SectionRole);
            if (!row.subtitle.isEmpty())
                item->setData(row.subtitle, NListViewType::SubtitleRole);
            if (row.headerColor.isValid())
                item->setData(row.headerColor, NListViewType::SectionForegroundRole);
            model->appendRow(item);
        }
    };

    NListView* defaultGroupList = new NListView(this);
    defaultGroupList->setMinimumSize(260, 240);
    defaultGroupList->setMaximumHeight(240);
    defaultGroupList->setMaximumWidth(320);
    defaultGroupList->setSectionsEnabled(true);
    defaultGroupList->setSelectionMode(QAbstractItemView::SingleSelection);
    auto* defaultGroupModel = new QStandardItemModel(defaultGroupList);
    fillGroupedModel(defaultGroupModel, QColor(), QColor());
    defaultGroupList->setModel(defaultGroupModel);

    NListView* customGroupList = new NListView(this);
    customGroupList->setMinimumSize(260, 240);
    customGroupList->setMaximumHeight(240);
    customGroupList->setMaximumWidth(320);
    customGroupList->setSectionsEnabled(true);
    customGroupList->setSectionHeaderHeight(36);
    QFont sectionFont = customGroupList->font();
    sectionFont.setPixelSize(16);
    sectionFont.setWeight(QFont::DemiBold);
    customGroupList->setSectionHeaderFont(sectionFont);
    customGroupList->setSelectionMode(QAbstractItemView::SingleSelection);
    auto* customGroupModel = new QStandardItemModel(customGroupList);
    fillGroupedModel(customGroupModel, nTheme->accentColor().normal(),
                     NThemeColor(NFluentColorKey::TextFillColorTertiary, nTheme->themeMode()));
    customGroupList->setModel(customGroupModel);

    QVBoxLayout* defaultGroupCol = new QVBoxLayout();
    defaultGroupCol->addWidget(new QLabel(QStringLiteral("默认：滚一下看分组吸顶"), this));
    defaultGroupCol->addWidget(defaultGroupList);
    QVBoxLayout* customGroupCol = new QVBoxLayout();
    customGroupCol->addWidget(new QLabel(QStringLiteral("自定义：16px / DemiBold + 分组着色"), this));
    customGroupCol->addWidget(customGroupList);
    groupLayout->addLayout(defaultGroupCol);
    groupLayout->addLayout(customGroupCol);
    groupLayout->addStretch();
    addSection(QStringLiteral("自定义分组 (标题 / 字体 / 颜色)"), groupLayout);

    QHBoxLayout* advancedLayout = new QHBoxLayout();
    advancedLayout->setSpacing(24);

    NListView* advancedList = new NListView(this);
    advancedList->setMinimumSize(300, 340);
    advancedList->setMaximumWidth(360);
    advancedList->setHeaderText(QStringLiteral("任务"));
    advancedList->setFooterText(QStringLiteral("共 0 项"));
    advancedList->setSectionsEnabled(true);
    advancedList->setReorderEnabled(true);
    advancedList->setSelectionIndicatorAnimated(true);
    advancedList->setSelectionMode(QAbstractItemView::SingleSelection);

    QStandardItemModel* advancedModel = new QStandardItemModel(advancedList);
    const struct RowSpec {
        QString section;
        QString title;
        QString subtitle;
    } rows[] = {
        {QStringLiteral("今天"), QStringLiteral("回复邮件"), QStringLiteral("09:00")},
        {QStringLiteral("今天"), QStringLiteral("站会"), QStringLiteral("10:30")},
        {QStringLiteral("今天"), QStringLiteral("代码评审"), QStringLiteral("14:00")},
        {QStringLiteral("本周"), QStringLiteral("写周报"), QStringLiteral("周五")},
        {QStringLiteral("本周"), QStringLiteral("整理需求"), QStringLiteral("待定")},
        {QStringLiteral("本周"), QStringLiteral("联调接口"), QStringLiteral("周三")},
        {QStringLiteral("稍后"), QStringLiteral("升级依赖"), QString()},
        {QStringLiteral("稍后"), QStringLiteral("补单元测试"), QString()},
        {QStringLiteral("稍后"), QStringLiteral("更新文档"), QString()},
        {QStringLiteral("稍后"), QStringLiteral("性能 profiling"), QString()},
        {QStringLiteral("稍后"), QStringLiteral("清理分支"), QString()},
        {QStringLiteral("稍后"), QStringLiteral("备份配置"), QString()},
    };
    const Qt::ItemFlags reorderFlags =
        Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    for (const RowSpec& row : rows) {
        QStandardItem* item = new QStandardItem(row.title);
        item->setData(row.section, NListViewType::SectionRole);
        if (!row.subtitle.isEmpty())
            item->setData(row.subtitle, NListViewType::SubtitleRole);
        item->setFlags(reorderFlags);
        advancedModel->appendRow(item);
    }
    advancedList->setModel(advancedModel);
    advancedList->setFooterText(
        QStringLiteral("共 %1 项 · 拖到新位置可重排").arg(advancedModel->rowCount()));

    QLabel* reorderHint = new QLabel(QStringLiteral("重排: 尚未拖动"), this);
    reorderHint->setWordWrap(true);
    connect(advancedList, &NListView::rowsReordered, this, [advancedList, reorderHint](int from, int to) {
        reorderHint->setText(QStringLiteral("重排: 行 %1 → %2").arg(from + 1).arg(to + 1));
        QAbstractItemModel* m = advancedList->model();
        if (m)
            advancedList->setFooterText(QStringLiteral("共 %1 项 · 拖到新位置可重排").arg(m->rowCount()));
    });

    QVBoxLayout* controlCol = new QVBoxLayout();
    controlCol->setSpacing(8);
    controlCol->addWidget(new QLabel(QStringLiteral("开关（即时生效）:"), this));

    auto wireToggle = [&](const QString& label, bool checked, auto setter) {
        NCheckBox* box = new NCheckBox(label, this);
        box->setChecked(checked);
        connect(box, &NCheckBox::toggled, this, [advancedList, setter](bool on) { setter(advancedList, on); });
        controlCol->addWidget(box);
    };
    wireToggle(QStringLiteral("分组 (SectionRole)"), true,
               [](NListView* v, bool on) { v->setSectionsEnabled(on); });
    wireToggle(QStringLiteral("拖拽重排 (鼠标拖放)"), true,
               [](NListView* v, bool on) { v->setReorderEnabled(on); });
    wireToggle(QStringLiteral("选中指示条动画"), true,
               [](NListView* v, bool on) { v->setSelectionIndicatorAnimated(on); });
    wireToggle(QStringLiteral("显示选中指示条"), true,
               [](NListView* v, bool on) { v->setSelectionIndicatorVisible(on); });

    controlCol->addSpacing(12);
    controlCol->addWidget(new QLabel(QStringLiteral("操作提示:"), this));
    QLabel* tips = new QLabel(
        QStringLiteral("· 单击选中后按住行拖动重排（与 Fluent-Qt 相同，不用 QDrag）\n"
                       "· 多选指示条见上方「不同选择模式」"),
        this);
    tips->setWordWrap(true);
    controlCol->addWidget(tips);
    controlCol->addWidget(reorderHint);
    controlCol->addStretch();

    QVBoxLayout* listCol = new QVBoxLayout();
    listCol->addWidget(new QLabel(QStringLiteral("头尾文字 + 分组 + 重排:"), this));
    listCol->addWidget(advancedList);

    advancedLayout->addLayout(listCol);
    advancedLayout->addLayout(controlCol, 1);
    advancedLayout->addStretch();
    addSection(QStringLiteral("高级能力演示"), advancedLayout);
}
