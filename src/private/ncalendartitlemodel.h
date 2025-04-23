#ifndef QTNATIVEUI_NCALENDARTITLEMODEL_H
#define QTNATIVEUI_NCALENDARTITLEMODEL_H

#include <QAbstractListModel>
#include <qlocale.h>

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

#endif // QTNATIVEUI_NCALENDARTITLEMODEL_H