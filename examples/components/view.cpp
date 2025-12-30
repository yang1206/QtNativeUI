#include "view.h"
#include <QStandardItemModel>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NScrollArea.h>
#include "widgets/ExampleSection.h"

ViewExample::ViewExample(QWidget* parent) : QWidget(parent) { initUI(); }

void ViewExample::initUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_scrollArea = new NScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget*     contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    contentLayout->addWidget(new ExampleSection("ListView", createListViewSection()));

    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);
}

QWidget* ViewExample::createListViewSection() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    // 基础 ListView 示例
    {
        layout->addWidget(new NLabel("基础 ListView", NLabelType::Subtitle));

        NListView* listView = new NListView(container);
        listView->setMinimumHeight(200);
        listView->setMaximumHeight(300);

        QStringListModel* model = new QStringListModel(listView);
        QStringList       items;
        items << "Item 1"
              << "Item 2"
              << "Item 3"
              << "Item 4"
              << "Item 5"
              << "Item 6"
              << "Item 7"
              << "Item 8"
              << "Item 9"
              << "Item 10";
        model->setStringList(items);
        listView->setModel(model);
        layout->addWidget(listView);
    }

    // 带图标的 ListView 示例
    {
        layout->addWidget(new NLabel("带图标的 ListView", NLabelType::Subtitle));

        NListView* listView = new NListView(container);
        listView->setMinimumHeight(200);
        listView->setMaximumHeight(300);

        QStandardItemModel* model = new QStandardItemModel(listView);

        struct ItemData {
            QString                text;
            NRegularIconType::Icon iconType;
        };

        QList<ItemData> items = {
            {"首页", NRegularIconType::Home24Regular},
            {"文档", NRegularIconType::Document24Regular},
            {"设置", NRegularIconType::Settings24Regular},
            {"用户", NRegularIconType::Person24Regular},
            {"日历", NRegularIconType::Calendar24Regular},
            {"邮件", NRegularIconType::Mail24Regular},
            {"搜索", NRegularIconType::Search24Regular},
            {"收藏", NRegularIconType::Star24Regular},
        };

        for (const auto& item : items) {
            QStandardItem* stdItem = new QStandardItem(item.text);
            stdItem->setIcon(nIcon->fromRegular(item.iconType));
            model->appendRow(stdItem);
        }

        listView->setModel(model);
        listView->setIconSize(QSize(35, 35));

        layout->addWidget(listView);
    }

    // 多选 ListView 示例
    {
        layout->addWidget(new NLabel("多选 ListView", NLabelType::Subtitle));

        NListView* listView = new NListView(container);
        listView->setMinimumHeight(200);
        listView->setMaximumHeight(300);
        listView->setSelectionMode(QAbstractItemView::MultiSelection);

        QStringListModel* model = new QStringListModel(listView);
        QStringList       items;
        items << "Apple"
              << "Banana"
              << "Cherry"
              << "Date"
              << "Elderberry"
              << "Fig"
              << "Grape"
              << "Honeydew";
        model->setStringList(items);
        listView->setModel(model);

        layout->addWidget(listView);
    }

    return container;
}
