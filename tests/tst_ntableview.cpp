// Copyright (C) 2024 QtNativeUI Project
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <QAbstractTableModel>
#include <QApplication>
#include <QHeaderView>
#include <QSignalSpy>
#include <QStandardItemModel>
#include <QTest>

#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NTableView.h>
#include <QtNativeUI/NTheme.h>

namespace {

QStandardItemModel* makeFileModel(QObject* parent) {
    auto* model = new QStandardItemModel(3, 3, parent);
    model->setHorizontalHeaderLabels({QStringLiteral("名称"), QStringLiteral("类型"), QStringLiteral("大小")});
    const QStringList names{QStringLiteral("c.txt"), QStringLiteral("a.txt"), QStringLiteral("b.txt")};
    const QStringList types{QStringLiteral("文档"), QStringLiteral("报告"), QStringLiteral("文档")};
    const QStringList sizes{QStringLiteral("30"), QStringLiteral("10"), QStringLiteral("20")};
    for (int row = 0; row < names.size(); ++row) {
        model->setItem(row, 0, new QStandardItem(names.at(row)));
        model->setItem(row, 1, new QStandardItem(types.at(row)));
        model->setItem(row, 2, new QStandardItem(sizes.at(row)));
        for (int column = 0; column < 3; ++column)
            model->item(row, column)->setEditable(true);
    }
    return model;
}

class SortableTableModel final : public QAbstractTableModel {
  public:
    explicit SortableTableModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {
        m_headers = {QStringLiteral("名称"), QStringLiteral("值")};
        m_rows    = {{QStringLiteral("gamma"), QStringLiteral("3")},
                     {QStringLiteral("alpha"), QStringLiteral("1")},
                     {QStringLiteral("beta"), QStringLiteral("2")}};
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return parent.isValid() ? 0 : m_rows.size();
    }
    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return parent.isValid() ? 0 : m_headers.size();
    }
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
            return {};
        return m_rows.at(index.row()).at(index.column());
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};
        return m_headers.value(section);
    }
    void sort(int column, Qt::SortOrder order) override {
        if (column < 0 || column >= m_headers.size())
            return;
        beginResetModel();
        std::sort(m_rows.begin(), m_rows.end(), [column, order](const QStringList& left, const QStringList& right) {
            const int compared = QString::localeAwareCompare(left.at(column), right.at(column));
            return order == Qt::AscendingOrder ? compared < 0 : compared > 0;
        });
        endResetModel();
    }

  private:
    QStringList        m_headers;
    QList<QStringList> m_rows;
};

} // namespace

class tst_NTableView : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    void constructionAndDefaults();
    void layoutProperties();
    void chromeFlags();
    void colorProperties();
    void modelBinding();
    void headerRemainsVisibleAfterResize();
    void verticalHeaderToggle();
    void sortingWithStandardModel();
    void sortingWithCustomModel();
    void rowSelection();
    void placeholder();
    void editingWithLineEdit();
    void f2StartsEditWhenEnabled();
    void f2IgnoredWhenEditDisabled();
    void themeChangeKeepsLayout();
    void paintAndReplaceModel();

  private:
    NTableView*        m_table = nullptr;
    QStandardItemModel* m_model = nullptr;
};

void tst_NTableView::initTestCase() {
    m_table = new NTableView;
    m_table->setObjectName(QStringLiteral("testTable"));
    m_table->resize(480, 240);
    m_model = makeFileModel(this);
    m_table->setModel(m_model);
    m_table->show();
    QVERIFY(QTest::qWaitForWindowExposed(m_table));
}

void tst_NTableView::cleanupTestCase() {
    delete m_table;
    m_table = nullptr;
    m_model = nullptr;
}

void tst_NTableView::init() {
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setSortingEnabled(true);
    m_table->setVerticalHeaderVisible(false);
    m_table->setPlaceholderText(QString());
    m_table->setBorderVisible(true);
    m_table->setBackgroundVisible(true);
    if (m_table->model() != m_model)
        m_table->setModel(m_model);
    if (m_table->selectionModel())
        m_table->selectionModel()->clearSelection();
    QTest::keyClick(m_table, Qt::Key_Escape);
}

