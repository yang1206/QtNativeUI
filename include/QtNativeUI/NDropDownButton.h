#ifndef QTNATIVEUI_NDROPDOWNBUTTON_H
#define QTNATIVEUI_NDROPDOWNBUTTON_H

#include "NMenu.h"
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

    void   setMenu(NMenu* menu);
    QMenu* menu() const;

    void setShowArrow(bool show);
    bool showArrow() const;

  public slots:
    void showMenu();

  protected:
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void drawDropDownArrow();

  private:
    void init();
};

#endif // QTNATIVEUI_NDROPDOWNBUTTON_H