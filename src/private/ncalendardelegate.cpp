#include "ncalendardelegate.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include "QtNativeUI/NTheme.h"

NCalendarDelegate::NCalendarDelegate(NCalendarModel* calendarModel, QObject* parent) : QStyledItemDelegate{parent} {
    _pIsTransparent = false;
    _calendarModel  = calendarModel;
    _pItemWidth     = 42;
    _pItemHeight    = 42;
    _nowDate        = QDate::currentDate();
    _themeMode      = nTheme->themeMode();
    _isDark         = nTheme->isDarkMode();

    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        _isDark    = nTheme->isDarkMode();
    });

    connect(_calendarModel,
            &NCalendarModel::displayModeChanged,
            this,
            &NCalendarDelegate::onCalendarModelDisplayModeChanged);
}

NCalendarDelegate::~NCalendarDelegate() {}

void NCalendarDelegate::onCalendarModelDisplayModeChanged() {
    switch (_calendarModel->getDisplayMode()) {
        case YearMode:
        case MonthMode: {
            _pItemWidth  = 53;
            _pItemHeight = 60;
            break;
        }
        case DayMode: {
            _pItemWidth  = 42;
            _pItemHeight = 42;
            break;
        }
    }
}

void NCalendarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (_pIsTransparent) {
        return;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    NCalendarType displayModel = _calendarModel->getDisplayMode();
    switch (displayModel) {
        case YearMode:
        case MonthMode: {
            _drawYearOrMonth(painter, option, index);
            break;
        }
        case DayMode: {
            _drawDays(painter, option, index);
            break;
        }
    }
    painter->restore();
}

QSize NCalendarDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    return QSize(_pItemWidth, _pItemHeight);
}

void NCalendarDelegate::_drawYearOrMonth(QPainter*                   painter,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex&          index) const {
    QRectF        itemRect   = option.rect;
    bool          isNow      = false;
    NCalendarData data       = index.data(Qt::UserRole).value<NCalendarData>();
    qreal         baseRadius = _pItemWidth * 0.5;

    // 当前日期绘制
    NCalendarType displayMode = _calendarModel->getDisplayMode();
    if ((displayMode == NCalendarType::YearMode && data.year == _nowDate.year()) ||
        (displayMode == NCalendarType::MonthMode && data.month == _nowDate.month() && data.year == _nowDate.year())) {
        isNow              = true;
        QColor accentColor = nTheme->accentColor().normal();
        QColor drawColor   = option.state.testFlag(QStyle::State_MouseOver) ? accentColor.lighter(110) : accentColor;

        painter->setPen(Qt::NoPen);
        painter->setBrush(drawColor);
        painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
    } else {
        // 悬停效果绘制
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setPen(Qt::NoPen);
            QColor hoverColor = _isDark ? NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark)
                                        : NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
            painter->setBrush(hoverColor);
            painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
        }
    }

    // 文字绘制
    QColor textColor;
    if (isNow) {
        textColor = _isDark ? NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Dark)
                            : NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Light);
    } else {
        textColor = _isDark ? NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark)
                            : NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    }

    painter->setPen(textColor);
    painter->drawText(itemRect,
                      Qt::AlignCenter,
                      displayMode == NCalendarType::YearMode ? QString::number(data.year)
                                                             : QString::number(data.month));

    QString desText = data.desText;
    if (!desText.isEmpty()) {
        itemRect.adjust(0, 10, 0, 0);
        QFont font = painter->font();
        font.setPixelSize(9);
        painter->setFont(font);
        painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
    }
}

void NCalendarDelegate::_drawDays(QPainter*                   painter,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex&          index) const {
    // TODO 不同模式选择绘制需要不同效果
    qreal    penWidth   = 1.5;
    qreal    baseRadius = _pItemWidth * 0.5 - penWidth;
    QVariant variant    = index.data(Qt::UserRole);

    if (variant.isValid()) {
        QRectF        itemRect = option.rect;
        bool          isNow    = false;
        NCalendarData data     = variant.value<NCalendarData>();

        // 当前日期绘制
        if (data.year == _nowDate.year() && data.month == _nowDate.month() && data.day == _nowDate.day()) {
            isNow              = true;
            QColor accentColor = nTheme->accentColor().normal();
            QColor drawColor = option.state.testFlag(QStyle::State_MouseOver) ? accentColor.lighter(110) : accentColor;

            if (option.state.testFlag(QStyle::State_Selected)) {
                painter->setPen(QPen(drawColor, penWidth));
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
                painter->setBrush(drawColor);
                painter->setPen(Qt::NoPen);
                painter->drawEllipse(itemRect.center(), baseRadius - 2 * penWidth, baseRadius - 2 * penWidth);
            } else {
                painter->setPen(Qt::NoPen);
                painter->setBrush(drawColor);
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
            }
        } else {
            // 选中和悬停效果绘制
            if (option.state.testFlag(QStyle::State_Selected)) {
                QColor accentColor = nTheme->accentColor().normal();
                QColor borderColor =
                    option.state.testFlag(QStyle::State_MouseOver) ? accentColor.lighter(110) : accentColor;

                painter->setPen(QPen(borderColor, penWidth));
                painter->setBrush(Qt::NoBrush);
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
            }

            if (option.state.testFlag(QStyle::State_MouseOver)) {
                painter->setPen(Qt::NoPen);
                QColor hoverColor = _isDark ? NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark)
                                            : NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);

                painter->setBrush(hoverColor);

                if (option.state.testFlag(QStyle::State_Selected)) {
                    painter->drawEllipse(itemRect.center(), baseRadius - penWidth, baseRadius - penWidth);
                } else {
                    painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
                }
            }
        }

        // 文字绘制
        QColor textColor;
        bool   isEnabled = option.state & QStyle::State_Enabled;

        if (isNow) {
            textColor = _isDark ? NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Dark)
                                : NThemeColor(NFluentColorKey::TextOnAccentFillColorPrimary, NThemeType::Light);
        } else if (!isEnabled) {
            textColor = _isDark ? NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark)
                                : NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
        } else {
            textColor = _isDark ? NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark)
                                : NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
        }

        painter->setPen(textColor);
        painter->drawText(itemRect, Qt::AlignCenter, QString::number(data.day));

        QString desText = data.desText;
        if (!desText.isEmpty()) {
            itemRect.adjust(0, 2 * penWidth, 0, 0);
            QFont font = painter->font();
            font.setPixelSize(9);
            painter->setFont(font);
            painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
        }
    }
}

void NCalendarDelegate::setSelectionMode(NCalendarWidget::DateSelectionMode mode) { _selectionMode = mode; }