void tst_NTableView::constructionAndDefaults() {
    NTableView table;
    QCOMPARE(table.selectionBehavior(), QAbstractItemView::SelectRows);
    QCOMPARE(table.selectionMode(), QAbstractItemView::SingleSelection);
    QCOMPARE(table.editTriggers(), QAbstractItemView::NoEditTriggers);
    QVERIFY(table.isSortingEnabled());
    QVERIFY(!table.horizontalHeader()->isHidden());
    QVERIFY(table.verticalHeader()->isHidden());
    QVERIFY(!table.isVerticalHeaderVisible());
    QVERIFY(table.getBorderVisible());
    QVERIFY(table.getBackgroundVisible());
    QCOMPARE(table.getRowHeight(), 36);
    QCOMPARE(table.getHeaderHeight(), 36);
    QCOMPARE(table.getCellHorizontalPadding(), 12);
    QCOMPARE(table.getBorderRadius(), NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt());
    QVERIFY(table.showGrid() == false);
}

void tst_NTableView::layoutProperties() {
    QSignalSpy rowSpy(m_table, &NTableView::pRowHeightChanged);
    QSignalSpy headerSpy(m_table, &NTableView::pHeaderHeightChanged);
    QSignalSpy radiusSpy(m_table, &NTableView::pBorderRadiusChanged);
    QSignalSpy padSpy(m_table, &NTableView::pCellHorizontalPaddingChanged);

    m_table->setRowHeight(44);
    QCOMPARE(m_table->getRowHeight(), 44);
    QCOMPARE(m_table->verticalHeader()->defaultSectionSize(), 44);
    QCOMPARE(rowSpy.count(), 1);
    m_table->setRowHeight(44);
    QCOMPARE(rowSpy.count(), 1);

    m_table->setHeaderHeight(40);
    QCOMPARE(m_table->getHeaderHeight(), 40);
    QCOMPARE(m_table->horizontalHeader()->height(), 40);
    QCOMPARE(headerSpy.count(), 1);

    m_table->setBorderRadius(12);
    QCOMPARE(m_table->getBorderRadius(), 12);
    QCOMPARE(radiusSpy.count(), 1);

    m_table->setCellHorizontalPadding(16);
    QCOMPARE(m_table->getCellHorizontalPadding(), 16);
    QCOMPARE(padSpy.count(), 1);

    m_table->setRowHeight(36);
    m_table->setHeaderHeight(36);
    m_table->setBorderRadius(NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt());
    m_table->setCellHorizontalPadding(12);
}

void tst_NTableView::chromeFlags() {
    QSignalSpy borderSpy(m_table, &NTableView::pBorderVisibleChanged);
    QSignalSpy backgroundSpy(m_table, &NTableView::pBackgroundVisibleChanged);

    m_table->setBorderVisible(false);
    QVERIFY(!m_table->getBorderVisible());
    QCOMPARE(borderSpy.count(), 1);
    m_table->setBorderVisible(true);
    QVERIFY(m_table->getBorderVisible());

    m_table->setBackgroundVisible(false);
    QVERIFY(!m_table->getBackgroundVisible());
    QCOMPARE(backgroundSpy.count(), 1);
    m_table->setBackgroundVisible(true);
    QVERIFY(m_table->getBackgroundVisible());
}

void tst_NTableView::colorProperties() {
    const QColor color(128, 64, 32);
    m_table->setLightBackgroundColor(color);
    QCOMPARE(m_table->getLightBackgroundColor(), color);
    m_table->setDarkBackgroundColor(color);
    QCOMPARE(m_table->getDarkBackgroundColor(), color);
    m_table->setLightHeaderBackgroundColor(color);
    QCOMPARE(m_table->getLightHeaderBackgroundColor(), color);
    m_table->setLightItemSelectedColor(color);
    QCOMPARE(m_table->getLightItemSelectedColor(), color);
    m_table->setLightBorderColor(color);
    QCOMPARE(m_table->getLightBorderColor(), color);
    m_table->setLightTextColor(color);
    QCOMPARE(m_table->getLightTextColor(), color);
}

