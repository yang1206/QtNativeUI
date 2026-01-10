#ifndef QTNATIVEUI_NOVERLAY_H
#define QTNATIVEUI_NOVERLAY_H

#include <QWidget>

#include "stdafx.h"

class NOverlayPrivate;

/**
 * @brief 遮罩层组件，用于加载状态、模态背景等场景
 */
class QTNATIVEUI_EXPORT NOverlay : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NOverlay)
    Q_PROPERTY_CREATE_Q_H(QColor, LightOverlayColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkOverlayColor)
    Q_PROPERTY_CREATE_Q_H(int, Opacity)
    Q_PROPERTY_CREATE_Q_H(bool, ClickToClose)
    Q_PROPERTY_CREATE_Q_H(int, AnimationDuration)

public:
    explicit NOverlay(QWidget* parent = nullptr);
    ~NOverlay();

    void     setContentWidget(QWidget* widget);
    QWidget* contentWidget() const;

    void showOverlay();
    void showOverlay(QWidget* parent);
    void hideOverlay();
    void close();

    void showLoading(const QString& text = "加载中...");
    void showLoading(QWidget* parent, const QString& text = "加载中...");
    void hideLoading();

    bool isAnimating() const;

Q_SIGNALS:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
};

#endif // QTNATIVEUI_NOVERLAY_H