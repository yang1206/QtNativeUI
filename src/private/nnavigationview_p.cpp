#include "nnavigationview_p.h"

#include <QGraphicsOpacityEffect>
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NNavigationView.h"
#include "QtNativeUI/NStackedWidget.h"
#include "QtNativeUI/NTheme.h"

#include <QHBoxLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QTimer>

#include "QtNativeUI/NNavigationRouter.h"

NNavigationViewPrivate::NNavigationViewPrivate(QObject* parent) : QObject(parent) { _themeMode = nTheme->themeMode(); }

NNavigationViewPrivate::~NNavigationViewPrivate() {}

void NNavigationViewPrivate::setupUI() {
    Q_Q(NNavigationView);

    _navigationBar = new NNavigationBar(q);

    _stackedWidget = new NStackedWidget(q);

    _mainLayout = new QHBoxLayout(q);
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addWidget(_navigationBar);
    _mainLayout->addWidget(_stackedWidget);

    connect(
        _navigationBar, &NNavigationBar::navigationNodeClicked, this, &NNavigationViewPrivate::onNavigationNodeClicked);
    connect(_navigationBar, &NNavigationBar::navigationNodeAdded, this, &NNavigationViewPrivate::onNavigationNodeAdded);
    connect(
        _navigationBar, &NNavigationBar::navigationNodeRemoved, this, &NNavigationViewPrivate::onNavigationNodeRemoved);
    connect(_navigationBar, &NNavigationBar::displayModeChange, this, &NNavigationViewPrivate::onDisplayModeChanged);

    connect(nTheme, &NTheme::themeModeChanged, q, [this](NThemeType::ThemeMode themeMode) { _themeMode = themeMode; });

    setupRouterConnections();
    _pDisplayMode        = NNavigationType::Maximal;
    _pPageTransitionType = NNavigationType::ScaleTransition;
    _isInitialized       = true;
}

void NNavigationViewPrivate::setupRouterConnections() {
    // 连接路由器信号
    NNavigationRouter* router = NNavigationRouter::getInstance();
    connect(router, &NNavigationRouter::routeChanged, this, &NNavigationViewPrivate::onRouteChanged);
}

void NNavigationViewPrivate::updateLayout() {
    if (!_isInitialized)
        return;

    _mainLayout->setContentsMargins(_isNavigationBarVisible ? 5 : 0, 0, 0, 0);
}

void NNavigationViewPrivate::onRouteChanged(const QString& pageKey, const QVariantMap& params) {
    Q_UNUSED(params);

    if (_pageMap.contains(pageKey)) {
        Q_Q(NNavigationView);
        q->navigation(pageKey, false);
    }
}

void NNavigationViewPrivate::handleNavigationDisplayMode() {
    Q_Q(NNavigationView);

    if (!_isInitialized)
        return;

    if (_pDisplayMode == NNavigationType::Auto) {
        int width = q->width();

        if (width >= 850) {
            _navigationBar->setDisplayMode(NNavigationType::Maximal, true);
        } else if (width >= 550 && width < 850) {
            _navigationBar->setDisplayMode(NNavigationType::Compact, true);
        } else if (width < 550) {
            _navigationBar->setDisplayMode(NNavigationType::Minimal, true);
        }
    } else {
        _navigationBar->setDisplayMode(_pDisplayMode, true);
    }
}

void NNavigationViewPrivate::setDisplayMode(NNavigationType::NavigationDisplayMode mode) {
    _pDisplayMode = mode;
    if (_navigationBar) {
        _navigationBar->setDisplayMode(mode, true);
    }

    if (mode == NNavigationType::Auto) {
        handleNavigationDisplayMode();
    }

    Q_Q(NNavigationView);
    emit q->displayModeChanged(mode);
}

void NNavigationViewPrivate::onNavigationNodeClicked(NNavigationType::NavigationNodeType nodeType, QString nodeKey) {
    Q_Q(NNavigationView);

    emit q->navigationNodeClicked(nodeType, nodeKey);

    QWidget* page = _pageMap.value(nodeKey);
    if (!page) {
        return;
    }

    int pageIndex = _stackedWidget->indexOf(page);
    if (_targetPageIndex == pageIndex || pageIndex < 0) {
        return;
    }

    bool isRouteBack = pageIndex < _targetPageIndex;
    _targetPageIndex = pageIndex;

    _stackedWidget->doPageSwitch(_pPageTransitionType, pageIndex, isRouteBack, _pageTransitionDuration);

    emit q->currentChanged(pageIndex);
}

void NNavigationViewPrivate::onNavigationNodeAdded(NNavigationType::NavigationNodeType nodeType,
                                                   QString                             nodeKey,
                                                   QWidget*                            page) {
    Q_Q(NNavigationView);

    emit q->navigationNodeAdded(nodeType, nodeKey, page);

    if (page) {
        _pageMap.insert(nodeKey, page);
        _stackedWidget->addWidget(page);
    }
}

void NNavigationViewPrivate::onNavigationNodeRemoved(NNavigationType::NavigationNodeType nodeType, QString nodeKey) {
    Q_Q(NNavigationView);

    emit q->navigationNodeRemoved(nodeType, nodeKey);

    if (_pageMap.contains(nodeKey)) {
        QWidget* page = _pageMap.value(nodeKey);
        _pageMap.remove(nodeKey);
        _stackedWidget->removeWidget(page);

        if (_stackedWidget->count() > 0 && _stackedWidget->currentIndex() < 0) {
            QWidget* currentWidget = _stackedWidget->widget(0);
            if (currentWidget) {
                QString pageKey;
                for (auto it = _pageMap.begin(); it != _pageMap.end(); ++it) {
                    if (it.value() == currentWidget) {
                        pageKey = it.key();
                        break;
                    }
                }
                if (!pageKey.isEmpty()) {
                    q->navigation(pageKey);
                }
            }
        }
    }
}

void NNavigationViewPrivate::onDisplayModeChanged(NNavigationType::NavigationDisplayMode displayMode) {
    Q_Q(NNavigationView);

    emit q->displayModeChanged(displayMode);
}
