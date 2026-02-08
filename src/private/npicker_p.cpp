#include "npicker_p.h"
#include <QPropertyAnimation>

NPickerPrivate::NPickerPrivate(QObject* parent) : QObject(parent) {}

NPickerPrivate::~NPickerPrivate() {}

void NPickerPrivate::_scroll(int delta) {
    _deltaAccumulator += delta;
    int steps = _deltaAccumulator / 120;

    if (steps == 0) {
        return;
    }

    _deltaAccumulator -= steps * 120;

    _targetScrollOffset -= steps * _pItemHeight;
    _targetScrollOffset   = qRound(_targetScrollOffset / _pItemHeight) * _pItemHeight;

    if (!_pLoopEnabled) {
        if (_targetScrollOffset < 0) {
            _targetScrollOffset = 0;
        }
        if (_targetScrollOffset > (_pItems.size() - 1) * _pItemHeight) {
            _targetScrollOffset = (_pItems.size() - 1) * _pItemHeight;
        }
    }

    _scrollAnimation->stop();
    _scrollAnimation->setStartValue(_pScrollOffset);
    _scrollAnimation->setEndValue(_targetScrollOffset);
    _scrollAnimation->start();
}

