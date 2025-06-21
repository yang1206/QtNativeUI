#ifndef NNAVIGATIONROUTER_H
#define NNAVIGATIONROUTER_H

#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/singleton.h"
#include "QtNativeUI/stdafx.h"

class QTNATIVEUI_EXPORT NNavigationRouter : public QObject {
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NNavigationRouter)
    Q_PROPERTY(int maxRouteCount READ maxRouteCount WRITE setMaxRouteCount)

  private:
    explicit NNavigationRouter(QObject* parent = nullptr);
    ~NNavigationRouter() override;

  Q_SIGNALS:
    void navigationRouterStateChanged(bool state);

  public:
    int  maxRouteCount() const { return _maxRouteCount; }
    void setMaxRouteCount(int count) { _maxRouteCount = count; }

    NNavigationRouterType::NavigationRouteType navigationRoute(QObject*           routeObject,
                                                         QString            routeFunctionName,
                                                         const QVariantMap& routeData      = {},
                                                         Qt::ConnectionType connectionType = Qt::AutoConnection);
    void                                 navigationRouteBack();

  private:
    int                 _maxRouteCount = 25;
    QQueue<QVariantMap> _routeQueue;
};

#endif // NNAVIGATIONROUTER_H