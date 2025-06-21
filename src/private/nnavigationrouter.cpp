#include "nnavigationrouter.h"

Q_SINGLETON_CREATE_CPP(NNavigationRouter)

NNavigationRouter::NNavigationRouter(QObject* parent) : QObject{parent} { _maxRouteCount = 25; }

NNavigationRouter::~NNavigationRouter() {}

NNavigationRouterType::NavigationRouteType NNavigationRouter::navigationRoute(QObject*           routeObject,
                                                                             QString            routeFunctionName,
                                                                             const QVariantMap& routeData,
                                                                             Qt::ConnectionType connectionType) {
    if (!routeObject) {
        return NNavigationRouterType::ObjectInvalid;
    }
    if (routeFunctionName.isEmpty()) {
        return NNavigationRouterType::FunctionNameInvalid;
    }

    if (_routeQueue.isEmpty()) {
        Q_EMIT navigationRouterStateChanged(true);
    } else {
        if (_routeQueue.count() >= _maxRouteCount) {
            _routeQueue.dequeue();
        }
    }

    QVariantMap saveData;
    saveData.insert("NRouteObject", QVariant::fromValue<QObject*>(routeObject));
    saveData.insert("NRouteFunctionName", routeFunctionName);
    saveData.insert("NRouteData", routeData);
    saveData.insert("NRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
    _routeQueue.enqueue(saveData);

    return NNavigationRouterType::Success;
}

void NNavigationRouter::navigationRouteBack() {
    int routeQueueCount = _routeQueue.count();
    if (routeQueueCount > 0) {
        if (routeQueueCount == 1) {
            Q_EMIT navigationRouterStateChanged(false);
        }
        QVariantMap routeSaveData = _routeQueue.last();
        _routeQueue.removeLast();
        QObject*           routeObject       = routeSaveData.value("NRouteObject").value<QObject*>();
        QString            routeFunctionName = routeSaveData.value("NRouteFunctionName").toString();
        QVariantMap        routeData         = routeSaveData.value("NRouteData").toMap();
        Qt::ConnectionType connectionType    = routeSaveData.value("NRouteConnectionType").value<Qt::ConnectionType>();
        QMetaObject::invokeMethod(
            routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
    }
}