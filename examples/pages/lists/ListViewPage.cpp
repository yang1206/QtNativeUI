#include "ListViewPage.h"
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NListView.h>

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
    QStringList items = {"首页", "文档", "设置", "帮助", "关于"};
    for (const QString& text : items) {
        QStandardItem* item = new QStandardItem(text);
        item->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
        iconModel->appendRow(item);
    }
    iconListView->setModel(iconModel);

    iconLayout->addWidget(iconListView);
    iconLayout->addStretch();

    addSection("带图标的列表", iconLayout);

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
    connect(interactiveListView->selectionModel(), &QItemSelectionModel::currentChanged,
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
}
