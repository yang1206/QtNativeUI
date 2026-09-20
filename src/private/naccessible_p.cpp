#include "naccessible_p.h"

#include <QVariant>
#include <QWidget>

namespace NA11y {

bool applyAutomaticAccessibleDescription(QWidget* widget, QString& trackedAutomatic, const QString& nextAutomatic) {
    if (!widget)
        return false;
    const QString current = widget->accessibleDescription();
    if (!current.isEmpty() && current != trackedAutomatic)
        return false;
    if (current == nextAutomatic) {
        trackedAutomatic = nextAutomatic;
        return false;
    }
    widget->setAccessibleDescription(nextAutomatic);
    trackedAutomatic = nextAutomatic;
    notifyAccessibleEvent(widget, QAccessible::DescriptionChanged);
    return true;
}

void notifyAccessibleEvent(QWidget* widget, QAccessible::Event type) {
#if QT_CONFIG(accessibility)
    if (!widget)
        return;
    QAccessibleEvent event(widget, type);
    QAccessible::updateAccessibility(&event);
#else
    Q_UNUSED(widget);
    Q_UNUSED(type);
#endif
}

void notifyAccessibleStateChange(QWidget* widget, const QAccessible::State& changed) {
#if QT_CONFIG(accessibility)
    if (!widget)
        return;
    QAccessibleStateChangeEvent event(widget, changed);
    QAccessible::updateAccessibility(&event);
#else
    Q_UNUSED(widget);
    Q_UNUSED(changed);
#endif
}

void notifyAccessibleValueChange(QWidget* widget, const QVariant& value) {
#if QT_CONFIG(accessibility)
    if (!widget)
        return;
    QAccessibleValueChangeEvent event(widget, value);
    QAccessible::updateAccessibility(&event);
#else
    Q_UNUSED(widget);
    Q_UNUSED(value);
#endif
}

} // namespace NA11y
