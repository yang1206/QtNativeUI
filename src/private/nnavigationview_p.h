#ifndef NNAVIGATIONVIEW_P_H
#define NNAVIGATIONVIEW_P_H

#include <QMap>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/stdafx.h"

class NNavigationView;
class NNavigationBar;
class NStackedWidget;
class QHBoxLayout;
class QPropertyAnimation;

class NNavigationViewPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NNavigationView)
    Q_PROPERTY_CREATE_D(NNavigationType::NavigationDisplayMode, DisplayMode)
    Q_PROPERTY_CREATE_D(NNavigationType::PageTransitionType, PageTransitionType)

  public:
    explicit NNavigationViewPrivate(QObject* parent = nullptr);
    ~NNavigationViewPrivate() override;

    void setupUI();
    void updateLayout();
    void handleNavigationDisplayMode();
    void setDisplayMode(NNavigationType::NavigationDisplayMode mode);
    void executePageTransition(QWidget* targetWidget);
    void setupRouterConnections();

  public slots:
    void onNavigationNodeClicked(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void onNavigationNodeAdded(NNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    void onNavigationNodeRemoved(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void onDisplayModeChanged(NNavigationType::NavigationDisplayMode displayMode);
    void onRouteChanged(const QString& pageKey, const QVariantMap& params);

  private:
    NNavigationBar* _navigationBar{nullptr};
    NStackedWidget* _stackedWidget{nullptr};
    QHBoxLayout*    _mainLayout{nullptr};

    QMap<QString, QWidget*> _pageMap;
    int                     _targetPageIndex{0};
    int                     _pageTransitionDuration{250};
    bool                    _isNavigationBarVisible{true};
    bool                    _isInitialized{false};
    NThemeType::ThemeMode   _themeMode{NThemeType::Light};
};

#endif // NNAVIGATIONVIEW_P_H