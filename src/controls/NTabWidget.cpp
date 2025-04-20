#include <QPainter>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
#include "../private/ntabwidget_p.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, LightSelectedBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, DarkSelectedBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, int, TabPosition)

NTabWidget::NTabWidget(QWidget* parent) : QWidget(parent), d_ptr(new NTabWidgetPrivate()) { init(); }

NTabWidget::~NTabWidget() {}

void NTabWidget::init() {
    Q_D(NTabWidget);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();

    setObjectName("NTabWidget");
    d->_pTabPosition  = NTabWidget::North;
    d->_pTabsClosable = false;
    d->_pCurrentIndex = -1;

    d->_pLightSelectedBackground = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkSelectedBackground  = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);

    // 创建和配置组件
    d->_tabBar = new NTabBar(this);
    d->_stack  = new QStackedWidget(this);
    d->_stack->setObjectName("stackedWidget");

    // 设置初始调色板
    updatePalette();

    d->updateLayout();

    connect(d->_tabBar, &NTabBar::currentChanged, this, [this](int index) {
        Q_D(NTabWidget);
        if (d->_stack->currentIndex() != index) {
            d->_stack->setCurrentIndex(index);
            d->_pCurrentIndex = index;
            emit currentChanged(index);
        }
    });

    connect(d->_tabBar, &NTabBar::tabCloseRequested, this, &NTabWidget::tabCloseRequested);

    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NTabWidget);
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        updatePalette();
        update();
    });
}

void NTabWidget::updatePalette() {
    Q_D(NTabWidget);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground);
    d->_stack->setAutoFillBackground(true);
    setPalette(pal);
}

NTabWidget::TabPosition NTabWidget::tabPosition() const {
    Q_D(const NTabWidget);
    return static_cast<TabPosition>(d->_pTabPosition);
}

void NTabWidget::setTabPosition(TabPosition position) {
    Q_D(NTabWidget);
    if (d->_pTabPosition != position) {
        d->_pTabPosition = position;
        d->updateLayout();
    }
}

int NTabWidget::addTab(QWidget* page, const QString& label) { return insertTab(-1, page, label); }

int NTabWidget::addTab(QWidget* page, const QIcon& icon, const QString& label) {
    return insertTab(-1, page, icon, label);
}

int NTabWidget::insertTab(int index, QWidget* page, const QString& label) {
    return insertTab(index, page, QIcon(), label);
}

int NTabWidget::insertTab(int index, QWidget* page, const QIcon& icon, const QString& label) {
    Q_D(NTabWidget);
    if (!page)
        return -1;

    page->setParent(d->_stack);

    int insertIndex = index;
    if (insertIndex < 0 || insertIndex > d->_stack->count())
        insertIndex = d->_stack->count();

    d->_stack->insertWidget(insertIndex, page);
    int tabIndex = d->_tabBar->insertTab(insertIndex, icon, label);

    if (d->_stack->count() == 1)
        setCurrentIndex(0);

    return tabIndex;
}

void NTabWidget::removeTab(int index) {
    Q_D(NTabWidget);
    if (index < 0 || index >= d->_stack->count())
        return;

    QWidget* widget = d->_stack->widget(index);

    d->_tabBar->removeTab(index);
    d->_stack->removeWidget(widget);
}

// 标签属性设置
QString NTabWidget::tabText(int index) const {
    Q_D(const NTabWidget);
    return d->_tabBar->tabText(index);
}

void NTabWidget::setTabText(int index, const QString& text) {
    Q_D(NTabWidget);
    d->_tabBar->setTabText(index, text);
}

QIcon NTabWidget::tabIcon(int index) const {
    Q_D(const NTabWidget);
    return d->_tabBar->tabIcon(index);
}

void NTabWidget::setTabIcon(int index, const QIcon& icon) {
    Q_D(NTabWidget);
    d->_tabBar->setTabIcon(index, icon);
}

QString NTabWidget::tabToolTip(int index) const {
    Q_D(const NTabWidget);
    return d->_tabBar->tabToolTip(index);
}

void NTabWidget::setTabToolTip(int index, const QString& tip) {
    Q_D(NTabWidget);
    d->_tabBar->setTabToolTip(index, tip);
}

NTabBar* NTabWidget::tabBar() const {
    Q_D(const NTabWidget);
    return d->_tabBar;
}

QWidget* NTabWidget::currentWidget() const {
    Q_D(const NTabWidget);
    return d->_stack->currentWidget();
}

QWidget* NTabWidget::widget(int index) const {
    Q_D(const NTabWidget);
    return d->_stack->widget(index);
}

int NTabWidget::count() const {
    Q_D(const NTabWidget);
    return d->_stack->count();
}

int NTabWidget::indexOf(QWidget* widget) const {
    Q_D(const NTabWidget);
    return d->_stack->indexOf(widget);
}

bool NTabWidget::tabsClosable() const {
    Q_D(const NTabWidget);
    return d->_pTabsClosable;
}

void NTabWidget::setTabsClosable(bool closable) {
    Q_D(NTabWidget);
    if (d->_pTabsClosable != closable) {
        d->_pTabsClosable = closable;
        d->_tabBar->setTabsClosable(closable);
    }
}

void NTabWidget::setMovable(bool movable) {
    Q_D(NTabWidget);
    d->_tabBar->setMovable(movable);
}

bool NTabWidget::isMovable() const {
    Q_D(const NTabWidget);
    return d->_tabBar->isMovable();
}

int NTabWidget::currentIndex() const {
    Q_D(const NTabWidget);
    return d->_pCurrentIndex;
}

void NTabWidget::setCurrentIndex(int index) {
    Q_D(NTabWidget);
    if (index < 0 || index >= d->_stack->count())
        return;

    if (d->_pCurrentIndex != index) {
        d->_pCurrentIndex = index;
        d->_tabBar->setCurrentIndex(index);
        d->_stack->setCurrentIndex(index);
        emit currentChanged(index);
    }
}

void NTabWidget::setCurrentWidget(QWidget* widget) {
    Q_D(NTabWidget);
    int index = d->_stack->indexOf(widget);
    if (index != -1)
        setCurrentIndex(index);
}

void NTabWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    update();
}

void NTabWidget::showEvent(QShowEvent* event) { QWidget::showEvent(event); }

void NTabWidget::changeEvent(QEvent* event) {
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::FontChange ||
        event->type() == QEvent::PaletteChange) {
        update();
    }
    QWidget::changeEvent(event);
}