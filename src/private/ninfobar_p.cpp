#include "ninfobar_p.h"

#include <QDateTime>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>

#include "QtNativeUI/NIcon.h"
#include "QtNativeUI/NInfoBar.h"
#include "QtNativeUI/NTheme.h"

QMap<NInfoBarType::PositionPolicy, QList<NInfoBar*>*> _infoBarActiveMap;

Q_SINGLETON_CREATE_CPP(NInfoBarManager)

NInfoBarManager::NInfoBarManager(QObject* parent) {}

NInfoBarManager::~NInfoBarManager() = default;

void NInfoBarManager::requestInfoBarEvent(NInfoBar* infoBar) {
    if (!infoBar) {
        return;
    }
    if (_infoBarEventMap.contains(infoBar)) {
        QList<QVariantMap> eventList = _infoBarEventMap.value(infoBar);
        QVariantMap        eventData = eventList.last();
        eventList.removeLast();
        if (eventList.isEmpty()) {
            _infoBarEventMap.remove(infoBar);
        } else {
            _infoBarEventMap[infoBar] = eventList;
        }
        QString     functionName = eventData.value("EventFunctionName").toString();
        QVariantMap functionData = eventData.value("EventFunctionData").toMap();
        QMetaObject::invokeMethod(infoBar->d_func(),
                                  functionName.toLocal8Bit().constData(),
                                  Qt::AutoConnection,
                                  Q_ARG(QVariantMap, functionData));
    }
}

void NInfoBarManager::postInfoBarCreateEvent(NInfoBar* infoBar) {
    if (!infoBar) {
        return;
    }
    updateActiveMap(infoBar, true);
    if (!_infoBarEventMap.contains(infoBar)) {
        QList<QVariantMap> eventList;
        QVariantMap        eventData;
        eventData.insert("EventFunctionName", "infoBarEnd");
        eventList.append(eventData);
        _infoBarEventMap.insert(infoBar, eventList);
    }
}

void NInfoBarManager::postInfoBarEndEvent(NInfoBar* infoBar) {
    if (!infoBar) {
        return;
    }
    updateActiveMap(infoBar, false);
    NInfoBarType::PositionPolicy position = infoBar->d_ptr->_position;
    foreach (auto otherInfoBar, *_infoBarActiveMap.value(position)) {
        if (otherInfoBar->d_ptr->_judgeCreateOrder(infoBar)) {
            QList<QVariantMap> eventList = _infoBarEventMap[otherInfoBar];
            QVariantMap        eventData;
            eventData.insert("EventFunctionName", "onOtherInfoBarEnd");
            QVariantMap functionData;
            functionData.insert("TargetPosY", otherInfoBar->d_ptr->_calculateTargetPosY());
            eventData.insert("EventFunctionData", functionData);
            // 若处于创建动画阶段，则合并事件动画
            if (otherInfoBar->d_ptr->getWorkMode() == WorkStatus::CreateAnimation) {
                while (eventList.count() > 1) {
                    eventList.removeLast();
                }
            }
            eventList.insert(1, eventData);
            _infoBarEventMap[otherInfoBar] = eventList;
            otherInfoBar->d_ptr->tryToRequestInfoBarEvent();
        }
    }
}

void NInfoBarManager::forcePostInfoBarEndEvent(NInfoBar* infoBar) {
    if (!infoBar) {
        return;
    }
    _infoBarEventMap.remove(infoBar);
    postInfoBarEndEvent(infoBar);
}

int NInfoBarManager::getInfoBarEventCount(NInfoBar* infoBar) {
    if (!infoBar) {
        return -1;
    }
    if (!_infoBarEventMap.contains(infoBar)) {
        return -1;
    }
    QList<QVariantMap> eventList = _infoBarEventMap[infoBar];
    return eventList.count();
}

