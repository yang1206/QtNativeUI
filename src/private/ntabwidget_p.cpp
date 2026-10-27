#include "ntabwidget_p.h"

#include <QPainter>
#include <QProxyStyle>
#include <QShortcut>
#include <QStyleOptionTabWidgetFrame>
#include <QStyleOptionToolButton>
#include <QToolButton>
#include <QtNativeUI/NIcon.h>
#include <QtNativeUI/NTheme.h>
#include "ntabbarstyle.h"

namespace {

class TabAddButton : public QToolButton {
  public:
    explicit TabAddButton(QWidget* parent) : QToolButton(parent) {
        setObjectName(QStringLiteral("NTabWidgetAddButton"));
        setAccessibleName(QStringLiteral("添加标签"));
        setAccessibleDescription(QStringLiteral("新建标签页，快捷键 Ctrl+T"));
        setToolTip(QStringLiteral("添加标签 (Ctrl+T)"));
        setAutoRaise(true);
        setIcon(nIcon->fromRegular(NRegularIconType::Add16Regular, 16));
        setIconSize(QSize(16, 16));
    }

  protected:
    void paintEvent(QPaintEvent*) override {
        QStyleOptionToolButton option;
        initStyleOption(&option);
        // 图标使用实时主题色，避免切换主题后沿用旧的缓存颜色。
        option.icon = QIcon();
        QPainter painter(this);
        nPaintTabToolButton(&painter, option, this);
        QFont font(nIcon->getRegularFontFamily());
        font.setPixelSize(iconSize().width());
        painter.setFont(font);
        painter.setPen(
            NThemeColor(isEnabled() ? NFluentColorKey::TextFillColorSecondary : NFluentColorKey::TextFillColorDisabled,
                        nTheme->isDarkMode() ? NThemeType::Dark : NThemeType::Light));
        painter.drawText(
            rect(), Qt::AlignCenter, nIcon->getIconText(static_cast<uint32_t>(NRegularIconType::Add16Regular)));
    }
};

class TabWidgetStyle : public QProxyStyle {
  public:
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override {
        const auto* tabs  = qobject_cast<const NTabWidget*>(widget);
        const auto* frame = qstyleoption_cast<const QStyleOptionTabWidgetFrame*>(option);
        if (!tabs || !frame)
            return QProxyStyle::subElementRect(element, option, widget);

        const int   border     = tabs->documentMode() ? 0 : qMax(0, tabs->getBorderWidth());
        const QRect inner      = frame->rect.adjusted(border, border, -border, -border);
        const int   spacing    = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
        const bool  horizontal = tabs->tabPosition() == QTabWidget::North || tabs->tabPosition() == QTabWidget::South;
        const bool  addVisible = tabs->getAddButtonVisible() && (!tabs->tabBarAutoHide() || tabs->count() > 1);
        const int   addSpace   = addVisible ? 5 * spacing : 0;
        const bool  stripVisible =
            !frame->tabBarSize.isEmpty() || addVisible ||
            (horizontal && (!frame->leftCornerWidgetSize.isEmpty() || !frame->rightCornerWidgetSize.isEmpty()));
        const int naturalCross = horizontal ? frame->tabBarSize.height() : frame->tabBarSize.width();
        const int barCross     = stripVisible ? qMax(naturalCross, tabs->tabBar()->getTabHeight()) : 0;
        const int cornerCross =
            horizontal ? qMax(frame->leftCornerWidgetSize.height(), frame->rightCornerWidgetSize.height()) : 0;
        const int stripCross     = stripVisible ? qMax(barCross, cornerCross) : 0;
        const int stripInset     = stripVisible ? spacing + stripCross - 1 : 0;
        const int leadingCorner  = horizontal ? frame->leftCornerWidgetSize.width() : 0;
        const int trailingCorner = horizontal ? frame->rightCornerWidgetSize.width() : 0;
        const int available      = qMax(
            0, (horizontal ? inner.width() : inner.height()) - 2 * spacing - leadingCorner - trailingCorner - addSpace);
        const int naturalLength = horizontal ? frame->tabBarSize.width() : frame->tabBarSize.height();
        const int barLength     = stripVisible ? qMin(available, qMax(0, naturalLength)) : 0;
        QRect     pane          = inner;
        QRect     bar;
        switch (tabs->tabPosition()) {
            case QTabWidget::North:
                pane.setTop(inner.top() + stripInset);
                bar = QRect(inner.left() + spacing + leadingCorner,
                            inner.top() + spacing + stripCross - barCross,
                            barLength,
                            barCross);
                break;
            case QTabWidget::South:
                pane.setBottom(inner.bottom() - stripInset);
                bar = QRect(inner.left() + spacing + leadingCorner, inner.bottom() - stripInset, barLength, barCross);
                break;
            case QTabWidget::West:
                pane.setLeft(inner.left() + stripInset);
                bar = QRect(inner.left() + spacing, inner.top() + spacing, barCross, barLength);
                break;
            case QTabWidget::East:
                pane.setRight(inner.right() - stripInset);
                bar = QRect(inner.right() - stripInset, inner.top() + spacing, barCross, barLength);
                break;
        }

        if (element == SE_TabWidgetTabBar)
            return horizontal ? QStyle::visualRect(frame->direction, inner, bar) : bar;
        if (element == SE_TabWidgetTabPane)
            return pane;
        if (element == SE_TabWidgetTabContents) {
            const int margin = qMax(0, tabs->getBorderRadius()) / 2;
            return pane.adjusted(margin, margin, -margin, -margin);
        }
        if (element == SE_TabWidgetLeftCorner || element == SE_TabWidgetRightCorner) {
            if (!horizontal || !stripVisible)
                return QRect();
            const QSize corner =
                element == SE_TabWidgetLeftCorner ? frame->leftCornerWidgetSize : frame->rightCornerWidgetSize;
            const int x = element == SE_TabWidgetLeftCorner ? inner.left() + spacing
                                                            : inner.right() - spacing - corner.width() + 1;
            const int y = tabs->tabPosition() == QTabWidget::North ? pane.top() - corner.height() + 1 : pane.bottom();
            return QStyle::visualRect(frame->direction, inner, QRect(QPoint(x, y), corner));
        }
        return QProxyStyle::subElementRect(element, option, widget);
    }

