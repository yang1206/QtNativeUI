#ifndef NTREECHECKCONTROLLER_P_H
#define NTREECHECKCONTROLLER_P_H

#include <QAbstractItemModel>
#include <QObject>
#include <QPersistentModelIndex>
#include <QPointer>
#include <QQueue>
#include <QTimer>
#include <QVector>
#include <functional>

// 在模型所属的界面线程分批处理勾选，不屏蔽模型信号，也不递归占用调用栈。
class NTreeCheckController : public QObject {
  public:
    explicit NTreeCheckController(std::function<void()> repaint, QObject* parent);
    void toggle(const QModelIndex& index);
    void cancel();
    void reconcile(const QModelIndex& index);
    void reconcileAncestors(const QModelIndex& index);
    QVector<QPersistentModelIndex> interrupt();
    bool isApplying() const { return m_applying; }
    bool affects(const QModelIndex& parent, int first, int last, bool removing) const;
    void externalChange(const QModelIndex& topLeft, const QModelIndex& bottomRight);

  private:
    struct Counts {
        int checked{0};
        int unchecked{0};
        int partial{0};
        void add(Qt::CheckState state);
        Qt::CheckState state(Qt::CheckState fallback) const;
    };
    struct Operation {
        QPersistentModelIndex root;
        Qt::CheckState target{Qt::Unchecked};
        bool reconcileOnly{false};
        bool ancestorsOnly{false};
    };
    struct Frame {
        QPersistentModelIndex index;
        int nextChild{0};
        bool started{false};
        bool checkable{false};
        bool enabled{false};
        Counts counts;
    };

    void enqueue(Operation operation);
    void process();
    bool writeState(const QPersistentModelIndex& index, Qt::CheckState state, int generation);

    QTimer m_timer;
    QQueue<Operation> m_pending;
    QVector<Frame> m_stack;
    Operation m_active;
    QPointer<QAbstractItemModel> m_model;
    QPersistentModelIndex m_ancestor;
    int m_ancestorChild{0};
    Counts m_ancestorCounts;
    int m_generation{0};
    bool m_busy{false};
    bool m_applying{false};
    std::function<void()> m_repaint;
};

#endif