void tst_NTableView::modelBinding() {
    QCOMPARE(m_table->model(), m_model);
    QCOMPARE(m_table->model()->rowCount(), 3);
    QCOMPARE(m_table->model()->columnCount(), 3);
    QCOMPARE(m_table->model()->headerData(0, Qt::Horizontal).toString(), QStringLiteral("名称"));
    QCOMPARE(m_table->model()->index(0, 0).data().toString(), QStringLiteral("c.txt"));

    auto* next = new QStandardItemModel(1, 2, m_table);
    next->setHorizontalHeaderLabels({QStringLiteral("左"), QStringLiteral("右")});
    next->setItem(0, 0, new QStandardItem(QStringLiteral("x")));
    next->setItem(0, 1, new QStandardItem(QStringLiteral("y")));
    m_table->setModel(next);
    QCOMPARE(m_table->model(), next);
    QCOMPARE(m_table->horizontalHeader()->model(), next);
    m_table->setModel(m_model);
    QCOMPARE(m_table->model(), m_model);
}

void tst_NTableView::headerRemainsVisibleAfterResize() {
    QVERIFY(m_table->horizontalHeader()->isVisible());
    QVERIFY(m_table->horizontalHeader()->height() >= m_table->getHeaderHeight());
    QVERIFY(m_table->viewport()->geometry().top() >= m_table->horizontalHeader()->height() - 1);

    m_table->resize(360, 180);
    QApplication::processEvents();
    QVERIFY(m_table->horizontalHeader()->isVisible());
    QVERIFY(m_table->horizontalHeader()->height() >= m_table->getHeaderHeight());
    QVERIFY(m_table->viewport()->geometry().top() >= m_table->horizontalHeader()->height() - 1);
    QVERIFY(m_table->viewport()->isVisible());
    QVERIFY(m_table->viewport()->height() > 0);
}

void tst_NTableView::verticalHeaderToggle() {
    QVERIFY(!m_table->isVerticalHeaderVisible());
    m_table->setVerticalHeaderVisible(true);
    QVERIFY(m_table->isVerticalHeaderVisible());
    QCOMPARE(m_table->verticalHeader()->width(), 48);
    QVERIFY(m_table->viewport()->geometry().left() >= m_table->verticalHeader()->width() - 1);
    QVERIFY(m_table->horizontalHeader()->isVisible());

    m_table->setVerticalHeaderVisible(false);
    QVERIFY(!m_table->isVerticalHeaderVisible());
    QVERIFY(m_table->horizontalHeader()->isVisible());
}

void tst_NTableView::sortingWithStandardModel() {
    m_table->sortByColumn(0, Qt::AscendingOrder);
    QCOMPARE(m_model->item(0, 0)->text(), QStringLiteral("a.txt"));
    QCOMPARE(m_model->item(1, 0)->text(), QStringLiteral("b.txt"));
    QCOMPARE(m_model->item(2, 0)->text(), QStringLiteral("c.txt"));

    m_table->sortByColumn(0, Qt::DescendingOrder);
    QCOMPARE(m_model->item(0, 0)->text(), QStringLiteral("c.txt"));
    QCOMPARE(m_model->item(2, 0)->text(), QStringLiteral("a.txt"));
}

void tst_NTableView::sortingWithCustomModel() {
    SortableTableModel model;
    NTableView         table;
    table.setModel(&model);
    table.show();
    QVERIFY(QTest::qWaitForWindowExposed(&table));

    table.sortByColumn(0, Qt::AscendingOrder);
    QCOMPARE(model.index(0, 0).data().toString(), QStringLiteral("alpha"));
    QCOMPARE(model.index(1, 0).data().toString(), QStringLiteral("beta"));
    QCOMPARE(model.index(2, 0).data().toString(), QStringLiteral("gamma"));

    table.sortByColumn(1, Qt::DescendingOrder);
    QCOMPARE(model.index(0, 1).data().toString(), QStringLiteral("3"));
    QCOMPARE(model.index(2, 1).data().toString(), QStringLiteral("1"));
}

