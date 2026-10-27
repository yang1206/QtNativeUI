#include <QAbstractButton>
#include <QApplication>
#include <QCursor>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QHelpEvent>
#include <QHoverEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMetaProperty>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionTab>
#include <QStyleOptionToolButton>
#include <QTimer>
#include <QToolButton>
#include <QtNativeUI/NTabBar.h>
#include <QtNativeUI/NTheme.h>
#include "../private/naccessible_p.h"
#include "../private/ntabbar_p.h"
#include "../private/ntabbarstyle.h"

Q_PROPERTY_CREATE_Q_CPP(NTabBar, int, BorderRadius)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBackgroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBackgroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderForegroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderForegroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForeground)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundPointerOver)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundPressed)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemIconForegroundDisabled)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemIconForegroundDisabled)

Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemSeparator)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemSeparator)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, LightItemHeaderBorderColorSelected)
Q_PROPERTY_CREATE_Q_CPP(NTabBar, QColor, DarkItemHeaderBorderColorSelected)

NTabBar::NTabBar(QWidget* parent) : QTabBar(parent), d_ptr(new NTabBarPrivate()) { init(); }
NTabBar::~NTabBar() = default;

void NTabBar::init() {
    Q_D(NTabBar);
    d->q_ptr      = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();
    d->_pLightItemHeaderBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Light);
    d->_pDarkItemHeaderBackground =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundSelected =
        NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundSelected =
        NThemeColor(NFluentColorKey::SolidBackgroundFillColorTertiary, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundPointerOver =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundPointerOver =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundPressed =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundPressed =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorDefault, NThemeType::Dark);
    d->_pLightItemHeaderBackgroundDisabled =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Light);
    d->_pDarkItemHeaderBackgroundDisabled =
        NThemeColor(NFluentColorKey::LayerOnMicaBaseAltFillColorTransparent, NThemeType::Dark);

    d->_pLightItemHeaderForeground            = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderForeground             = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundSelected    = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundSelected     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundPointerOver = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundPointerOver  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundPressed     = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkItemHeaderForegroundPressed      = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightItemHeaderForegroundDisabled    = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkItemHeaderForegroundDisabled     = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightItemIconForeground            = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemIconForeground             = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemIconForegroundSelected    = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkItemIconForegroundSelected     = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightItemIconForegroundPointerOver = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkItemIconForegroundPointerOver  = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    d->_pLightItemIconForegroundPressed     = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Light);
    d->_pDarkItemIconForegroundPressed      = NThemeColor(NFluentColorKey::TextFillColorTertiary, NThemeType::Dark);
    d->_pLightItemIconForegroundDisabled    = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkItemIconForegroundDisabled     = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);

    d->_pLightItemSeparator = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Light);
    d->_pDarkItemSeparator  = NThemeColor(NFluentColorKey::DividerStrokeColorDefault, NThemeType::Dark);

    d->_pLightItemHeaderBorderColorSelected = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkItemHeaderBorderColorSelected  = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);

    d->_pBorderRadius = NRadiusToken(NDesignTokenKey::CornerRadiusMedium).toInt();

    d->_pTabHeight = NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt() +
                     2 * NSpacingToken(NDesignTokenKey::SpacingM).toInt() +
                     NSpacingToken(NDesignTokenKey::SpacingS).toInt();
    setObjectName(QStringLiteral("NTabBar"));
    setAccessibleName(QStringLiteral("标签页"));
    auto* tabStyle = new NTabBarStyle();
    tabStyle->setParent(this);
    setStyle(tabStyle);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_Hover);
    setMouseTracking(true);
    setUsesScrollButtons(true);
    setExpanding(false);
    setElideMode(Qt::ElideRight);
    setDrawBase(true);
    setIconSize(QSize(16, 16));
    QFont tabFont = font();
    tabFont.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeCaption).toInt());
    setFont(tabFont);
    // 保留既有颜色属性，并让运行时设置颜色后立即重绘。
    const auto& meta    = NTabBar::staticMetaObject;
    const auto  repaint = QWidget::staticMetaObject.method(QWidget::staticMetaObject.indexOfSlot("update()"));
    for (int i = meta.propertyOffset(); i < meta.propertyCount(); ++i) {
        const auto property = meta.property(i);
        if (property.hasNotifySignal())
            connect(this, property.notifySignal(), this, repaint);
    }
    connect(this, &QTabBar::currentChanged, this, [this](int index) {
        Q_D(NTabBar);
        d->hideToolTip();
        d->updateCloseButton(d->closeCurrentIndex);
        d->updateCloseButton(index);
        d->closeCurrentIndex = index;
        if (d->_pTabWidthMode == TabWidthMode::Compact)
            d->scheduleLayout();
    });
    for (auto* button : findChildren<QToolButton*>(QString(), Qt::FindDirectChildrenOnly))
        button->installEventFilter(this);
    connect(this, &QTabBar::tabMoved, this, [this](int from, int to) {
        Q_D(NTabBar);
        if (from >= 0 && from < d->tabs.size() && to >= 0 && to < d->tabs.size())
            d->tabs.move(from, to);
        d->_hoverIndex = -1;
        d->hideToolTip();
        d->scheduleCloseButtons();
        NA11y::notifyAccessibleEvent(this, QAccessible::ObjectReorder);
    });
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode mode) {
        Q_D(NTabBar);
        d->_themeMode = mode;
        d->_isDark    = nTheme->isDarkMode();
        update();
        for (auto* button : findChildren<QAbstractButton*>(QString(), Qt::FindDirectChildrenOnly))
            button->update();
    });
}

