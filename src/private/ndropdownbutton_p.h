#ifndef QTNATIVEUI_NDROPDOWNBUTTON_P_H
#define QTNATIVEUI_NDROPDOWNBUTTON_P_H

#include <QtNativeUI/NDropDownButton.h>

class NDropDownButtonPrivate : public QObject {
    Q_OBJECT

  public:
    explicit NDropDownButtonPrivate(QObject* parent = nullptr);
    ~NDropDownButtonPrivate();
    Q_D_CREATE(NDropDownButton)

  private:
    NMenu* _menu{nullptr};
    bool   _menuVisible{false};
    bool   _showArrow{true};    // 是否显示下拉箭头
    int    _arrowSize{18};      // 下拉箭头大小
    int    _horizontalPadding;  // 按钮左右两侧的内边距
    int    _contentArrowSpacing{8}; // 文本/图标内容与下拉箭头之间的间距
    int    _iconTextSpacing{4}; // 图标和文本之间的间距
};

#endif // QTNATIVEUI_NDROPDOWNBUTTON_P_H