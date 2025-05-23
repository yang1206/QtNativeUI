#include "QtNativeUI/NFlyout.h"
#include <QApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include "../private/nflyout_p.h"
#include "QtNativeUI/NTheme.h"
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, DarkBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, int, BorderWidth)
Q_PROPERTY_CREATE_Q_CPP(NFlyout, Qt::Edge, Placement)
NFlyout::NFlyout(QWidget* parent) : QWidget(parent), d_ptr(new NFlyoutPrivate()) {
    Q_D(NFlyout);
    d->q_ptr = this;

    // 设置默认属性
    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    d->_pBorderWidth  = 1;
    d->_pPlacement    = Qt::BottomEdge;
    // 设置窗口属性
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose, false);

    // 设置最小尺寸
    setMinimumSize(100, 50);

    // 设置鼠标追踪
    setMouseTracking(true);

    // 初始化UI
    d->setupUI();

    // 设置事件过滤器
    setupEventFilter();

    // 连接主题变更信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NFlyout);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });
}

NFlyout::NFlyout(QWidget* content, QWidget* parent) : QWidget(parent), d_ptr(new NFlyoutPrivate()) {
    Q_D(NFlyout);
    d->q_ptr = this;

    // 设置窗口属性
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose, false);

    // 设置最小尺寸
    setMinimumSize(100, 50);

    // 设置鼠标追踪
    setMouseTracking(true);

    // 初始化UI
    d->setupUI();

    // 设置事件过滤器
    setupEventFilter();

    // 连接主题变更信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NFlyout);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    // 设置内容
    setContent(content);
}

NFlyout::~NFlyout() { QApplication::instance()->removeEventFilter(this); }

void NFlyout::setContent(QWidget* content) {
    Q_D(NFlyout);

    if (content == d->_pContent)
        return;
    if (d->_pContent && d->_mainLayout) {
        d->_mainLayout->removeWidget(d->_pContent);
        d->_pContent->setParent(nullptr);
    }

    d->_pContent = content;
    if (content && d->_mainLayout) {
        content->setParent(this);
        d->_mainLayout->addWidget(content);
    }

    update();
}

QWidget* NFlyout::content() const {
    Q_D(const NFlyout);
    return d->_pContent;
}

void NFlyout::setTarget(QWidget* target) {
    Q_D(NFlyout);
    d->_pTarget = target;
}

QWidget* NFlyout::target() const {
    Q_D(const NFlyout);
    return d->_pTarget;
}

bool NFlyout::isOpen() const {
    Q_D(const NFlyout);
    return d->_isOpen;
}

void NFlyout::setAnimationType(NFlyoutAnimationType type) {
    Q_D(NFlyout);
    if (d->_animationType != type) {
        d->_animationType = type;

        if (d->_animManager) {
            delete d->_animManager;
            d->_animManager = nullptr;
        }
    }
}

NFlyoutAnimationType NFlyout::animationType() const {
    Q_D(const NFlyout);
    return d->_animationType;
}

void NFlyout::setLightDismissMode(LightDismissMode mode) {
    Q_D(NFlyout);
    d->_lightDismissMode = mode;
}

NFlyout::LightDismissMode NFlyout::lightDismissMode() const {
    Q_D(const NFlyout);
    return d->_lightDismissMode;
}

void NFlyout::setShadowEffect(int blurRadius, const QPoint& offset) {
    Q_D(NFlyout);
    if (d->_shadowEffect) {
        d->_shadowEffect->setBlurRadius(blurRadius);
        d->_shadowEffect->setOffset(offset.x(), offset.y());

        QColor shadowColor = d->_isDark ? QColor(0, 0, 0, 80) : QColor(0, 0, 0, 30);
        d->_shadowEffect->setColor(shadowColor);
    }
}

NFlyout*
NFlyout::make(QWidget* content, QWidget* target, QWidget* parent, NFlyoutAnimationType animType, bool isDeleteOnClose) {
    NFlyout* flyout = new NFlyout(content, parent);

    // 配置属性
    flyout->setAnimationType(animType);
    flyout->setAttribute(Qt::WA_DeleteOnClose, isDeleteOnClose);

    if (target) {
        flyout->show();
        flyout->showAt(target);
    }

    return flyout;
}

void NFlyout::showAt(QWidget* target) {
    if (!target)
        return;

    Q_D(NFlyout);
    d->_pTarget = target;
    emit opening();

    // 根据placement属性计算位置
    QRect  rect = d->calculatePlacement(target, d->_pPlacement);
    QPoint pos  = rect.topLeft();

    // 使用动画管理器执行动画
    exec(pos);

    // 设置为打开状态
    d->_isOpen = true;
    emit opened();
}

