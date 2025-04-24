//
// Created by Yang1206 on 2025/4/24.
//

#ifndef NCALENDARHEADER_H
#define NCALENDARHEADER_H

#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <qlocale.h>
#include "QtNativeUI/NEnums.h"

class NCalendarTitleModel : public QAbstractListModel {
    Q_OBJECT
  public:
    explicit NCalendarTitleModel(QObject* parent = nullptr);
    ~NCalendarTitleModel();

    void setFirstDayOfWeek(int day) { _firstDayOfWeek = day; }
    void setLocale(const QLocale& locale);

  protected:
    virtual int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

  private:
    int     _firstDayOfWeek{1};
    QLocale _locale{QLocale::system()};
};

class NCalendarTitleDelegate : public QStyledItemDelegate {
    Q_OBJECT
  public:
    explicit NCalendarTitleDelegate(QObject* parent = nullptr);
    ~NCalendarTitleDelegate();

    void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  private:
    NThemeType::ThemeMode _themeMode;
    bool                  _isDark{false};
};

#endif // NCALENDARHEADER_H
