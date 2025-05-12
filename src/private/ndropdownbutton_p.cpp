#include "ndropdownbutton_p.h"
#include "QtNativeUI/NDropDownButton.h"
#include "QtNativeUI/NAnimation.h"

NDropDownButtonPrivate::NDropDownButtonPrivate(QObject* parent) : QObject(parent) {
    _arrowYOffset = 0;
}

NDropDownButtonPrivate::~NDropDownButtonPrivate() {
    if (_arrowAnimation) {
        delete _arrowAnimation;
        _arrowAnimation = nullptr;
    }
} 