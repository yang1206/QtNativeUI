#include "nnavigationview_p.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QShortcut>
#include <QResizeEvent>
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NNavigationRouter.h"
#include "QtNativeUI/NNavigationView.h"
#include "QtNativeUI/NStackedWidget.h"

NNavigationViewPrivate::NNavigationViewPrivate(QObject* parent) : QObject(parent) {}
NNavigationViewPrivate::~NNavigationViewPrivate() {}

void NNavigationViewPrivate::setupUI() {
    Q_Q(NNavigationView);
    _pDisplayMode = NNavigationType::Auto;
    _pPageTransitionType = NNavigationType::ScaleTransition;
    _stackedWidget = new NStackedWidget(q);
    _paneSpacer = new QWidget(q);
    _paneSpacer->setObjectName("navigationPaneSpacer");
    _mainLayout = new QHBoxLayout(q);
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addWidget(_paneSpacer);
    _mainLayout->addWidget(_stackedWidget, 1);
    _paneScrim = new QWidget(q);
    _paneScrim->setObjectName("navigationPaneScrim");
    _paneScrim->setStyleSheet("#navigationPaneScrim { background-color: rgba(0, 0, 0, 40); }");
    _paneScrim->hide();
    _paneScrim->installEventFilter(this);
    _navigationBar = new NNavigationBar(q);
    _navigationBar->installEventFilter(this);
    connect(_navigationBar, &NNavigationBar::navigationNodeClicked, this, &NNavigationViewPrivate::onNavigationNodeClicked);
    connect(_navigationBar, &NNavigationBar::navigationNodeAdded, this, &NNavigationViewPrivate::onNavigationNodeAdded);
    connect(_navigationBar, &NNavigationBar::navigationNodeRemoved, this, &NNavigationViewPrivate::onNavigationNodeRemoved);
    connect(_navigationBar, &NNavigationBar::paneToggleRequested, q, [q] { q->setPaneOpen(!q->isPaneOpen()); });
    auto* escape = new QShortcut(QKeySequence(Qt::Key_Escape), q);
    connect(escape, &QShortcut::activated, q, [this, q] {
        if (_effectiveDisplayMode != NNavigationType::Maximal) q->setPaneOpen(false);
    });
    setupRouterConnections();
    _isInitialized = true;
    handleNavigationDisplayMode();
}

void NNavigationViewPrivate::setupRouterConnections() {
    auto* router = _navigationBar->router();
    connect(router, &NNavigationRouter::routeChanged, this, &NNavigationViewPrivate::onRouteChanged);
    connect(router, &NNavigationRouter::routeBack, this, [this](const QString&, const QString& to, const QVariantMap&) {
        switchPage(to, true);
    });
}

void NNavigationViewPrivate::updateLayout() {
    if (!_isInitialized) return;
    Q_Q(NNavigationView);
    const bool minimal = _effectiveDisplayMode == NNavigationType::Minimal;
    const auto mode = _isPaneOpen ? NNavigationType::Maximal : (minimal ? NNavigationType::Minimal : NNavigationType::Compact);
    _navigationBar->setDisplayMode(mode, q->isVisible());
    const int reserved = !_isNavigationBarVisible || minimal ? 0
        : (_effectiveDisplayMode == NNavigationType::Maximal ? _navigationBar->width() : 47);
    _paneSpacer->setFixedWidth(reserved);
    _paneSpacer->setVisible(reserved > 0);
    _mainLayout->setContentsMargins(0, _isNavigationBarVisible && minimal ? 48 : 0, 0, 0);
    _mainLayout->activate();
    _navigationBar->setFixedHeight(minimal && !_isPaneOpen ? 48 : q->height());
    _navigationBar->move(0, 0);
    _navigationBar->setVisible(_isNavigationBarVisible);
    const bool overlay = _isNavigationBarVisible && _isPaneOpen && _effectiveDisplayMode != NNavigationType::Maximal;
    _paneScrim->setGeometry(q->rect());
    _paneScrim->setVisible(overlay);
    if (overlay) _paneScrim->raise();
    _navigationBar->setIsTransparent(!overlay);
    _navigationBar->raise();
}

bool NNavigationViewPrivate::eventFilter(QObject* watched, QEvent* event) {
    if (_isInitialized && watched == _navigationBar && event->type() == QEvent::Resize
        && _isNavigationBarVisible && _effectiveDisplayMode == NNavigationType::Maximal) {
        _paneSpacer->setFixedWidth(static_cast<QResizeEvent*>(event)->size().width());
        _mainLayout->activate();
    }
    if (watched == _paneScrim && event->type() == QEvent::MouseButtonPress) {
        Q_Q(NNavigationView);
        q->setPaneOpen(false);
        return true;
    }
    return QObject::eventFilter(watched, event);
}

