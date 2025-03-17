#ifndef QTNATIVEUI_NCHECKBOX_H
#define QTNATIVEUI_NCHECKBOX_H

#include <QCheckBox>
#include "stdafx.h"

class NCheckBoxPrivate;
class QTNATIVEUI_EXPORT NCheckBox : public QCheckBox {
    Q_OBJECT
    Q_Q_CREATE(NCheckBox)

  public:
    explicit NCheckBox(QWidget* parent = nullptr);
    explicit NCheckBox(const QString& text, QWidget* parent = nullptr);
    ~NCheckBox();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    void init();
};

#endif // QTNATIVEUI_NCHECKBOX_H 