void tst_NTableView::rowSelection() {
    const QModelIndex index = m_model->index(1, 0);
    m_table->setCurrentIndex(index);
    QVERIFY(m_table->selectionModel()->isRowSelected(1, QModelIndex()));
    QCOMPARE(m_table->currentIndex().row(), 1);
}

void tst_NTableView::placeholder() {
    NTableView table;
    QVERIFY(!table.isShowingPlaceholder());
    table.setPlaceholderText(QStringLiteral("暂无数据"));
    QVERIFY(table.isShowingPlaceholder());

    auto* empty = new QStandardItemModel(&table);
    table.setModel(empty);
    QVERIFY(table.isShowingPlaceholder());

    empty->setColumnCount(1);
    empty->appendRow(new QStandardItem(QStringLiteral("row")));
    QVERIFY(!table.isShowingPlaceholder());
}

void tst_NTableView::editingWithLineEdit() {
    m_table->setEditTriggers(QAbstractItemView::AllEditTriggers);
    const QModelIndex index = m_model->index(0, 0);
    m_table->setCurrentIndex(index);
    m_table->setFocus(Qt::OtherFocusReason);
    QApplication::processEvents();
    m_table->edit(index);
    QApplication::processEvents();

    auto* editor = m_table->findChild<NLineEdit*>();
    QVERIFY(editor);
    editor->setText(QStringLiteral("edited.txt"));
    QTest::keyClick(editor, Qt::Key_Return);
    QApplication::processEvents();
    QCOMPARE(m_model->item(0, 0)->text(), QStringLiteral("edited.txt"));
}

void tst_NTableView::f2StartsEditWhenEnabled() {
    m_table->setEditTriggers(QAbstractItemView::EditKeyPressed);
    m_table->setCurrentIndex(m_model->index(0, 1));
    m_table->setFocus(Qt::OtherFocusReason);
    QTest::keyClick(m_table, Qt::Key_F2);
    QApplication::processEvents();
    auto* editor = m_table->findChild<NLineEdit*>();
    QVERIFY(editor);
    QTest::keyClick(editor, Qt::Key_Escape);
    QApplication::processEvents();
}

void tst_NTableView::f2IgnoredWhenEditDisabled() {
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setCurrentIndex(m_model->index(0, 1));
    m_table->setFocus(Qt::OtherFocusReason);
    QTest::keyClick(m_table, Qt::Key_F2);
    QApplication::processEvents();
    QVERIFY(m_table->findChild<NLineEdit*>() == nullptr);
}

void tst_NTableView::themeChangeKeepsLayout() {
    const auto original = nTheme->themeMode();
    nTheme->setThemeMode(NThemeType::ThemeMode::Dark);
    QApplication::processEvents();
    QVERIFY(m_table->horizontalHeader()->isVisible());
    QVERIFY(m_table->viewport()->geometry().top() >= m_table->horizontalHeader()->height() - 1);

    nTheme->setThemeMode(NThemeType::ThemeMode::Light);
    QApplication::processEvents();
    QVERIFY(m_table->horizontalHeader()->isVisible());
    nTheme->setThemeMode(original);
}

void tst_NTableView::paintAndReplaceModel() {
    m_table->update();
    m_table->viewport()->update();
    m_table->horizontalHeader()->viewport()->update();
    QApplication::processEvents();
    QVERIFY(m_table->horizontalHeader()->isVisible());

    m_table->setModel(nullptr);
    QVERIFY(m_table->isShowingPlaceholder() == false);
    m_table->setPlaceholderText(QStringLiteral("empty"));
    QVERIFY(m_table->isShowingPlaceholder());
    m_table->setModel(m_model);
    m_table->setPlaceholderText(QString());
    QVERIFY(!m_table->isShowingPlaceholder());
}

QTEST_MAIN(tst_NTableView)
#include "tst_ntableview.moc"
