//
// Created by Yang1206 on 2025/4/24.
//

#include "ncalendarheader.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <qlocale.h>
#include "QtNativeUI/NTheme.h"

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

NCalendarTitleDelegate::NCalendarTitleDelegate(QObject* parent) : QStyledItemDelegate{parent} {
    _themeMode = nTheme->themeMode();
    _isDark    = nTheme->isDarkMode();

    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        _isDark    = nTheme->isDarkMode();
    });
}

NCalendarTitleDelegate::~NCalendarTitleDelegate() {}

void NCalendarTitleDelegate::paint(QPainter*                   painter,
                                   const QStyleOptionViewItem& option,
                                   const QModelIndex&          index) const {
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRectF itemRect = option.rect;

    QString title = index.data(Qt::UserRole).toString();
    if (!title.isEmpty()) {
        QColor textColor = _isDark ? NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark)
                                   : NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);

        painter->setPen(textColor);
        QFont font = painter->font();
        font.setWeight(QFont::Bold);
        painter->setFont(font);
        painter->drawText(itemRect, Qt::AlignCenter, title);
    }
    painter->restore();
}

QSize NCalendarTitleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    return QSize(42, 30);
}