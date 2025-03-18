#ifndef QTNATIVEUI_NDROPDOWNBUTTON_P_H
#define QTNATIVEUI_NDROPDOWNBUTTON_P_H

#include <QtNativeUI/NDropDownButton.h>
#include "QtNativeUI/NEnums.h"

class NDropDownButtonPrivate : public QObject {
    Q_OBJECT

  public:
    explicit NDropDownButtonPrivate(QObject* parent = nullptr);
    ~NDropDownButtonPrivate();
    Q_D_CREATE(NDropDownButton)

  private:
    QMenu* _menu{nullptr};
    bool   _menuVisible{false};
    int    _arrowSize{8};    // 下拉箭头大小
    int    _arrowSpacing{4}; // 箭头和文本之间的间距
};

#endif // QTNATIVEUI_NDROPDOWNBUTTON_P_H