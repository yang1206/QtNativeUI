#ifndef NTREEITEMDELEGATE_P_H
#define NTREEITEMDELEGATE_P_H

#include <QStyledItemDelegate>

class NTreeView;
class NTreeViewPrivate;

class NTreeItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
  public:
    explicit NTreeItemDelegate(NTreeView* view, NTreeViewPrivate* data, QObject* parent = nullptr);

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    bool  editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                      const QModelIndex& index) override;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void     setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void     setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void     updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const override;

    QRectF backgroundRect(const QStyleOptionViewItem& option) const;
    QRectF chevronRect(const QStyleOptionViewItem& option) const;
    QRectF checkBoxRect(const QStyleOptionViewItem& option) const;

  private:
    NTreeView*        m_view{nullptr};
    NTreeViewPrivate* m_data{nullptr};
};

#endif
