//
// Created by Yang1206 on 2025/4/6.
//

#ifndef NTOOLTIP_H
#define NTOOLTIP_H

#include <QFrame>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include "NColor.h"
#include "stdafx.h"

class QGraphicsDropShadowEffect;
class QHBoxLayout;
class QLabel;
class QWidget;

enum class NToolTipPosition { TOP = 0, BOTTOM, LEFT, RIGHT, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT };

class NToolTipPrivate;
class QTNATIVEUI_EXPORT NToolTip : public QFrame {
    Q_OBJECT
    Q_Q_CREATE(NToolTip)

    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)

    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, ContentPadding)
    Q_PROPERTY_CREATE_Q_H(int, ShadowRadius)

    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(int duration READ duration WRITE setDuration)

  public:
    explicit NToolTip(const QString& text = "", QWidget* parent = nullptr);
    ~NToolTip();

    QString text() const;
    void    setText(const QString& text);

    int  duration() const;
    void setDuration(int duration);

    void adjustPosition(QWidget* widget, NToolTipPosition position = NToolTipPosition::TOP);

    static NToolTip* createToolTip(const QString& text, QWidget* parent = nullptr);
    static void      showText(const QString& text, QWidget* parent, NToolTipPosition position = NToolTipPosition::TOP);

  protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

  private:
    void     init();
    QWidget* createContainer();
};

class QTNATIVEUI_EXPORT NToolTipFilter : public QObject {
    Q_OBJECT

  public:
    explicit NToolTipFilter(QWidget* parent, int showDelay = 300, NToolTipPosition position = NToolTipPosition::TOP);
    ~NToolTipFilter();

    void hideToolTip();
    void showToolTip();
    void setShowDelay(int delay);
    void setPosition(NToolTipPosition position);

  protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    NToolTip* createToolTip();
    bool      canShowToolTip() const;

    bool             isEnter      = false;
    NToolTip*        tooltip      = nullptr;
    int              tooltipDelay = 300;
    NToolTipPosition position;
    QTimer*          timer = nullptr;
};

#endif // NTOOLTIP_H