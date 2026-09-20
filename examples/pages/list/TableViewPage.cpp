#include "TableViewPage.h"

#include <QAbstractTableModel>
#include <QHBoxLayout>
#include <algorithm>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QtNativeUI/NCheckBox.h>
#include <QtNativeUI/NFluentColors.h>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NSlider.h>
#include <QtNativeUI/NTableView.h>
#include <QtNativeUI/NTheme.h>

namespace {

class DemoFileTableModel final : public QAbstractTableModel {
  public:
    explicit DemoFileTableModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {
        m_headers = {
            QStringLiteral("名称"), QStringLiteral("修改日期"), QStringLiteral("类型"), QStringLiteral("大小")};
        for (int i = 1; i <= 48; ++i) {
            m_rows.append({QStringLiteral("项目文件 %1.txt").arg(i),
                           QStringLiteral("2026-03-%1").arg((i % 28) + 1),
                           i % 3 == 0 ? QStringLiteral("文档") : QStringLiteral("报告"),
                           QStringLiteral("%1 KB").arg(12 + i * 3)});
        }
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return parent.isValid() ? 0 : m_rows.size();
    }
    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return parent.isValid() ? 0 : m_headers.size();
    }
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_rows.size() || index.column() >= m_headers.size())
            return {};
        if (role == Qt::DisplayRole)
            return m_rows.at(index.row()).at(index.column());
        return {};
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (orientation != Qt::Horizontal || section < 0 || section >= m_headers.size())
            return {};
        if (role == Qt::DisplayRole)
            return m_headers.at(section);
        return {};
    }

    void sort(int column, Qt::SortOrder order) override {
        if (column < 0 || column >= m_headers.size())
            return;
        const auto compare = [column](const QStringList& left, const QStringList& right) {
            const QString a = left.at(column);
            const QString b = right.at(column);
            if (column == 3) {
                const int ka = a.section(QLatin1Char(' '), 0, 0).toInt();
                const int kb = b.section(QLatin1Char(' '), 0, 0).toInt();
                return ka < kb;
            }
            return QString::localeAwareCompare(a, b) < 0;
        };
        beginResetModel();
        std::stable_sort(m_rows.begin(), m_rows.end(), [order, compare](const QStringList& left, const QStringList& right) {
            const bool less = compare(left, right);
            return order == Qt::AscendingOrder ? less : !less;
        });
        endResetModel();
    }

  private:
    QStringList        m_headers;
    QList<QStringList> m_rows;
};

NTableView* makePrimaryTable(QWidget* parent, QAbstractItemModel* model) {
    auto* table = new NTableView(parent);
    table->setFixedSize(680, 252);
    table->setModel(model);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setColumnWidth(0, 170);
    table->setColumnWidth(1, 130);
    table->setColumnWidth(2, 110);
    return table;
}

void refreshTableChrome(NTableView* table) {
    if (!table)
        return;
    if (table->viewport())
        table->viewport()->update();
    if (table->horizontalHeader())
        table->horizontalHeader()->viewport()->update();
    if (table->verticalHeader() && table->isVerticalHeaderVisible())
        table->verticalHeader()->viewport()->update();
    table->update();
}

void applyFluentTableChrome(NTableView* table) {
    if (!table)
        return;
    table->setLightBackgroundColor(NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Light));
    table->setDarkBackgroundColor(NThemeColor(NFluentColorKey::ControlSolidFillColorDefault, NThemeType::Dark));
    table->setLightHeaderBackgroundColor(NThemeColor(NFluentColorKey::LayerFillColorAlt, NThemeType::Light));
    table->setDarkHeaderBackgroundColor(NThemeColor(NFluentColorKey::LayerFillColorAlt, NThemeType::Dark));
    table->setLightItemSelectedColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light));
    table->setDarkItemSelectedColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark));
    table->setLightItemHoverColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light));
    table->setDarkItemHoverColor(NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark));
    table->setLightBorderColor(NThemeColor(NFluentColorKey::CardStrokeColorDefaultSolid, NThemeType::Light));
    table->setDarkBorderColor(NThemeColor(NFluentColorKey::CardStrokeColorDefaultSolid, NThemeType::Dark));
    table->setLightTextColor(NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light));
    table->setDarkTextColor(NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark));
    table->setLightHeaderTextColor(NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light));
    table->setDarkHeaderTextColor(NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark));
    table->setBorderRadius(8);
    table->setRowHeight(36);
    table->setHeaderHeight(36);
    table->setCellHorizontalPadding(12);
    refreshTableChrome(table);
}

