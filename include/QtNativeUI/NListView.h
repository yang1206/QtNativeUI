//
// Created by Yang1206 on 2025/12/29.
//

#ifndef NLISTVIEW_H
#define NLISTVIEW_H

#include <QListView>

class QItemSelection;
class QMouseEvent;
class QPaintEvent;

#include "NColor.h"
#include "stdafx.h"

class NListViewPrivate;
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

  public:
    explicit NListView(QWidget* parent = nullptr);
    ~NListView() override;

    void setModel(QAbstractItemModel* model) override;

    bool isShowingPlaceholder() const;

  protected:
    void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;
    void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) override;
    void scrollContentsBy(int dx, int dy) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

  private:
    friend class NListViewPrivate;
    void init();
    void connectModelSignals(QAbstractItemModel* model);
    void updateAutomaticAccessibleDescription();
};

#endif // NLISTVIEW_H
