#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QVBoxLayout>
#include <QtNativeUI/NCalendarWidget.h>

#include "../private/ncalendardelegate.h"
#include "../private/ncalendarmodel.h"
#include "../private/ncalendartitledelegate.h"
#include "../private/ncalendartitlemodel.h"
#include "../private/ncalendarwidget_p.h"
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/NScrollBar.h"
#include "QtNativeUI/NTheme.h"
#include "nbaselistview.h"

Q_PROPERTY_CREATE_Q_CPP(NCalendarWidget, int, BorderRadius)

NCalendarWidget::NCalendarWidget(QWidget* parent) : QWidget{parent}, d_ptr(new NCalendarWidgetPrivate()) {
    Q_D(NCalendarWidget);
    setFixedSize(305, 340);
    setObjectName("NCalendarWidget");
    d->q_ptr          = this;
    d->_pBorderRadius = NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt();

    // 日历标题
    d->_calendarTitleView = new NBaseListView(this);
    d->_calendarTitleView->setFlow(NBaseListView::LeftToRight);
    d->_calendarTitleView->setViewMode(NBaseListView::IconMode);
    d->_calendarTitleView->setResizeMode(NBaseListView::Adjust);
    d->_calendarTitleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarTitleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarTitleView->setModel(new NCalendarTitleModel(this));
    d->_calendarTitleView->setItemDelegate(new NCalendarTitleDelegate(this));
    d->_calendarTitleView->setFixedHeight(30);

    // 日历内容
    d->_calendarView = new NBaseListView(this);
    d->_calendarView->setFlow(NBaseListView::LeftToRight);
    d->_calendarView->setViewMode(NBaseListView::IconMode);
    d->_calendarView->setResizeMode(NBaseListView::Adjust);
    // if (NScrollBar* vScrollBar = dynamic_cast<NScrollBar*>(d->_calendarView->verticalScrollBar())) {
    //     vScrollBar->setSpeedLimit(6);
    // }
    d->_calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->_calendarView->setSelectionMode(QAbstractItemView::NoSelection);
    d->_calendarModel = new NCalendarModel(this);
    d->_calendarView->setModel(d->_calendarModel);
    d->_calendarDelegate = new NCalendarDelegate(d->_calendarModel, this);
    d->_calendarView->setItemDelegate(d->_calendarDelegate);
    connect(d->_calendarView, &QListView::clicked, d, &NCalendarWidgetPrivate::onCalendarViewClicked);

    // 模式切换按钮
    d->_modeSwitchButton = new NPushButton(this);
    d->_modeSwitchButton->setText("1924年1月");
    QFont switchButtonFont = d->_modeSwitchButton->font();
    switchButtonFont.setWeight(QFont::Bold);
    d->_modeSwitchButton->setFont(switchButtonFont);
    d->_modeSwitchButton->setTransparentBackground(true);
    d->_modeSwitchButton->setFixedHeight(36);
    d->_modeSwitchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(d->_modeSwitchButton, &NPushButton::clicked, d, &NCalendarWidgetPrivate::onSwitchButtonClicked);
    connect(
        d->_calendarView->verticalScrollBar(), &QScrollBar::valueChanged, d, [=]() { d->_updateSwitchButtonText(); });

    // 翻页按钮
    d->_upButton = new NPushButton(this);
    d->_upButton->setFixedSize(36, 36);
    d->_upButton->setFluentIcon(NRegularIconType::ChevronUp16Regular);
    d->_upButton->setTransparentBackground(true);
    connect(d->_upButton, &NPushButton::clicked, d, &NCalendarWidgetPrivate::onUpButtonClicked);

    d->_downButton = new NPushButton(this);
    d->_downButton->setFixedSize(36, 36);
    d->_downButton->setFluentIcon(NRegularIconType::ChevronDown16Regular);
    d->_downButton->setTransparentBackground(true);
    connect(d->_downButton, &NPushButton::clicked, d, &NCalendarWidgetPrivate::onDownButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(5, 5, 10, 0);
    buttonLayout->addWidget(d->_modeSwitchButton);
    buttonLayout->addWidget(d->_upButton);
    buttonLayout->addWidget(d->_downButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 0, 0);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(d->_calendarTitleView);
    mainLayout->addWidget(d->_calendarView);

    d->_themeMode = nTheme->themeMode();
    d->_isDark    = nTheme->isDarkMode();
    connect(nTheme, &NTheme::themeModeChanged, this, [=](NThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        d->_isDark    = nTheme->isDarkMode();
        update();
    });

    setVisible(true);
    QDate currentDate     = QDate::currentDate();
    d->_lastSelectedYear  = currentDate.year();
    d->_lastSelectedMonth = currentDate.month();
    d->_scrollToDate(currentDate);
}

NCalendarWidget::~NCalendarWidget() {}

void NCalendarWidget::setSelectedDate(QDate selectedDate) {
    Q_D(NCalendarWidget);
    if (!selectedDate.isValid() || selectedDate.daysTo(d->_calendarModel->getMaximumDate()) < 0 ||
        selectedDate.daysTo(d->_calendarModel->getMinimumDate()) > 0) {
        return;
    }
    d->_pSelectedDate = selectedDate;
    d->_calendarView->selectionModel()->setCurrentIndex(d->_calendarModel->getIndexFromDate(selectedDate),
                                                        QItemSelectionModel::Select);
    Q_EMIT pSelectedDateChanged();
}

QDate NCalendarWidget::getSelectedDate() const {
    Q_D(const NCalendarWidget);
    return d->_pSelectedDate;
    // return d->_calendarModel->getDateFromIndex(d->_calendarView->selectionModel()->currentIndex());
}

void NCalendarWidget::setMinimumDate(QDate minimudate) {
    Q_D(NCalendarWidget);
    if (!minimudate.isValid() || minimudate.daysTo(d->_calendarModel->getMaximumDate()) < 0) {
        return;
    }
    d->_calendarModel->setMaximumDate(minimudate);
    Q_EMIT pMinimumDateChanged();
}

QDate NCalendarWidget::getMinimumDate() const {
    Q_D(const NCalendarWidget);
    return d->_calendarModel->getMinimumDate();
}

void NCalendarWidget::setMaximumDate(QDate maximumDate) {
    Q_D(NCalendarWidget);
    if (!maximumDate.isValid() || maximumDate.daysTo(d->_calendarModel->getMinimumDate()) > 0) {
        return;
    }
    d->_calendarModel->setMaximumDate(maximumDate);
    Q_EMIT pMaximumDateChanged();
}

QDate NCalendarWidget::getMaximumDate() const {
    Q_D(const NCalendarWidget);
    return d->_calendarModel->getMaximumDate();
}

void NCalendarWidget::paintEvent(QPaintEvent* event) {
    Q_D(NCalendarWidget);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect baseRect = rect();
    baseRect.adjust(d->_borderWidth, d->_borderWidth, -d->_borderWidth, -d->_borderWidth);
    painter.setPen(Qt::NoPen);

    // 背景
    QColor bgColor = d->_isDark ? NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Dark)
                                : NThemeColor(NFluentColorKey::CardBackgroundFillColorDefault, NThemeType::Light);
    painter.setBrush(bgColor);

    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);

    // 缩放动画
    if (!d->_isSwitchAnimationFinished) {
        painter.save();
        QRect pixRect = QRect(baseRect.x(), d->_borderWidth + 45, baseRect.width(), baseRect.height() - 45);
        painter.setOpacity(d->_pPixOpacity);
        painter.translate(pixRect.center());
        painter.scale(d->_pZoomRatio, d->_pZoomRatio);
        painter.translate(-pixRect.center());
        painter.drawPixmap(pixRect, d->_isDrawNewPix ? d->_newCalendarViewPix : d->_oldCalendarViewPix);
        painter.restore();
    }

    // 分割线
    QColor borderColor = d->_isDark ? NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Dark)
                                    : NThemeColor(NFluentColorKey::CardStrokeColorDefault, NThemeType::Light);
    painter.setPen(QPen(borderColor, d->_borderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(baseRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.drawLine(QPointF(baseRect.x(), 45), QPointF(baseRect.right(), 45));

    painter.restore();
}