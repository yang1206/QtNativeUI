#include "ncalendartitlemodel.h"

#include <qlocale.h>

NCalendarTitleModel::NCalendarTitleModel(QObject* parent) : QAbstractListModel{parent} {
    _firstDayOfWeek = QLocale::system().firstDayOfWeek();
}

NCalendarTitleModel::~NCalendarTitleModel() {}

int NCalendarTitleModel::rowCount(const QModelIndex& parent) const { return 7; }

QVariant NCalendarTitleModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::UserRole) {
        QLocale locale = QLocale::system();
        if (index.row() >= 0 && index.row() < 7) {
            return locale.dayName(index.row() == 0 ? 7 : index.row(), QLocale::NarrowFormat);
        }
    }
    return QVariant();
}