void NInfoBarManager::updateActiveMap(NInfoBar* infoBar, bool isActive) {
    if (!infoBar) {
        return;
    }
    NInfoBarType::PositionPolicy position = infoBar->d_ptr->_position;
    if (isActive) {
        if (_infoBarActiveMap.contains(position)) {
            _infoBarActiveMap[position]->append(infoBar);
        } else {
            QList<NInfoBar*>* infoBarList = new QList<NInfoBar*>();
            infoBarList->append(infoBar);
            _infoBarActiveMap.insert(position, infoBarList);
        }
    } else {
        if (_infoBarActiveMap.contains(position)) {
            if (_infoBarActiveMap[position]->count() > 0) {
                _infoBarActiveMap[position]->removeOne(infoBar);
            }
        }
    }
}

NInfoBarPrivate::NInfoBarPrivate(QObject* parent) : QObject{parent} {
    setProperty("InfoBarClosedY", 0);
    setProperty("InfoBarFinishY", 0);
    _createTime = QDateTime::currentMSecsSinceEpoch();
}

NInfoBarPrivate::~NInfoBarPrivate() {}

void NInfoBarPrivate::tryToRequestInfoBarEvent() {
    Q_Q(NInfoBar);
    if (!_isInfoBarCreateAnimationFinished || _isInfoBarEventAnimationStart) {
        return;
    }
    NInfoBarManager::getInstance()->requestInfoBarEvent(q);
}

WorkStatus NInfoBarPrivate::getWorkMode() const {
    if (!_isInfoBarCreateAnimationFinished) {
        return WorkStatus::CreateAnimation;
    }
    if (_isInfoBarEventAnimationStart) {
        return WorkStatus::OtherEventAnimation;
    }
    return WorkStatus::Idle;
}

