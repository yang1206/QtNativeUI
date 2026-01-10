#include "view.h"
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QTimer>
#include <QVBoxLayout>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NLabel.h>
#include <QtNativeUI/NListView.h>
#include <QtNativeUI/NOverlay.h>
#include <QtNativeUI/NProgressRing.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NScrollArea.h>
#include <QtNativeUI/NSlider.h>
#include <QtNativeUI/NSpinBox.h>
#include <QtNativeUI/NTreeView.h>
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
    contentLayout->addWidget(new ExampleSection("TreeView", createTreeViewSection()));
    contentLayout->addWidget(new ExampleSection("Overlay", createOverlaySection()));

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

QWidget* ViewExample::createTreeViewSection() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    {
        layout->addWidget(new NLabel("基础 TreeView", NLabelType::Subtitle));

        NTreeView* treeView = new NTreeView(container);
        treeView->setMinimumHeight(250);
        treeView->setMaximumHeight(350);

        QStandardItemModel* model = new QStandardItemModel(treeView);

        QStandardItem* root1 = new QStandardItem("文档");
        root1->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
        root1->appendRow(new QStandardItem("报告.docx"));
        root1->appendRow(new QStandardItem("计划.xlsx"));
        root1->appendRow(new QStandardItem("笔记.txt"));

        QStandardItem* root2 = new QStandardItem("图片");
        root2->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
        QStandardItem* subFolder = new QStandardItem("旅行");
        subFolder->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
        subFolder->appendRow(new QStandardItem("photo1.jpg"));
        subFolder->appendRow(new QStandardItem("photo2.jpg"));
        root2->appendRow(subFolder);
        root2->appendRow(new QStandardItem("wallpaper.png"));

        QStandardItem* root3 = new QStandardItem("音乐");
        root3->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
        root3->appendRow(new QStandardItem("song1.mp3"));
        root3->appendRow(new QStandardItem("song2.mp3"));

        model->appendRow(root1);
        model->appendRow(root2);
        model->appendRow(root3);

        treeView->setModel(model);
        treeView->expandAll();

        layout->addWidget(treeView);
    }

    {
        layout->addWidget(new NLabel("可配置 TreeView", NLabelType::Subtitle));

        NTreeView* treeView = new NTreeView(container);
        treeView->setMinimumHeight(250);
        treeView->setMaximumHeight(350);

        QStandardItemModel* model = new QStandardItemModel(treeView);

        QStandardItem* root1 = new QStandardItem("项目");
        root1->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
        QStandardItem* src = new QStandardItem("src");
        src->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
        src->appendRow(new QStandardItem("main.cpp"));
        src->appendRow(new QStandardItem("utils.cpp"));
        root1->appendRow(src);
        root1->appendRow(new QStandardItem("README.md"));

        QStandardItem* root2 = new QStandardItem("资源");
        root2->setIcon(nIcon->fromRegular(NRegularIconType::Folder24Regular));
        root2->appendRow(new QStandardItem("icon.png"));
        root2->appendRow(new QStandardItem("style.qss"));

        model->appendRow(root1);
        model->appendRow(root2);

        treeView->setModel(model);
        treeView->expandAll();

        layout->addWidget(treeView);

        auto createSliderRow = [](const QString& label, int min, int max, int value) {
            QWidget*     row       = new QWidget;
            QHBoxLayout* rowLayout = new QHBoxLayout(row);
            rowLayout->setContentsMargins(0, 0, 0, 0);

            NLabel* nameLabel = new NLabel(label);
            nameLabel->setFixedWidth(120);

            NSlider* slider = new NSlider(Qt::Horizontal);
            slider->setRange(min, max);
            slider->setValue(value);

            NSpinBox* spinBox = new NSpinBox;
            spinBox->setRange(min, max);
            spinBox->setValue(value);
            spinBox->setFixedWidth(80);

            QObject::connect(slider, &NSlider::valueChanged, spinBox, &NSpinBox::setValue);
            QObject::connect(spinBox, QOverload<int>::of(&NSpinBox::valueChanged), slider, &NSlider::setValue);

            rowLayout->addWidget(nameLabel);
            rowLayout->addWidget(slider, 1);
            rowLayout->addWidget(spinBox);

            return std::make_tuple(row, slider, spinBox);
        };

        auto [itemHeightRow, itemHeightSlider, itemHeightSpinBox] = createSliderRow("ItemHeight", 24, 60, 32);
        layout->addWidget(itemHeightRow);
        connect(itemHeightSlider, &NSlider::valueChanged, treeView, &NTreeView::setItemHeight);

        auto [borderRadiusRow, borderRadiusSlider, borderRadiusSpinBox] = createSliderRow("BorderRadius", 0, 20, 8);
        layout->addWidget(borderRadiusRow);
        connect(borderRadiusSlider, &NSlider::valueChanged, treeView, &NTreeView::setBorderRadius);

        auto [itemBorderRadiusRow, itemBorderRadiusSlider, itemBorderRadiusSpinBox] =
            createSliderRow("ItemBorderRadius", 0, 12, 4);
        layout->addWidget(itemBorderRadiusRow);
        connect(itemBorderRadiusSlider, &NSlider::valueChanged, treeView, &NTreeView::setItemBorderRadius);

        auto [indentationRow, indentationSlider, indentationSpinBox] = createSliderRow("Indentation", 10, 40, 20);
        layout->addWidget(indentationRow);
        connect(indentationSlider, &NSlider::valueChanged, treeView, [treeView](int value) {
            treeView->setIndentation(value);
        });
    }

    {
        layout->addWidget(new NLabel("文件系统 TreeView", NLabelType::Subtitle));

        NTreeView* treeView = new NTreeView(container);
        treeView->setMinimumHeight(300);
        treeView->setMaximumHeight(400);

        QFileSystemModel* model = new QFileSystemModel(treeView);
        model->setRootPath(QDir::homePath());
        model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

        treeView->setModel(model);
        treeView->setRootIndex(model->index(QDir::homePath()));
        treeView->setColumnHidden(1, true);
        treeView->setColumnHidden(2, true);
        treeView->setColumnHidden(3, true);

        layout->addWidget(treeView);
    }

    return container;
}

