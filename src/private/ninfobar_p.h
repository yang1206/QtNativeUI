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
    // 请求事件堆栈调用
    void requestInfoBarEvent(NInfoBar* infoBar);
    // 发布创建事件
    void postInfoBarCreateEvent(NInfoBar* infoBar);
    // 发布终止事件
    void postInfoBarEndEvent(NInfoBar* infoBar);
    // 强制发布终止事件
    void forcePostInfoBarEndEvent(NInfoBar* infoBar);
    // 获取当前事件数量
    int getInfoBarEventCount(NInfoBar* infoBar);
    // 更新活动序列
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

    // 位置数据
    int _leftPadding{20}; // 左边框到图标中心
    int _iconSize{12};
    int _titleLeftSpacing{30};           // 图标中心到Title左侧
    int _textLeftSpacing{15};            // Title右侧到Text左侧
    int _closeButtonLeftRightMargin{20}; // closeButton左右总Margin
    int _closeButtonWidth{30};
    int _messageBarHorizontalMargin{20};
    int _messageBarVerticalBottomMargin{20};
    int _messageBarVerticalTopMargin{50};
    int _messageBarSpacing{15};
    int _shadowBorderWidth{6};

    // 布局相关
    bool            _isLongMessage{false};   // 是否为长消息需要换行显示
    QList<QWidget*> _additionalWidgets;      // 额外添加的控件
    QBoxLayout*     _widgetsLayout{nullptr}; // 用于放置额外控件的布局

    // 逻辑数据
    bool             _isInfoBarCreateAnimationFinished{false};
    bool             _isReadyToEnd{false};
    bool             _isNormalDisplay{false};
    bool             _isInfoBarEventAnimationStart{false};
    NPushButton*     _closeButton{nullptr};
    bool             _showCloseButton{true};
    Q_INVOKABLE void _infoBarCreate(int displayMsec);

    // 初始坐标计算
    void _calculateInitialPos(int& startX, int& startY, int& endX, int& endY);
    // 获取总高度和次序信息
    QList<int> _getOtherInfoBarTotalData(bool isJudgeCreateOrder = false);
    // 计算目标坐标
    qreal _calculateTargetPosY();

    // 创建次序判断
    bool _judgeCreateOrder(NInfoBar* otherInfoBar);
    // 初始化普通InfoBar widget
    void _initializeInfoBar();

    // 计算文本布局
    void _calculateTextLayout();

    // 绘制文本
    void _drawText(QPainter* painter);

    // 绘制函数
    void _drawCommonBackground(QPainter* painter, const QColor& bgColor);
    void _drawIconWithCircle(QPainter* painter, NRegularIconType::Icon iconType, const QColor& circleBgColor);
    void _drawSuccess(QPainter* painter);
    void _drawWarning(QPainter* painter);
    void _drawInformation(QPainter* painter);
    void _drawError(QPainter* painter);
};

#endif // QTNATIVEUI_NINFOBAR_P_H