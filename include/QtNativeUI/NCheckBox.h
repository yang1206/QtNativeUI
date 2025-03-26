#ifndef QTNATIVEUI_NCHECKBOX_H
#define QTNATIVEUI_NCHECKBOX_H

#include <QCheckBox>
#include "NIconEnums.h"
#include "stdafx.h"

class NCheckBoxPrivate;
class QTNATIVEUI_EXPORT NCheckBox : public QCheckBox {
    Q_OBJECT
    Q_Q_CREATE(NCheckBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderPressColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderPressColor)

  public:
    explicit NCheckBox(QWidget* parent = nullptr);
    explicit NCheckBox(const QString& text, QWidget* parent = nullptr);
    ~NCheckBox();

    virtual void setChecked(bool checked);
    virtual void nextCheckState() override;

  protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void changeEvent(QEvent* event) override;

    void drawCheckBox(QPainter* painter);
    void drawText(QPainter* painter);

  private:
    void init();
    void updateAccentColors();
};

#endif // QTNATIVEUI_NCHECKBOX_H