void NInfoBarPrivate::onOtherInfoBarEnd(QVariantMap eventData) {
    Q_Q(NInfoBar);
    _isInfoBarEventAnimationStart         = true;
    qreal               targetPosY        = eventData.value("TargetPosY").toReal();
    QPropertyAnimation* closePosAnimation = new QPropertyAnimation(this, "InfoBarClosedY");
    connect(closePosAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        q->move(q->pos().x(), value.toUInt());
    });
    connect(closePosAnimation, &QPropertyAnimation::finished, this, [this, q]() {
        _isInfoBarEventAnimationStart = false;
        if (NInfoBarManager::getInstance()->getInfoBarEventCount(q) > 1) {
            NInfoBarManager::getInstance()->requestInfoBarEvent(q);
        }
        if (_isReadyToEnd) {
            NInfoBarManager::getInstance()->requestInfoBarEvent(q);
        }
    });
    closePosAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closePosAnimation->setDuration(200);
    closePosAnimation->setStartValue(q->pos().y());
    closePosAnimation->setEndValue(targetPosY);
    closePosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NInfoBarPrivate::infoBarEnd(const QVariantMap& eventData) {
    Q_Q(NInfoBar);
    NInfoBarManager::getInstance()->postInfoBarEndEvent(q);
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(q);
    q->setGraphicsEffect(opacityEffect);

    QPropertyAnimation* barFinishedOpacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    connect(barFinishedOpacityAnimation, &QPropertyAnimation::finished, q, [=]() { q->deleteLater(); });
    barFinishedOpacityAnimation->setDuration(300);
    barFinishedOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barFinishedOpacityAnimation->setStartValue(1);
    barFinishedOpacityAnimation->setEndValue(0);
    barFinishedOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NInfoBarPrivate::onCloseButtonClicked() {
    Q_Q(NInfoBar);
    if (_isReadyToEnd) {
        return;
    }
    _isReadyToEnd    = true;
    _isNormalDisplay = false;
    NInfoBarManager::getInstance()->forcePostInfoBarEndEvent(q);
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(q);
    q->setGraphicsEffect(opacityEffect);

    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    connect(opacityAnimation, &QPropertyAnimation::finished, q, [=]() { q->deleteLater(); });
    opacityAnimation->setStartValue(_pOpacity);
    opacityAnimation->setEndValue(0);
    opacityAnimation->setDuration(220);
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NInfoBarPrivate::_infoBarCreate(int displayMsec) {
    Q_Q(NInfoBar);
    q->show();
    QFont font = q->font();
    font.setPixelSize(16);
    font.setWeight(QFont::Bold);
    q->setFont(font);
    int titleWidth = q->fontMetrics().horizontalAdvance(_title);
    font.setPixelSize(14);
    font.setWeight(QFont::Medium);
    q->setFont(font);
    int textWidth  = q->fontMetrics().horizontalAdvance(_message);
    int fixedWidth = _closeButtonLeftRightMargin + _leftPadding + _titleLeftSpacing + _textLeftSpacing +
                     _closeButtonWidth + titleWidth + textWidth + 2 * _shadowBorderWidth;
    q->setFixedWidth(fixedWidth > 500 ? 500 : fixedWidth);
    NInfoBarManager::getInstance()->postInfoBarCreateEvent(q);
    int startX = 0;
    int startY = 0;
    int endX   = 0;
    int endY   = 0;
    _calculateInitialPos(startX, startY, endX, endY);
    QPropertyAnimation* barPosAnimation = new QPropertyAnimation(q, "pos");
    connect(barPosAnimation, &QPropertyAnimation::finished, q, [q, displayMsec, this]() {
        _isNormalDisplay                  = true;
        _isInfoBarCreateAnimationFinished = true;
        if (NInfoBarManager::getInstance()->getInfoBarEventCount(q) > 1) {
            NInfoBarManager::getInstance()->requestInfoBarEvent(q);
        }
        if (displayMsec > 0) {
            QTimer::singleShot(displayMsec, q, [this, q]() {
                _isReadyToEnd = true;
                NInfoBarManager::getInstance()->requestInfoBarEvent(q);
            });
        }
    });
    switch (_position) {
        case NInfoBarType::Top:
        case NInfoBarType::Bottom: {
            barPosAnimation->setDuration(250);
            break;
        }
        default: {
            barPosAnimation->setDuration(450);
            break;
        }
    }
    barPosAnimation->setStartValue(QPoint(startX, startY));
    barPosAnimation->setEndValue(QPoint(endX, endY));
    barPosAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NInfoBarPrivate::_calculateInitialPos(int& startX, int& startY, int& endX, int& endY) {
    Q_Q(NInfoBar);
    QList<int> resultList         = _getOtherInfoBarTotalData();
    int        minimumHeightTotal = resultList[0];
    int        indexLessCount     = resultList[1];
    switch (_position) {
        case NInfoBarType::Top: {
            // 25动画距离
            startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
            startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin - 25;
            endX   = startX;
            endY   = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
            break;
        }
        case NInfoBarType::Left: {
            startX = -q->minimumWidth();
            startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
            endX   = _messageBarHorizontalMargin;
            endY   = startY;
            break;
        }
        case NInfoBarType::Bottom: {
            startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
            startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                     _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin - 25;
            endX = startX;
            endY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                   _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
            break;
        }
        case NInfoBarType::Right: {
            startX = q->parentWidget()->width();
            startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
            endX   = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
            endY   = startY;
            break;
        }
        case NInfoBarType::TopRight: {
            startX = q->parentWidget()->width();
            startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
            endX   = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
            endY   = startY;
            break;
        }
        case NInfoBarType::TopLeft: {
            startX = -q->minimumWidth();
            startY = minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
            endX   = _messageBarHorizontalMargin;
            endY   = startY;
            break;
        }
        case NInfoBarType::BottomRight: {
            startX = q->parentWidget()->width();
            startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                     _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
            endX = q->parentWidget()->width() - q->minimumWidth() - _messageBarHorizontalMargin;
            endY = startY;
            break;
        }
        case NInfoBarType::BottomLeft: {
            startX = -q->minimumWidth();
            startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                     _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
            endX = _messageBarHorizontalMargin;
            endY = startY;
            break;
        }
    }
    if (endY < _messageBarVerticalTopMargin ||
        endY > q->parentWidget()->height() - _messageBarVerticalBottomMargin - q->minimumHeight()) {
        NInfoBarManager::getInstance()->updateActiveMap(q, false);
        q->deleteLater();
    }
}

QList<int> NInfoBarPrivate::_getOtherInfoBarTotalData(bool isJudgeCreateOrder) {
    Q_Q(NInfoBar);
    QList<int>        resultList;
    int               minimumHeightTotal = 0;
    int               indexLessCount     = 0;
    QList<NInfoBar*>* infoBarList        = _infoBarActiveMap[_position];
    for (auto infoBar : *infoBarList) {
        if (infoBar == q) {
            continue;
        }
        if (!isJudgeCreateOrder || (isJudgeCreateOrder && _judgeCreateOrder(infoBar))) {
            indexLessCount++;
            minimumHeightTotal += infoBar->minimumHeight();
        }
    }
    resultList.append(minimumHeightTotal);
    resultList.append(indexLessCount);
    return resultList;
}

qreal NInfoBarPrivate::_calculateTargetPosY() {
    Q_Q(NInfoBar);
    QList<int> resultList         = _getOtherInfoBarTotalData(true);
    int        minimumHeightTotal = resultList[0];
    int        indexLessCount     = resultList[1];
    switch (_position) {
        case NInfoBarType::Top:
        case NInfoBarType::TopRight:
        case NInfoBarType::TopLeft: {
            return minimumHeightTotal + _messageBarSpacing * indexLessCount + _messageBarVerticalTopMargin;
        }
        case NInfoBarType::Left:
        case NInfoBarType::Right: {
            return minimumHeightTotal + _messageBarSpacing * indexLessCount + q->parentWidget()->height() / 2;
        }
        case NInfoBarType::Bottom:
        case NInfoBarType::BottomRight:
        case NInfoBarType::BottomLeft: {
            return q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                   _messageBarSpacing * indexLessCount - _messageBarVerticalBottomMargin;
        }
    }
    return 0;
}

bool NInfoBarPrivate::_judgeCreateOrder(NInfoBar* otherInfoBar) {
    if (otherInfoBar->d_ptr->_createTime < _createTime) {
        return true;
    } else {
        return false;
    }
}

void NInfoBarPrivate::_drawCommonBackground(QPainter* painter, const QColor& bgColor) {
    Q_Q(NInfoBar);
    QRect foregroundRect(_shadowBorderWidth,
                         _shadowBorderWidth,
                         q->width() - 2 * _shadowBorderWidth,
                         q->height() - 2 * _shadowBorderWidth);
    painter->setBrush(bgColor);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
}

void NInfoBarPrivate::_drawIconWithCircle(QPainter*              painter,
                                          NRegularIconType::Icon iconType,
                                          const QColor&          circleBgColor) {
    Q_Q(NInfoBar);
    painter->save();
    int iconY = _isLongMessage ? 15 + q->fontMetrics().height() / 2 : q->height() / 2;

    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + _iconSize, iconY), 9, 9);
    painter->setClipPath(textPath);
    QColor iconColor = nTheme->isDarkMode() ? NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light)
                                            : NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    painter->fillPath(textPath, circleBgColor);

    painter->drawPixmap(_leftPadding + _iconSize / 2,
                        iconY - _iconSize / 2,
                        _iconSize,
                        _iconSize,
                        nIcon->fromRegular(iconType, _iconSize, iconColor).pixmap(_iconSize, _iconSize));
    painter->restore();
}

