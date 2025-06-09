#include "QtNativeUI/NRouter.h"
#include <algorithm>

namespace QtNativeUI {

// 全局路由器实例
NRouter* nrouter = nullptr;

// NRouteItem 实现
NRouteItem::NRouteItem(QStackedWidget* stacked, const QString& routeKey) : stacked(stacked), routeKey(routeKey) {}

bool NRouteItem::operator==(const NRouteItem& other) const {
    return other.stacked == stacked && other.routeKey == routeKey;
}

bool NRouteItem::operator!=(const NRouteItem& other) const { return !(*this == other); }

// NStackedHistory 实现
NStackedHistory::NStackedHistory(QStackedWidget* stacked) : m_stacked(stacked), m_defaultRouteKey() {
    m_history.append(m_defaultRouteKey);
}

int NStackedHistory::size() const { return m_history.size(); }

bool NStackedHistory::isEmpty() const { return size() <= 1; }

bool NStackedHistory::push(const QString& routeKey) {
    if (!m_history.isEmpty() && m_history.last() == routeKey) {
        return false;
    }

    m_history.append(routeKey);
    return true;
}

void NStackedHistory::pop() {
    if (isEmpty()) {
        return;
    }

    m_history.removeLast();
    goToTop();
}

void NStackedHistory::remove(const QString& routeKey) {
    if (!m_history.contains(routeKey)) {
        return;
    }

    // 保留第一个元素（默认路由）
    QString defaultRoute = m_history.first();
    m_history.removeAll(routeKey);

    // 确保历史记录不为空
    if (m_history.isEmpty()) {
        m_history.append(defaultRoute);
    }

    // 移除连续重复项
    for (int i = m_history.size() - 1; i > 0; --i) {
        if (m_history[i] == m_history[i - 1]) {
            m_history.removeAt(i);
        }
    }

    goToTop();
}

QString NStackedHistory::top() const { return m_history.isEmpty() ? QString() : m_history.last(); }

void NStackedHistory::setDefaultRouteKey(const QString& routeKey) {
    m_defaultRouteKey = routeKey;

    if (m_history.isEmpty()) {
        m_history.append(routeKey);
    } else {
        m_history[0] = routeKey;
    }
}

void NStackedHistory::goToTop() {
    if (m_stacked && !m_history.isEmpty()) {
        QWidget* widget = m_stacked->findChild<QWidget*>(top());
        if (widget) {
            m_stacked->setCurrentWidget(widget);
        }
    }
}

// NRouter 实现
NRouter::NRouter(QObject* parent) : QObject(parent) {
    if (!nrouter) {
        nrouter = this;
    }
}

void NRouter::setDefaultRouteKey(QStackedWidget* stacked, const QString& routeKey) {
    if (!m_stackHistories.contains(stacked)) {
        m_stackHistories.insert(stacked, NStackedHistory(stacked));
    }

    m_stackHistories[stacked].setDefaultRouteKey(routeKey);
}

void NRouter::push(QStackedWidget* stacked, const QString& routeKey) {
    NRouteItem item(stacked, routeKey);

    if (!m_stackHistories.contains(stacked)) {
        m_stackHistories.insert(stacked, NStackedHistory(stacked));
    }

    // 不添加重复的历史记录
    bool success = m_stackHistories[stacked].push(routeKey);
    if (success) {
        m_history.append(item);
    }

    emit emptyChanged(!m_history.isEmpty());
}

void NRouter::pop() {
    if (m_history.isEmpty()) {
        return;
    }

    NRouteItem item = m_history.last();
    m_history.removeLast();

    emit emptyChanged(!m_history.isEmpty());

    if (m_stackHistories.contains(item.stacked)) {
        m_stackHistories[item.stacked].pop();
    }
}

void NRouter::remove(const QString& routeKey) {
    // 移除所有匹配routeKey的项
    m_history.erase(std::remove_if(m_history.begin(),
                                   m_history.end(),
                                   [routeKey](const NRouteItem& item) { return item.routeKey == routeKey; }),
                    m_history.end());

    // 移除连续重复项
    for (int i = m_history.size() - 1; i > 0; --i) {
        if (m_history[i].routeKey == m_history[i - 1].routeKey) {
            m_history.removeAt(i);
        }
    }

    emit emptyChanged(!m_history.isEmpty());

    // 更新所有包含该routeKey的堆栈历史
    for (auto it = m_stackHistories.begin(); it != m_stackHistories.end(); ++it) {
        QStackedWidget* stacked = it.key();
        QWidget*        widget  = stacked->findChild<QWidget*>(routeKey);
        if (widget) {
            it.value().remove(routeKey);
        }
    }
}

bool NRouter::isEmpty() const { return m_history.isEmpty(); }

} // namespace QtNativeUI