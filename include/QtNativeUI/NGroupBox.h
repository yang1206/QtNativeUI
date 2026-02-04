#ifndef QTNATIVEUI_NGROUPBOX_H
#define QTNATIVEUI_NGROUPBOX_H

#include <QWidget>
#include "NIconEnums.h"
#include "stdafx.h"

class NGroupBoxPrivate;
class QTNATIVEUI_EXPORT NGroupBox : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NGroupBox)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool flat READ isFlat WRITE setFlat)
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled USER true)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTitleColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTitleColor)
    Q_PROPERTY_CREATE_Q_H(int, TitleSpacing)
    Q_PROPERTY_CREATE_Q_H(bool, ShowBorder)
    Q_PROPERTY_CREATE_Q_H(int, ContentMargin)
    Q_PROPERTY_CREATE_Q_H(int, TitleHeight)

  public:
    enum GroupBoxStyle { Standard, Card, Outlined };
    Q_ENUM(GroupBoxStyle)

    explicit NGroupBox(QWidget* parent = nullptr);
    explicit NGroupBox(const QString& title, QWidget* parent = nullptr);
    ~NGroupBox();

    void          setGroupBoxStyle(GroupBoxStyle style);
    GroupBoxStyle groupBoxStyle() const;

    void    setTitle(const QString& title);
    QString title() const;

    void          setAlignment(Qt::Alignment alignment);
    Qt::Alignment alignment() const;

    void setFlat(bool flat);
    bool isFlat() const;

    void setCheckable(bool checkable);
    bool isCheckable() const;

    void setChecked(bool checked);
    bool isChecked() const;

    void setTitleIcon(NRegularIconType::Icon icon, int size = 16);
    void setTitleIcon(NFilledIconType::Icon icon, int size = 16);
    void clearTitleIcon();

  signals:
    void toggled(bool checked);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void changeEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void drawBackground(QPainter* painter);
    void drawBorder(QPainter* painter);
    void drawTitle(QPainter* painter);

    QRect titleRect() const;
    QRect contentRect() const;

  private:
    void init();
    void updateLayout();
    void updateTitleIcon();
    void updateChildrenEnabled();
};
#endif // QTNATIVEUI_NGROUPBOX_H