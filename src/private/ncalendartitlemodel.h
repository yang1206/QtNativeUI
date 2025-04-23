#ifndef QTNATIVEUI_NCALENDARTITLEMODEL_H
#define QTNATIVEUI_NCALENDARTITLEMODEL_H

#include <QAbstractListModel>

class NCalendarTitleModel : public QAbstractListModel {
    Q_OBJECT
  public:
    explicit NCalendarTitleModel(QObject* parent = nullptr);
    ~NCalendarTitleModel();

    void setFirstDayOfWeek(int day) { _firstDayOfWeek = day; }

  protected:
    virtual int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

  private:
    int _firstDayOfWeek{1};
};

#endif // QTNATIVEUI_NCALENDARTITLEMODEL_H