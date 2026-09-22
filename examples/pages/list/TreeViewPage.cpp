#include "TreeViewPage.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QStyle>
#include <QVBoxLayout>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTreeView.h>

TreeViewPage::TreeViewPage(QWidget* parent)
    : BasePage("TreeView 树形视图", "TreeView 提供树形视图控件，支持层级结构数据展示。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NTreeView* basicTreeView = new NTreeView(this);
    basicTreeView->setMinimumSize(300, 300);
    basicTreeView->setHeaderText(QStringLiteral("资源管理器"));
    basicTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    basicTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStandardItemModel* basicModel = new QStandardItemModel(basicTreeView);
    basicModel->setHorizontalHeaderLabels({QStringLiteral("名称")});

    auto makeItem = [this](const QString& text, QStyle::StandardPixmap icon) {
        auto* item = new QStandardItem(text);
        item->setIcon(style()->standardIcon(icon));
        item->setFlags((item->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled) & ~Qt::ItemIsEditable);
        return item;
    };

    QStandardItem* root1 = makeItem(QStringLiteral("文档"), QStyle::SP_DirIcon);
    root1->appendRow(makeItem(QStringLiteral("快速开始"), QStyle::SP_FileIcon));
    root1->appendRow(makeItem(QStringLiteral("API 参考"), QStyle::SP_FileIcon));
    root1->appendRow(makeItem(QStringLiteral("示例"), QStyle::SP_FileIcon));
    basicModel->appendRow(root1);

    QStandardItem* root2 = makeItem(QStringLiteral("项目"), QStyle::SP_DirIcon);
    QStandardItem* src = makeItem(QStringLiteral("src"), QStyle::SP_DirIcon);
    src->appendRow(makeItem(QStringLiteral("main.cpp"), QStyle::SP_FileIcon));
    src->appendRow(makeItem(QStringLiteral("utils.cpp"), QStyle::SP_FileIcon));
    root2->appendRow(src);
    root2->appendRow(makeItem(QStringLiteral("CMakeLists.txt"), QStyle::SP_FileIcon));
    basicModel->appendRow(root2);

    QStandardItem* root3 = makeItem(QStringLiteral("设置"), QStyle::SP_DirIcon);
    root3->appendRow(makeItem(QStringLiteral("常规"), QStyle::SP_FileDialogDetailedView));
    root3->appendRow(makeItem(QStringLiteral("高级"), QStyle::SP_FileDialogDetailedView));
    basicModel->appendRow(root3);

    basicTreeView->setModel(basicModel);
    basicTreeView->expand(basicModel->index(0, 0));

    NPushButton* expandBtn = new NPushButton(QStringLiteral("展开全部"), this);
    NPushButton* collapseBtn = new NPushButton(QStringLiteral("折叠全部"), this);
    connect(expandBtn, &NPushButton::clicked, basicTreeView, &NTreeView::expandAll);
    connect(collapseBtn, &NPushButton::clicked, basicTreeView, &NTreeView::collapseAll);

    QHBoxLayout* expandBtns = new QHBoxLayout();
    expandBtns->addWidget(expandBtn);
    expandBtns->addWidget(collapseBtn);
    expandBtns->addStretch();

    NTreeView* emptyTreeView = new NTreeView(this);
    emptyTreeView->setMinimumSize(220, 300);
    emptyTreeView->setPlaceholderText(QStringLiteral("暂无数据"));
    emptyTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    emptyTreeView->setModel(new QStandardItemModel(emptyTreeView));

    QVBoxLayout* basicCol = new QVBoxLayout();
    basicCol->addWidget(new QLabel(QStringLiteral("基本:"), this));
    basicCol->addWidget(basicTreeView);
    basicCol->addLayout(expandBtns);
    QVBoxLayout* emptyCol = new QVBoxLayout();
    emptyCol->addWidget(new QLabel(QStringLiteral("空状态:"), this));
    emptyCol->addWidget(emptyTreeView);
    basicLayout->addLayout(basicCol);
    basicLayout->addLayout(emptyCol);
    basicLayout->addStretch();

    addSection(QStringLiteral("基本树形视图"), basicLayout);

    QHBoxLayout* selectionLayout = new QHBoxLayout();
    selectionLayout->setSpacing(16);

    NTreeView* selectionTreeView = new NTreeView(this);
    selectionTreeView->setMinimumSize(300, 300);
    selectionTreeView->setSelectionMode(QAbstractItemView::MultiSelection);
    selectionTreeView->setHeaderText(QStringLiteral("多选列表"));
    selectionTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStandardItemModel* selectionModel = new QStandardItemModel(selectionTreeView);
    selectionModel->setHorizontalHeaderLabels({QStringLiteral("项目")});

    auto makeCheckable = [this](const QString& text, QStyle::StandardPixmap icon, bool tristate) {
        auto* item = new QStandardItem(text);
        item->setIcon(style()->standardIcon(icon));
        Qt::ItemFlags flags = (item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled)
                              & ~Qt::ItemIsEditable;
        if (tristate)
            flags |= Qt::ItemIsAutoTristate;
        item->setFlags(flags);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
        return item;
    };

    for (int i = 1; i <= 3; ++i) {
        QStandardItem* group = makeCheckable(QStringLiteral("分类 %1").arg(i), QStyle::SP_DirIcon, true);
        for (int j = 1; j <= 3; ++j)
            group->appendRow(makeCheckable(QStringLiteral("子项 %1-%2").arg(i).arg(j), QStyle::SP_FileIcon, false));
        selectionModel->appendRow(group);
    }

    selectionTreeView->setModel(selectionModel);
    selectionTreeView->expand(selectionModel->index(0, 0));

    QLabel* selectionLabel = new QLabel(QStringLiteral("选中项: 无"), this);
    connect(selectionTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [selectionLabel](const QModelIndex& current, const QModelIndex&) {
                if (current.isValid())
                    selectionLabel->setText(QStringLiteral("选中项: ") + current.data().toString());
                else
                    selectionLabel->setText(QStringLiteral("选中项: 无"));
            });

    QVBoxLayout* selectionInfoLayout = new QVBoxLayout();
    selectionInfoLayout->addWidget(selectionLabel);
    selectionInfoLayout->addStretch();

    selectionLayout->addWidget(selectionTreeView);
    selectionLayout->addLayout(selectionInfoLayout);
    selectionLayout->addStretch();

    addSection(QStringLiteral("多选 / 勾选"), selectionLayout);

    QHBoxLayout* editLayout = new QHBoxLayout();
    editLayout->setSpacing(16);

    NTreeView* editTreeView = new NTreeView(this);
    editTreeView->setMinimumSize(300, 220);
    editTreeView->setHeaderText(QStringLiteral("可编辑"));
    editTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    editTreeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked
                                  | QAbstractItemView::EditKeyPressed);

    QStandardItemModel* editModel = new QStandardItemModel(editTreeView);
    editModel->setHorizontalHeaderLabels({QStringLiteral("名称")});

    QStandardItem* folder = makeItem(QStringLiteral("笔记"), QStyle::SP_DirIcon);
    folder->appendRow(makeItem(QStringLiteral("草稿"), QStyle::SP_FileIcon));
    QStandardItem* editable = makeItem(QStringLiteral("未命名"), QStyle::SP_FileIcon);
    editable->setFlags(editable->flags() | Qt::ItemIsEditable);
    folder->appendRow(editable);
    folder->appendRow(makeItem(QStringLiteral("归档"), QStyle::SP_FileIcon));
    editModel->appendRow(folder);

    editTreeView->setModel(editModel);
    editTreeView->expandAll();

    editLayout->addWidget(editTreeView);
    editLayout->addStretch();
    addSection(QStringLiteral("内联编辑"), editLayout);
}