int  NTabBar::hoverIndex() const { return d_ptr->_hoverIndex; }
bool NTabBar::tabsClosable() const { return QTabBar::tabsClosable(); }

void NTabBar::setTabsClosable(bool closable) {
    QTabBar::setTabsClosable(closable);
    if (closable) {
        for (int index = 0; index < count(); ++index)
            d_ptr->initializeCloseButton(index);
    }
    d_ptr->scheduleCloseButtons();
    d_ptr->scheduleLayout();
}

void NTabBar::setTabClosable(int index, bool closable) {
    Q_D(NTabBar);
    if (index < 0 || index >= d->tabs.size() || d->tabs[index].closable == closable)
        return;
    d->tabs[index].closable = closable;
    d->updateCloseButton(index);
    d->scheduleCloseButtons();
    d->scheduleLayout();
}
bool NTabBar::getTabClosable(int index) const {
    return index >= 0 && index < d_ptr->tabs.size() && d_ptr->tabs[index].closable;
}
void NTabBar::requestTabClose(int index) {
    if (tabsClosable() && getTabClosable(index) && isTabEnabled(index))
        emit tabCloseRequested(index);
}
void NTabBar::onCloseButtonClicked() { requestTabClose(d_ptr->closeButtonIndex(sender())); }

NTabBar::TabWidthMode NTabBar::getTabWidthMode() const { return d_ptr->_pTabWidthMode; }
void                  NTabBar::setTabWidthMode(TabWidthMode mode) {
    Q_D(NTabBar);
    if (d->_pTabWidthMode == mode)
        return;
    d->_pTabWidthMode = mode;
    setExpanding(false);
    d->scheduleLayout();
    d->scheduleCloseButtons();
    emit pTabWidthModeChanged();
}
NTabBar::CloseButtonDisplayMode NTabBar::getCloseButtonDisplayMode() const { return d_ptr->_pCloseButtonDisplayMode; }
void                            NTabBar::setCloseButtonDisplayMode(CloseButtonDisplayMode mode) {
    Q_D(NTabBar);
    if (d->_pCloseButtonDisplayMode == mode)
        return;
    d->_pCloseButtonDisplayMode = mode;
    d->scheduleCloseButtons();
    d->scheduleLayout();
    emit pCloseButtonDisplayModeChanged();
}
int  NTabBar::getTabMinimumWidth() const { return d_ptr->_pTabMinimumWidth; }
void NTabBar::setTabMinimumWidth(int width) {
    Q_D(NTabBar);
    width = qMax(1, width);
    if (width == d->_pTabMinimumWidth)
        return;
    d->_pTabMinimumWidth = width;
    if (d->_pTabMaximumWidth < width) {
        d->_pTabMaximumWidth = width;
        emit pTabMaximumWidthChanged();
    }
    d->scheduleLayout();
    emit pTabMinimumWidthChanged();
}
int  NTabBar::getTabMaximumWidth() const { return d_ptr->_pTabMaximumWidth; }
void NTabBar::setTabMaximumWidth(int width) {
    Q_D(NTabBar);
    width = qMax(1, width);
    if (width == d->_pTabMaximumWidth)
        return;
    d->_pTabMaximumWidth = width;
    if (d->_pTabMinimumWidth > width) {
        d->_pTabMinimumWidth = width;
        emit pTabMinimumWidthChanged();
    }
    d->scheduleLayout();
    emit pTabMaximumWidthChanged();
}
int  NTabBar::getTabHeight() const { return d_ptr->_pTabHeight; }
void NTabBar::setTabHeight(int height) {
    Q_D(NTabBar);
    height = qMax(1, height);
    if (d->_pTabHeight == height)
        return;
    d->_pTabHeight = height;
    d->scheduleLayout();
    emit pTabHeightChanged();
}
bool NTabBar::getTabDragEnabled() const { return d_ptr->_pTabDragEnabled; }
void NTabBar::setTabDragEnabled(bool enabled) {
    if (d_ptr->_pTabDragEnabled == enabled)
        return;
    d_ptr->_pTabDragEnabled = enabled;
    if (enabled) {
        d_ptr->acceptDropsBeforeTabDrag = acceptDrops();
        setAcceptDrops(true);
    } else if (acceptDrops() && !changeCurrentOnDrag()) {
        setAcceptDrops(d_ptr->acceptDropsBeforeTabDrag);
    }
    emit pTabDragEnabledChanged();
}

