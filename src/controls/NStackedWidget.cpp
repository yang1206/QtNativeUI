#include "QtNativeUI/NStackedWidget.h"
#include "../private/nnavigationanimation_p.h"

#include <QPainter>
#include <QPen>

NStackedWidget::NStackedWidget(QWidget* parent) : QStackedWidget(parent) {
    m_borderRadius = 10;
    m_themeMode    = nTheme->themeMode();
    m_background   = NThemeColor(NFluentColorKey::LayerFillColorDefault, m_themeMode);

    m_animationManager = new NNavigationAnimationManager(this, this);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        m_background = NThemeColor(NFluentColorKey::LayerFillColorDefault, mode);
        m_themeMode  = mode;
        update();
    });
    // 子控件依靠父背景传播清理每帧；TranslucentBackground 会同时禁用背景清理，留下拖影。
    setAutoFillBackground(false);
    setObjectName("NStackedWidget");
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    pal.setColor(QPalette::Base, Qt::transparent);
    setPalette(pal);
}

NStackedWidget::~NStackedWidget() {
    // 基类析构时会先释放栈布局，再发送 Hide；提前移除动画的事件过滤器。
    delete m_animationManager;
}

void NStackedWidget::setBackGroundColor(QColor color) {
    m_background = color;
    update();
}

void NStackedWidget::setBorderRadius(int radius) {
    m_borderRadius = radius;
    update();
}

void NStackedWidget::doPageSwitch(NNavigationType::PageTransitionType transitionType,
                                  int                                 targetIndex,
                                  bool                                isRouteBack,
                                  int                                 duration) {
    m_animationManager->executeTransition(transitionType, targetIndex, isRouteBack, duration);
}

void NStackedWidget::paintEvent(QPaintEvent* event) {
    QStackedWidget::paintEvent(event);

    QPainter painter(this);
    QRect    targetRect = this->rect();
    targetRect.adjust(1, 1, 10, 10);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(NThemeColor(NFluentColorKey::ControlStrokeColorOnAccentDisabled, m_themeMode), 1.5));
    // painter.setBrush(m_background);

    if (m_borderRadius >= 1) {
        painter.drawRoundedRect(targetRect, m_borderRadius, m_borderRadius);
    } else {
        painter.drawRect(targetRect);
    }
    painter.restore();

    m_animationManager->paintTransition(&painter, rect(), m_borderRadius);
}
