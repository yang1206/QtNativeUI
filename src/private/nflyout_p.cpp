#include "nflyout_p.h"
#include <QApplication>
#include <QCursor>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QScreen>
#include "QtNativeUI/NFlyout.h"
#include "QtNativeUI/NTheme.h"

QHash<NFlyoutAnimationType, NFlyoutAnimationManager* (*) (NFlyout*)> NFlyoutAnimationManager::s_managers;

// 动画管理器基类实现
NFlyoutAnimationManager::NFlyoutAnimationManager(NFlyout* flyout, QObject* parent) : QObject(parent), m_flyout(flyout) {
    m_aniGroup   = new QParallelAnimationGroup(this);
    m_slideAni   = new QPropertyAnimation(flyout, "pos", this);
    m_opacityAni = new QPropertyAnimation(flyout, "windowOpacity", this);

    setup();
}

void NFlyoutAnimationManager::setup() const {
    m_slideAni->setDuration(187);
    m_opacityAni->setDuration(187);

    m_opacityAni->setStartValue(0.0);
    m_opacityAni->setEndValue(1.0);

    m_slideAni->setEasingCurve(QEasingCurve::OutQuad);
    m_opacityAni->setEasingCurve(QEasingCurve::OutQuad);

    m_aniGroup->addAnimation(m_slideAni);
    m_aniGroup->addAnimation(m_opacityAni);
}

void NFlyoutAnimationManager::exec(const QPoint& pos) {
    // 基类中不实现，由子类具体实现
}

QPoint NFlyoutAnimationManager::position(QWidget* target) {
    // 基类中不实现，由子类具体实现
    return QPoint();
}

QPoint NFlyoutAnimationManager::adjustPosition(const QPoint& pos) const {
    QScreen* screen = QGuiApplication::screenAt(QCursor::pos());
    if (!screen) {
        screen = QGuiApplication::primaryScreen();
    }

    QRect screenRect = screen->geometry();
    int   w          = m_flyout->sizeHint().width() + 5;
    int   h          = m_flyout->sizeHint().height();

    int x = qMax(screenRect.left(), qMin(pos.x(), screenRect.right() - w));
    int y = qMax(screenRect.top(), qMin(pos.y() - 4, screenRect.bottom() - h + 5));

    return QPoint(x, y);
}

NFlyoutAnimationManager* NFlyoutAnimationManager::create(NFlyoutAnimationType type, NFlyout* flyout) {
    if (s_managers.contains(type)) {
        return s_managers[type](flyout);
    }

    // 默认使用PULL_UP类型
    return new PullUpAnimationManager(flyout);
}

// PullUp动画管理器实现
void PullUpAnimationManager::exec(const QPoint& pos) {
    QPoint adjustedPos = adjustPosition(pos);
    m_slideAni->setStartValue(adjustedPos + QPoint(0, 8));
    m_slideAni->setEndValue(adjustedPos);
    m_aniGroup->start();
}

QPoint PullUpAnimationManager::position(QWidget* target) {
    QPoint pos = target->mapToGlobal(QPoint(0, 0));
    int    x   = pos.x() + target->width() / 2 - m_flyout->sizeHint().width() / 2;
    int    y   = pos.y() - m_flyout->sizeHint().height() + m_flyout->layout()->contentsMargins().bottom();
    return QPoint(x, y);
}

// DropDown动画管理器实现
void DropDownAnimationManager::exec(const QPoint& pos) {
    QPoint adjustedPos = adjustPosition(pos);
    m_slideAni->setStartValue(adjustedPos - QPoint(0, 8));
    m_slideAni->setEndValue(adjustedPos);
    m_aniGroup->start();
}

QPoint DropDownAnimationManager::position(QWidget* target) {
    QPoint pos = target->mapToGlobal(QPoint(0, target->height()));
    int    x   = pos.x() + target->width() / 2 - m_flyout->sizeHint().width() / 2;
    int    y   = pos.y() - m_flyout->layout()->contentsMargins().top() + 8;
    return QPoint(x, y);
}

// SlideLeft动画管理器实现
void SlideLeftAnimationManager::exec(const QPoint& pos) {
    QPoint adjustedPos = adjustPosition(pos);
    m_slideAni->setStartValue(adjustedPos + QPoint(8, 0));
    m_slideAni->setEndValue(adjustedPos);
    m_aniGroup->start();
}

