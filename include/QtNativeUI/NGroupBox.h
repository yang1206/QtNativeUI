#ifndef QTNATIVEUI_NGROUPBOX_H
#define QTNATIVEUI_NGROUPBOX_H

#include <QGroupBox>
#include "NIconEnums.h"
#include "stdafx.h"

class NGroupBoxPrivate;
class QTNATIVEUI_EXPORT NGroupBox : public QGroupBox {
    Q_OBJECT
    Q_Q_CREATE(NGroupBox)
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
    Q_PROPERTY_CREATE_Q_H(int, CollapseIndicatorSize)

  public:
    enum GroupBoxStyle {
        Standard,    // 标准样式
        Card,        // 卡片样式
        Outlined     // 轮廓样式
    };
    Q_ENUM(GroupBoxStyle)

    explicit NGroupBox(QWidget* parent = nullptr);
    explicit NGroupBox(const QString& title, QWidget* parent = nullptr);
    ~NGroupBox();

    void setGroupBoxStyle(GroupBoxStyle style);
    GroupBoxStyle groupBoxStyle() const;

    void setTitleIcon(NRegularIconType::Icon icon, int size = 16);
    void setTitleIcon(NFilledIconType::Icon icon, int size = 16);
    void clearTitleIcon();

    void setExpandedIcon(NRegularIconType::Icon icon);
    void setExpandedIcon(NFilledIconType::Icon icon);
    void setCollapsedIcon(NRegularIconType::Icon icon);
    void setCollapsedIcon(NFilledIconType::Icon icon);

    void setCollapsible(bool collapsible);
    bool isCollapsible() const;

    void setCollapsed(bool collapsed);
    bool isCollapsed() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

  signals:
    void collapsedChanged(bool collapsed);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void changeEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void drawBackground(QPainter* painter);
    void drawBorder(QPainter* painter);
    void drawTitle(QPainter* painter);
    void drawCollapseIndicator(QPainter* painter);

    QRect titleRect() const;
    QRect contentRect() const;

  private:
    void init();
    void updateLayout();
    void updateTitleIcon();
};

#endif // QTNATIVEUI_NGROUPBOX_H