    int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override {
        if (metric == PM_DefaultFrameWidth)
            return 0;
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
};

} // namespace

NTabWidgetPrivate::NTabWidgetPrivate(QObject* parent) : QObject(parent) {}
NTabWidgetPrivate::~NTabWidgetPrivate() = default;

void NTabWidgetPrivate::initStyle() {
    Q_Q(NTabWidget);
    addButton = new TabAddButton(q);
    addButton->hide();
    QObject::connect(addButton, &QToolButton::clicked, q, &NTabWidget::tabAddRequested);
    closeShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_W), q);
    closeShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    QObject::connect(
        closeShortcut, &QShortcut::activated, q, [q]() { q->tabBar()->requestTabClose(q->currentIndex()); });
    closeAlternativeShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F4), q);
    closeAlternativeShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    QObject::connect(
        closeAlternativeShortcut, &QShortcut::activated, q, [q]() { q->tabBar()->requestTabClose(q->currentIndex()); });
    addShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_T), q);
    addShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    QObject::connect(addShortcut, &QShortcut::activated, q, [q]() {
        if (q->getAddButtonVisible())
            emit q->tabAddRequested();
    });
    auto* tabStyle = new TabWidgetStyle;
    tabStyle->setParent(q);
    q->setStyle(tabStyle);
}

void NTabWidgetPrivate::updateAddButtonGeometry() {
    Q_Q(NTabWidget);
    if (!addButton)
        return;
    const bool visible = _pAddButtonVisible && (!q->tabBarAutoHide() || q->count() > 1);
    addButton->setVisible(visible);
    if (!visible)
        return;

    const int   spacing = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
    const int   extent  = 4 * spacing;
    const QRect bar     = q->tabBar()->geometry();
    QRect       button;
    if (q->tabPosition() == QTabWidget::North || q->tabPosition() == QTabWidget::South) {
        int x  = q->layoutDirection() == Qt::RightToLeft ? bar.left() - spacing - extent : bar.right() + 1 + spacing;
        x      = qBound(0, x, qMax(0, q->width() - extent));
        button = QRect(x, bar.top() + (bar.height() - extent) / 2, extent, extent);
    } else {
        button = QRect(bar.left() + (bar.width() - extent) / 2,
                       qMin(bar.bottom() + 1 + spacing, qMax(0, q->height() - extent)),
                       extent,
                       extent);
    }
    addButton->setGeometry(button);
    addButton->raise();
}
