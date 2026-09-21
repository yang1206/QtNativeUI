#ifndef NLISTITEMDELEGATE_P_H
#define NLISTITEMDELEGATE_P_H

#include <QColor>
#include <QStyledItemDelegate>

class NListView;
class NListViewPrivate;

class NListItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
  public:
    explicit NListItemDelegate(NListView* view, NListViewPrivate* data, QObject* parent = nullptr);
    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool  editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                      const QModelIndex& index) override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void     setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void     setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void     updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const override;
    void     paintStickySection(QWidget* viewport) const;

  private:
    void paintListRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paintIconRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QRect checkBoxRect(const QRect& itemRect) const;
    QRect iconCheckBoxRect(const QRect& tileRect) const;
    bool  rowStartsSection(const QModelIndex& index) const;
    int   sectionHeaderHeight() const;
    void  paintSectionHeader(QPainter* painter, const QRect& itemRect, const QString& title,
                             const QColor& textColor) const;
    NListView*        m_view{nullptr};
    NListViewPrivate* m_data{nullptr};
};

#endif
