#include "ntabbar_p.h"

#include <QAbstractButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeySequence>
#include <QMimeData>
#include <QStyle>
#include <QTimer>
#include <QtNativeUI/NTheme.h>
#include "naccessible_p.h"

namespace {

constexpr auto nativeCloseButtonProperty      = "_nTabNativeCloseButton";
constexpr auto closeButtonInitializedProperty = "_nTabCloseInitialized";

} // namespace

NTabBarPrivate::NTabBarPrivate(QObject* parent) : QObject(parent) {}
NTabBarPrivate::~NTabBarPrivate() = default;

bool NTabBarPrivate::vertical() const {
    Q_Q(const NTabBar);
    return q->shape() == QTabBar::RoundedWest || q->shape() == QTabBar::RoundedEast ||
           q->shape() == QTabBar::TriangularWest || q->shape() == QTabBar::TriangularEast;
}
bool NTabBarPrivate::compact(int index) const {
    Q_Q(const NTabBar);
    // WinUI 的 compact 状态以图标作为可识别内容；无图标时退回内容宽度，避免只剩省略号。
    return _pTabWidthMode == NTabBar::TabWidthMode::Compact && index != q->currentIndex() &&
           !q->tabIcon(index).isNull();
}
bool NTabBarPrivate::reserveCloseButton(int index) const {
    Q_Q(const NTabBar);
    return q->tabsClosable() && q->getTabClosable(index) &&
           _pCloseButtonDisplayMode != NTabBar::CloseButtonDisplayMode::Never && !compact(index);
}
QAbstractButton* NTabBarPrivate::closeButton(int index) const {
    Q_Q(const NTabBar);
    if (index < 0 || index >= q->count())
        return nullptr;
    for (const auto side : {QTabBar::LeftSide, QTabBar::RightSide}) {
        auto* button = qobject_cast<QAbstractButton*>(q->tabButton(index, side));
        if (button && button->property(nativeCloseButtonProperty).toBool())
            return button;
    }
    return nullptr;
}
bool NTabBarPrivate::isCloseButton(const QObject* object) const {
    return object && object->property(nativeCloseButtonProperty).toBool();
}
int NTabBarPrivate::closeButtonIndex(const QObject* object) const {
    Q_Q(const NTabBar);
    const auto* widget = qobject_cast<const QWidget*>(object);
    if (!widget || widget->parentWidget() != q || !isCloseButton(object))
        return -1;
    const int index = q->tabAt(widget->geometry().center());
    return index >= 0 && (q->tabButton(index, QTabBar::RightSide) == widget ||
                          q->tabButton(index, QTabBar::LeftSide) == widget)
               ? index
               : -1;
}
bool NTabBarPrivate::initializeCloseButton(int index) {
    Q_Q(NTabBar);
    if (index < 0 || index >= q->count() || !q->tabsClosable())
        return false;
    const auto side =
        static_cast<QTabBar::ButtonPosition>(q->style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, nullptr, q));
    auto* button = qobject_cast<QAbstractButton*>(q->tabButton(index, side));
    if (button && !button->property(nativeCloseButtonProperty).toBool() &&
        qstrcmp(button->metaObject()->className(), "CloseButton") == 0) {
        button->setProperty(nativeCloseButtonProperty, true);
        return true;
    }
    return false;
}
void NTabBarPrivate::updateCloseButton(int index) {
    Q_Q(NTabBar);
    if (index < 0 || index >= q->count())
        return;
    auto* button = closeButton(index);
    if (!button)
        return;
    if (!button->property(closeButtonInitializedProperty).toBool()) {
        button->setProperty(closeButtonInitializedProperty, true);
        button->installEventFilter(q);
    }
    const int spacing = NSpacingToken(NDesignTokenKey::SpacingM).toInt();
    button->setFixedSize(vertical() ? QSize(3 * spacing, 4 * spacing) : QSize(4 * spacing, 3 * spacing));
    const bool visible =
        reserveCloseButton(index) && (_pCloseButtonDisplayMode == NTabBar::CloseButtonDisplayMode::Always ||
                                      index == q->currentIndex() || index == _hoverIndex);
    const bool            enabled     = q->isTabEnabled(index) && reserveCloseButton(index);
    const Qt::FocusPolicy focusPolicy = visible && index == q->currentIndex() ? Qt::StrongFocus : Qt::NoFocus;
    const bool            stateChanged =
        (!button->isHidden()) != visible || button->isEnabled() != enabled || button->focusPolicy() != focusPolicy;
    button->setEnabled(enabled);
    button->setVisible(visible);
    button->setFocusPolicy(focusPolicy);
    QString tabName;
#if QT_CONFIG(accessibility)
    tabName = q->accessibleTabName(index);
#endif
    if (tabName.isEmpty()) {
        tabName = q->tabText(index);
        tabName.remove(QLatin1Char('&'));
    }
    const QString accessibleName = QStringLiteral("关闭标签：") + tabName;
    const bool    nameChanged    = button->accessibleName() != accessibleName;
    button->setAccessibleName(accessibleName);
    button->setAccessibleDescription(QStringLiteral("关闭此标签；当前标签快捷键 %1")
                                         .arg(QKeySequence(Qt::CTRL | Qt::Key_W).toString(QKeySequence::NativeText)));
    button->setToolTip(button->accessibleName());
    if (nameChanged)
        NA11y::notifyAccessibleEvent(button, QAccessible::NameChanged);
    if (stateChanged) {
        QAccessible::State changed;
        changed.disabled  = true;
        changed.focusable = true;
        changed.invisible = true;
        NA11y::notifyAccessibleStateChange(button, changed);
    }
    button->update();
}
void NTabBarPrivate::updateCloseButtons() {
    Q_Q(NTabBar);
    if (!q->tabsClosable()) {
        closeCurrentIndex = -1;
        return;
    }
    for (int i = 0; i < q->count(); ++i)
        updateCloseButton(i);
    closeCurrentIndex = q->currentIndex();
}
void NTabBarPrivate::scheduleCloseButtons() {
    Q_Q(NTabBar);
    if (buttonsPending)
        return;
    buttonsPending = true;
    QTimer::singleShot(0, q, [this]() {
        buttonsPending = false;
        updateCloseButtons();
    });
}
void NTabBarPrivate::scheduleLayout() {
    Q_Q(NTabBar);
    if (layoutPending)
        return;
    layoutPending = true;
    QTimer::singleShot(0, q, [this, q]() {
        layoutPending = false;
        q->invalidateTabLayout();
    });
}
void NTabBarPrivate::updateHover(int index) {
    Q_Q(NTabBar);
    if (_hoverIndex == index)
        return;
    const int previous = _hoverIndex;
    _hoverIndex        = index;
    hideToolTip();
    updateCloseButton(previous);
    updateCloseButton(index);
    const int radius = qMax(0, q->getBorderRadius());
    if (previous >= 0)
        q->update(q->tabRect(previous).adjusted(-radius - 1, -1, radius + 1, 1));
    if (index >= 0)
        q->update(q->tabRect(index).adjusted(-radius - 1, -1, radius + 1, 1));
}
void NTabBarPrivate::hideToolTip() {
    if (tooltip)
        tooltip->hide();
}
void NTabBarPrivate::showToolTip(int index, const QString& overrideText) {
    Q_Q(NTabBar);
    if (index < 0 || index >= q->count()) {
        hideToolTip();
        return;
    }
    QString text = overrideText.isEmpty() ? q->tabToolTip(index) : overrideText;
    if (text.isEmpty())
        text = q->tabText(index);
    if (text.isEmpty()) {
        hideToolTip();
        return;
    }
    if (!tooltip)
        tooltip = NToolTip::createToolTip(text, q);
    else
        tooltip->setText(text);
    if (!tooltipAnchor) {
        tooltipAnchor = new QWidget(q);
        tooltipAnchor->setAttribute(Qt::WA_TransparentForMouseEvents);
        tooltipAnchor->hide();
    }
    tooltipAnchor->setGeometry(q->tabRect(index));
    NToolTipPosition position = NToolTipPosition::BOTTOM;
    if (q->shape() == QTabBar::RoundedSouth || q->shape() == QTabBar::TriangularSouth)
        position = NToolTipPosition::TOP;
    else if (q->shape() == QTabBar::RoundedWest || q->shape() == QTabBar::TriangularWest)
        position = NToolTipPosition::RIGHT;
    else if (q->shape() == QTabBar::RoundedEast || q->shape() == QTabBar::TriangularEast)
        position = NToolTipPosition::LEFT;
    tooltip->setDuration(5000);
    tooltip->adjustSize();
    tooltip->adjustPosition(tooltipAnchor, position);
    tooltip->show();
}
int NTabBarPrivate::droppedIndex(const QMimeData* data) const {
    if (!data)
        return -1;
    const auto object = QJsonDocument::fromJson(data->data("application/x-qtnativeui-tab")).object();
    if (QUuid(object.value("source").toString()) != sourceId)
        return -1;
    const QUuid id(object.value("tab").toString());
    for (int i = 0; i < tabs.size(); ++i) {
        if (tabs[i].id == id)
            return i;
    }
    return -1;
}
