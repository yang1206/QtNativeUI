#ifndef QTNATIVEUI_NTABBAR_H
#define QTNATIVEUI_NTABBAR_H

#include <QWidget>
#include "stdafx.h"

class NTabBarPrivate;
class QTNATIVEUI_EXPORT NTabBar : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NTabBar)

    // 属性声明
    // ...

  public:
    explicit NTabBar(QWidget* parent = nullptr);
    ~NTabBar();

    // 公共方法
    // ...

  public slots:
    // 槽函数
    // ...

  signals:
    // 信号
    // ...

  protected:
    // 保护方法和事件处理
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;
    // ...

  private:
    void init();
    // 其他私有辅助方法
    // ...
};

#endif // QTNATIVEUI_NTABBAR_H