#ifndef QTNATIVEUI_NPUSHBUTTON_P_H
#define QTNATIVEUI_NPUSHBUTTON_P_H

#include <QtNativeUI/NPushButton.h>

class NPushButtonPrivate : public QObject {
    Q_OBJECT
    N_DECLARE_PUBLIC(NPushButton)
  public:
    NPushButtonPrivate(NPushButton* q);
    ~NPushButtonPrivate();

  private:
    bool _isPressed{false};
};

#endif // QTNATIVEUI_NPUSHBUTTON_P_H