void NNavigationViewPrivate::handleNavigationDisplayMode() {
    if (!_isInitialized) return;
    Q_Q(NNavigationView);
    auto mode = _pDisplayMode;
    if (mode == NNavigationType::Auto) {
        mode = q->width() >= 850 ? NNavigationType::Maximal : (q->width() >= 550 ? NNavigationType::Compact : NNavigationType::Minimal);
    }
    if (mode != _effectiveDisplayMode) {
        _effectiveDisplayMode = mode;
        const bool open = mode == NNavigationType::Maximal;
        if (_isPaneOpen != open) {
            _isPaneOpen = open;
            emit q->paneOpenChanged(open);
        }
        emit q->displayModeChanged(mode);
    }
    updateLayout();
}

void NNavigationViewPrivate::setDisplayMode(NNavigationType::NavigationDisplayMode mode) {
    _pDisplayMode = mode;
    handleNavigationDisplayMode();
}

void NNavigationViewPrivate::onRouteChanged(const QString& key, const QVariantMap&) {
    if (key.isEmpty()) {
        _stackedWidget->doPageSwitch(NNavigationType::NoTransition, _stackedWidget->currentIndex(), false, 0);
        if (auto* page = _stackedWidget->currentWidget()) page->hide();
        _currentPage.clear();
        if (_currentPageIndex != -1) {
            _currentPageIndex = -1;
            Q_Q(NNavigationView);
            emit q->currentChanged(-1);
        }
        return;
    }
    switchPage(key, false);
}

void NNavigationViewPrivate::switchPage(const QString& key, bool back) {
    auto page = _pageMap.value(key);
    if (!page) return;
    const int index = _stackedWidget->indexOf(page);
    if (index < 0) return;
    const bool changed = _currentPage != page || _currentPageIndex != index;
    _currentPage = page;
    _currentPageIndex = index;
    if (_stackedWidget->currentIndex() != index) {
        _stackedWidget->doPageSwitch(_pPageTransitionType, index, back, _pageTransitionDuration);
    } else {
        page->show();
    }
    if (_effectiveDisplayMode != NNavigationType::Maximal) {
        Q_Q(NNavigationView);
        q->setPaneOpen(false);
    }
    if (changed) {
        Q_Q(NNavigationView);
        emit q->currentChanged(index);
    }
}

void NNavigationViewPrivate::onNavigationNodeClicked(NNavigationType::NavigationNodeType type, QString key) {
    Q_Q(NNavigationView);
    emit q->navigationNodeClicked(type, key);
}

void NNavigationViewPrivate::onNavigationNodeAdded(NNavigationType::NavigationNodeType type, QString key, QWidget* page) {
    Q_Q(NNavigationView);
    if (page) {
        _pageMap.insert(key, page);
        _stackedWidget->addWidget(page);
        if (!_currentPage) page->hide();
    }
    emit q->navigationNodeAdded(type, key, page);
}

void NNavigationViewPrivate::onNavigationNodeRemoved(NNavigationType::NavigationNodeType type, QString key) {
    Q_Q(NNavigationView);
    auto page = _pageMap.take(key);
    if (page) {
        _stackedWidget->removeWidget(page);
        page->hide();
    }
    if (_currentPage && _navigationBar->router()->containsPage(_navigationBar->router()->currentRouteKey())) {
        const int index = _stackedWidget->indexOf(_currentPage);
        if (index != _currentPageIndex) {
            _currentPageIndex = index;
            emit q->currentChanged(index);
        }
    }
    emit q->navigationNodeRemoved(type, key);
    if (_navigationBar->router()->currentRouteKey().isEmpty()) {
        // 优先使用栈选出的相邻页面；整棵子树的路由已先行注销。
        auto* current = _stackedWidget->currentWidget();
        for (int pass = 0; pass < 2; ++pass) {
            for (auto it = _pageMap.cbegin(); it != _pageMap.cend(); ++it) {
                if (!it.value() || (pass == 0 && it.value() != current)) continue;
                if (_navigationBar->router()->containsPage(it.key())) {
                    if (_navigationBar->router()->navigateTo(it.key()) == NNavigationRouterType::Success) return;
                }
            }
        }
        if (auto* page = _stackedWidget->currentWidget()) page->hide();
    }
}
