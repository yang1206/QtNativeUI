#include <QAbstractListModel>
#include <QAbstractItemDelegate>
#include <QContextMenuEvent>
#include <QCoreApplication>
#include <QImage>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QPointer>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QScrollBar>
#include <QSignalSpy>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTest>

#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NTreeView.h>

namespace {

void populate(QStandardItemModel& model, int rows = 4) {
    model.setColumnCount(1);
    for (int row = 0; row < rows; ++row)
        model.appendRow(new QStandardItem(QStringLiteral("item %1").arg(row)));
}

void showTree(NTreeView& view, QAbstractItemView::SelectionMode mode = QAbstractItemView::SingleSelection) {
    view.setSelectionMode(mode);
    view.resize(320, 220);
    view.setEditTriggers(QAbstractItemView::NoEditTriggers);
    view.show();
    QCoreApplication::processEvents();
}

QImage snapshot(NTreeView& view) {
    QImage image(view.viewport()->size(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    view.viewport()->render(&image);
    return image;
}

bool hasMarkAt(NTreeView& view, const QModelIndex& index) {
    const QRect row = view.visualRect(index);
    // 读取标记内部的实色像素，避开圆角和抗锯齿边缘。
    const QPoint sample(row.left() + 5, row.center().y());
    if (row.isEmpty() || !view.viewport()->rect().contains(sample))
        return false;
    return snapshot(view).pixelColor(sample).rgb() == nTheme->accentColor().normal().rgb();
}

QStandardItem* checkItem(const QString& text) {
    auto* item = new QStandardItem(text);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setEditable(false);
    return item;
}

void pressSpace(NTreeView& view) {
    QKeyEvent event(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
    QCoreApplication::sendEvent(&view, &event);
}

class CountingTreeView : public NTreeView {
  public:
    mutable int rectQueries{0};

    QRect visualRect(const QModelIndex& index) const override {
        ++rectQueries;
        return NTreeView::visualRect(index);
    }
};

class MovableModel : public QAbstractListModel {
  public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return parent.isValid() ? 0 : m_rows.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        return index.isValid() && role == Qt::DisplayRole ? QVariant(m_rows.at(index.row())) : QVariant();
    }

    bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count,
                  const QModelIndex& destinationParent, int destinationChild) override {
        if (sourceParent.isValid() || destinationParent.isValid() || count != 1
            || sourceRow < 0 || sourceRow >= m_rows.size() || destinationChild < 0
            || destinationChild > m_rows.size())
            return false;
        if (!beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild))
            return false;
        m_rows.move(sourceRow, destinationChild > sourceRow ? destinationChild - 1 : destinationChild);
        endMoveRows();
        return true;
    }

  private:
    QStringList m_rows{QStringLiteral("a"), QStringLiteral("b"), QStringLiteral("c")};
};

} // namespace

class tst_NTreeView : public QObject {
    Q_OBJECT

  private slots:
    void clearAndRefillAfterRightClick_data();
    void clearAndRefillAfterRightClick();
    void removePreviousDuringTransition();
    void removeCurrentAndParentDuringTransition();
    void sortingAndColumnChanges();
    void rowMovement();
    void proxyFilteringDuringTransition();
    void replaceModelAndSelectionModel();
    void destroyModelAndSelectionModel();
    void resetAndRootPlaceholder();
    void mousePressUsesActualHitAndButton();
    void rightClickPreservesSelection_data();
    void rightClickPreservesSelection();
    void keyboardSelectionAndScrolling();
    void revealInterruptedByModelChanges();
    void expandingAnotherBranchFinalizesPendingCollapse();
    void multiSelectionPaintVisitsOnlyVisibleRows();
    void checkStateControlsMultiSelectionAppearance();
    void mouseAndKeyboardChecksShareParentState();
    void largeCheckOperationYieldsAndCancelsOnReset();
    void resetFromCheckDataChangedIsSafe();
    void revealGeometryAndAnimationSwitch();
    void unrelatedInsertionKeepsReveal();
    void largeRevealVisitsOnlyViewportRows();
    void customStyleIsDestroyedWithView();
    void multiColumnContentsAndEditorStayInsideCell();
    void repeatedCheckUsesLastRequestedState();
};

void tst_NTreeView::clearAndRefillAfterRightClick_data() {
    QTest::addColumn<QString>("placeholder");
    QTest::newRow("without-placeholder") << QString();
    QTest::newRow("with-placeholder") << QStringLiteral("暂无记录");
}

