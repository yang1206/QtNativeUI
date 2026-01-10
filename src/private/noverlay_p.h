#ifndef QTNATIVEUI_NOVERLAY_P_H
#define QTNATIVEUI_NOVERLAY_P_H

#include <QColor>
#include <QObject>
#include <QPointer>
#include "QtNativeUI/stdafx.h"

class NOverlay;

class NOverlayPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NOverlay)
    Q_PROPERTY_CREATE_D(QColor, LightOverlayColor)
    Q_PROPERTY_CREATE_D(QColor, DarkOverlayColor)
    Q_PROPERTY_CREATE_D(int, Opacity)
    Q_PROPERTY_CREATE_D(bool, ClickToClose)
    Q_PROPERTY_CREATE_D(int, AnimationDuration)
    Q_PROPERTY_CREATE(int, CurrentOpacity)

  public:
    explicit NOverlayPrivate(QObject* parent = nullptr);
    ~NOverlayPrivate();

    void   init();
    void   doAnimation(int endValue);
    void   updateContentPosition();
    QColor currentOverlayColor() const;
    QWidget* createLoadingWidget(const QString& text);

    QPointer<QWidget> _contentWidget;
    QPointer<QWidget> _loadingWidget;
    bool              _isAnimating{false};
    bool              _closeAfterAnimation{false};
};

#endif // QTNATIVEUI_NOVERLAY_P_H