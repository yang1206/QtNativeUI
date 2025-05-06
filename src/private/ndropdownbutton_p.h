#ifndef QTNATIVEUI_NDROPDOWNBUTTON_P_H
#define QTNATIVEUI_NDROPDOWNBUTTON_P_H

#include <QtNativeUI/NDropDownButton.h>
#include <QtNativeUI/NAnimation.h>

class NDropDownButtonPrivate : public QObject {
    Q_OBJECT

  public:
    explicit NDropDownButtonPrivate(QObject* parent = nullptr);
    ~NDropDownButtonPrivate();
    Q_D_CREATE(NDropDownButton)

    // 箭头的Y偏移量（用于动画）
    Q_PROPERTY(qreal arrowYOffset READ arrowYOffset WRITE setArrowYOffset)
    qreal arrowYOffset() const { return _arrowYOffset; }
    void setArrowYOffset(qreal offset) {
        _arrowYOffset = offset;
        if (q_ptr) {
            q_ptr->update();
        }
    }

  private:
    NMenu* _menu{nullptr};
    bool   _menuVisible{false};
    bool   _showArrow{true};    // 是否显示下拉箭头
    int    _arrowSize{18};      // 下拉箭头大小
    int    _horizontalPadding;  // 按钮左右两侧的内边距
    int    _contentArrowSpacing{8}; // 文本/图标内容与下拉箭头之间的间距
    int    _iconTextSpacing{4}; // 图标和文本之间的间距
    qreal  _arrowYOffset{0};    // 箭头Y轴偏移量（用于动画）
    QtNativeUI::NTranslateYAnimation* _arrowAnimation{nullptr}; // 箭头动画
};

#endif // QTNATIVEUI_NDROPDOWNBUTTON_P_H