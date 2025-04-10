#ifndef QTNATIVEUI_NTABWIDGET_H
#define QTNATIVEUI_NTABWIDGET_H

#include <QWidget>
#include "stdafx.h"

class NTabWidgetPrivate;
class QTNATIVEUI_EXPORT NTabWidget : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NTabWidget)

    // 属性声明
    // ...

  public:
    explicit NTabWidget(QWidget* parent = nullptr);
    ~NTabWidget();

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
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event) override;
    // ...

  private:
    void init();
    // 其他私有辅助方法
    // ...
};

#endif // QTNATIVEUI_NTABWIDGET_H