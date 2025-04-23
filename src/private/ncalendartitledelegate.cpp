#include "ncalendartitledelegate.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include "QtNativeUI/NTheme.h"

NCalendarTitleDelegate::NCalendarTitleDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = nTheme->themeMode();
    _isDark = nTheme->isDarkMode();

    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        _isDark = nTheme->isDarkMode();
    });
}

NCalendarTitleDelegate::~NCalendarTitleDelegate()
{
}

void NCalendarTitleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRectF itemRect = option.rect;

    // 文字绘制
    QString title = index.data(Qt::UserRole).toString();
    if (!title.isEmpty())
    {
        QColor textColor = _isDark ?
            NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark) :
            NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);

        painter->setPen(textColor);
        QFont font = painter->font();
        font.setWeight(QFont::Bold);
        painter->setFont(font);
        painter->drawText(itemRect, Qt::AlignCenter, title);
    }
    painter->restore();
}

QSize NCalendarTitleDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(42, 30);
}