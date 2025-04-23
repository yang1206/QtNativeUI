#ifndef QTNATIVEUI_NCALENDARMODEL_H
#define QTNATIVEUI_NCALENDARMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QMetaType>
#include "QtNativeUI/stdafx.h"

enum NCalendarType {
    YearMode  = 0x0001,
    MonthMode = 0x0002,
    DayMode   = 0x0003,
};

struct NCalendarData {
  public:
    NCalendarData() {};
    ~NCalendarData() {};
    NCalendarData(int year, int month, int day, QString desText = "")
        : year(year), month(month), day(day), desText(desText) {};
    NCalendarData(const NCalendarData& other) {
        year    = other.year;
        month   = other.month;
        day     = other.day;
        desText = other.desText;
    }
    int     year  = 1924;
    int     month = 1;
    int     day   = 1;
    QString desText{""};
};
Q_DECLARE_METATYPE(NCalendarData);

class NCalendarModel : public QAbstractListModel {
    Q_OBJECT
    Q_PRIVATE_CREATE_Q_H(QDate, MinimumDate)
    Q_PRIVATE_CREATE_Q_H(QDate, MaximumDate)
  public:
    explicit NCalendarModel(QObject* parent = nullptr);
    ~NCalendarModel();

    void          setDisplayMode(NCalendarType displayType);
    NCalendarType getDisplayMode() const;

    void setFirstDayOfWeek(int day);
    int  firstDayOfWeek() const;
    void setLocale(const QLocale& locale);

    void invalidate() {
        beginResetModel();
        endResetModel();
    }

    QModelIndex      getIndexFromDate(QDate date);
    QDate            getDateFromIndex(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role) const override;

  Q_SIGNALS:
    void currentYearMonthChanged(QString date);
    void displayModeChanged();

  protected:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  private:
    int           _offset{0};
    NCalendarType _displayMode{NCalendarType::DayMode};
    QLocale       _locale{QLocale::system()};
    int           _dayRowCount{0};
    int           _firstDayOfWeek{1};
    QDate         _pMinimumDate;
    QDate         _pMaximumDate;
    void          _initRowCount();
    int           _getCurrentDay(int row) const;
};

#endif // QTNATIVEUI_NCALENDARMODEL_H