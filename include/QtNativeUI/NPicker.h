#ifndef QTNATIVEUI_NPICKER_H
#define QTNATIVEUI_NPICKER_H

#include <QWidget>
#include "stdafx.h"

class NPickerPrivate;
/**
 * @brief 滚轮选择器组件
 */
class QTNATIVEUI_EXPORT NPicker : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NPicker)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QStringList, Items)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, VisibleItemCount)
    Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
    Q_PROPERTY_CREATE_Q_H(bool, Container)
    Q_PROPERTY_CREATE_Q_H(bool, LoopEnabled)

  public:
    explicit NPicker(QWidget* parent = nullptr);
    ~NPicker() override;

    void    setCurrentData(const QString& data);
    QString getCurrentData() const;

  signals:
    void currentDataChanged(const QString& data);

  protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif // QTNATIVEUI_NPICKER_H