void NFlyout::exec(const QPoint& pos) {
    Q_D(NFlyout);

    // 确保窗口可见
    show();

    // 使用动画管理器执行动画
    d->animationManager()->exec(pos);
}

void NFlyout::hide() {
    Q_D(NFlyout);

    if (!d->_isOpen)
        return;

    emit closing();
    QWidget::hide();
    d->_isOpen = false;
    emit closed();
}

void NFlyout::fadeOut() {
    Q_D(NFlyout);

    // 创建淡出动画
    if (!d->_fadeOutAnimation) {
        d->_fadeOutAnimation = new QPropertyAnimation(this, "windowOpacity", this);
        d->_fadeOutAnimation->setDuration(120);
        connect(d->_fadeOutAnimation, &QPropertyAnimation::finished, this, &NFlyout::hide);
    }

    d->_fadeOutAnimation->setStartValue(1.0);
    d->_fadeOutAnimation->setEndValue(0.0);
    d->_fadeOutAnimation->start();
}

void NFlyout::showEvent(QShowEvent* event) {
    activateWindow();
    QWidget::showEvent(event);
}

void NFlyout::hideEvent(QHideEvent* event) { QWidget::hideEvent(event); }

void NFlyout::paintEvent(QPaintEvent* event) {
    Q_D(NFlyout);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    // 根据当前主题选择颜色
    QColor backgroundColor = d->_isDark ? d->_pDarkBackgroundColor : d->_pLightBackgroundColor;
    QColor borderColor     = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
    // 绘制背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(backgroundColor);

    // 计算内容矩形
    QRect contentRect = rect().adjusted(1, 1, -1, -1);

    painter.drawRoundedRect(contentRect, d->_pBorderRadius, d->_pBorderRadius);

    // 绘制边框
    painter.setPen(QPen(borderColor, d->_pBorderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(contentRect, d->_pBorderRadius, d->_pBorderRadius);

    QWidget::paintEvent(event);
}

void NFlyout::enterEvent(QEnterEvent* event) {
    Q_D(NFlyout);
    d->_isHovered = true;
    update();
    QWidget::enterEvent(event);
}

void NFlyout::leaveEvent(QEvent* event) {
    Q_D(NFlyout);
    d->_isHovered = false;
    update();
    QWidget::leaveEvent(event);
}

void NFlyout::mousePressEvent(QMouseEvent* event) {
    Q_D(NFlyout);
    d->_isPressed = true;
    update();
    QWidget::mousePressEvent(event);
}

void NFlyout::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NFlyout);
    d->_isPressed = false;
    update();
    QWidget::mouseReleaseEvent(event);
}

void NFlyout::keyPressEvent(QKeyEvent* event) {
    // 如果按下Esc键，关闭Flyout
    if (event->key() == Qt::Key_Escape) {
        hide();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

bool NFlyout::eventFilter(QObject* watched, QEvent* event) {
    Q_D(NFlyout);

    if (isVisible() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent    = static_cast<QMouseEvent*>(event);
        QWidget*     clickedWidget = QApplication::widgetAt(mouseEvent->globalPosition().toPoint());

        // 如果点击的不是Flyout或其子控件，也不是目标控件或其子控件
        if (clickedWidget && clickedWidget != this && !isAncestorOf(clickedWidget) &&
            (!d->_pTarget || (clickedWidget != d->_pTarget && !d->_pTarget->isAncestorOf(clickedWidget)))) {
            // 严格检查Light Dismiss模式
            if (d->_lightDismissMode == LightDismissMode::On) {
                fadeOut();
                return true; // 阻止事件传播
            } else if (d->_lightDismissMode == LightDismissMode::Auto) {
                // Auto模式下默认启用
                fadeOut();
                return true;
            }
            // Off模式下不做任何处理，让事件继续传播
        }
    }

    // 调用基类方法继续处理其他事件
    return QWidget::eventFilter(watched, event);
}

void NFlyout::updatePosition() {
    Q_D(NFlyout);

    if (!d->_pTarget)
        return;

    QPoint pos = calculatePositionForTarget(d->_pTarget);
    if (!pos.isNull()) {
        move(pos);
    }
}

void NFlyout::setupEventFilter() {
    // 移除可能存在的旧过滤器
    QApplication::instance()->removeEventFilter(this);

    // 重新安装全局应用程序事件过滤器
    QApplication::instance()->installEventFilter(this);
}

QPoint NFlyout::calculatePositionForTarget(QWidget* target) {
    Q_D(NFlyout);

    if (!target)
        return QPoint();

    return d->animationManager()->position(target);
}