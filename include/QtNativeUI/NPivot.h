#ifndef QTNATIVEUI_NPIVOT_H
#define QTNATIVEUI_NPIVOT_H

#include <QIcon>
#include <QWidget>
#include "NIconEnums.h"
#include "stdafx.h"

namespace NPivotType {
enum Alignment { Left = 0, Center = 1, Right = 2 };
}

class NPivotPrivate;
class QTNATIVEUI_EXPORT NPivot : public QWidget {
    Q_OBJECT
    Q_Q_CREATE(NPivot)
    Q_PROPERTY(qreal indicatorX READ indicatorX WRITE setIndicatorX)
    Q_PROPERTY(qreal indicatorWidth READ indicatorWidth WRITE setIndicatorWidth)
    Q_PROPERTY_CREATE_Q_H(int, ItemSpacing)
    Q_PROPERTY_CREATE_Q_H(int, IconSpacing)
    Q_PROPERTY_CREATE_Q_H(int, IndicatorHeight)
    Q_PROPERTY_CREATE_Q_H(int, IndicatorRadius)
    Q_PROPERTY_CREATE_Q_H(int, AnimationDuration)
    Q_PROPERTY_CREATE_Q_H(int, BadgeSize)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColorSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColorSelected)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColorHover)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColorHover)
    Q_PROPERTY_CREATE_Q_H(QColor, LightTextColorDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkTextColorDisabled)
    Q_PROPERTY_CREATE_Q_H(QColor, LightIndicatorColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkIndicatorColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBadgeColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBadgeColor)
    Q_PROPERTY_CREATE_Q_H(QColor, LightBadgeTextColor)
    Q_PROPERTY_CREATE_Q_H(QColor, DarkBadgeTextColor)

  public:
    explicit NPivot(QWidget* parent = nullptr);
    ~NPivot();

    int  addItem(const QString& text);
    int  addItem(const QIcon& icon, const QString& text);
    int  addItem(NRegularIconType::Icon icon, const QString& text, int iconSize = 16);
    int  addItem(NFilledIconType::Icon icon, const QString& text, int iconSize = 16);
    int  insertItem(int index, const QString& text);
    int  insertItem(int index, const QIcon& icon, const QString& text);
    void removeItem(int index);
    void clear();

    int     count() const;
    int     currentIndex() const;
    QString itemText(int index) const;
    void    setItemText(int index, const QString& text);
    void    setItemIcon(int index, const QIcon& icon);
    void    setItemIcon(int index, NRegularIconType::Icon icon, int size = 16);
    void    setItemIcon(int index, NFilledIconType::Icon icon, int size = 16);
    QIcon   itemIcon(int index) const;
    void    setItemEnabled(int index, bool enabled);
    bool    isItemEnabled(int index) const;

    void setItemBadge(int index, int count);
    int  itemBadge(int index) const;
    void clearItemBadge(int index);

    void  setItemFont(const QFont& font);
    QFont itemFont() const;
    void  setSelectedItemFont(const QFont& font);
    QFont selectedItemFont() const;

    void                  setAlignment(NPivotType::Alignment alignment);
    NPivotType::Alignment alignment() const;

    void            addRightWidget(QWidget* widget, int spacing = 8);
    void            removeRightWidget(QWidget* widget);
    void            clearRightWidgets();
    QList<QWidget*> rightWidgets() const;

    qreal indicatorX() const;
    void  setIndicatorX(qreal x);
    qreal indicatorWidth() const;
    void  setIndicatorWidth(qreal width);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

  public slots:
    void setCurrentIndex(int index);

  signals:
    void currentIndexChanged(int index);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

  private:
    void  init();
    int   itemAtPos(const QPoint& pos) const;
    QRect itemRect(int index) const;
    QRect textRect(int index) const;
    int   itemWidth(int index) const;
    void  startIndicatorAnimation(int toIndex);
    void  updateRightWidgetsPosition();
    void  drawBadge(QPainter* painter, const QRect& itemRect, int badge);
    int   getAlignmentOffset() const;
};

#endif // QTNATIVEUI_NPIVOT_H
