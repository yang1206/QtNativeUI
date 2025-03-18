#ifndef QTNATIVEUI_NDROPDOWNBUTTON_H
#define QTNATIVEUI_NDROPDOWNBUTTON_H

#include <QMenu>
#include "NPushButton.h"
#include "stdafx.h"

class NDropDownButtonPrivate;
class QTNATIVEUI_EXPORT NDropDownButton : public NPushButton {
    Q_OBJECT
    Q_Q_CREATE(NDropDownButton)

  public:
    explicit NDropDownButton(QWidget* parent = nullptr);
    explicit NDropDownButton(const QString& text, QWidget* parent = nullptr);
    ~NDropDownButton();

    void   setMenu(QMenu* menu);
    QMenu* menu() const;

  protected:
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;

    // void drawDropDownArrow(QPainter* painter);

  private:
    void init();
    // void showDropDownMenu();
    // void updateAccentColors();
};

#endif // QTNATIVEUI_NDROPDOWNBUTTON_H