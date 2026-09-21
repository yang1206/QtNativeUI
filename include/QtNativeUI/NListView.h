//
// Created by Yang1206 on 2025/12/29.
//

#ifndef NLISTVIEW_H
#define NLISTVIEW_H

#include <QFont>
#include <QListView>

class QItemSelection;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;
class QWheelEvent;

#include "NColor.h"
#include "stdafx.h"

class NListViewPrivate;
class NListViewDragReorderController;
class QTNATIVEUI_EXPORT NListView : public QListView {
    Q_OBJECT
    Q_Q_CREATE(NListView)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemSelectedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightItemPressedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkItemPressedColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPlaceholderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPlaceholderTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, ItemBorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, PlaceholderText)
    Q_PROPERTY_CREATE_Q_H(bool, BorderVisible)
    Q_PROPERTY_CREATE_Q_H(bool, BackgroundVisible)
    Q_PROPERTY_CREATE_Q_H(bool, SelectionIndicatorVisible)
    Q_PROPERTY_CREATE_Q_H(QString, HeaderText)
    Q_PROPERTY_CREATE_Q_H(QString, FooterText)
    Q_PROPERTY_CREATE_Q_H(bool, SectionsEnabled)
    Q_PROPERTY_CREATE_Q_H(int, SectionHeaderHeight)
    Q_PROPERTY_CREATE_Q_H(QColor, LightSectionTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSectionTextColor)
    Q_PROPERTY_CREATE_Q_H(QFont, SectionHeaderFont)
    Q_PROPERTY_CREATE_Q_H(bool, SelectionIndicatorAnimated)
    Q_PROPERTY_CREATE_Q_H(bool, ReorderEnabled)

  public:
    explicit NListView(QWidget* parent = nullptr);
    ~NListView() override;

    void setModel(QAbstractItemModel* model) override;
    void setSelectionModel(QItemSelectionModel* selectionModel) override;

    bool  isShowingPlaceholder() const;
    QRect visualRect(const QModelIndex& index) const override;
  Q_SIGNALS:
    void rowsReordered(int sourceRow, int destinationRow);

  protected:
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void startDrag(Qt::DropActions supportedActions) override;
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;
    void scrollContentsBy(int dx, int dy) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

  private:
    friend class NListViewPrivate;
    friend class NListViewDragReorderController;
    friend class NListViewSelectMarkController;
    QRect  layoutVisualRect(const QModelIndex& index) const;
    QRect  indicatorItemRect(const QModelIndex& index) const;
    QPoint viewportPosFromMouseEvent(const QMouseEvent* event) const;
    void   init();
    void   connectModelSignals(QAbstractItemModel* model);
    void   updateAutomaticAccessibleDescription();
    void   finishReorderDrop();
};

#endif // NLISTVIEW_H
