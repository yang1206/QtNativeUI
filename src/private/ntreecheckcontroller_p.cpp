#include "ntreecheckcontroller_p.h"

#include <QElapsedTimer>
#include <utility>

NTreeCheckController::NTreeCheckController(std::function<void()> repaint, QObject* parent)
    : QObject(parent), m_repaint(std::move(repaint)) {
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this]() { process(); });
}

void NTreeCheckController::Counts::add(Qt::CheckState state) {
    if (state == Qt::Checked)
        ++checked;
    else if (state == Qt::PartiallyChecked)
        ++partial;
    else
        ++unchecked;
}

Qt::CheckState NTreeCheckController::Counts::state(Qt::CheckState fallback) const {
    if (partial > 0 || (checked > 0 && unchecked > 0))
        return Qt::PartiallyChecked;
    if (checked > 0)
        return Qt::Checked;
    return unchecked > 0 ? Qt::Unchecked : fallback;
}

void NTreeCheckController::cancel() {
    ++m_generation;
    m_timer.stop();
    m_pending.clear();
    m_stack.clear();
    m_active = Operation();
    m_model.clear();
    m_ancestor = QModelIndex();
    m_ancestorChild = 0;
    m_ancestorCounts = Counts();
    m_busy = false;
}

QVector<QPersistentModelIndex> NTreeCheckController::interrupt() {
    QVector<QPersistentModelIndex> repairs;
    const auto remember = [&repairs](const QPersistentModelIndex& root) {
        if (!root.isValid())
            return;
        const QPersistentModelIndex parent(root.parent());
        const auto candidate = parent.isValid() ? parent : root;
        if (!repairs.contains(candidate))
            repairs.append(candidate);
    };
    if (m_busy)
        remember(m_active.root);
    for (const auto& operation : m_pending)
        remember(operation.root);
    cancel();
    return repairs;
}

void NTreeCheckController::toggle(const QModelIndex& index) {
    if (!index.isValid() || !(index.flags() & Qt::ItemIsEnabled)
        || !(index.flags() & Qt::ItemIsUserCheckable))
        return;
    Qt::CheckState state = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
    if (m_busy && m_active.root == index && !m_active.reconcileOnly)
        state = m_active.target;
    for (const auto& operation : m_pending) {
        if (operation.root == index && !operation.reconcileOnly)
            state = operation.target;
    }
    enqueue({QPersistentModelIndex(index), state == Qt::Checked ? Qt::Unchecked : Qt::Checked, false});
}

bool NTreeCheckController::affects(const QModelIndex& parent, int first, int last, bool removing) const {
    const auto affected = [&](const QPersistentModelIndex& root) {
        if (!root.isValid())
            return false;
        QModelIndex walk(parent);
        while (walk.isValid()) {
            if (walk == root)
                return true;
            walk = walk.parent();
        }
        if (!removing)
            return false;
        walk = root;
        while (walk.isValid() && walk.parent() != parent)
            walk = walk.parent();
        return walk.isValid() && walk.row() >= first && walk.row() <= last;
    };
    if (m_busy && affected(m_active.root))
        return true;
    if (m_busy && m_stack.isEmpty() && affected(m_ancestor))
        return true;
    for (const auto& operation : m_pending) {
        if (affected(operation.root))
            return true;
    }
    return false;
}

void NTreeCheckController::externalChange(const QModelIndex& topLeft, const QModelIndex& bottomRight) {
    if (m_applying || !affects(topLeft.parent(), topLeft.row(), bottomRight.row(), true))
        return;
    for (const auto& repair : interrupt())
        reconcile(repair);
}

void NTreeCheckController::reconcile(const QModelIndex& index) {
    if (index.isValid())
        enqueue({QPersistentModelIndex(index), Qt::Unchecked, true});
}

void NTreeCheckController::reconcileAncestors(const QModelIndex& index) {
    if (index.isValid())
        enqueue({QPersistentModelIndex(index), Qt::Unchecked, true, true});
}

void NTreeCheckController::enqueue(Operation operation) {
    // 同一节点尚未执行的连续点击只保留最后一个意图。
    for (int i = m_pending.size() - 1; i >= 0; --i) {
        if (m_pending.at(i).root == operation.root) {
            if (operation.reconcileOnly && !m_pending.at(i).reconcileOnly)
                return;
            if (operation.reconcileOnly && operation.ancestorsOnly
                && m_pending.at(i).reconcileOnly && !m_pending.at(i).ancestorsOnly)
                return;
            m_pending.removeAt(i);
        }
    }
    const bool deferred = operation.reconcileOnly;
    m_pending.enqueue(std::move(operation));
    if (deferred) {
        if (!m_timer.isActive())
            m_timer.start(1);
    } else if (!m_applying) {
        process();
    }
}

