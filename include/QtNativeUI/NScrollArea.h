#ifndef QTNATIVEUI_NSCROLLAREA_H
#define QTNATIVEUI_NSCROLLAREA_H

#include <QScrollArea>
#include "stdafx.h"

class NScrollAreaPrivate;
class QTNATIVEUI_EXPORT NScrollArea : public QScrollArea {
    Q_OBJECT
    Q_Q_CREATE(NScrollArea)

  public:
    explicit NScrollArea(QWidget* parent = nullptr);
    ~NScrollArea();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    // 私有实现
};

#endif // QTNATIVEUI_NSCROLLAREA_H