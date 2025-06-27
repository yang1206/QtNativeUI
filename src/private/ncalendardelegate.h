#ifndef QTNATIVEUI_NCALENDARDELEGATE_H
#define QTNATIVEUI_NCALENDARDELEGATE_H

#include <QStyledItemDelegate>
#include "QtNativeUI/NCalendarWidget.h"
#include "QtNativeUI/NEnums.h"
#include "ncalendarmodel.h"

class NCalendarDelegate : public QStyledItemDelegate {
    Q_OBJECT
    Q_PROPERTY_CREATE(int, ItemWidth)
    Q_PROPERTY_CREATE(int, ItemHeight)
    Q_PROPERTY_CREATE(bool, IsTransparent)
  public:
    explicit NCalendarDelegate(NCalendarModel* calendarModel, QObject* parent = nullptr);
    ~NCalendarDelegate();

    Q_SLOT void onCalendarModelDisplayModeChanged();

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void  setSelectionMode(NCalendarWidget::DateSelectionMode mode);

  private:
    NCalendarModel*                    _calendarModel{nullptr};
    NThemeType::ThemeMode              _themeMode;
    bool                               _isDark{false};
    QDate                              _nowDate;
    NCalendarWidget::DateSelectionMode _selectionMode{NCalendarWidget::SingleDate};
    void _drawYearOrMonth(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void _drawDays(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif // QTNATIVEUI_NCALENDARDELEGATE_H