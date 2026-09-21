// Copyright (C) 2024 QtNativeUI Project
// SPDX-License-Identifier: GPL-3.0-only

#include <QApplication>
#include <QCoreApplication>
#include <QStandardItemModel>
#include <QStyle>
#include <QTest>

#include <QAbstractItemDelegate>

#include <QtNativeUI/NEnums.h>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NListView.h>

class tst_NListView : public QObject {
    Q_OBJECT

  private slots:
    void placeholderWhenEmpty();
    void selectionIndicatorToggle();
    void subtitleIncreasesRowHeight();
    void placeholderAccessibleDescription();
    void modelDestroyWhileViewAlive();
    void editableEditorUsesNLineEdit();
    void selectionIndicatorSurvivesModelReset();
    void selectionIndicatorAfterRowRemoved();
    void sectionHeaderIncreasesRowHeight();
    void iconModeSizeHintUsesGrid();
    void stickySectionHeaderAfterScroll();
};

void tst_NListView::placeholderWhenEmpty() {
    NListView view;
    view.setPlaceholderText(QStringLiteral("empty"));
    QVERIFY(view.isShowingPlaceholder());
    auto* model = new QStandardItemModel(&view);
    view.setModel(model);
    QVERIFY(view.isShowingPlaceholder());
    model->appendRow(new QStandardItem(QStringLiteral("a")));
    QVERIFY(!view.isShowingPlaceholder());
}

void tst_NListView::selectionIndicatorToggle() {
    NListView view;
    QCOMPARE(view.getSelectionIndicatorVisible(), true);
    view.setSelectionIndicatorVisible(false);
    QCOMPARE(view.getSelectionIndicatorVisible(), false);
}

void tst_NListView::subtitleIncreasesRowHeight() {
    NListView view;
    view.setItemHeight(36);
    auto* model = new QStandardItemModel(&view);
    QStandardItem* item = new QStandardItem(QStringLiteral("title"));
    item->setData(QStringLiteral("subtitle"), NListViewType::SubtitleRole);
    model->appendRow(item);
    view.setModel(model);
    const QModelIndex index = model->index(0, 0);
    const QSize       hint  = view.itemDelegate()->sizeHint(QStyleOptionViewItem(), index);
    QVERIFY(hint.height() >= 54);
}

void tst_NListView::placeholderAccessibleDescription() {
    NListView view;
    view.setPlaceholderText(QStringLiteral("no items"));
    view.setModel(new QStandardItemModel(&view));
    QCOMPARE(view.accessibleDescription(), QStringLiteral("no items"));
}

void tst_NListView::modelDestroyWhileViewAlive() {
    NListView view;
    view.setPlaceholderText(QStringLiteral("empty"));
    auto* model = new QStandardItemModel();
    model->appendRow(new QStandardItem(QStringLiteral("row")));
    view.setModel(model);
    delete model;
    view.setModel(nullptr);
    QCoreApplication::processEvents();
    QVERIFY(view.isShowingPlaceholder());
}

void tst_NListView::editableEditorUsesNLineEdit() {
    NListView view;
    view.setEditTriggers(QAbstractItemView::AllEditTriggers);
    auto* model = new QStandardItemModel(&view);
    auto* item  = new QStandardItem(QStringLiteral("edit me"));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    model->appendRow(item);
    view.setModel(model);
    const QModelIndex idx = model->index(0, 0);
    view.setCurrentIndex(idx);
    view.setFocus(Qt::OtherFocusReason);
    QApplication::processEvents();
    view.edit(idx);
    QApplication::processEvents();
    auto* editor = view.findChild<NLineEdit*>();
    QVERIFY(editor != nullptr);
    QTest::keyClick(editor, Qt::Key_Escape);
}