QSize NTabBar::tabSizeHint(int index) const {
    Q_D(const NTabBar);
    const int gap        = NSpacingToken(NDesignTokenKey::SpacingS).toInt();
    const int padding    = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
    auto      buttonSize = [this, d, index](ButtonPosition side) {
        QWidget* button = tabButton(index, side);
        if (!button || (d->isCloseButton(button) && !d->reserveCloseButton(index)))
            return QSize();
        return button->size().expandedTo(button->sizeHint());
    };
    const QSize leftButton  = buttonSize(LeftSide);
    const QSize rightButton = buttonSize(RightSide);
    const int   buttonLength =
        d->vertical() ? leftButton.height() + rightButton.height() : leftButton.width() + rightButton.width();
    const int buttonCross =
        d->vertical() ? qMax(leftButton.width(), rightButton.width()) : qMax(leftButton.height(), rightButton.height());
    const int buttonCount = int(!leftButton.isEmpty()) + int(!rightButton.isEmpty());
    const int height      = qMax(d->_pTabHeight, qMax(fontMetrics().height() + 2 * gap, buttonCross + 2 * gap));
    int       width       = d->_pTabMaximumWidth;
    if (d->compact(index)) {
        width = iconSize().width() + 2 * padding + buttonLength + buttonCount * gap;
    } else if (d->_pTabWidthMode != TabWidthMode::Equal) {
        // QTabBar 会用 TextShowMnemonic 再做一次 elide；使用相同度量并保留 1px 取整余量，
        // 避免内容宽度恰好贴边时被提前省略。
        width = fontMetrics().size(Qt::TextShowMnemonic, tabText(index)).width() + 1 + 2 * padding;
        if (!tabIcon(index).isNull())
            width += iconSize().width() + padding + gap / 2;
        width += buttonLength + buttonCount * gap;
        width = qBound(d->_pTabMinimumWidth, width, d->_pTabMaximumWidth);
    } else {
        width = qMax(width, buttonLength + buttonCount * gap + 2 * padding);
    }
    QSize result(width, height);
    if (d->vertical())
        result.transpose();
    return result;
}
QSize NTabBar::minimumTabSizeHint(int index) const {
    QSize result = tabSizeHint(index);
    if (!d_ptr->compact(index)) {
        if (d_ptr->vertical())
            result.setHeight(qMin(result.height(), d_ptr->_pTabMinimumWidth));
        else
            result.setWidth(qMin(result.width(), d_ptr->_pTabMinimumWidth));
    }
    return result;
}
void NTabBar::invalidateTabLayout() {
    // 告知 Qt 重新读取自定义尺寸提示；合并到一次事件循环，避免属性设置时重复布局。
    QEvent change(QEvent::StyleChange);
    QTabBar::changeEvent(&change);
    updateGeometry();
    update();
}
void NTabBar::tabLayoutChange() {
    QTabBar::tabLayoutChange();
    bool initialized = false;
    if (tabsClosable()) {
        for (int index = 0; index < count(); ++index)
            initialized = d_ptr->initializeCloseButton(index) || initialized;
    }
    d_ptr->scheduleCloseButtons();
    if (initialized)
        d_ptr->scheduleLayout();
}
void NTabBar::tabInserted(int index) {
    d_ptr->tabs.insert(index, NTabBarPrivate::TabState());
    QTabBar::tabInserted(index);
    d_ptr->initializeCloseButton(index);
    d_ptr->hideToolTip();
    d_ptr->scheduleCloseButtons();
    d_ptr->scheduleLayout();
    NA11y::notifyAccessibleEvent(this, QAccessible::ObjectReorder);
}
void NTabBar::tabRemoved(int index) {
    if (index >= 0 && index < d_ptr->tabs.size())
        d_ptr->tabs.removeAt(index);
    QTabBar::tabRemoved(index);
    d_ptr->_hoverIndex = -1;
    d_ptr->hideToolTip();
    d_ptr->scheduleCloseButtons();
    NA11y::notifyAccessibleEvent(this, QAccessible::ObjectReorder);
}

