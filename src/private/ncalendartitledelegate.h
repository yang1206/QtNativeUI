#ifndef QTNATIVEUI_NCALENDARTITLEDELEGATE_H
#define QTNATIVEUI_NCALENDARTITLEDELEGATE_H

#include <QStyledItemDelegate>
#include "QtNativeUI/NEnums.h"

class NCalendarTitleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NCalendarTitleDelegate(QObject* parent = nullptr);
    ~NCalendarTitleDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    NThemeType::ThemeMode _themeMode;
    bool _isDark{false};
};

#endif // QTNATIVEUI_NCALENDARTITLEDELEGATE_H