QWidget* ViewExample::createOverlaySection() {
    QWidget*     container = new QWidget;
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(16);

    {
        layout->addWidget(new NLabel("基础遮罩层", NLabelType::Subtitle));

        QHBoxLayout* btnLayout = new QHBoxLayout();

        NPushButton* showBtn = new NPushButton("显示遮罩层 (点击关闭)", container);
        connect(showBtn, &NPushButton::clicked, this, [this]() {
            NOverlay* overlay = new NOverlay(window());
            overlay->setAttribute(Qt::WA_DeleteOnClose);
            overlay->showOverlay();
        });
        btnLayout->addWidget(showBtn);

        NPushButton* loadingBtn = new NPushButton("显示加载遮罩 (3秒后关闭)", container);
        connect(loadingBtn, &NPushButton::clicked, this, [this, loadingBtn]() {
            NOverlay* overlay = new NOverlay(window());
            overlay->setAttribute(Qt::WA_DeleteOnClose);
            overlay->showLoading("正在加载数据...");

            QTimer::singleShot(3000, overlay, &NOverlay::close);
        });
        btnLayout->addWidget(loadingBtn);

        NPushButton* localLoadingBtn = new NPushButton("局部加载遮罩", container);
        connect(localLoadingBtn, &NPushButton::clicked, this, [this, localLoadingBtn]() {
            NOverlay* overlay = new NOverlay();
            overlay->setAttribute(Qt::WA_DeleteOnClose);
            overlay->showLoading(localLoadingBtn, "按钮加载中...");

            QTimer::singleShot(2000, overlay, &NOverlay::close);
        });
        btnLayout->addWidget(localLoadingBtn);

        btnLayout->addStretch();
        layout->addLayout(btnLayout);
    }

    {
        layout->addWidget(new NLabel("自定义颜色遮罩", NLabelType::Subtitle));

        QHBoxLayout* btnLayout = new QHBoxLayout();

        NPushButton* blueBtn = new NPushButton("蓝色遮罩", container);
        connect(blueBtn, &NPushButton::clicked, this, [this]() {
            NOverlay* overlay = new NOverlay(window());
            overlay->setAttribute(Qt::WA_DeleteOnClose);
            overlay->setLightOverlayColor(QColor(0, 100, 200));
            overlay->setDarkOverlayColor(QColor(0, 80, 160));
            overlay->setOpacity(120);
            overlay->showOverlay();
        });
        btnLayout->addWidget(blueBtn);

        NPushButton* redBtn = new NPushButton("红色遮罩", container);
        connect(redBtn, &NPushButton::clicked, this, [this]() {
            NOverlay* overlay = new NOverlay(window());
            overlay->setAttribute(Qt::WA_DeleteOnClose);
            overlay->setLightOverlayColor(QColor(200, 50, 50));
            overlay->setDarkOverlayColor(QColor(160, 40, 40));
            overlay->setOpacity(100);
            overlay->showOverlay();
        });
        btnLayout->addWidget(redBtn);

        btnLayout->addStretch();
        layout->addLayout(btnLayout);
    }

    return container;
}