void NTabBar::mousePressEvent(QMouseEvent* event) {
    Q_D(NTabBar);
    d->hideToolTip();
    const int index  = tabAt(event->position().toPoint());
    d->pressedId     = event->button() == Qt::LeftButton && index >= 0 && index < d->tabs.size() && isTabEnabled(index)
                           ? d->tabs[index].id
                           : QUuid();
    d->pressPosition = event->position().toPoint();
    d->dragStarted   = false;
    QPointer<NTabBar> guard(this);
    QTabBar::mousePressEvent(event);
    if (guard && event->button() == Qt::LeftButton && index >= 0)
        emit tabBarPressed(index);
}
void NTabBar::mouseMoveEvent(QMouseEvent* event) {
    Q_D(NTabBar);
    d->updateHover(tabAt(event->position().toPoint()));
    const QPoint point    = event->position().toPoint();
    const QRect  dragArea = rect().adjusted(-QApplication::startDragDistance(),
                                           -QApplication::startDragDistance(),
                                           QApplication::startDragDistance(),
                                           QApplication::startDragDistance());
    if (d->_pTabDragEnabled && !d->dragStarted && !d->pressedId.isNull() && (event->buttons() & Qt::LeftButton) &&
        !dragArea.contains(point) &&
        (point - d->pressPosition).manhattanLength() >= QApplication::startDragDistance()) {
        int index = -1;
        for (int i = 0; i < d->tabs.size(); ++i) {
            if (d->tabs[i].id == d->pressedId) {
                index = i;
                break;
            }
        }
        if (index >= 0 && isTabEnabled(index)) {
            d->dragStarted = true;
            // 只在真正起拖时截图。稳定 ID 不占用业务自己的 tabData。
            const QPixmap     pixmap = grab(tabRect(index));
            QMouseEvent       release(QEvent::MouseButtonRelease,
                                event->position(),
                                event->globalPosition(),
                                Qt::LeftButton,
                                Qt::NoButton,
                                event->modifiers());
            QPointer<NTabBar> guard(this);
            QTabBar::mouseReleaseEvent(&release);
            if (!guard)
                return;
            QPointer<QDrag> drag     = new QDrag(this);
            auto*           mimeData = new QMimeData;
            mimeData->setData(
                "application/x-qtnativeui-tab",
                QJsonDocument(QJsonObject{{"source", d->sourceId.toString()}, {"tab", d->pressedId.toString()}})
                    .toJson(QJsonDocument::Compact));
            mimeData->setProperty("DragType", "NTabBarDrag");
            mimeData->setProperty("TabIndex", index);
            mimeData->setProperty("TabIcon", tabIcon(index));
            mimeData->setProperty("TabText", tabText(index));
            mimeData->setProperty("NTabBarObject", QVariant::fromValue(this));
            mimeData->setProperty("QDragObject", QVariant::fromValue(drag.data()));
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(pixmap.rect().center() / pixmap.devicePixelRatioF());
            // 信号用于在开始前配置拖拽对象；执行和生命周期由组件统一管理。
            emit tabDragStarted(drag);
            if (!guard)
                return;
            if (drag)
                drag->exec(Qt::MoveAction, Qt::MoveAction);
            if (drag)
                drag->deleteLater();
            if (guard)
                d->pressedId = QUuid();
            return;
        }
    }
    QTabBar::mouseMoveEvent(event);
}
void NTabBar::mouseReleaseEvent(QMouseEvent* event) {
    Q_D(NTabBar);
    d->pressedId = QUuid();
    if (event->button() == Qt::MiddleButton) {
        requestTabClose(tabAt(event->position().toPoint()));
        event->accept();
        return;
    }
    QTabBar::mouseReleaseEvent(event);
}
void NTabBar::leaveEvent(QEvent* event) {
    if (!rect().contains(mapFromGlobal(QCursor::pos()))) {
        d_ptr->updateHover(-1);
        d_ptr->hideToolTip();
    }
    QTabBar::leaveEvent(event);
}
void NTabBar::changeEvent(QEvent* event) {
    QTabBar::changeEvent(event);
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::FontChange ||
        event->type() == QEvent::PaletteChange) {
        d_ptr->hideToolTip();
        d_ptr->scheduleCloseButtons();
        update();
    }
}
void NTabBar::keyPressEvent(QKeyEvent* event) {
    if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_W || event->key() == Qt::Key_F4)) {
        requestTabClose(currentIndex());
        event->accept();
        return;
    }
    QTabBar::keyPressEvent(event);
}
bool NTabBar::event(QEvent* event) {
    if (event->type() == QEvent::ToolTip) {
        const int index = tabAt(static_cast<QHelpEvent*>(event)->pos());
        d_ptr->showToolTip(index);
        event->accept();
        return true;
    }
    if (event->type() == QEvent::HoverMove)
        d_ptr->updateHover(tabAt(static_cast<QHoverEvent*>(event)->position().toPoint()));
    if (event->type() == QEvent::Wheel || event->type() == QEvent::Resize || event->type() == QEvent::Hide)
        d_ptr->hideToolTip();
    return QTabBar::event(event);
}
bool NTabBar::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::Paint) {
        auto*      toolButton   = qobject_cast<QToolButton*>(watched);
        const bool scrollButton = toolButton && (toolButton->objectName() == QStringLiteral("ScrollLeftButton") ||
                                                 toolButton->objectName() == QStringLiteral("ScrollRightButton"));
        if (scrollButton) {
            QStyleOptionToolButton option;
            option.initFrom(toolButton);
            option.arrowType = toolButton->arrowType();
            option.icon      = toolButton->icon();
            option.iconSize  = toolButton->iconSize();
            if (toolButton->isDown())
                option.state |= QStyle::State_Sunken;
            QPainter painter(toolButton);
            nPaintTabToolButton(&painter, option, toolButton);
            return true;
        }
        if (d_ptr->isCloseButton(watched)) {
            auto*        button = qobject_cast<QAbstractButton*>(watched);
            QStyleOption option;
            option.initFrom(button);
            if (button->isDown())
                option.state |= QStyle::State_Sunken;
            QPainter painter(button);
            style()->drawPrimitive(QStyle::PE_IndicatorTabClose, &option, &painter, button);
            return true;
        }
    }
    const int index = d_ptr->closeButtonIndex(watched);
    if (index >= 0) {
        if (event->type() == QEvent::Enter)
            d_ptr->updateHover(index);
        if (event->type() == QEvent::ToolTip) {
            d_ptr->showToolTip(index, QStringLiteral("关闭标签：") + tabText(index));
            event->accept();
            return true;
        }
    }
    return QTabBar::eventFilter(watched, event);
}
void NTabBar::setTabToolTip(int index, const QString& tip) {
    QTabBar::setTabToolTip(index, tip);
    d_ptr->hideToolTip();
}
#if QT_CONFIG(accessibility)
void NTabBar::setAccessibleTabName(int index, const QString& name) {
    QTabBar::setAccessibleTabName(index, name);
    d_ptr->updateCloseButton(index);
}
#endif
void NTabBar::dragEnterEvent(QDragEnterEvent* event) {
    if (d_ptr->_pTabDragEnabled && d_ptr->droppedIndex(event->mimeData()) >= 0)
        event->acceptProposedAction();
    else
        QTabBar::dragEnterEvent(event);
}
void NTabBar::dragMoveEvent(QDragMoveEvent* event) {
    if (d_ptr->_pTabDragEnabled && d_ptr->droppedIndex(event->mimeData()) >= 0)
        event->acceptProposedAction();
    else
        QTabBar::dragMoveEvent(event);
}
void NTabBar::dropEvent(QDropEvent* event) {
    const int from = d_ptr->droppedIndex(event->mimeData());
    if (!d_ptr->_pTabDragEnabled || from < 0) {
        QTabBar::dropEvent(event);
        return;
    }
    int to = tabAt(event->position().toPoint());
    if (to < 0)
        to = count() - 1;
    moveTab(from, to);
    event->acceptProposedAction();
    emit tabDropped(const_cast<QMimeData*>(event->mimeData()));
}
