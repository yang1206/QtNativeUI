#include <QtNativeUI/NCalendarWidget.h>
#include "../private/ncalendarwidget_p.h"
#include "../private/ncalendarwidgetstyle.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkBackgroundColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightHeaderBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkHeaderBackgroundColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightDateTextColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkDateTextColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightDateTextDisabledColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkDateTextDisabledColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightDateBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkDateBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightDateBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkDateBackgroundHoverColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightDateBackgroundSelectedColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkDateBackgroundSelectedColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightHeaderTextColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkHeaderTextColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightWeekdayTextColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkWeekdayTextColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightWeekendTextColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkWeekendTextColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, LightTodayBackgroundColor)
Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, QColor, DarkTodayBackgroundColor)

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, int, BorderRadius)

NCalendarWidget::NCalendarWidget(QWidget* parent) 
    : QCalendarWidget(parent), d_ptr(new NCalendarWidgetPrivate()) 
{
    init();
}

NCalendarWidget::~NCalendarWidget() {}

void NCalendarWidget::init() {
    Q_D(NCalendarWidget);
    d->q_ptr = this;
    d->_themeMode = nTheme->themeMode();
    d->_isDark = nTheme->isDarkMode();
    
    // 初始化颜色属性
    d->_pLightBackgroundColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Light);
    d->_pDarkBackgroundColor = NThemeColor(NFluentColorKey::ControlFillColorDefault, NThemeType::Dark);
    
    d->_pLightHeaderBackgroundColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkHeaderBackgroundColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    
    d->_pLightDateTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkDateTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    d->_pLightDateTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Light);
    d->_pDarkDateTextDisabledColor = NThemeColor(NFluentColorKey::TextFillColorDisabled, NThemeType::Dark);
    
    d->_pLightDateBackgroundColor = Qt::transparent;
    d->_pDarkDateBackgroundColor = Qt::transparent;
    d->_pLightDateBackgroundHoverColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Light);
    d->_pDarkDateBackgroundHoverColor = NThemeColor(NFluentColorKey::SubtleFillColorSecondary, NThemeType::Dark);
    d->_pLightDateBackgroundSelectedColor = nTheme->accentColor().normal();
    d->_pDarkDateBackgroundSelectedColor = nTheme->accentColor().normal();
    
    d->_pLightHeaderTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    d->_pDarkHeaderTextColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    
    d->_pLightWeekdayTextColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkWeekdayTextColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    
    d->_pLightWeekendTextColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Light);
    d->_pDarkWeekendTextColor = NThemeColor(NFluentColorKey::TextFillColorSecondary, NThemeType::Dark);
    
    d->_pLightTodayBackgroundColor = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    d->_pDarkTodayBackgroundColor = NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark);
    
    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    
    // 设置样式
    setObjectName("NCalendarWidget");
    setStyleSheet("#NCalendarWidget{background-color:transparent;}");
    
    d->_calendarStyle = new NCalendarWidgetStyle(d, style());
    setStyle(d->_calendarStyle);
    
    // Fluent 设计风格设置
    setGridVisible(false);
    setNavigationBarVisible(true);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
    
    // 设置字体
    QFont font = this->font();
    font.setPixelSize(NDesignToken(NDesignTokenKey::FontSizeBody).toInt());
    setFont(font);
    
    // 连接主题变化信号
    connect(nTheme, &NTheme::themeModeChanged, this, [this](NThemeType::ThemeMode themeMode) {
        Q_D(NCalendarWidget);
        d->_themeMode = themeMode;
        d->_isDark = nTheme->isDarkMode();
        update();
    });
    
    // 连接强调色变化信号
    connect(nTheme, &NTheme::accentColorChanged, this, [this](const NAccentColor& accentColor) {
        Q_D(NCalendarWidget);
        d->_pLightDateBackgroundSelectedColor = accentColor.normal();
        d->_pDarkDateBackgroundSelectedColor = accentColor.normal();
        update();
    });
}

void NCalendarWidget::paintEvent(QPaintEvent* event) {
    QCalendarWidget::paintEvent(event);
}

bool NCalendarWidget::event(QEvent* event) {
    return QCalendarWidget::event(event);
}