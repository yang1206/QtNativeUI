#ifndef QTNATIVEUI_NPUSHBUTTON_H
#define QTNATIVEUI_NPUSHBUTTON_H

#include <QPushButton>

#include "stdafx.h"

class NPushButtonPrivate;
class QTNATIVEUI_EXPORT NPushButton : public QPushButton {
    Q_OBJECT
    N_DECLARE_PRIVATE(NPushButton)

  public:
    explicit NPushButton(QWidget* parent = nullptr);
    explicit NPushButton(const QString& text, QWidget* parent = nullptr);
    ~NPushButton();

  protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // QTNATIVEUI_NPUSHBUTTON_H