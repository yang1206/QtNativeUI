#ifndef QTNATIVEUI_NPROGRESSBAR_H
#define QTNATIVEUI_NPROGRESSBAR_H

#include <QProgressBar>
#include "stdafx.h"

class NProgressBarPrivate;
class QTNATIVEUI_EXPORT NProgressBar : public QProgressBar {
    Q_OBJECT
    Q_Q_CREATE(NProgressBar)

  public:
    explicit NProgressBar(QWidget* parent = nullptr);
    ~NProgressBar();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    // 私有实现
};

#endif // QTNATIVEUI_NPROGRESSBAR_H