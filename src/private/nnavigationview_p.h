#ifndef NNAVIGATIONVIEW_P_H
#define NNAVIGATIONVIEW_P_H

#include <QMap>
#include <QPointer>

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
    void setupRouterConnections();
    void switchPage(const QString& key, bool back);
    bool eventFilter(QObject* watched, QEvent* event) override;

  public slots:
    void onNavigationNodeClicked(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void onNavigationNodeAdded(NNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    void onNavigationNodeRemoved(NNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void onRouteChanged(const QString& pageKey, const QVariantMap& params);

  private:
    NNavigationBar* _navigationBar{nullptr};
    NStackedWidget* _stackedWidget{nullptr};
    QHBoxLayout*    _mainLayout{nullptr};

    QMap<QString, QPointer<QWidget>> _pageMap;
    QPointer<QWidget> _currentPage;
    int _currentPageIndex{-1};
    QWidget* _paneSpacer{nullptr};
    QWidget* _paneScrim{nullptr};
    NNavigationType::NavigationDisplayMode _effectiveDisplayMode{NNavigationType::Maximal};
    bool _isPaneOpen{true};
    int                     _pageTransitionDuration{250};
    bool                    _isNavigationBarVisible{true};
    bool                    _isInitialized{false};
};

#endif // NNAVIGATIONVIEW_P_H