void NInfoBarPrivate::_drawIconWithCircle(QPainter*             painter,
                                          NFilledIconType::Icon iconType,
                                          const QColor&         circleBgColor) {
    Q_Q(NInfoBar);
    painter->save();
    int iconY = _isLongMessage ? 15 + q->fontMetrics().height() / 2 : q->height() / 2;

    QPainterPath textPath;
    textPath.addEllipse(QPoint(_leftPadding + _iconSize, iconY), 9, 9);
    painter->setClipPath(textPath);
    QColor iconColor = nTheme->isDarkMode() ? NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light)
                                            : NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    painter->fillPath(textPath, circleBgColor);

    painter->drawPixmap(_leftPadding + _iconSize / 2,
                        iconY - _iconSize / 2,
                        _iconSize,
                        _iconSize,
                        nIcon->fromFilled(iconType, _iconSize, iconColor).pixmap(_iconSize, _iconSize));
    painter->restore();
}

void NInfoBarPrivate::_drawSuccess(QPainter* painter) {
    _drawCommonBackground(painter, NThemeColor(NFluentColorKey::SystemFillColorSuccessBackground, _themeMode));
    _drawIconWithCircle(painter,
                        NRegularIconType::Checkmark12Regular,
                        NThemeColor(NFluentColorKey::SystemFillColorSuccess, _themeMode));
    painter->setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode));
}

