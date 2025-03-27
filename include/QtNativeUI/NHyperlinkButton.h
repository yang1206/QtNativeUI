#ifndef QTNATIVEUI_NHYPERLINKBUTTON_H
#define QTNATIVEUI_NHYPERLINKBUTTON_H

#include <QPushButton>
#include "stdafx.h"

class NHyperlinkButtonPrivate;
class QTNATIVEUI_EXPORT NHyperlinkButton : public QPushButton {
    Q_OBJECT
    Q_Q_CREATE(NHyperlinkButton)
    Q_PROPERTY_CREATE_Q_H(QColor, DisabledColor)
    Q_PROPERTY_CREATE_Q_H(QColor, BackgroundHoverColor)
    Q_PROPERTY_CREATE_Q_H(QColor, BackgroundPressColor)

public:
    explicit NHyperlinkButton(QWidget* parent = nullptr);
    explicit NHyperlinkButton(QString text, QWidget* parent = nullptr);
    ~NHyperlinkButton();

    void setUrl(const QString& url);
    void setUrl(const QUrl& url);
    QUrl url() const;

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void changeEvent(QEvent* event) override;

signals:
    void linkActivated(const QUrl& url);

private:
    void drawText(QPainter* painter);
};

#endif // QTNATIVEUI_NHYPERLINKBUTTON_H