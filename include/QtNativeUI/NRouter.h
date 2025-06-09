#ifndef NROUTER_H
#define NROUTER_H

#include <QMap>
#include <QObject>
#include <QStackedWidget>
#include <QString>
#include <QVector>
#include <QWidget>
#include <memory>

namespace QtNativeUI {

class NRouteItem {
  public:
    NRouteItem(QStackedWidget* stacked = nullptr, const QString& routeKey = QString());

    bool operator==(const NRouteItem& other) const;
    bool operator!=(const NRouteItem& other) const;

    QStackedWidget* stacked;
    QString         routeKey;
};

class NStackedHistory {
  public:
    explicit NStackedHistory(QStackedWidget* stacked = nullptr);

    int  size() const;
    bool isEmpty() const;

    bool push(const QString& routeKey);
    void pop();
    void remove(const QString& routeKey);

    QString top() const;
    void    setDefaultRouteKey(const QString& routeKey);
    void    goToTop();

  private:
    QStackedWidget*  m_stacked;
    QString          m_defaultRouteKey;
    QVector<QString> m_history;
};

class NRouter : public QObject {
    Q_OBJECT
  public:
    explicit NRouter(QObject* parent = nullptr);

    void setDefaultRouteKey(QStackedWidget* stacked, const QString& routeKey);
    void push(QStackedWidget* stacked, const QString& routeKey);
    void pop();
    void remove(const QString& routeKey);

    bool isEmpty() const;

  signals:
    void emptyChanged(bool empty);

  private:
    QVector<NRouteItem>                    m_history;
    QMap<QStackedWidget*, NStackedHistory> m_stackHistories;
};

// 全局路由器实例
extern NRouter* nrouter;

} // namespace QtNativeUI

#endif // NROUTER_H