void NInfoBarPrivate::_drawWarning(QPainter* painter) {
    _drawCommonBackground(painter, NThemeColor(NFluentColorKey::SystemFillColorCautionBackground, _themeMode));
    _drawIconWithCircle(
        painter, NFilledIconType::Important12Filled, NThemeColor(NFluentColorKey::SystemFillColorCaution, _themeMode));
    painter->setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode));
}

void NInfoBarPrivate::_drawInformation(QPainter* painter) {
    Q_Q(NInfoBar);
    QColor bgColor = NThemeColor(NFluentColorKey::SystemFillColorSolidAttentionBackground, _themeMode);
    QRect  foregroundRect(_shadowBorderWidth,
                         _shadowBorderWidth,
                         q->width() - 2 * _shadowBorderWidth,
                         q->height() - 2 * _shadowBorderWidth);
    painter->setBrush(bgColor);
    painter->drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);

    painter->save();
    QColor iconColor =
        !nTheme->isDarkMode() == NThemeType::Light ? nTheme->accentColor().light() : nTheme->accentColor().dark();
    int iconSize = _iconSize * 2;

    // 计算图标垂直位置，与标题保持一致
    int iconY = _isLongMessage ? 15 + q->fontMetrics().height() / 2 : q->height() / 2;

    painter->drawPixmap(
        _leftPadding,
        iconY - iconSize / 2,
        iconSize,
        iconSize,
        nIcon->fromFilled(NFilledIconType::Info12Filled, iconSize, iconColor).pixmap(iconSize, iconSize));
    painter->restore();

    QColor textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);
    painter->setPen(textColor);
}

void NInfoBarPrivate::_drawError(QPainter* painter) {
    _drawCommonBackground(painter, NThemeColor(NFluentColorKey::SystemFillColorCriticalBackground, _themeMode));
    _drawIconWithCircle(
        painter, NRegularIconType::Dismiss12Regular, NThemeColor(NFluentColorKey::SystemFillColorCritical, _themeMode));
    painter->setPen(NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode));
}

void NInfoBarPrivate::_initializeInfoBar() {
    Q_Q(NInfoBar);

    q->setMinimumHeight(60);

    _calculateTextLayout();

    q->updateGeometry();
}

