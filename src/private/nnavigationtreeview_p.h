#ifndef NNavigationTreeView_H
#define NNavigationTreeView_H

#include <QTreeView>

class NNavigationStyle;
class NToolTip;
class NScrollBar;

class NNavigationTreeView : public QTreeView {
    Q_OBJECT

  public:
    explicit NNavigationTreeView(QWidget* parent = nullptr);
    ~NNavigationTreeView();

    void navigationNodeStateChange(QVariantMap data);

  public slots:
    void onCustomContextMenuRequested(const QPoint& pos);

  signals:
    void navigationClicked(const QModelIndex& index);

  protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    NNavigationStyle* _navigationStyle{nullptr};
    NToolTip*         _compactToolTip{nullptr};
};

#endif // NNavigationTreeView_H