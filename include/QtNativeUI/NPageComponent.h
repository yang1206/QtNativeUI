#ifndef NPAGECOMPONENT_H
#define NPAGECOMPONENT_H

#include <QVariantMap>
#include <QWidget>
#include "QtNativeUI/stdafx.h"

class QTNATIVEUI_EXPORT NPageComponent : public QWidget {
    Q_OBJECT
  public:
    explicit NPageComponent(QWidget* parent = nullptr);
    virtual ~NPageComponent();

    // 路由生命周期方法
    virtual void onRouteEnter(const QVariantMap& params);
    virtual void onRouteLeave(QVariantMap& outParams);

    // 路由守卫方法
    virtual bool beforeRouteEnter(const QVariantMap& params);
    virtual bool beforeRouteLeave(QVariantMap& outParams);

    // 获取和设置页面路由键
    QString routeKey() const;
    void    setRouteKey(const QString& key);

    // 导航方法
    void navigateTo(const QString& pageKey, const QVariantMap& params = {});
    void navigateBack(const QVariantMap& params = {});

    QString pageTitle() const;
    void    setPageTitle(const QString& title);

  private:
    QString _routeKey;
    QString m_pageTitle;
};

#endif // NPAGECOMPONENT_H