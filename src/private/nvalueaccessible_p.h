#ifndef NVALUEACCESSIBLE_P_H
#define NVALUEACCESSIBLE_P_H

#include <QAccessible>
#include <QAccessibleWidget>
#include <QStringList>
#include <QVariant>

class QWidget;

namespace NA11y::detail {

#if QT_CONFIG(accessibility)

/** Private base for custom controls that expose one bounded semantic value. */
class ValueAccessibleAdapter : public QAccessibleWidget,
                               public QAccessibleValueInterface {
public:
    ValueAccessibleAdapter(QWidget* widget, QAccessible::Role role);
    ~ValueAccessibleAdapter() override = default;

    void* interface_cast(QAccessible::InterfaceType type) override;
    QAccessible::State state() const override;
    QString text(QAccessible::Text type) const override;

    QStringList actionNames() const override;
    void doAction(const QString& actionName) override;
    QStringList keyBindingsForAction(
        const QString& actionName) const override;

protected:
    virtual bool accessibleValueReadOnly() const = 0;
    virtual bool accessibleValueBusy() const { return false; }
    virtual bool accessibleValueAnimated() const { return false; }
    virtual bool accessibleValueInvalid() const { return false; }
    virtual bool accessibleValueEditable() const
    {
        return !accessibleValueReadOnly();
    }
    virtual bool canIncreaseAccessibleValue() const { return false; }
    virtual bool canDecreaseAccessibleValue() const { return false; }
    virtual void changeAccessibleValueByStep(int direction)
    {
        Q_UNUSED(direction)
    }
    virtual QString accessibleValueText() const;
};

#endif // QT_CONFIG(accessibility)

void notifyValueAccessibilityValue(QWidget* widget, const QVariant& value);
void notifyValueAccessibilityState(QWidget* widget,
                                   const QAccessible::State& changed);
void notifyValueAccessibilityText(QWidget* widget, QAccessible::Event event);

} // namespace NA11y::detail

#endif // NVALUEACCESSIBLE_P_H
