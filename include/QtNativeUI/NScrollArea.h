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


    void setUseCustomScrollBars(bool use);
    bool useCustomScrollBars() const;
    

    void enableTransparentBackground();
    

    void setViewportMargins(int left, int top, int right, int bottom);
    void setViewportMargins(int margins);
    

    QRect contentRect() const;

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    
  private:
    void initScrollBars();
    void updateScrollBars();
};

#endif // QTNATIVEUI_NSCROLLAREA_H