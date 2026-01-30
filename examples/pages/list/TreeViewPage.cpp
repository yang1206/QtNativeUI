#include "TreeViewPage.h"
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NTreeView.h>

TreeViewPage::TreeViewPage(QWidget* parent)
    : BasePage("TreeView 树形视图", "TreeView 提供树形视图控件，支持层级结构数据展示。", parent) {

    QHBoxLayout* basicLayout = new QHBoxLayout();
    basicLayout->setSpacing(16);

    NTreeView* basicTreeView = new NTreeView(this);
    basicTreeView->setMinimumSize(300, 300);

    QStandardItemModel* basicModel = new QStandardItemModel(basicTreeView);
    basicModel->setHorizontalHeaderLabels({"名称"});

    QStandardItem* root1 = new QStandardItem("文档");
    root1->appendRow(new QStandardItem("快速开始"));
    root1->appendRow(new QStandardItem("API 参考"));
    root1->appendRow(new QStandardItem("示例"));
    basicModel->appendRow(root1);

    QStandardItem* root2 = new QStandardItem("项目");
    QStandardItem* src = new QStandardItem("src");
    src->appendRow(new QStandardItem("main.cpp"));
    src->appendRow(new QStandardItem("utils.cpp"));
    root2->appendRow(src);
    root2->appendRow(new QStandardItem("CMakeLists.txt"));
    basicModel->appendRow(root2);

    QStandardItem* root3 = new QStandardItem("设置");
    root3->appendRow(new QStandardItem("常规"));
    root3->appendRow(new QStandardItem("高级"));
    basicModel->appendRow(root3);

    basicTreeView->setModel(basicModel);
    basicTreeView->expandAll();

    basicLayout->addWidget(basicTreeView);
    basicLayout->addStretch();

    addSection("基本树形视图", basicLayout);

    QHBoxLayout* selectionLayout = new QHBoxLayout();
    selectionLayout->setSpacing(16);

    NTreeView* selectionTreeView = new NTreeView(this);
    selectionTreeView->setMinimumSize(300, 300);
    selectionTreeView->setSelectionMode(QAbstractItemView::MultiSelection);

    QStandardItemModel* selectionModel = new QStandardItemModel(selectionTreeView);
    selectionModel->setHorizontalHeaderLabels({"项目"});

    for (int i = 1; i <= 3; ++i) {
        QStandardItem* parent = new QStandardItem(QString("分类 %1").arg(i));
        for (int j = 1; j <= 3; ++j) {
            parent->appendRow(new QStandardItem(QString("子项 %1-%2").arg(i).arg(j)));
        }
        selectionModel->appendRow(parent);
    }

    selectionTreeView->setModel(selectionModel);
    selectionTreeView->expandAll();

    QLabel* selectionLabel = new QLabel("选中项: 无", this);
    connect(selectionTreeView->selectionModel(), &QItemSelectionModel::currentChanged,
            [selectionLabel](const QModelIndex& current, const QModelIndex&) {
                if (current.isValid()) {
                    selectionLabel->setText("选中项: " + current.data().toString());
                } else {
                    selectionLabel->setText("选中项: 无");
                }
            });

    QVBoxLayout* selectionInfoLayout = new QVBoxLayout();
    selectionInfoLayout->addWidget(selectionLabel);
    selectionInfoLayout->addStretch();

    selectionLayout->addWidget(selectionTreeView);
    selectionLayout->addLayout(selectionInfoLayout);
    selectionLayout->addStretch();

    addSection("多选模式", selectionLayout);
}
