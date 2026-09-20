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
    QVERIFY(view.edit(idx));
    QVERIFY(qobject_cast<NLineEdit*>(QApplication::focusWidget()) != nullptr);
    view.closeEditor(QApplication::focusWidget(), QAbstractItemDelegate::RevertModelCache);
}

QTEST_MAIN(tst_NListView)
#include "tst_nlistview.moc"