void applyBrandTableChrome(NTableView* table) {
    if (!table)
        return;
    table->setLightBackgroundColor(QColor(255, 251, 253));
    table->setDarkBackgroundColor(QColor(36, 28, 40));
    table->setLightHeaderBackgroundColor(QColor(255, 236, 244));
    table->setDarkHeaderBackgroundColor(QColor(52, 38, 56));
    table->setLightItemHoverColor(QColor(255, 228, 238));
    table->setDarkItemHoverColor(QColor(68, 48, 72));
    table->setLightItemSelectedColor(QColor(255, 210, 228));
    table->setDarkItemSelectedColor(QColor(92, 58, 98));
    table->setLightBorderColor(QColor(214, 96, 140));
    table->setDarkBorderColor(QColor(180, 110, 150));
    table->setLightTextColor(QColor(72, 24, 48));
    table->setDarkTextColor(QColor(255, 232, 242));
    table->setLightHeaderTextColor(QColor(120, 48, 72));
    table->setDarkHeaderTextColor(QColor(255, 200, 220));
    table->setBorderRadius(14);
    table->setRowHeight(44);
    table->setHeaderHeight(40);
    table->setCellHorizontalPadding(18);
    refreshTableChrome(table);
}

} // namespace

TableViewPage::TableViewPage(QWidget* parent)
    : BasePage("TableView 表格", "验证行选择、排序、滚动、单元格编辑、空状态与自定义样式属性。", parent) {
    QHBoxLayout* primaryLayout = new QHBoxLayout();
    primaryLayout->setSpacing(16);
    DemoFileTableModel* fileModel = new DemoFileTableModel(this);
    NTableView*         primary   = makePrimaryTable(this, fileModel);
    primary->setAccessibleName(QStringLiteral("大型只读表格"));
    QLabel* selectionLabel = new QLabel(QStringLiteral("当前行: （未选择）"), this);
    connect(primary->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            [selectionLabel, fileModel](const QModelIndex& current, const QModelIndex&) {
                if (!current.isValid()) {
                    selectionLabel->setText(QStringLiteral("当前行: （未选择）"));
                    return;
                }
                selectionLabel->setText(
                    QStringLiteral("当前行: %1").arg(fileModel->data(fileModel->index(current.row(), 0)).toString()));
            });
    QVBoxLayout* primaryInfo = new QVBoxLayout();
    primaryInfo->addWidget(
        new QLabel(QStringLiteral("点击列标题排序（升序/降序切换）；拖拽调整列宽；滚轮滚动。"), this));
    primaryInfo->addWidget(selectionLabel);
    primaryInfo->addStretch();
    primaryLayout->addWidget(primary);
    primaryLayout->addLayout(primaryInfo);
    primaryLayout->addStretch();
    addSection(QStringLiteral("行选择与排序"), primaryLayout);

    QHBoxLayout*        multiLayout  = new QHBoxLayout();
    QStandardItemModel* projectModel = new QStandardItemModel(this);
    projectModel->setHorizontalHeaderLabels({QStringLiteral("项目"), QStringLiteral("负责人"), QStringLiteral("状态")});
    const QStringList projects = {QStringLiteral("Fluent UI"), QStringLiteral("QtNativeUI"), QStringLiteral("Gallery")};
    const QStringList owners   = {QStringLiteral("Alice"), QStringLiteral("Bob"), QStringLiteral("Carol")};
    const QStringList status   = {QStringLiteral("进行中"), QStringLiteral("已完成"), QStringLiteral("待办")};
    for (int i = 0; i < projects.size(); ++i) {
        projectModel->appendRow(
            {new QStandardItem(projects.at(i)), new QStandardItem(owners.at(i)), new QStandardItem(status.at(i))});
    }
    NTableView* multi = makePrimaryTable(this, projectModel);
    multi->setSelectionMode(QAbstractItemView::ExtendedSelection);
    multi->setAccessibleName(QStringLiteral("多选表格"));
    multiLayout->addWidget(multi);
    multiLayout->addStretch();
    addSection(QStringLiteral("多行选择 (Extended)"), multiLayout);

    QHBoxLayout*        editLayout = new QHBoxLayout();
    QStandardItemModel* editModel  = new QStandardItemModel(this);
    editModel->setHorizontalHeaderLabels(
        {QStringLiteral("任务"), QStringLiteral("负责人"), QStringLiteral("进度 %")});
    const QStringList editTasks  = {QStringLiteral("接口联调"), QStringLiteral("UI 走查"), QStringLiteral("文档补全")};
    const QStringList editOwners = {QStringLiteral("Alice"), QStringLiteral("Bob"), QStringLiteral("Carol")};
    const QStringList editProgress = {QStringLiteral("35"), QStringLiteral("80"), QStringLiteral("10")};
    for (int i = 0; i < editTasks.size(); ++i) {
        auto* task = new QStandardItem(editTasks.at(i));
        auto* owner = new QStandardItem(editOwners.at(i));
        auto* progress = new QStandardItem(editProgress.at(i));
        task->setEditable(true);
        owner->setEditable(true);
        progress->setEditable(true);
        editModel->appendRow({task, owner, progress});
    }
    NTableView* editable = new NTableView(this);
    editable->setMinimumSize(520, 180);
    editable->setModel(editModel);
    editable->setSelectionBehavior(QAbstractItemView::SelectItems);
    editable->setSelectionMode(QAbstractItemView::SingleSelection);
    editable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked
                              | QAbstractItemView::EditKeyPressed);
    editable->horizontalHeader()->setStretchLastSection(true);
    editable->setColumnWidth(0, 180);
    editable->setColumnWidth(1, 120);
    editable->setAccessibleName(QStringLiteral("可编辑表格"));
    QVBoxLayout* editInfo = new QVBoxLayout();
    editInfo->addWidget(new QLabel(QStringLiteral("双击单元格、选中后单击或按 F2 进入编辑。"), this));
    editInfo->addStretch();
    editLayout->addWidget(editable);
    editLayout->addLayout(editInfo);
    editLayout->addStretch();
    addSection(QStringLiteral("单元格编辑"), editLayout);

    QHBoxLayout*        emptyLayout = new QHBoxLayout();
    QStandardItemModel* emptyModel  = new QStandardItemModel(this);
    emptyModel->setHorizontalHeaderLabels({QStringLiteral("列 A"), QStringLiteral("列 B")});
    NTableView* emptyTable = new NTableView(this);
    emptyTable->setMinimumSize(400, 180);
    emptyTable->setPlaceholderText(QStringLiteral("暂无记录，点击下方按钮加载数据"));
    emptyTable->setModel(emptyModel);
    NPushButton* loadButton = new NPushButton(QStringLiteral("加载示例数据"), this);
    connect(loadButton, &NPushButton::clicked, this, [emptyModel]() {
        if (emptyModel->rowCount() > 0)
            return;
        emptyModel->appendRow({new QStandardItem(QStringLiteral("条目 1")), new QStandardItem(QStringLiteral("值 1"))});
        emptyModel->appendRow({new QStandardItem(QStringLiteral("条目 2")), new QStandardItem(QStringLiteral("值 2"))});
    });
    QVBoxLayout* emptySide = new QVBoxLayout();
    emptySide->addWidget(emptyTable);
    emptySide->addWidget(loadButton);
    emptyLayout->addLayout(emptySide);
    emptyLayout->addStretch();
    addSection(QStringLiteral("空状态占位"), emptyLayout);

    QHBoxLayout* chromeLayout = new QHBoxLayout();
    NTableView*  chromeTable  = makePrimaryTable(this, new DemoFileTableModel(this));
    chromeTable->setMinimumSize(480, 200);
    NCheckBox* borderCheck = new NCheckBox(QStringLiteral("显示边框"), this);
    borderCheck->setChecked(true);
    NCheckBox* backgroundCheck = new NCheckBox(QStringLiteral("显示背景"), this);
    backgroundCheck->setChecked(true);
    NCheckBox* rowHeaderCheck = new NCheckBox(QStringLiteral("显示行号列"), this);
    rowHeaderCheck->setChecked(false);
    connect(borderCheck, &NCheckBox::toggled, chromeTable, [chromeTable](bool on) { chromeTable->setBorderVisible(on); });
    connect(backgroundCheck, &NCheckBox::toggled, chromeTable,
            [chromeTable](bool on) { chromeTable->setBackgroundVisible(on); });
    connect(rowHeaderCheck, &NCheckBox::toggled, chromeTable,
            [chromeTable](bool on) { chromeTable->setVerticalHeaderVisible(on); });
    QVBoxLayout* chromeOptions = new QVBoxLayout();
    chromeOptions->addWidget(borderCheck);
    chromeOptions->addWidget(backgroundCheck);
    chromeOptions->addWidget(rowHeaderCheck);
    chromeOptions->addStretch();
    chromeLayout->addWidget(chromeTable);
    chromeLayout->addLayout(chromeOptions);
    chromeLayout->addStretch();
    addSection(QStringLiteral("边框与背景"), chromeLayout);

    QHBoxLayout*        styleLayout = new QHBoxLayout();
    QStandardItemModel* styleModel  = new QStandardItemModel(this);
    styleModel->setHorizontalHeaderLabels({QStringLiteral("模块"), QStringLiteral("版本"), QStringLiteral("说明")});
    styleModel->appendRow({new QStandardItem(QStringLiteral("核心库")),
                           new QStandardItem(QStringLiteral("1.2.0")),
                           new QStandardItem(QStringLiteral("主题与控件"))});
    styleModel->appendRow({new QStandardItem(QStringLiteral("示例程序")),
                           new QStandardItem(QStringLiteral("0.9.3")),
                           new QStandardItem(QStringLiteral("Gallery 演示"))});
    styleModel->appendRow({new QStandardItem(QStringLiteral("文档站点")),
                           new QStandardItem(QStringLiteral("—")),
                           new QStandardItem(QStringLiteral("VitePress"))});
    NTableView* styleTable = new NTableView(this);
    styleTable->setMinimumSize(500, 200);
    styleTable->setModel(styleModel);
    styleTable->horizontalHeader()->setStretchLastSection(true);
    styleTable->setColumnWidth(0, 120);
    styleTable->setColumnWidth(1, 80);
    styleTable->setAccessibleName(QStringLiteral("自定义样式表格"));

    QLabel* radiusLabel = new QLabel(QStringLiteral("圆角: 8"), this);
    NSlider* radiusSlider = new NSlider(Qt::Horizontal, this);
    radiusSlider->setRange(0, 16);
    radiusSlider->setValue(styleTable->getBorderRadius());
    QLabel* rowHeightLabel = new QLabel(QStringLiteral("行高: 36"), this);
    NSlider* rowHeightSlider = new NSlider(Qt::Horizontal, this);
    rowHeightSlider->setRange(28, 52);
    rowHeightSlider->setValue(styleTable->getRowHeight());
    QLabel* paddingLabel = new QLabel(QStringLiteral("左右内边距: 12"), this);
    NSlider* paddingSlider = new NSlider(Qt::Horizontal, this);
    paddingSlider->setRange(4, 28);
    paddingSlider->setValue(styleTable->getCellHorizontalPadding());

    connect(radiusSlider, &NSlider::valueChanged, this, [styleTable, radiusLabel](int value) {
        styleTable->setBorderRadius(value);
        radiusLabel->setText(QStringLiteral("圆角: %1").arg(value));
    });
    connect(rowHeightSlider, &NSlider::valueChanged, this, [styleTable, rowHeightLabel](int value) {
        styleTable->setRowHeight(value);
        rowHeightLabel->setText(QStringLiteral("行高: %1").arg(value));
    });
    connect(paddingSlider, &NSlider::valueChanged, this, [styleTable, paddingLabel](int value) {
        styleTable->setCellHorizontalPadding(value);
        paddingLabel->setText(QStringLiteral("左右内边距: %1").arg(value));
    });

    NPushButton* brandButton = new NPushButton(QStringLiteral("品牌色预设"), this);
    NPushButton* resetButton = new NPushButton(QStringLiteral("Fluent 默认"), this);
    connect(brandButton, &NPushButton::clicked, this, [styleTable, radiusSlider, rowHeightSlider, paddingSlider,
                                                       radiusLabel, rowHeightLabel, paddingLabel]() {
        applyBrandTableChrome(styleTable);
        radiusSlider->setValue(styleTable->getBorderRadius());
        rowHeightSlider->setValue(styleTable->getRowHeight());
        paddingSlider->setValue(styleTable->getCellHorizontalPadding());
        radiusLabel->setText(QStringLiteral("圆角: %1").arg(styleTable->getBorderRadius()));
        rowHeightLabel->setText(QStringLiteral("行高: %1").arg(styleTable->getRowHeight()));
        paddingLabel->setText(QStringLiteral("左右内边距: %1").arg(styleTable->getCellHorizontalPadding()));
    });
    connect(resetButton, &NPushButton::clicked, this, [styleTable, radiusSlider, rowHeightSlider, paddingSlider,
                                                       radiusLabel, rowHeightLabel, paddingLabel]() {
        applyFluentTableChrome(styleTable);
        radiusSlider->setValue(styleTable->getBorderRadius());
        rowHeightSlider->setValue(styleTable->getRowHeight());
        paddingSlider->setValue(styleTable->getCellHorizontalPadding());
        radiusLabel->setText(QStringLiteral("圆角: %1").arg(styleTable->getBorderRadius()));
        rowHeightLabel->setText(QStringLiteral("行高: %1").arg(styleTable->getRowHeight()));
        paddingLabel->setText(QStringLiteral("左右内边距: %1").arg(styleTable->getCellHorizontalPadding()));
    });

    QVBoxLayout* styleControls = new QVBoxLayout();
    styleControls->addWidget(new QLabel(QStringLiteral("通过 Light/Dark 颜色属性、圆角、行高、内边距自定义外观。"), this));
    styleControls->addWidget(radiusLabel);
    styleControls->addWidget(radiusSlider);
    styleControls->addWidget(rowHeightLabel);
    styleControls->addWidget(rowHeightSlider);
    styleControls->addWidget(paddingLabel);
    styleControls->addWidget(paddingSlider);
    QHBoxLayout* presetRow = new QHBoxLayout();
    presetRow->addWidget(brandButton);
    presetRow->addWidget(resetButton);
    presetRow->addStretch();
    styleControls->addLayout(presetRow);
    styleControls->addStretch();

    styleLayout->addWidget(styleTable);
    styleLayout->addLayout(styleControls);
    styleLayout->addStretch();
    addSection(QStringLiteral("自定义样式"), styleLayout);
}
