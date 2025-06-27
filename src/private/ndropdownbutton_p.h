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
    bool   _showArrow{true};
    int    _arrowSize{18};
    int    _horizontalPadding;
    int    _contentArrowSpacing{8};    int                               _iconTextSpacing{4};
    qreal                             _arrowYOffset{0};
    QtNativeUI::NTranslateYAnimation* _arrowAnimation{nullptr};
};

#endif // QTNATIVEUI_NDROPDOWNBUTTON_P_H