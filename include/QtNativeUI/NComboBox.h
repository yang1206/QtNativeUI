#ifndef NCOMBOBOX_H
#define NCOMBOBOX_H

#include <QComboBox>
#include "stdafx.h"

class NComboBoxPrivate;
class QTNATIVEUI_EXPORT NComboBox : public QComboBox {
    Q_OBJECT
    Q_Q_CREATE(NComboBox)
    // 背景颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundFocusColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundDisabledColor)

    // 文字颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextDisabledColor)

    // 边框颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderDisabledColor)

    // 底边线颜色
    Q_PROPERTY_CREATE_Q_H(QColor, LightBottomLineColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBottomLineColor)

    // 下拉按钮颜色属性
    Q_PROPERTY_CREATE_Q_H(QColor, LightDropdownBgColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDropdownBgColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDropdownHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDropdownHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDropdownPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDropdownPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDropdownDisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDropdownDisabledColor)

    // 选中项颜色
    Q_PROPERTY_CREATE_Q_H(QColor, LightSelectionBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSelectionBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightSelectionTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkSelectionTextColor)

    // 边框属性
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, BorderWidth)

  public:
    explicit NComboBox(QWidget* parent = nullptr);
    explicit NComboBox(const QStringList& items, QWidget* parent = nullptr);
    ~NComboBox();

    QLineEdit* getLineEdit() const;

  protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void showPopup() override;
    void hidePopup() override;

  private:
    void init();
};

#endif // NCOMBOBOX_H