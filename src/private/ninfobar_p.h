#ifndef QTNATIVEUI_NINFOBAR_P_H
#define QTNATIVEUI_NINFOBAR_P_H

#include <QMap>
#include <QObject>
#include <QVariantMap>
#include "QtNativeUI/NEnums.h"
#include "QtNativeUI/NInfoBar.h"
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/singleton.h"
#include "QtNativeUI/stdafx.h"

class QBoxLayout;
enum WorkStatus {
    Idle                = 0x0000,
    CreateAnimation     = 0x0001,
    OtherEventAnimation = 0x0002,
};

class NInfoBar;
class NInfoBarManager : public QObject {
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NInfoBarManager)
  private:
    explicit NInfoBarManager(QObject* parent = nullptr);
    ~NInfoBarManager();

  public:
    void requestInfoBarEvent(NInfoBar* infoBar);
    void postInfoBarCreateEvent(NInfoBar* infoBar);
    void postInfoBarEndEvent(NInfoBar* infoBar);
    void forcePostInfoBarEndEvent(NInfoBar* infoBar);
    int  getInfoBarEventCount(NInfoBar* infoBar);
    void updateActiveMap(NInfoBar* infoBar, bool isActive);

  private:
    QMap<NInfoBar*, QList<QVariantMap>> _infoBarEventMap;
};

class QPushButton;
class QPainter;
class NInfoBarPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NInfoBar)
    Q_PROPERTY_CREATE(qreal, Opacity)
  public:
    explicit NInfoBarPrivate(QObject* parent = nullptr);
    ~NInfoBarPrivate();
    void             tryToRequestInfoBarEvent();
    WorkStatus       getWorkMode() const;
    Q_INVOKABLE void onOtherInfoBarEnd(QVariantMap eventData);
    Q_INVOKABLE void infoBarEnd(const QVariantMap& eventData);
    Q_SLOT void      onCloseButtonClicked();

  private:
    friend class NInfoBarManager;
    NThemeType::ThemeMode        _themeMode;
    int                          _borderRadius{6};
    QString                      _title{""};
    QString                      _message{""};
    NInfoBarType::PositionPolicy _position;
    NInfoBarType::InfoMode       _severity;
    qreal                        _createTime{0};

    int _leftPadding{20};
    int _iconSize{14};
    int _titleLeftSpacing{30};
    int _textLeftSpacing{15};
    int _closeButtonLeftRightMargin{20};
    int _closeButtonWidth{30};
    int _messageBarHorizontalMargin{20};
    int _messageBarVerticalBottomMargin{20};
    int _messageBarVerticalTopMargin{50};
    int _messageBarSpacing{15};
    int _shadowBorderWidth{6};

    bool            _isLongMessage{false};
    QList<QWidget*> _additionalWidgets;
    QBoxLayout*     _widgetsLayout{nullptr};

    bool             _isInfoBarCreateAnimationFinished{false};
    bool             _isReadyToEnd{false};
    bool             _isNormalDisplay{false};
    bool             _isInfoBarEventAnimationStart{false};
    NPushButton*     _closeButton{nullptr};
    bool             _showCloseButton{true};
    Q_INVOKABLE void _infoBarCreate(int displayMsec);

    void _calculateInitialPos(int& startX, int& startY, int& endX, int& endY);

    QList<int> _getOtherInfoBarTotalData(bool isJudgeCreateOrder = false);

    qreal _calculateTargetPosY();

    bool _judgeCreateOrder(NInfoBar* otherInfoBar);

    void _initializeInfoBar();

    void _calculateTextLayout();

    void _drawText(QPainter* painter);

    void _drawCommonBackground(QPainter* painter, const QColor& bgColor);
    void _drawIconWithCircle(QPainter* painter, NRegularIconType::Icon iconType, const QColor& circleBgColor);
    void _drawIconWithCircle(QPainter* painter, NFilledIconType::Icon iconType, const QColor& circleBgColor);
    void _drawSuccess(QPainter* painter);
    void _drawWarning(QPainter* painter);
    void _drawInformation(QPainter* painter);
    void _drawError(QPainter* painter);
};

#endif // QTNATIVEUI_NINFOBAR_P_H