QPoint SlideLeftAnimationManager::position(QWidget* target) {
    QPoint pos = target->mapToGlobal(QPoint(0, 0));
    int    x   = pos.x() - m_flyout->sizeHint().width() + 8;
    int    y   = pos.y() - m_flyout->sizeHint().height() / 2 + target->height() / 2 +
            m_flyout->layout()->contentsMargins().top();
    return QPoint(x, y);
}

// SlideRight动画管理器实现
void SlideRightAnimationManager::exec(const QPoint& pos) {
    QPoint adjustedPos = adjustPosition(pos);
    m_slideAni->setStartValue(adjustedPos - QPoint(8, 0));
    m_slideAni->setEndValue(adjustedPos);
    m_aniGroup->start();
}

QPoint SlideRightAnimationManager::position(QWidget* target) {
    QPoint pos = target->mapToGlobal(QPoint(target->width(), 0));
    int    x   = pos.x() - 8;
    int    y   = pos.y() - m_flyout->sizeHint().height() / 2 + target->height() / 2 +
            m_flyout->layout()->contentsMargins().top();
    return QPoint(x, y);
}

// FadeIn动画管理器实现
void FadeInAnimationManager::exec(const QPoint& pos) {
    m_flyout->move(adjustPosition(pos));
    m_aniGroup->removeAnimation(m_slideAni);
    m_aniGroup->start();
}

QPoint FadeInAnimationManager::position(QWidget* target) {
    QPoint pos = target->mapToGlobal(QPoint(0, 0));
    int    x   = pos.x() + target->width() / 2 - m_flyout->sizeHint().width() / 2;
    int    y   = pos.y() - m_flyout->sizeHint().height() + m_flyout->layout()->contentsMargins().bottom();
    return QPoint(x, y);
}

// None动画管理器实现
void NoneAnimationManager::exec(const QPoint& pos) { m_flyout->move(adjustPosition(pos)); }

QPoint NoneAnimationManager::position(QWidget* target) {
    QHBoxLayout* layout  = qobject_cast<QHBoxLayout*>(m_flyout->layout());
    QMargins     margins = layout ? layout->contentsMargins() : QMargins();

    return target->mapToGlobal(QPoint(-margins.left(), -m_flyout->sizeHint().height() + margins.bottom() - 8));
}

NFlyoutPrivate::NFlyoutPrivate(QObject* parent)
    : QObject(parent),
      _pContent(nullptr),
      _pTarget(nullptr),
      q_ptr(nullptr),
      _themeMode(nTheme->themeMode()),
      _isDark(nTheme->isDarkMode()),
      _isHovered(false),
      _isPressed(false),
      _isOpen(false),
      _shadowBorderWidth(5),
      _lightDismissMode(NFlyout::Auto),
      _animationType(NFlyoutAnimationType::PULL_UP),
      _isDeleteOnClose(true),
      _animManager(nullptr),
      _fadeOutAnimation(nullptr),
      _shadowEffect(nullptr),
      _mainLayout(nullptr) {
    // 设置背景色和边框色
    _pLightBackgroundColor = QColor(248, 248, 248);
    _pDarkBackgroundColor  = QColor(40, 40, 40);
    _pLightBorderColor     = NThemeColor(NFluentColorKey::SurfaceStrokeColorFlyout, NThemeType::Light);
    _pDarkBorderColor      = NThemeColor(NFluentColorKey::SurfaceStrokeColorFlyout, NThemeType::Dark);

    // 设置默认属性
    _pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    _pBorderWidth  = 1;
    _pPlacement    = Qt::BottomEdge;
}

NFlyoutPrivate::~NFlyoutPrivate() {
    delete _animManager;
    delete _fadeOutAnimation;
}

void NFlyoutPrivate::setupUI() {
    Q_Q(NFlyout);

    if (!q)
        return;

    // 创建主布局
    _mainLayout = new QHBoxLayout(q);
    _mainLayout->setContentsMargins(15, 8, 15, 20);

    if (_pContent) {
        _mainLayout->addWidget(_pContent);

        QColor shadowColor = _isDark ? QColor(0, 0, 0, 80) : QColor(0, 0, 0, 30);
        _shadowEffect      = new QGraphicsDropShadowEffect(_pContent);
        _shadowEffect->setBlurRadius(35);
        _shadowEffect->setOffset(0, 8);
        _shadowEffect->setColor(shadowColor);
        _pContent->setGraphicsEffect(_shadowEffect);
    }
}

