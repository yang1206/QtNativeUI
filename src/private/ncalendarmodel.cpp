#include "ncalendarmodel.h"

NCalendarModel::NCalendarModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _pMinimumDate.setDate(1924, 1, 1);
    _pMaximumDate.setDate(2124, 12, 31);
    _initRowCount();
}

NCalendarModel::~NCalendarModel()
{
}

void NCalendarModel::setMinimumDate(QDate minimudate)
{
    _pMinimumDate = minimudate;
    _initRowCount();
}

QDate NCalendarModel::getMinimumDate() const
{
    return _pMinimumDate;
}

void NCalendarModel::setMaximumDate(QDate maximumDate)
{
    _pMaximumDate = maximumDate;
    _initRowCount();
}

QDate NCalendarModel::getMaximumDate() const
{
    return _pMaximumDate;
}

void NCalendarModel::setDisplayMode(NCalendarType displayMode)
{
    beginResetModel();
    _displayMode = displayMode;
    endResetModel();
    Q_EMIT displayModeChanged();
}

NCalendarType NCalendarModel::getDisplayMode() const
{
    return _displayMode;
}

QModelIndex NCalendarModel::getIndexFromDate(QDate date)
{
    switch (_displayMode)
    {
    case YearMode:
    {
        return this->index(date.year() - _pMinimumDate.year());
    }
    case MonthMode:
    {
        return this->index((date.year() - _pMinimumDate.year()) * 12 + date.month() - 1);
    }
    case DayMode:
    {
        return this->index(_pMinimumDate.daysTo(date) + _offset);
    }
    }
    return QModelIndex();
}

QDate NCalendarModel::getDateFromIndex(const QModelIndex& index) const
{
    if (!index.isValid() || index.row() < _offset)
    {
        return QDate();
    }
    return _pMinimumDate.addDays(index.row() - _offset);
}

QVariant NCalendarModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        switch (_displayMode)
        {
        case YearMode:
        {
            return QVariant::fromValue<NCalendarData>(NCalendarData(_pMinimumDate.year() + index.row(), 1, 1));
        }
        case MonthMode:
        {
            int year = _pMinimumDate.year() + index.row() / 12;
            int month = index.row() % 12 + 1;
            if (month == 1)
            {
                return QVariant::fromValue<NCalendarData>(NCalendarData(year, month, 1, QString::number(year)));
            }
            else
            {
                return QVariant::fromValue<NCalendarData>(NCalendarData(_pMinimumDate.year() + index.row() / 12, month, 1));
            }
        }
        case DayMode:
        {
            if (index.row() >= _offset)
            {
                QDate date = getDateFromIndex(index);
                if (date.day() == 1)
                {
                    return QVariant::fromValue<NCalendarData>(NCalendarData(date.year(), date.month(), date.day(), QString("%1月").arg(date.month())));
                }
                else
                {
                    return QVariant::fromValue<NCalendarData>(NCalendarData(date.year(), date.month(), date.day()));
                }
            }
        }
        }
    }
    return QVariant();
}

int NCalendarModel::rowCount(const QModelIndex& parent) const
{
    switch (_displayMode)
    {
    case YearMode:
    {
        return _pMaximumDate.year() - _pMinimumDate.year() + 1;
    }
    case MonthMode:
    {
        return (_pMaximumDate.year() - _pMinimumDate.year() + 1) * 12;
    }
    case DayMode:
    {
        return _dayRowCount;
    }
    }
    return 0;
}

void NCalendarModel::_initRowCount()
{
    _dayRowCount = _pMinimumDate.daysTo(_pMaximumDate);
    _offset = _pMinimumDate.dayOfWeek();
    _dayRowCount += _offset + 1;
}

int NCalendarModel::_getCurrentDay(int row) const
{
    return _pMinimumDate.addDays(row - _offset).day();
}