void tst_NTreeView::clearAndRefillAfterRightClick() {
    QFETCH(QString, placeholder);
    QStandardItemModel model;
    NTreeView view;
    view.setPlaceholderText(placeholder);
    view.setModel(&model);
    showTree(view);
    // 同时覆盖旧标记仍在过渡、清空后的立即绘制、快速重填和重新选择。
    for (int iteration = 0; iteration < 20; ++iteration) {
        populate(model);
        view.setCurrentIndex(model.index(0, 0));
        snapshot(view);
        view.setCurrentIndex(model.index(2, 0));
        QTest::mouseClick(view.viewport(), Qt::RightButton, Qt::NoModifier,
                          view.visualRect(model.index(2, 0)).center());
        model.clear();
        QVERIFY(!view.currentIndex().isValid());
        QVERIFY(!view.selectionModel()->hasSelection());
        snapshot(view);
        populate(model);
        view.setCurrentIndex(model.index(1, 0));
        QVERIFY(hasMarkAt(view, model.index(1, 0)));
        model.clear();
    }
}

void tst_NTreeView::removePreviousDuringTransition() {
    QStandardItemModel model;
    populate(model);
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.setCurrentIndex(model.index(0, 0));
    snapshot(view);
    view.setCurrentIndex(model.index(2, 0));
    QVERIFY(model.removeRow(0));
    QCOMPARE(view.currentIndex().data().toString(), QStringLiteral("item 2"));
    QVERIFY(hasMarkAt(view, view.currentIndex()));
}

void tst_NTreeView::removeCurrentAndParentDuringTransition() {
    QStandardItemModel model;
    auto* parent = new QStandardItem(QStringLiteral("folder"));
    parent->appendRow(new QStandardItem(QStringLiteral("first")));
    parent->appendRow(new QStandardItem(QStringLiteral("second")));
    model.appendRow(parent);
    model.appendRow(new QStandardItem(QStringLiteral("other")));
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.expandAll();
    view.setCurrentIndex(parent->child(0)->index());
    view.setCurrentIndex(parent->child(1)->index());
    parent->removeRow(1);
    QCOMPARE(parent->rowCount(), 1);
    snapshot(view);
    view.setCurrentIndex(parent->child(0)->index());
    QVERIFY(model.removeRow(0));
    snapshot(view);
    view.setCurrentIndex(model.index(0, 0));
    QTRY_VERIFY(hasMarkAt(view, view.currentIndex()));
}

void tst_NTreeView::sortingAndColumnChanges() {
    QStandardItemModel model;
    populate(model);
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.setCurrentIndex(model.index(2, 0));
    model.sort(0, Qt::DescendingOrder);
    QCOMPARE(view.currentIndex().data().toString(), QStringLiteral("item 2"));
    QVERIFY(hasMarkAt(view, view.currentIndex()));
    QVERIFY(model.insertColumn(1));
    QVERIFY(model.removeColumn(1));
    QVERIFY(hasMarkAt(view, view.currentIndex()));
    QVERIFY(model.removeColumn(0));
    snapshot(view);
    QVERIFY(!view.currentIndex().isValid());
}

void tst_NTreeView::rowMovement() {
    MovableModel model;
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.setCurrentIndex(model.index(0, 0));
    view.setCurrentIndex(model.index(2, 0));
    QVERIFY(model.moveRows(QModelIndex(), 2, 1, QModelIndex(), 0));
    QCOMPARE(view.currentIndex().data().toString(), QStringLiteral("c"));
    QCOMPARE(view.currentIndex().row(), 0);
    QVERIFY(hasMarkAt(view, view.currentIndex()));
}

void tst_NTreeView::proxyFilteringDuringTransition() {
    QStandardItemModel source;
    populate(source);
    QSortFilterProxyModel proxy;
    proxy.setSourceModel(&source);
    NTreeView view;
    view.setModel(&proxy);
    showTree(view);
    view.setCurrentIndex(proxy.index(0, 0));
    view.setCurrentIndex(proxy.index(2, 0));
    proxy.setFilterFixedString(QStringLiteral("item 1"));
    snapshot(view);
    QCOMPARE(proxy.rowCount(), 1);
    view.setCurrentIndex(proxy.index(0, 0));
    QTRY_VERIFY(hasMarkAt(view, view.currentIndex()));
    source.clear();
    snapshot(view);
}

