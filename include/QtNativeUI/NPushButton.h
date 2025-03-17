#ifndef QTNATIVEUI_NPUSHBUTTON_H
#define QTNATIVEUI_NPUSHBUTTON_H

#include <QPropertyAnimation>
#include <QPushButton>

#include "NColor.h"
#include "NIconEnums.h"
#include "stdafx.h"

class NPushButtonPrivate;
class QTNATIVEUI_EXPORT NPushButton : public QPushButton {
    Q_OBJECT
    Q_Q_CREATE(NPushButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextPressColor)

  public:
    enum ButtonType {
        Standard, // 标准按钮
        Accent    // 强调色按钮
    };
    Q_ENUM(ButtonType)

    explicit NPushButton(QWidget* parent = nullptr);
    explicit NPushButton(QString text, QWidget* parent = nullptr);
    ~NPushButton();

    void       setButtonType(ButtonType type);
    ButtonType buttonType() const;

    void setAccentColor(const NAccentColor& color);
    void setAccentColor(const QColor& color);

    void setFluentIcon(NRegularIconType::Icon icon, int size = 16, const QColor& color = QColor());
    void setFluentIcon(NFilledIconType::Icon icon, int size = 16, const QColor& color = QColor());

  protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

    void drawBackground(QPainter* painter);
    void drawBorder(QPainter* painter);
    void drawIcon(QPainter* painter);
    void drawText(QPainter* painter);

  private:
    void updateAccentColors();
    void resetAccentColor();
    void updateFluentIcon();
};

#endif // QTNATIVEUI_NPUSHBUTTON_H