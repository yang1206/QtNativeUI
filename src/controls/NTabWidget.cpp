#include <QCoreApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QStyleOptionTab>
#include <QStyleOptionTabWidgetFrame>
#include <QToolButton>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTabWidget.h>
#include <QtNativeUI/NTheme.h>
#include "../private/ntabwidget_p.h"

namespace {

QSize framePadding(const NTabWidget* tabs) {
    const int   border = tabs->documentMode() ? 0 : qMax(0, tabs->getBorderWidth());
    QSize       extra(2 * border, 2 * border);
    const bool  horizontal  = tabs->tabPosition() == QTabWidget::North || tabs->tabPosition() == QTabWidget::South;
    const auto* leftCorner  = tabs->cornerWidget(Qt::TopLeftCorner);
    const auto* rightCorner = tabs->cornerWidget(Qt::TopRightCorner);
    const bool  hasCorner =
        horizontal && ((leftCorner && !leftCorner->isHidden()) || (rightCorner && !rightCorner->isHidden()));
    const bool addVisible = tabs->getAddButtonVisible() && (!tabs->tabBarAutoHide() || tabs->count() > 1);
    const bool stripVisible =
        (!tabs->tabBar()->isHidden() && !tabs->tabBar()->sizeHint().isEmpty()) || addVisible || hasCorner;
    if (stripVisible) {
        const int spacing  = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
        const int addSpace = addVisible ? 5 * spacing : 0;
        extra += horizontal ? QSize(2 * spacing + addSpace, spacing) : QSize(spacing, 2 * spacing + addSpace);
    }
    return extra;
}

} // namespace

Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, LightSelectedBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, DarkSelectedBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, LightBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, QColor, DarkBorderColor)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NTabWidget, int, BorderWidth)

NTabWidget::NTabWidget(QWidget* parent) : QTabWidget(parent), d_ptr(new NTabWidgetPrivate()) { init(); }

NTabWidget::~NTabWidget() = default;

void NTabWidget::init() {
    Q_D(NTabWidget);
    d->q_ptr                     = this;
    d->_themeMode                = nTheme->themeMode();
    d->_isDark                   = nTheme->isDarkMode();
    d->_pAddButtonVisible        = false;
    d->_pLightSelectedBackground = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkSelectedBackground  = NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);
    d->_pLightBorderColor        = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkBorderColor         = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);
    d->_pBorderRadius            = NRadiusToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    d->_pBorderWidth             = 1;

    setObjectName(QStringLiteral("NTabWidget"));
    setAccessibleName(QStringLiteral("标签页控件"));
    setTabBar(new NTabBar(this));
    tabBar()->setExpanding(false);
    tabBar()->setDrawBase(false);
    setAutoFillBackground(false);
    d->initStyle();
    updatePalette();

    connect(this, &NTabWidget::pLightSelectedBackgroundChanged, this, &NTabWidget::updatePalette);
    connect(this, &NTabWidget::pDarkSelectedBackgroundChanged, this, &NTabWidget::updatePalette);
    connect(this, &NTabWidget::pLightBorderColorChanged, this, &NTabWidget::updatePalette);
    connect(this, &NTabWidget::pDarkBorderColorChanged, this, &NTabWidget::updatePalette);
    connect(this, &NTabWidget::pBorderRadiusChanged, this, &NTabWidget::adjustContentMargins);
    connect(this, &NTabWidget::pBorderWidthChanged, this, &NTabWidget::adjustContentMargins);
    connect(this, &QTabWidget::currentChanged, this, [this]() { update(); });
    connect(tabBar(), &QTabBar::tabMoved, this, [this]() { update(); });
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NTabWidget);
        d->_themeMode = mode;
        d->_isDark    = nTheme->isDarkMode();
        updatePalette();
        d->addButton->update();
    });
}

void NTabWidget::updatePalette() {
    Q_D(NTabWidget);
    if (d->updatingPalette)
        return;
    d->updatingPalette      = true;
    QPalette     pal        = palette();
    const QColor background = d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground;
    const QColor foreground =
        NThemeColor(NFluentColorKey::TextFillColorPrimary, d->_isDark ? NThemeType::Dark : NThemeType::Light);
    pal.setColor(QPalette::Window, background);
    pal.setColor(QPalette::Base, background);
    pal.setColor(QPalette::WindowText, foreground);
    pal.setColor(QPalette::Text, foreground);
    if (pal != palette())
        setPalette(pal);
    // 页面通过 Qt 正常继承调色板，保留业务页面显式设置的背景和填充策略。
    tabBar()->setLightItemHeaderBackgroundSelected(d->_pLightSelectedBackground);
    tabBar()->setDarkItemHeaderBackgroundSelected(d->_pDarkSelectedBackground);
    tabBar()->setLightItemHeaderBorderColorSelected(d->_pLightBorderColor);
    tabBar()->setDarkItemHeaderBorderColorSelected(d->_pDarkBorderColor);
    d->updatingPalette = false;
    update();
}

void NTabWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    Q_D(NTabWidget);
    QStyleOptionTabWidgetFrame option;
    initStyleOption(&option);
    const QRect  pane        = style()->subElementRect(QStyle::SE_TabWidgetTabPane, &option, this);
    const int    borderWidth = qMax(0, d->_pBorderWidth);
    const bool   framed      = !documentMode();
    const int    frameInset  = framed ? borderWidth : 0;
    const QRect  inner       = rect().adjusted(frameInset, frameInset, -frameInset, -frameInset);
    const qreal  radius      = framed ? qBound(0.0, qreal(d->_pBorderRadius), qMin(width(), height()) / 2.0) : 0.0;
    const QColor background  = d->_isDark ? d->_pDarkSelectedBackground : d->_pLightSelectedBackground;
    const QColor borderColor = d->_isDark ? d->_pDarkBorderColor : d->_pLightBorderColor;
    const QColor stripBackground =
        NThemeColor(NFluentColorKey::SolidBackgroundFillColorBase, d->_isDark ? NThemeType::Dark : NThemeType::Light);
    const int   footRadius = qMax(0, NRadiusToken(NDesignTokenKey::CornerRadiusDefault).toInt());
    const auto* bar        = tabBar();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 标签栏和内容区共用完整外框；栏两侧、空白区和顶部留白都属于组件表面。
    QPainterPath surface;
    if (framed)
        surface.addRoundedRect(QRectF(rect()), radius, radius);
    else
        surface.addRect(QRectF(rect()));
    painter.fillPath(surface, stripBackground);
    painter.save();
    painter.setClipPath(surface);
    painter.fillRect(pane, background);
    painter.restore();

    if (borderWidth > 0 && pane.isValid() && pane != inner) {
        // 内容区只保留与标签栏相邻的分隔线，不再单独绘制圆角外框。
        QRegion dividerClip(rect());
        if (bar->isVisible() && currentIndex() >= 0) {
            QRect selected = bar->tabRect(currentIndex()).intersected(bar->rect());
            selected.translate(bar->pos());
            QRect gap;
            switch (tabPosition()) {
                case North:
                    gap = QRect(selected.left() - footRadius, pane.top() - 1, selected.width() + 2 * footRadius, 3);
                    break;
                case South:
                    gap = QRect(selected.left() - footRadius, pane.bottom() - 1, selected.width() + 2 * footRadius, 3);
                    break;
                case West:
                    gap = QRect(pane.left() - 1, selected.top() - footRadius, 3, selected.height() + 2 * footRadius);
                    break;
                case East:
                    gap = QRect(pane.right() - 1, selected.top() - footRadius, 3, selected.height() + 2 * footRadius);
                    break;
            }
            if (!selected.isEmpty())
                dividerClip -= gap;
        }
        painter.save();
        painter.setClipRegion(dividerClip);
        painter.setPen(QPen(borderColor, 1));
        switch (tabPosition()) {
            case North:
                painter.drawLine(QPointF(inner.left(), pane.top() + 0.5), QPointF(inner.right() + 1, pane.top() + 0.5));
                break;
            case South:
                painter.drawLine(QPointF(inner.left(), pane.bottom() + 0.5),
                                 QPointF(inner.right() + 1, pane.bottom() + 0.5));
                break;
            case West:
                painter.drawLine(QPointF(pane.left() + 0.5, inner.top()),
                                 QPointF(pane.left() + 0.5, inner.bottom() + 1));
                break;
            case East:
                painter.drawLine(QPointF(pane.right() + 0.5, inner.top()),
                                 QPointF(pane.right() + 0.5, inner.bottom() + 1));
                break;
        }
        painter.restore();
    }

    // 复用标签栏轮廓补绘栏外的外扩弧，填充底面和分隔线使用同一坐标。
    if (bar->isVisible() && currentIndex() >= 0) {
        const QRect selected = bar->tabRect(currentIndex());
        if (bar->rect().contains(selected)) {
            QRect footprint = selected;
            if (tabPosition() == North || tabPosition() == South)
                footprint.adjust(-footRadius, 0, footRadius, 0);
            else
                footprint.adjust(0, -footRadius, 0, footRadius);
            const QRegion overflow = QRegion(footprint).subtracted(QRegion(bar->rect()));
            if (!overflow.isEmpty()) {
                QStyleOptionTab tabOption;
                tabOption.initFrom(bar);
                tabOption.rect     = selected;
                tabOption.shape    = bar->shape();
                tabOption.tabIndex = currentIndex();
                tabOption.state |= QStyle::State_Selected;
                if (!bar->isTabEnabled(currentIndex()))
                    tabOption.state &= ~QStyle::State_Enabled;
                painter.save();
                painter.translate(bar->pos());
                painter.setClipRegion(overflow);
                bar->style()->drawControl(QStyle::CE_TabBarTabShape, &tabOption, &painter, bar);
                painter.restore();
            }
        }
    }

    if (framed && borderWidth > 0) {
        const qreal  inset   = borderWidth / 2.0;
        const QRectF outline = QRectF(rect()).adjusted(inset, inset, -inset, -inset);
        if (outline.isValid()) {
            const qreal outlineRadius = qMax(0.0, radius - inset);
            painter.setPen(QPen(borderColor, borderWidth));
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(outline, outlineRadius, outlineRadius);
        }
    }
}

