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

    // 创建导航栏
    _navigationBar = new NNavigationBar(q);

    // 创建堆叠窗口
    _stackedWidget = new NStackedWidget(q);

    // 设置布局
    _mainLayout = new QHBoxLayout(q);
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addWidget(_navigationBar);
    _mainLayout->addWidget(_stackedWidget);

    // 连接导航栏信号
    connect(
        _navigationBar, &NNavigationBar::navigationNodeClicked, this, &NNavigationViewPrivate::onNavigationNodeClicked);
    connect(_navigationBar, &NNavigationBar::navigationNodeAdded, this, &NNavigationViewPrivate::onNavigationNodeAdded);
    connect(
        _navigationBar, &NNavigationBar::navigationNodeRemoved, this, &NNavigationViewPrivate::onNavigationNodeRemoved);
    connect(_navigationBar, &NNavigationBar::displayModeChange, this, &NNavigationViewPrivate::onDisplayModeChanged);

    // 连接主题变化信号
    connect(nTheme, &NTheme::themeModeChanged, q, [this](NThemeType::ThemeMode themeMode) { _themeMode = themeMode; });

    _pDisplayMode  = NNavigationType::Maximal;
    _isInitialized = true;
}

void NNavigationViewPrivate::updateLayout() {
    if (!_isInitialized)
        return;

    // 根据导航栏可见性调整布局
    _mainLayout->setContentsMargins(_isNavigationBarVisible ? 5 : 0, 0, 0, 0);
}

void NNavigationViewPrivate::handleNavigationDisplayMode() {
    Q_Q(NNavigationView);

    if (!_isInitialized)
        return;

    // 根据窗口宽度自动调整导航栏显示模式
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

    // 转发信号
    emit q->navigationNodeClicked(nodeType, nodeKey);

    // 处理页面切换
    QWidget* page = _pageMap.value(nodeKey);
    if (!page) {
        return; // 页面不存在
    }

    int pageIndex = _stackedWidget->indexOf(page);
    if (_targetPageIndex == pageIndex || pageIndex < 0) {
        return; // 已经是当前页面或页面索引无效
    }

    _targetPageIndex = pageIndex;

    // 延时执行动画，等待导航栏动画完成
    QTimer::singleShot(180, q, [this, pageIndex]() {
        QWidget* targetWidget = _stackedWidget->widget(pageIndex);
        if (!targetWidget)
            return;

        // 设置当前索引
        _stackedWidget->setCurrentIndex(pageIndex);

        executePageTransition(targetWidget);
        // 发送当前页面变化信号
        Q_Q(NNavigationView);
        emit q->currentChanged(pageIndex);
    });
}

void NNavigationViewPrivate::onNavigationNodeAdded(NNavigationType::NavigationNodeType nodeType,
                                                   QString                             nodeKey,
                                                   QWidget*                            page) {
    Q_Q(NNavigationView);

    // 转发信号
    emit q->navigationNodeAdded(nodeType, nodeKey, page);

    // 添加页面到堆叠窗口
    if (page) {
        _pageMap.insert(nodeKey, page);
        _stackedWidget->addWidget(page);
    }
}

void NNavigationViewPrivate::onNavigationNodeRemoved(NNavigationType::NavigationNodeType nodeType, QString nodeKey) {
    Q_Q(NNavigationView);

    // 转发信号
    emit q->navigationNodeRemoved(nodeType, nodeKey);

    // 从堆叠窗口中移除页面
    if (_pageMap.contains(nodeKey)) {
        QWidget* page = _pageMap.value(nodeKey);
        _pageMap.remove(nodeKey);
        _stackedWidget->removeWidget(page);

        // 如果当前没有页面，尝试导航到第一个可用页面
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
    // 如果设置为无动画，直接返回
    if (_pPageTransitionType == NNavigationType::None) {
        return;
    }

    // 根据动画类型创建不同的动画
    QPropertyAnimation* animation = nullptr;
    switch (_pPageTransitionType) {
        case NNavigationType::SlideVertical: {
            // 垂直滑动动画（从下方滑入）
            animation       = new QPropertyAnimation(targetWidget, "pos");
            QPoint finalPos = targetWidget->pos();
            QPoint startPos = finalPos;
            startPos.setY(finalPos.y() + 80); // 从下方进入
            animation->setStartValue(startPos);
            animation->setEndValue(finalPos);
            break;
        }
        case NNavigationType::SlideHorizontal: {
            // 水平滑动动画（从右侧滑入）
            animation       = new QPropertyAnimation(targetWidget, "pos");
            QPoint finalPos = targetWidget->pos();
            QPoint startPos = finalPos;
            startPos.setX(finalPos.x() + 80); // 从右侧进入
            animation->setStartValue(startPos);
            animation->setEndValue(finalPos);
            break;
        }
        case NNavigationType::Fade: {
            // 淡入淡出动画
            animation = new QPropertyAnimation(targetWidget, "windowOpacity");
            targetWidget->setWindowOpacity(0);
            animation->setStartValue(0.0);
            animation->setEndValue(1.0);
            break;
        }
        default:
            // 默认为垂直滑动
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