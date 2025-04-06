//
// Created by Yang1206 on 2025/4/6.
//

#ifndef NTOOLTIP_P_H
#define NTOOLTIP_P_H

#include <QObject>
#include <QPoint>
#include <QtNativeUI/NToolTip.h>
#include "QtNativeUI/NEnums.h"

class QGraphicsDropShadowEffect;
class QLabel;
class QHBoxLayout;
class QWidget;
class QPropertyAnimation;
class QTimer;

class NToolTipPositionManager {
  public:
    NToolTipPositionManager() {}
    virtual ~NToolTipPositionManager() {}

    QPoint position(NToolTip* tooltip, QWidget* parent);

    virtual QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) = 0;

    static NToolTipPositionManager* create(NToolTipPosition position);
};

class TopToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class BottomToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class LeftToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class RightToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class TopLeftToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class TopRightToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class BottomLeftToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class BottomRightToolTipManager : public NToolTipPositionManager {
  public:
    QPoint calculatePosition(NToolTip* tooltip, QWidget* parent) override;
};

class NToolTipPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NToolTip)

    Q_PROPERTY_CREATE_D(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_D(QColor, LightTextColor)
    Q_PROPERTY_CREATE_D(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_D(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_D(QColor, DarkBorderColor)

    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ContentPadding)
    Q_PROPERTY_CREATE_D(int, ShadowRadius)

  public:
    explicit NToolTipPrivate(QObject* parent = nullptr);
    ~NToolTipPrivate();

    QString text;
    int     duration = 1000;

    QWidget*     container       = nullptr;
    QLabel*      label           = nullptr;
    QHBoxLayout* containerLayout = nullptr;

    QPropertyAnimation*        opacityAnimation = nullptr;
    QGraphicsDropShadowEffect* shadowEffect     = nullptr;
    QTimer*                    timer            = nullptr;

    NThemeType::ThemeMode themeMode;
    bool                  isDark = false;

    void updateStyle();
};

#endif // NTOOLTIP_P_H