bool NTreeCheckController::writeState(const QPersistentModelIndex& index, Qt::CheckState state, int generation) {
    if (generation != m_generation || !m_model || !index.isValid() || index.model() != m_model.data())
        return false;
    QPointer<NTreeCheckController> guard(this);
    if (index.data(Qt::CheckStateRole).toInt() != int(state))
        m_model->setData(index, state, Qt::CheckStateRole);
    // setData 的外部槽可能清空、替换或销毁模型；返回后不能继续使用旧帧。
    return guard && generation == m_generation && m_model && index.isValid();
}

void NTreeCheckController::process() {
    if (m_applying)
        return;
    m_timer.stop();
    QPointer<NTreeCheckController> guard(this);
    m_applying = true;
    const int generation = m_generation;
    QElapsedTimer elapsed;
    elapsed.start();
    int steps = 0;
    while (generation == m_generation && steps++ < 128 && elapsed.elapsed() < 4) {
        if (!m_busy) {
            if (m_pending.isEmpty())
                break;
            m_active = m_pending.dequeue();
            if (!m_active.root.isValid())
                continue;
            m_model = const_cast<QAbstractItemModel*>(m_active.root.model());
            if (!m_active.ancestorsOnly)
                m_stack.append(Frame{m_active.root});
            m_ancestor = m_active.ancestorsOnly ? m_active.root : QPersistentModelIndex(m_active.root.parent());
            m_ancestorChild = 0;
            m_ancestorCounts = Counts();
            m_busy = true;
        }
        if (!m_stack.isEmpty()) {
            Frame& frame = m_stack.last();
            if (!frame.index.isValid() || !m_model) {
                cancel();
                break;
            }
            if (!frame.started) {
                frame.started = true;
                const auto flags = frame.index.flags();
                frame.checkable = flags & Qt::ItemIsUserCheckable;
                frame.enabled = flags & Qt::ItemIsEnabled;
                if (!m_active.reconcileOnly && frame.enabled && frame.checkable
                    && m_model->rowCount(frame.index) > 0) {
                    const QPersistentModelIndex index(frame.index);
                    if (!writeState(index, Qt::PartiallyChecked, generation))
                        break;
                }
            }
            // 禁用节点保留原有状态；非勾选容器仍可包含可勾选后代。
            if ((frame.enabled || m_active.reconcileOnly)
                && frame.nextChild < m_model->rowCount(frame.index)) {
                const QModelIndex child = m_model->index(frame.nextChild++, 0, frame.index);
                if (child.isValid())
                    m_stack.append(Frame{QPersistentModelIndex(child)});
                continue;
            }
            const Frame finished(frame);
            m_stack.removeLast();
            if (finished.checkable && finished.enabled) {
                const auto existing = static_cast<Qt::CheckState>(finished.index.data(Qt::CheckStateRole).toInt());
                auto fallback = m_active.reconcileOnly ? existing : m_active.target;
                if (m_active.reconcileOnly && existing == Qt::PartiallyChecked
                    && (finished.index.flags() & Qt::ItemIsAutoTristate))
                    fallback = Qt::Unchecked;
                if (!writeState(finished.index, finished.counts.state(fallback), generation))
                    break;
            }
            if (!m_stack.isEmpty() && finished.checkable)
                m_stack.last().counts.add(static_cast<Qt::CheckState>(finished.index.data(Qt::CheckStateRole).toInt()));
            continue;
        }
        // 子树处理后，只扫描祖先的直接子节点，并同样服从本轮时间和步数预算。
        if (m_ancestor.isValid() && m_model) {
            if (m_ancestorChild < m_model->rowCount(m_ancestor)) {
                const QModelIndex child = m_model->index(m_ancestorChild++, 0, m_ancestor);
                if (child.flags() & Qt::ItemIsUserCheckable)
                    m_ancestorCounts.add(static_cast<Qt::CheckState>(child.data(Qt::CheckStateRole).toInt()));
            } else {
                const QPersistentModelIndex ancestor(m_ancestor);
                if ((ancestor.flags() & Qt::ItemIsUserCheckable) && (ancestor.flags() & Qt::ItemIsEnabled)) {
                    auto existing = static_cast<Qt::CheckState>(ancestor.data(Qt::CheckStateRole).toInt());
                    if (existing == Qt::PartiallyChecked && (ancestor.flags() & Qt::ItemIsAutoTristate))
                        existing = Qt::Unchecked;
                    if (!writeState(ancestor, m_ancestorCounts.state(existing), generation))
                        break;
                }
                m_ancestor = ancestor.parent();
                m_ancestorChild = 0;
                m_ancestorCounts = Counts();
            }
        } else {
            m_busy = false;
        }
    }
    if (!guard)
        return;
    m_applying = false;
    if (m_repaint)
        m_repaint();
    if (m_busy || !m_pending.isEmpty())
        m_timer.start(1);
}
