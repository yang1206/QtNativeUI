#ifndef QTNATIVEUI_NEXPANDER_H
#define QTNATIVEUI_NEXPANDER_H

#include <QWidget>
#include "NIconEnums.h"
#include "stdafx.h"

class NExpanderPrivate;
class QTNATIVEUI_EXPORT NExpander : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NExpander)
    Q_PROPERTY(QString header READ header WRITE setHeader)
    Q_PROPERTY(bool expanded READ isExpanded WRITE setExpanded NOTIFY expandedChanged)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBackgroundColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBorderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightHeaderColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkHeaderColor)
    Q_PROPERTY_CREATE_Q_H(int, HeaderHeight)
    Q_PROPERTY_CREATE_Q_H(int, ContentPadding)
    Q_PROPERTY_CREATE_Q_H(int, AnimationDuration)

  public:
    explicit NExpander(QWidget* parent = nullptr);
    explicit NExpander(const QString& header, QWidget* parent = nullptr);
    ~NExpander();

    void    setHeader(const QString& header);
    QString header() const;

    void setExpanded(bool expanded);
    bool isExpanded() const;

    void setHeaderIcon(NRegularIconType::Icon icon, int size = 16);
    void setHeaderIcon(NFilledIconType::Icon icon, int size = 16);
    void clearHeaderIcon();

    QWidget* contentWidget() const;

  signals:
    void expandedChanged(bool expanded);

  protected:
    void resizeEvent(QResizeEvent* event) override;

  private:
    void init();
};

#endif // QTNATIVEUI_NEXPANDER_H
