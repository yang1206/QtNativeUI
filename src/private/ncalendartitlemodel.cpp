#include "ncalendartitlemodel.h"

#include <qlocale.h>

NCalendarTitleModel::NCalendarTitleModel(QObject* parent) : QAbstractListModel{parent} {
    _firstDayOfWeek = _locale.firstDayOfWeek();
}

NCalendarTitleModel::~NCalendarTitleModel() {}

void NCalendarTitleModel::setLocale(const QLocale& locale) {
    if (_locale != locale) {
        beginResetModel();
        _locale = locale;
        endResetModel();
    }
}

int NCalendarTitleModel::rowCount(const QModelIndex& parent) const { return 7; }

QVariant NCalendarTitleModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::UserRole) {
        if (index.row() >= 0 && index.row() < 7) {
            return _locale.dayName(index.row() == 0 ? 7 : index.row(), QLocale::NarrowFormat);
        }
    }
    return QVariant();
}