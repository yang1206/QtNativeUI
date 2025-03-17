#ifndef QTNATIVEUI_NCHECKBOX_P_H
#define QTNATIVEUI_NCHECKBOX_P_H

#include <QtNativeUI/NCheckBox.h>

class NCheckBoxPrivate : public QObject {
    Q_OBJECT

  public:
    explicit NCheckBoxPrivate(QObject* parent = nullptr);
    ~NCheckBoxPrivate();
    Q_D_CREATE(NCheckBox)
};

#endif // QTNATIVEUI_NCHECKBOX_P_H