void tst_NTreeView::replaceModelAndSelectionModel() {
    QStandardItemModel first;
    QStandardItemModel second;
    populate(first);
    populate(second);
    NTreeView view;
    view.setModel(&first);
    showTree(view);
    view.setCurrentIndex(first.index(0, 0));
    view.setCurrentIndex(first.index(2, 0));
    view.setModel(&second);
    view.setCurrentIndex(second.index(1, 0));
    first.clear();
    QVERIFY(hasMarkAt(view, second.index(1, 0)));

    QItemSelectionModel replacement(&second);
    replacement.setCurrentIndex(second.index(2, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    auto* oldSelection = view.selectionModel();
    view.setSelectionModel(&replacement);
    delete oldSelection;
    QCOMPARE(view.currentIndex(), second.index(2, 0));
    QVERIFY(hasMarkAt(view, second.index(2, 0)));
    // reset() 不发 selectionChanged，下一次绘制也必须清除已失效的选择标记。
    replacement.reset();
    QVERIFY(!hasMarkAt(view, second.index(2, 0)));
    view.setModel(nullptr);
    snapshot(view);
}

void tst_NTreeView::destroyModelAndSelectionModel() {
    NTreeView view;
    auto* model = new QStandardItemModel;
    populate(*model);
    view.setModel(model);
    showTree(view);
    view.setCurrentIndex(model->index(0, 0));
    view.setCurrentIndex(model->index(2, 0));
    delete model;
    snapshot(view);
    QVERIFY(!view.currentIndex().isValid());

    QStandardItemModel next;
    populate(next);
    view.setModel(&next);
    auto* selection = new QItemSelectionModel(&next);
    view.setSelectionModel(selection);
    view.setCurrentIndex(next.index(1, 0));
    delete selection;
    snapshot(view);
    auto* replacement = new QItemSelectionModel(&next, &view);
    view.setSelectionModel(replacement);
    view.setCurrentIndex(next.index(2, 0));
    QVERIFY(hasMarkAt(view, view.currentIndex()));
}

void tst_NTreeView::resetAndRootPlaceholder() {
    QStandardItemModel model;
    auto* folder = new QStandardItem(QStringLiteral("empty folder"));
    model.appendRow(folder);
    NTreeView view;
    view.setPlaceholderText(QStringLiteral("暂无数据"));
    view.setModel(&model);
    showTree(view);
    view.setCurrentIndex(folder->index());
    view.reset();
    snapshot(view);
    QVERIFY(!view.selectionModel()->hasSelection());
    view.setRootIndex(folder->index());
    QVERIFY(view.isShowingPlaceholder());
    QCOMPARE(view.accessibleDescription(), QStringLiteral("暂无数据"));
    folder->appendRow(new QStandardItem(QStringLiteral("child")));
    QVERIFY(!view.isShowingPlaceholder());
    view.setCurrentIndex(folder->child(0)->index());
    QVERIFY(hasMarkAt(view, view.currentIndex()));
    model.clear();
    snapshot(view);
}

void tst_NTreeView::mousePressUsesActualHitAndButton() {
    QStandardItemModel model;
    populate(model, 2);
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    const QModelIndex first = model.index(0, 0);
    view.setCurrentIndex(first);
    QTRY_VERIFY(hasMarkAt(view, first));
    const QRect row = view.visualRect(first);
    const QPoint sample(view.viewport()->width() - 12, row.center().y());
    const QColor selectedColor = snapshot(view).pixelColor(sample);
    QTest::mousePress(view.viewport(), Qt::RightButton, Qt::NoModifier, row.center());
    QCOMPARE(snapshot(view).pixelColor(sample), selectedColor);
    QTest::mouseRelease(view.viewport(), Qt::RightButton, Qt::NoModifier, row.center());

    view.clearSelection();
    const QPoint blank(100, view.viewport()->height() - 10);
    QVERIFY(!view.indexAt(blank).isValid());
    QTest::mouseMove(view.viewport(), blank);
    QCoreApplication::processEvents();
    const QColor idleColor = snapshot(view).pixelColor(sample);
    QTest::mousePress(view.viewport(), Qt::LeftButton, Qt::NoModifier, blank);
    QCOMPARE(snapshot(view).pixelColor(sample), idleColor);
    QTest::mouseRelease(view.viewport(), Qt::LeftButton, Qt::NoModifier, blank);
}

void tst_NTreeView::rightClickPreservesSelection_data() {
    QTest::addColumn<int>("mode");
    QTest::addColumn<bool>("hasSelection");
    QTest::newRow("single-empty") << int(QAbstractItemView::SingleSelection) << false;
    QTest::newRow("single-selected") << int(QAbstractItemView::SingleSelection) << true;
    QTest::newRow("extended-empty") << int(QAbstractItemView::ExtendedSelection) << false;
    QTest::newRow("extended-selected") << int(QAbstractItemView::ExtendedSelection) << true;
    QTest::newRow("multi-empty") << int(QAbstractItemView::MultiSelection) << false;
    QTest::newRow("multi-selected") << int(QAbstractItemView::MultiSelection) << true;
}

void tst_NTreeView::rightClickPreservesSelection() {
    QFETCH(int, mode);
    QFETCH(bool, hasSelection);
    QStandardItemModel model;
    populate(model);
    NTreeView view;
    view.setModel(&model);
    view.setContextMenuPolicy(Qt::CustomContextMenu);
    showTree(view, static_cast<QAbstractItemView::SelectionMode>(mode));
    auto* selection = view.selectionModel();
    selection->clear();
    if (hasSelection) {
        view.setCurrentIndex(model.index(0, 0));
        if (mode != QAbstractItemView::SingleSelection)
            selection->select(model.index(1, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
    const QModelIndex current = view.currentIndex();
    const QModelIndexList selected = selection->selectedIndexes();
    const QModelIndex target = model.index(2, 0);
    const QPoint position = view.visualRect(target).center();
    QSignalSpy currentSpy(selection, &QItemSelectionModel::currentChanged);
    QSignalSpy selectionSpy(selection, &QItemSelectionModel::selectionChanged);
    for (const auto modifier : {Qt::NoModifier, Qt::ControlModifier, Qt::ShiftModifier}) {
        QTest::mousePress(view.viewport(), Qt::RightButton, modifier, position);
        QCOMPARE(view.currentIndex(), current);
        QCOMPARE(selection->selectedIndexes(), selected);
        QTest::mouseRelease(view.viewport(), Qt::RightButton, modifier, position);
        QCOMPARE(view.currentIndex(), current);
        QCOMPARE(selection->selectedIndexes(), selected);
        QVERIFY(!hasMarkAt(view, target));
    }
    QCOMPARE(currentSpy.count(), 0);
    QCOMPARE(selectionSpy.count(), 0);

    // 上下文菜单仍接收被右键命中的节点坐标，无需把该节点设为 currentIndex。
    QSignalSpy menuSpy(&view, &QWidget::customContextMenuRequested);
    QContextMenuEvent menuEvent(QContextMenuEvent::Mouse, position, view.viewport()->mapToGlobal(position));
    QCoreApplication::sendEvent(view.viewport(), &menuEvent);
    QCOMPARE(menuSpy.count(), 1);
    QCOMPARE(view.indexAt(menuSpy.first().at(0).toPoint()), target);
    QCOMPARE(view.currentIndex(), current);
    QCOMPARE(selection->selectedIndexes(), selected);
}

void tst_NTreeView::keyboardSelectionAndScrolling() {
    QStandardItemModel model;
    populate(model, 40);
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.setCurrentIndex(model.index(0, 0));
    QTest::keyClick(&view, Qt::Key_Down);
    QCOMPARE(view.currentIndex(), model.index(1, 0));
    view.setCurrentIndex(model.index(39, 0));
    QTRY_VERIFY(hasMarkAt(view, view.currentIndex()));
    view.verticalScrollBar()->setValue(0);
    snapshot(view);
    view.scrollTo(view.currentIndex());
    QTRY_VERIFY(hasMarkAt(view, view.currentIndex()));
}

void tst_NTreeView::revealInterruptedByModelChanges() {
    QStandardItemModel model;
    auto* folder = new QStandardItem(QStringLiteral("folder"));
    folder->appendRow(new QStandardItem(QStringLiteral("child")));
    model.appendRow(folder);
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.expandAll();
    view.toggleExpanded(folder->index());
    folder->removeRow(0);
    QCOMPARE(folder->rowCount(), 0);
    QVERIFY(!view.isExpanded(folder->index()));
    folder->appendRow(new QStandardItem(QStringLiteral("replacement")));
    view.toggleExpanded(folder->index());
    model.clear();
    snapshot(view);
    populate(model);
    view.setCurrentIndex(model.index(0, 0));
    QVERIFY(hasMarkAt(view, view.currentIndex()));
}

void tst_NTreeView::expandingAnotherBranchFinalizesPendingCollapse() {
    QStandardItemModel model;
    for (int row = 0; row < 2; ++row) {
        auto* folder = new QStandardItem(QString::number(row));
        folder->appendRow(new QStandardItem(QStringLiteral("child")));
        model.appendRow(folder);
    }
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.expandAll();
    view.collapse(model.index(1, 0));
    view.toggleExpanded(model.index(0, 0));
    view.expand(model.index(1, 0));
    QVERIFY(!view.isExpanded(model.index(0, 0)));
    QVERIFY(view.isExpanded(model.index(1, 0)));
    QTRY_COMPARE(view.chevronRotation(model.index(1, 0)), 1.0);
}

void tst_NTreeView::multiSelectionPaintVisitsOnlyVisibleRows() {
    QStandardItemModel model;
    populate(model, 4000);
    model.setColumnCount(2);
    CountingTreeView view;
    view.setModel(&model);
    showTree(view, QAbstractItemView::ExtendedSelection);
    view.selectAll();
    QCoreApplication::processEvents();
    view.rectQueries = 0;
    snapshot(view);
    QVERIFY2(view.rectQueries < 400, "多选标记不应逐帧查询全部 4000 个选中节点的几何位置");
    QVERIFY(hasMarkAt(view, model.index(0, 0)));
    view.scrollTo(model.index(3999, 0));
    QVERIFY(hasMarkAt(view, model.index(3999, 0)));
    view.setColumnHidden(0, true);
    view.rectQueries = 0;
    snapshot(view);
    QVERIFY(view.rectQueries < 400);
}

void tst_NTreeView::checkStateControlsMultiSelectionAppearance() {
    QStandardItemModel model;
    model.appendRow(checkItem(QStringLiteral("unchecked")));
    model.appendRow(checkItem(QStringLiteral("checked")));
    NTreeView view;
    view.setModel(&model);
    showTree(view, QAbstractItemView::MultiSelection);
    const QColor selected(20, 230, 90);
    view.setLightItemSelectedColor(selected);
    view.setDarkItemSelectedColor(selected);
    // 模拟旧代码遗留的“Qt 已选中、实际未勾选”状态。
    view.selectionModel()->select(model.index(0, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
    QVERIFY(!hasMarkAt(view, model.index(0, 0)));
    const QPoint sample(view.viewport()->width() - 12, view.visualRect(model.index(0, 0)).center().y());
    QVERIFY(snapshot(view).pixelColor(sample) != selected);
    model.item(1)->setCheckState(Qt::Checked);
    QVERIFY(hasMarkAt(view, model.index(1, 0)));
    model.item(1)->setCheckState(Qt::PartiallyChecked);
    QVERIFY(!hasMarkAt(view, model.index(1, 0)));
    model.item(1)->setCheckState(Qt::Unchecked);
    QVERIFY(!hasMarkAt(view, model.index(1, 0)));
}

void tst_NTreeView::mouseAndKeyboardChecksShareParentState() {
    QStandardItemModel model;
    auto* folder = checkItem(QStringLiteral("folder"));
    folder->setAutoTristate(true);
    folder->appendRow(checkItem(QStringLiteral("first")));
    folder->appendRow(checkItem(QStringLiteral("second")));
    model.appendRow(folder);
    NTreeView view;
    view.setModel(&model);
    showTree(view, QAbstractItemView::MultiSelection);
    view.expandAll();
    const QModelIndex first = folder->child(0)->index();
    const QRect firstRect = view.visualRect(first);
    QTest::mouseClick(view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      QPoint(firstRect.left() + 85, firstRect.center().y()));
    QTRY_COMPARE(folder->child(0)->checkState(), Qt::Checked);
    QTRY_COMPARE(folder->checkState(), Qt::PartiallyChecked);
    QVERIFY(hasMarkAt(view, first));
    QVERIFY(!hasMarkAt(view, folder->index()));
    QTest::mouseClick(view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      QPoint(firstRect.left() + 23, firstRect.center().y()));
    QTRY_COMPARE(folder->child(0)->checkState(), Qt::Unchecked);
    QTRY_COMPARE(folder->checkState(), Qt::Unchecked);
    QVERIFY(!hasMarkAt(view, first));
    QTest::mouseClick(view.viewport(), Qt::LeftButton, Qt::NoModifier,
                      QPoint(firstRect.left() + 85, firstRect.center().y()));
    QTRY_COMPARE(folder->child(0)->checkState(), Qt::Checked);
    view.setCurrentIndex(folder->child(1)->index());
    pressSpace(view);
    QTRY_COMPARE(folder->checkState(), Qt::Checked);
    QVERIFY(hasMarkAt(view, folder->index()));
    view.setCurrentIndex(folder->index());
    pressSpace(view);
    QTRY_COMPARE(folder->checkState(), Qt::Unchecked);
    QCOMPARE(folder->child(0)->checkState(), Qt::Unchecked);
    QCOMPARE(folder->child(1)->checkState(), Qt::Unchecked);
    QVERIFY(!hasMarkAt(view, first));
    folder->child(1)->setEnabled(false);
    pressSpace(view);
    QTRY_COMPARE(folder->child(0)->checkState(), Qt::Checked);
    QTRY_COMPARE(folder->checkState(), Qt::PartiallyChecked);
    QCOMPARE(folder->child(1)->checkState(), Qt::Unchecked);
    folder->removeRow(1);
    QTRY_COMPARE(folder->checkState(), Qt::Checked);
}

void tst_NTreeView::largeCheckOperationYieldsAndCancelsOnReset() {
    QStandardItemModel model;
    auto* folder = checkItem(QStringLiteral("large folder"));
    folder->setAutoTristate(true);
    for (int row = 0; row < 1000; ++row)
        folder->appendRow(checkItem(QString::number(row)));
    model.appendRow(folder);
    NTreeView view;
    view.setModel(&model);
    showTree(view, QAbstractItemView::MultiSelection);
    view.setCurrentIndex(folder->index());
    pressSpace(view);
    int checked = 0;
    for (int row = 0; row < 1000; ++row)
        checked += folder->child(row)->checkState() == Qt::Checked;
    QVERIFY(checked < 1000); // 首轮有明确步数预算，后续工作交回事件循环。
    QTRY_COMPARE(folder->checkState(), Qt::Checked);
    QCOMPARE(folder->child(999)->checkState(), Qt::Checked);
    pressSpace(view);
    model.clear();
    auto* replacement = checkItem(QStringLiteral("replacement"));
    model.appendRow(replacement);
    QCoreApplication::processEvents();
    QCOMPARE(replacement->checkState(), Qt::Unchecked);
    snapshot(view);
}

void tst_NTreeView::resetFromCheckDataChangedIsSafe() {
    QStandardItemModel model;
    model.appendRow(checkItem(QStringLiteral("reset from signal")));
    NTreeView view;
    view.setModel(&model);
    showTree(view, QAbstractItemView::MultiSelection);
    view.setCurrentIndex(model.index(0, 0));
    bool reset = false;
    connect(&model, &QAbstractItemModel::dataChanged, &view, [&]() {
        if (!reset) {
            reset = true;
            model.clear();
        }
    });
    pressSpace(view);
    QVERIFY(reset);
    QCOMPARE(model.rowCount(), 0);
    snapshot(view);
}

void tst_NTreeView::revealGeometryAndAnimationSwitch() {
    QStandardItemModel model;
    auto* folder = new QStandardItem(QStringLiteral("folder"));
    for (int row = 0; row < 3; ++row)
        folder->appendRow(new QStandardItem(QString::number(row)));
    model.appendRow(folder);
    model.appendRow(new QStandardItem(QStringLiteral("sibling")));
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.expand(folder->index());
    const QModelIndex child = folder->child(2)->index();
    const QPoint hiddenPoint = view.visualRect(child).center();
    QVERIFY(!view.indexAt(hiddenPoint).isValid());
    const QModelIndex sibling = model.index(1, 0);
    QCOMPARE(view.indexAt(view.visualRect(sibling).center()), sibling);
    // 关闭动效立即结束当前会话，真实布局、命中位置和属性读取保持一致。
    QVERIFY(view.setProperty("animated", false));
    QVERIFY(!view.isAnimated());
    QVERIFY(!view.isRevealAnimationEnabled());
    QCOMPARE(view.indexAt(hiddenPoint), child);
    view.toggleExpanded(folder->index());
    QVERIFY(!view.isExpanded(folder->index()));
    view.setRevealAnimationEnabled(true);
    view.toggleExpanded(folder->index());
    QVERIFY(view.isAnimated());
    view.setAnimated(false);
    QCOMPARE(view.chevronRotation(folder->index()), 1.0);
}

void tst_NTreeView::unrelatedInsertionKeepsReveal() {
    QStandardItemModel model;
    auto* folder = new QStandardItem(QStringLiteral("folder"));
    folder->appendRow(new QStandardItem(QStringLiteral("child")));
    model.appendRow(folder);
    NTreeView view;
    view.setModel(&model);
    showTree(view);
    view.expand(folder->index());
    model.appendRow(new QStandardItem(QStringLiteral("unrelated")));
    QVERIFY(view.chevronRotation(folder->index()) < 1.0);
    folder->appendRow(new QStandardItem(QStringLiteral("affected")));
    QCOMPARE(view.chevronRotation(folder->index()), 1.0);
}

void tst_NTreeView::largeRevealVisitsOnlyViewportRows() {
    QStandardItemModel model;
    auto* folder = new QStandardItem(QStringLiteral("large folder"));
    for (int row = 0; row < 10000; ++row)
        folder->appendRow(new QStandardItem(QString::number(row)));
    model.appendRow(folder);
    CountingTreeView view;
    view.setModel(&model);
    showTree(view);
    view.rectQueries = 0;
    view.expand(folder->index());
    QVERIFY2(view.rectQueries < 500, "展开动效不应查询视口之外全部 10000 行的几何位置");
}

void tst_NTreeView::customStyleIsDestroyedWithView() {
    auto* view = new NTreeView;
    QPointer<QObject> style;
    for (auto* child : view->children()) {
        if (QByteArray(child->metaObject()->className()) == "NTreeViewStyle")
            style = child;
    }
    QVERIFY(style);
    delete view;
    QVERIFY(style.isNull());
}

void tst_NTreeView::multiColumnContentsAndEditorStayInsideCell() {
    QStandardItemModel model;
    auto* first = checkItem(QStringLiteral("long first-column text"));
    first->appendRow(new QStandardItem(QStringLiteral("child")));
    model.appendRow({first, new QStandardItem(QStringLiteral("second column"))});
    NTreeView view;
    view.setModel(&model);
    showTree(view, QAbstractItemView::MultiSelection);
    QStyleOptionViewItem option;
    option.initFrom(&view);
    option.rect = QRect(10, 10, 30, 32);
    option.index = model.index(0, 0);
    QImage image(160, 60, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    {
        QPainter painter(&image);
        view.itemDelegate()->paint(&painter, option, option.index);
    }
    for (int y = 0; y < image.height(); ++y) {
        for (int x = option.rect.right() + 1; x < image.width(); ++x)
            QCOMPARE(image.pixelColor(x, y), QColor(Qt::white));
    }
    option.rect = QRect(100, 10, 150, 32);
    option.index = model.index(0, 1);
    QWidget* editor = view.itemDelegate()->createEditor(view.viewport(), option, option.index);
    QVERIFY(editor);
    view.itemDelegate()->updateEditorGeometry(editor, option, option.index);
    QCOMPARE(editor->geometry().left(), option.rect.left() + 12);
    QVERIFY(editor->geometry().right() <= option.rect.right());
    delete editor;
}

void tst_NTreeView::repeatedCheckUsesLastRequestedState() {
    QStandardItemModel model;
    auto* folder = checkItem(QStringLiteral("folder"));
    for (int row = 0; row < 1000; ++row)
        folder->appendRow(checkItem(QString::number(row)));
    model.appendRow(folder);
    NTreeView view;
    view.setModel(&model);
    showTree(view, QAbstractItemView::MultiSelection);
    view.setCurrentIndex(folder->index());
    pressSpace(view);
    pressSpace(view);
    QTRY_COMPARE(folder->checkState(), Qt::Unchecked);
    QCOMPARE(folder->child(0)->checkState(), Qt::Unchecked);
    QCOMPARE(folder->child(999)->checkState(), Qt::Unchecked);
}

QTEST_MAIN(tst_NTreeView)
#include "tst_ntreeview.moc"
