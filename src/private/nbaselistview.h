//
// Created by Yang1206 on 2025/4/23.
//

#ifndef NBASELISTVIEW_H
#define NBASELISTVIEW_H

#include <QListView>
#include <QModelIndex>

class NScrollBar;
class NBaseListView : public QListView {
    Q_OBJECT
  public:
    explicit NBaseListView(QWidget* parent = nullptr);
    ~NBaseListView();
  Q_SIGNALS:
    Q_SIGNAL void mousePress(const QModelIndex& index);
    Q_SIGNAL void mouseRelease(const QModelIndex& index);
    Q_SIGNAL void mouseDoubleClick(const QModelIndex& index);

  protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // NBASELISTVIEW_H
