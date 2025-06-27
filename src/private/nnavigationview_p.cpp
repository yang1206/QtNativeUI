#include "nnavigationview_p.h"
#include "QtNativeUI/NNavigationBar.h"
#include "QtNativeUI/NNavigationView.h"
#include "QtNativeUI/NStackedWidget.h"
#include "QtNativeUI/NTheme.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QTimer>

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

    _pDisplayMode  = NNavigationType::Maximal;
    _isInitialized = true;
}

void NNavigationViewPrivate::updateLayout() {
    if (!_isInitialized)
        return;

    _mainLayout->setContentsMargins(_isNavigationBarVisible ? 5 : 0, 0, 0, 0);
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

    _targetPageIndex = pageIndex;

    QTimer::singleShot(180, q, [this, pageIndex]() {
        QWidget* targetWidget = _stackedWidget->widget(pageIndex);
        if (!targetWidget)
            return;

        _stackedWidget->setCurrentIndex(pageIndex);

        executePageTransition(targetWidget);
        Q_Q(NNavigationView);
        emit q->currentChanged(pageIndex);
    });
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

void NNavigationViewPrivate::executePageTransition(QWidget* targetWidget) {
    if (_pPageTransitionType == NNavigationType::None) {
        return;
    }

    QPropertyAnimation* animation = nullptr;
    switch (_pPageTransitionType) {
        case NNavigationType::SlideVertical: {
            animation       = new QPropertyAnimation(targetWidget, "pos");
            QPoint finalPos = targetWidget->pos();
            QPoint startPos = finalPos;
            startPos.setY(finalPos.y() + 80);
            animation->setStartValue(startPos);
            animation->setEndValue(finalPos);
            break;
        }
        case NNavigationType::SlideHorizontal: {
            animation       = new QPropertyAnimation(targetWidget, "pos");
            QPoint finalPos = targetWidget->pos();
            QPoint startPos = finalPos;
            startPos.setX(finalPos.x() + 80);
            animation->setStartValue(startPos);
            animation->setEndValue(finalPos);
            break;
        }
        case NNavigationType::Fade: {
            animation = new QPropertyAnimation(targetWidget, "windowOpacity");
            targetWidget->setWindowOpacity(0);
            animation->setStartValue(0.0);
            animation->setEndValue(1.0);
            break;
        }
        default:
            animation       = new QPropertyAnimation(targetWidget, "pos");
            QPoint finalPos = targetWidget->pos();
            QPoint startPos = finalPos;
            startPos.setY(finalPos.y() + 80);

            animation->setStartValue(startPos);
            animation->setEndValue(finalPos);
            break;
    }

    if (animation) {
        animation->setEasingCurve(QEasingCurve::OutCubic);
        animation->setDuration(_pageTransitionDuration);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}