//
// Created by Yang1206 on 2025/12/30.
//

#ifndef NTREEVIEW_H
#define NTREEVIEW_H

#include <QTreeView>

class QItemSelection;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;
class QWheelEvent;

#include "NColor.h"
#include "stdafx.h"

class NTreeViewPrivate;
class QTNATIVEUI_EXPORT NTreeView : public QTreeView {
    Q_OBJECT
    Q_Q_CREATE(NTreeView)
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
    Q_PROPERTY_CREATE_Q_H(int, Indentation)
    Q_PROPERTY_CREATE_Q_H(QString, PlaceholderText)
    Q_PROPERTY_CREATE_Q_H(QString, HeaderText)
    Q_PROPERTY_CREATE_Q_H(bool, BorderVisible)
    Q_PROPERTY_CREATE_Q_H(bool, BackgroundVisible)
    Q_PROPERTY_CREATE_Q_H(bool, SelectionIndicatorVisible)

  public:
    explicit NTreeView(QWidget* parent = nullptr);
    ~NTreeView() override;

    void setModel(QAbstractItemModel* model) override;
    bool isShowingPlaceholder() const;
    void  expandAll();
    void  collapseAll();
    void  toggleExpanded(const QModelIndex& index);
    qreal chevronRotation(const QModelIndex& index) const;

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;
    void scrollContentsBy(int dx, int dy) override;
    void drawBranches(QPainter* painter, const QRect& rect, const QModelIndex& index) const override;
    void drawRow(QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex& index) const override;

  private:
    friend class NTreeViewPrivate;
    friend class NTreeItemDelegate;
    void init();
    void connectModelSignals(QAbstractItemModel* model);
    void updateAutomaticAccessibleDescription();
};

#endif // NTREEVIEW_H