void NInfoBarPrivate::_calculateTextLayout() {
    Q_Q(NInfoBar);

    // 计算标题宽度
    QFont titleFont = q->font();
    titleFont.setPixelSize(16);
    titleFont.setWeight(QFont::Bold);
    QFontMetrics titleMetrics(titleFont);
    int          titleWidth  = titleMetrics.horizontalAdvance(_title);
    int          titleHeight = titleMetrics.height();

    // 计算消息宽度
    QFont messageFont = q->font();
    messageFont.setPixelSize(14);
    messageFont.setWeight(QFont::Medium);
    QFontMetrics messageMetrics(messageFont);

    // 计算可用宽度
    int closeButtonSpace = _showCloseButton ? (_closeButtonWidth + _closeButtonLeftRightMargin) : 10;
    int availableWidth   = q->width() - _leftPadding - _titleLeftSpacing - closeButtonSpace - _shadowBorderWidth * 2;

    // 为额外控件预留空间
    if (!_additionalWidgets.isEmpty()) {
        int widgetsWidth = 0;
        for (auto widget : _additionalWidgets) {
            widgetsWidth += widget->width() + 8; // 8是间距
        }
        availableWidth -= (widgetsWidth + 10); // 10是额外的边距
    }

    // 计算标题后剩余的宽度
    int remainingWidth = availableWidth - titleWidth - _textLeftSpacing;

    // 计算消息在一行内的宽度
    int messageWidth = messageMetrics.horizontalAdvance(_message);

    // 判断是否需要换行显示消息
    _isLongMessage = (messageWidth > remainingWidth) || (_message.length() > 40) || (_message.contains('\n'));

    // 根据是否换行设置控件高度
    if (_isLongMessage) {
        QRect messageRect(0, 0, availableWidth - 20, 1000);
        QRect boundingRect =
            messageMetrics.boundingRect(messageRect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, _message);

        int topMargin           = 15;
        int titleMessageSpacing = 8;
        int bottomMargin        = 15;

        int minHeight = topMargin + titleHeight + titleMessageSpacing + boundingRect.height() + bottomMargin;

        q->setMinimumHeight(std::max(60, minHeight));
    } else {
        q->setMinimumHeight(60);
    }
}

void NInfoBarPrivate::_drawText(QPainter* painter) {
    Q_Q(NInfoBar);

    QColor textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, _themeMode);
    painter->setPen(textColor);

    // 设置标题字体
    QFont font = q->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(16);
    painter->setFont(font);

    int titleX         = _leftPadding + _titleLeftSpacing;
    int titleTopMargin = 15; // 固定顶部边距
    int titleY         = _isLongMessage ? titleTopMargin : (q->height() / 2 - painter->fontMetrics().height() / 2);

    painter->drawText(QRect(titleX, titleY, q->width() / 2, painter->fontMetrics().height()),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      _title);

    font.setWeight(QFont::Medium);
    font.setPixelSize(14);
    painter->setFont(font);

    int closeButtonSpace = _showCloseButton ? (_closeButtonWidth + _closeButtonLeftRightMargin) : 10;

    if (_isLongMessage) {
        int messageX     = titleX;
        int messageY     = titleY + painter->fontMetrics().height() + 8;
        int messageWidth = q->width() - messageX - closeButtonSpace - 20;

        if (!_additionalWidgets.isEmpty()) {
            int widgetsWidth = 0;
            for (auto widget : _additionalWidgets) {
                widgetsWidth += widget->width() + 8;
            }
            messageWidth -= (widgetsWidth + 10);
        }

        painter->drawText(QRect(messageX, messageY, messageWidth, q->height() - messageY - 10),
                          Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop,
                          _message);
    } else {
        int titleWidth   = painter->fontMetrics().horizontalAdvance(_title);
        int messageX     = titleX + titleWidth + _textLeftSpacing;
        int messageWidth = q->width() - messageX - closeButtonSpace - 20;

        if (!_additionalWidgets.isEmpty()) {
            int widgetsWidth = 0;
            for (auto widget : _additionalWidgets) {
                widgetsWidth += widget->width() + 8;
            }
            messageWidth -= (widgetsWidth + 10);
        }

        painter->drawText(QRect(messageX, titleY + 1, messageWidth, painter->fontMetrics().height()),
                          Qt::AlignLeft | Qt::AlignVCenter,
                          _message);
    }
}