void NTabWidget::resizeEvent(QResizeEvent* event) {
    QTabWidget::resizeEvent(event);
    d_ptr->updateAddButtonGeometry();
}

void NTabWidget::tabInserted(int index) {
    QTabWidget::tabInserted(index);
    adjustContentMargins();
}

void NTabWidget::adjustContentMargins() {
    // 内容边距由 style 的 SE_TabWidgetTabContents 提供，不依赖 Qt 内部对象名。
    updateGeometry();
    QCoreApplication::postEvent(this, new QEvent(QEvent::LayoutRequest));
    update();
}

void NTabWidget::showEvent(QShowEvent* event) {
    QTabWidget::showEvent(event);
    d_ptr->updateAddButtonGeometry();
}

void NTabWidget::changeEvent(QEvent* event) {
    QTabWidget::changeEvent(event);
    if (event->type() == QEvent::PaletteChange || event->type() == QEvent::StyleChange ||
        event->type() == QEvent::FontChange)
        updatePalette();
}

bool NTabWidget::event(QEvent* event) {
    const bool result = QTabWidget::event(event);
    if (event->type() == QEvent::LayoutRequest || event->type() == QEvent::LayoutDirectionChange) {
        d_ptr->updateAddButtonGeometry();
        update();
    }
    return result;
}

void NTabWidget::keyPressEvent(QKeyEvent* event) {
    if (event->modifiers() == Qt::ControlModifier) {
        if (event->key() == Qt::Key_W || event->key() == Qt::Key_F4) {
            tabBar()->requestTabClose(currentIndex());
            event->accept();
            return;
        }
        if (event->key() == Qt::Key_T && getAddButtonVisible()) {
            emit tabAddRequested();
            event->accept();
            return;
        }
    }
    QTabWidget::keyPressEvent(event);
}

void NTabWidget::setTabsClosable(bool closable) {
    // 由 QTabWidget 维护关闭信号的连接，避免重复转发。
    QTabWidget::setTabsClosable(closable);
    tabBar()->setTabsClosable(closable);
}
bool     NTabWidget::tabsClosable() const { return QTabWidget::tabsClosable(); }
NTabBar* NTabWidget::tabBar() const { return static_cast<NTabBar*>(QTabWidget::tabBar()); }

void NTabWidget::setTabClosable(int index, bool closable) { tabBar()->setTabClosable(index, closable); }
bool NTabWidget::getTabClosable(int index) const { return tabBar()->getTabClosable(index); }
void NTabWidget::setTabWidthMode(NTabBar::TabWidthMode mode) { tabBar()->setTabWidthMode(mode); }
NTabBar::TabWidthMode NTabWidget::getTabWidthMode() const { return tabBar()->getTabWidthMode(); }
void                  NTabWidget::setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode mode) {
    tabBar()->setCloseButtonDisplayMode(mode);
}
NTabBar::CloseButtonDisplayMode NTabWidget::getCloseButtonDisplayMode() const {
    return tabBar()->getCloseButtonDisplayMode();
}
void NTabWidget::setTabDragEnabled(bool enabled) { tabBar()->setTabDragEnabled(enabled); }
bool NTabWidget::getTabDragEnabled() const { return tabBar()->getTabDragEnabled(); }
void NTabWidget::setAddButtonVisible(bool visible) {
    Q_D(NTabWidget);
    if (visible == d->_pAddButtonVisible)
        return;
    d->_pAddButtonVisible = visible;
    d->updateAddButtonGeometry();
    adjustContentMargins();
    emit pAddButtonVisibleChanged();
}
bool NTabWidget::getAddButtonVisible() const { return d_ptr->_pAddButtonVisible; }

QSize NTabWidget::sizeHint() const { return QTabWidget::sizeHint() + framePadding(this); }
QSize NTabWidget::minimumSizeHint() const { return QTabWidget::minimumSizeHint() + framePadding(this); }
int   NTabWidget::heightForWidth(int width) const {
    const QSize padding = framePadding(this);
    const int   height  = QTabWidget::heightForWidth(qMax(0, width - padding.width()));
    return height < 0 ? height : height + padding.height();
}