void tst_NListView::selectionIndicatorSurvivesModelReset() {
    NListView view;
    auto* model = new QStandardItemModel(&view);
    model->appendRow(new QStandardItem(QStringLiteral("a")));
    model->appendRow(new QStandardItem(QStringLiteral("b")));
    view.setModel(model);
    view.setCurrentIndex(model->index(0, 0));
    model->removeRows(0, model->rowCount());
    model->appendRow(new QStandardItem(QStringLiteral("x")));
    view.setCurrentIndex(model->index(0, 0));
    QCoreApplication::processEvents();
    QVERIFY(view.currentIndex().isValid());
    QCOMPARE(view.currentIndex().data().toString(), QStringLiteral("x"));
}

void tst_NListView::selectionIndicatorAfterRowRemoved() {
    NListView view;
    auto* model = new QStandardItemModel(&view);
    model->appendRow(new QStandardItem(QStringLiteral("a")));
    model->appendRow(new QStandardItem(QStringLiteral("b")));
    view.setModel(model);
    view.setCurrentIndex(model->index(0, 0));
    model->removeRow(0);
    QCoreApplication::processEvents();
    QVERIFY(view.currentIndex().isValid());
    QCOMPARE(view.currentIndex().data().toString(), QStringLiteral("b"));
}

void tst_NListView::sectionHeaderIncreasesRowHeight() {
    NListView view;
    view.setItemHeight(36);
    view.setSectionsEnabled(true);
    view.setSectionHeaderHeight(28);
    auto* model = new QStandardItemModel(&view);
    auto* first = new QStandardItem(QStringLiteral("a"));
    first->setData(QStringLiteral("Group"), NListViewType::SectionRole);
    model->appendRow(first);
    auto* second = new QStandardItem(QStringLiteral("b"));
    second->setData(QStringLiteral("Group"), NListViewType::SectionRole);
    model->appendRow(second);
    view.setModel(model);
    const int firstHeight  = view.itemDelegate()->sizeHint(QStyleOptionViewItem(), model->index(0, 0)).height();
    const int secondHeight = view.itemDelegate()->sizeHint(QStyleOptionViewItem(), model->index(1, 0)).height();
    QVERIFY(firstHeight >= secondHeight + 28);
    view.setSectionHeaderHeight(40);
    const int taller = view.itemDelegate()->sizeHint(QStyleOptionViewItem(), model->index(0, 0)).height();
    QVERIFY(taller >= secondHeight + 40);
}

void tst_NListView::iconModeSizeHintUsesGrid() {
    NListView view;
    view.setViewMode(QListView::IconMode);
    view.setIconSize(QSize(48, 48));
    view.setGridSize(QSize(104, 112));
    auto* model = new QStandardItemModel(&view);
    auto* item  = new QStandardItem(QStringLiteral("tile"));
    item->setIcon(view.style()->standardIcon(QStyle::SP_FileIcon));
    model->appendRow(item);
    view.setModel(model);
    const QSize hint = view.itemDelegate()->sizeHint(QStyleOptionViewItem(), model->index(0, 0));
    QCOMPARE(hint, QSize(104, 112));
}

void tst_NListView::stickySectionHeaderAfterScroll() {
    NListView view;
    view.setSectionsEnabled(true);
    view.setItemHeight(36);
    view.setSectionHeaderHeight(28);
    view.resize(240, 120);
    auto* model = new QStandardItemModel(&view);
    for (int i = 0; i < 10; ++i) {
        auto* item = new QStandardItem(QStringLiteral("row %1").arg(i));
        item->setData(i < 5 ? QStringLiteral("A") : QStringLiteral("B"), NListViewType::SectionRole);
        model->appendRow(item);
    }
    view.setModel(model);
    view.show();
    QVERIFY(QTest::qWaitForWindowExposed(&view));
    view.scrollTo(model->index(6, 0), QAbstractItemView::PositionAtTop);
    view.viewport()->repaint();
    QVERIFY(view.visualRect(model->index(0, 0)).bottom() < 0);
    QVERIFY(view.visualRect(model->index(5, 0)).top() <= 0);
}

QTEST_MAIN(tst_NListView)
#include "tst_nlistview.moc"
