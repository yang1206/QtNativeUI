#ifndef QTNATIVEUI_NLINEEDIT_H
#define QTNATIVEUI_NLINEEDIT_H

#include <QLineEdit>

#include "NIconEnums.h"
#include "stdafx.h"

class NLineEditPrivate;
class QTNATIVEUI_EXPORT NLineEdit : public QLineEdit {
    Q_OBJECT
    Q_Q_CREATE(NLineEdit)

    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBottomLineColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderWidth)

  public:
    explicit NLineEdit(QWidget* parent = nullptr);
    explicit NLineEdit(const QString& text, QWidget* parent = nullptr);
    ~NLineEdit();

    void     addAction(QAction* action, ActionPosition position);
    QAction* addAction(NRegularIconType::Icon icon, ActionPosition position);
    QAction* addAction(NFilledIconType::Icon icon, ActionPosition position);

  Q_SIGNALS:
    void focusIn(QString text);
    void focusOut(QString text);

  protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

  private:
    void init();
};

#endif // QTNATIVEUI_NLINEEDIT_H