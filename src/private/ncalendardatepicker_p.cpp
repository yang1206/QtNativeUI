#include "ncalendardatepicker_p.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>

#include "QtNativeUI/NIcon.h"

NCalendarDatePickerPrivate::NCalendarDatePickerPrivate(NCalendarDatePicker* q)
    : QObject(q), q_ptr(q), button(nullptr), contentWidget(nullptr), dateLabel(nullptr), iconLabel(nullptr) {
    _pBorderRadius    = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();
    _themeMode        = nTheme->themeMode();
    _isDark           = nTheme->isDarkMode();
    _pSelectedDate    = QDate();
    _pMinimumDate     = QDate(1601, 1, 1);
    _pMaximumDate     = QDate(9999, 12, 31);
    _pPlaceholderText = QObject::tr("Pick a date");
    _pDateFormat      = "MM/dd/yyyy"; // 与图片格式匹配
    _locale           = QLocale::system();
    _isAccentStyle    = false;
    _selectionMode    = NCalendarWidget::SingleDate;
}

NCalendarDatePickerPrivate::~NCalendarDatePickerPrivate() {}

void NCalendarDatePickerPrivate::initUi() {
    q_ptr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // 创建主按钮，作为容器背景
    button = new NPushButton(q_ptr);
    button->setObjectName("NCalendarDatePickerButton");
    button->setTransparentBackground(false);
    button->setBorderRadius(_pBorderRadius);

    // 创建外部容器，并设置为事件透明
    contentWidget = new QWidget(q_ptr);
    contentWidget->setObjectName("NCalendarDatePickerContent");
    contentWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    // 创建日期标签
    dateLabel = new QLabel(contentWidget);
    dateLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 创建图标标签
    iconLabel = new QLabel(contentWidget);
    iconLabel->setFixedSize(16, 16);
    QIcon calendarIcon = nIcon->fromRegular(NRegularIconType::CalendarLtr12Regular, 16);
    iconLabel->setPixmap(calendarIcon.pixmap(16, 16));

    // 设置布局，左侧日期，右侧图标
    QHBoxLayout* layout = new QHBoxLayout(contentWidget);
    layout->setContentsMargins(10, 0, 10, 0);
    layout->setSpacing(8);
    layout->addWidget(dateLabel, 1);
    layout->addStretch();
    layout->addWidget(iconLabel);

    // 设置主布局
    QHBoxLayout* mainLayout = new QHBoxLayout(q_ptr);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(button);

    // 确保内容显示在按钮上方
    contentWidget->raise();
    contentWidget->resize(q_ptr->size());

    // 初始文本设置
    updateDisplayText();

    // 连接主题变化
    QObject::connect(nTheme, &NTheme::themeModeChanged, [this](NThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        _isDark    = nTheme->isDarkMode();
        updateDisplayText();
        q_ptr->update();
    });
}

void NCalendarDatePickerPrivate::updateDisplayText() {
    QString displayText = _pPlaceholderText;
    if (_pSelectedDate.isValid()) {
        displayText = _locale.toString(_pSelectedDate, _pDateFormat);
    }

    // 更新日期标签文本
    dateLabel->setText(displayText);

    // 根据暗黑模式更新颜色
    QColor textColor;
    if (_isDark) {
        textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Dark);
    } else {
        textColor = NThemeColor(NFluentColorKey::TextFillColorPrimary, NThemeType::Light);
    }

    // 设置颜色样式表
    QString colorStyle = QString("color: %1;").arg(textColor.name());
    dateLabel->setStyleSheet(colorStyle);

    // 更新图标颜色
    QIcon calendarIcon = nIcon->fromRegular(NRegularIconType::CalendarLtr12Regular, 16, textColor);
    iconLabel->setPixmap(calendarIcon.pixmap(16, 16));
}