NFlyoutAnimationManager* NFlyoutPrivate::animationManager() {
    if (!_animManager) {
        // 根据放置方向选择合适的动画类型
        NFlyoutAnimationType animType = _animationType;

        // 如果没有显式设置动画类型，则根据placement选择
        if (_animationType == NFlyoutAnimationType::PULL_UP) {
            switch (_pPlacement) {
                case Qt::TopEdge:
                    animType = NFlyoutAnimationType::DROP_DOWN;
                    break;
                case Qt::BottomEdge:
                    animType = NFlyoutAnimationType::PULL_UP;
                    break;
                case Qt::LeftEdge:
                    animType = NFlyoutAnimationType::SLIDE_RIGHT;
                    break;
                case Qt::RightEdge:
                    animType = NFlyoutAnimationType::SLIDE_LEFT;
                    break;
                default:
                    break;
            }
        }

        _animManager = NFlyoutAnimationManager::create(animType, q_func());
    }
    return _animManager;
}

void NFlyoutPrivate::updateThemeColors() { _isDark = nTheme->isDarkMode(); }

QRect NFlyoutPrivate::calculatePlacement(const QWidget* target, const Qt::Edge placement) const {
    if (!target || !q_ptr) {
        return QRect();
    }

    QSize  flyoutSize      = q_ptr->sizeHint();
    QRect  targetRect      = target->rect();
    QPoint targetGlobalPos = target->mapToGlobal(QPoint(0, 0));

    // 在目标控件的位置基础上计算Flyout的位置
    QRect result;
    switch (placement) {
        case Qt::TopEdge:
            // 在目标控件上方显示
            result = QRect(targetGlobalPos.x() + (targetRect.width() - flyoutSize.width()) / 2,
                           targetGlobalPos.y() - flyoutSize.height() - 10, // 额外空间避免遮挡
                           flyoutSize.width(),
                           flyoutSize.height());
            break;
        case Qt::BottomEdge:
            // 在目标控件下方显示
            result = QRect(targetGlobalPos.x() + (targetRect.width() - flyoutSize.width()) / 2,
                           targetGlobalPos.y() + targetRect.height() + 10, // 额外空间避免遮挡
                           flyoutSize.width(),
                           flyoutSize.height());
            break;
        case Qt::LeftEdge:
            // 在目标控件左侧显示
            result = QRect(targetGlobalPos.x() - flyoutSize.width() - 10, // 额外空间避免遮挡
                           targetGlobalPos.y() + (targetRect.height() - flyoutSize.height()) / 2,
                           flyoutSize.width(),
                           flyoutSize.height());
            break;
        case Qt::RightEdge:
            // 在目标控件右侧显示
            result = QRect(targetGlobalPos.x() + targetRect.width() + 10, // 额外空间避免遮挡
                           targetGlobalPos.y() + (targetRect.height() - flyoutSize.height()) / 2,
                           flyoutSize.width(),
                           flyoutSize.height());
            break;
        default:
            // 默认显示在下方
            result = QRect(targetGlobalPos.x() + (targetRect.width() - flyoutSize.width()) / 2,
                           targetGlobalPos.y() + targetRect.height() + 10,
                           flyoutSize.width(),
                           flyoutSize.height());
    }

    // 确保Flyout不会超出屏幕边界
    QScreen* screen = QGuiApplication::screenAt(targetGlobalPos);
    if (!screen) {
        screen = QGuiApplication::primaryScreen();
    }

    QRect screenRect = screen->availableGeometry();

    if (result.left() < screenRect.left()) {
        result.moveLeft(screenRect.left() + 10);
    } else if (result.right() > screenRect.right()) {
        result.moveRight(screenRect.right() - 10);
    }

    if (result.top() < screenRect.top()) {
        result.moveTop(screenRect.top() + 10);
    } else if (result.bottom() > screenRect.bottom()) {
        result.moveBottom(screenRect.bottom() - 10);
    }

    return result;
}
