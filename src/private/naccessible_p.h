#ifndef NACCESSIBLE_P_H
#define NACCESSIBLE_P_H

#include <QAccessible>
#include <QString>
#include <QVariant>

class QTableView;
class QWidget;

namespace NA11y {

bool applyAutomaticAccessibleDescription(QWidget* widget, QString& trackedAutomatic, const QString& nextAutomatic);
void notifyAccessibleEvent(QWidget* widget, QAccessible::Event type);
void notifyAccessibleStateChange(QWidget* widget, const QAccessible::State& changed);
void notifyAccessibleValueChange(QWidget* widget, const QVariant& value);
} // namespace NA11y

#endif
