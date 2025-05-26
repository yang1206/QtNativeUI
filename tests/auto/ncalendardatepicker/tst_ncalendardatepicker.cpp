#include <QtTest>
#include <QApplication>
#include <QSignalSpy>
#include <QtNativeUI/NCalendarDatePicker.h>
#include <QtNativeUI/NTheme.h>

class tst_NCalendarDatePicker : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 测试创建与默认属性
    void testConstruction();
    void testDefaultProperties();
    
    // 测试日期操作
    void testSelectedDate();
    void testDateRange();
    void testMultipleDates();
    
    // 测试日期格式和显示
    void testDateFormat();
    void testPlaceholderText();
    
    // 测试本地化
    void testLocale();
    
    // 测试边框属性
    void testBorderRadius();
    
    // 测试选择模式
    void testSelectionMode();
    
    // 测试信号
    void testSignals();
};

void tst_NCalendarDatePicker::initTestCase() {
    // 测试开始前执行一次
}

void tst_NCalendarDatePicker::cleanupTestCase() {
    // 测试结束后执行一次
}

void tst_NCalendarDatePicker::init() {
    // 每个测试前执行
}

void tst_NCalendarDatePicker::cleanup() {
    // 每个测试后执行
}

void tst_NCalendarDatePicker::testConstruction() {
    // 测试默认构造函数
    NCalendarDatePicker picker;
    QVERIFY(picker.getSelectedDate().isNull());
    QVERIFY(picker.isEnabled());
}

void tst_NCalendarDatePicker::testDefaultProperties() {
    NCalendarDatePicker picker;
    
    // 检查默认边框半径
    QCOMPARE(picker.getBorderRadius(), NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt());
    
    // 默认占位文本
    QCOMPARE(picker.getPlaceholderText(), tr("Pick a date"));
    
    // 默认日期格式
    QCOMPARE(picker.getDateFormat(), QString("yyyy-MM-dd"));
    
    // 默认最小日期
    QCOMPARE(picker.getMinimumDate(), QDate(1601, 1, 1));
    
    // 默认最大日期
    QCOMPARE(picker.getMaximumDate(), QDate(9999, 12, 31));
    
    // 默认选择模式
    QCOMPARE(picker.dateSelectionMode(), NCalendarWidget::SingleDate);
}

void tst_NCalendarDatePicker::testSelectedDate() {
    NCalendarDatePicker picker;
    
    // 初始状态下无选定日期
    QVERIFY(picker.getSelectedDate().isNull());
    
    // 设置选定日期
    QDate testDate(2023, 10, 15);
    QSignalSpy spy(&picker, &NCalendarDatePicker::pSelectedDateChanged);
    QSignalSpy spyDateSelected(&picker, &NCalendarDatePicker::dateSelected);
    
    picker.setSelectedDate(testDate);
    
    QCOMPARE(picker.getSelectedDate(), testDate);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spyDateSelected.count(), 1);
    QCOMPARE(spyDateSelected.at(0).at(0).toDate(), testDate);
    
    // 设置无效日期不应改变当前值
    QDate invalidDate;
    picker.setSelectedDate(invalidDate);
    QCOMPARE(picker.getSelectedDate(), testDate); // 应该仍然是之前的有效日期
}

void tst_NCalendarDatePicker::testDateRange() {
    NCalendarDatePicker picker;
    
    // 设置日期范围选择模式
    picker.setDateSelectionMode(NCalendarWidget::DateRange);
    QCOMPARE(picker.dateSelectionMode(), NCalendarWidget::DateRange);
    
    // 设置日期范围
    QDate startDate(2023, 10, 1);
    QDate endDate(2023, 10, 7);
    
    QSignalSpy spy(&picker, &NCalendarDatePicker::selectedDateRangeChanged);
    QSignalSpy spySelectedDateChanged(&picker, &NCalendarDatePicker::pSelectedDateChanged);
    
    picker.setDateRange(startDate, endDate);
    
    QCOMPARE(picker.selectedDateRange().first, startDate);
    QCOMPARE(picker.selectedDateRange().second, endDate);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spySelectedDateChanged.count(), 1);
    
    // 选定的日期应该是范围的开始日期
    QCOMPARE(picker.getSelectedDate(), startDate);
}

void tst_NCalendarDatePicker::testMultipleDates() {
    NCalendarDatePicker picker;
    
    // 设置多选模式
    picker.setDateSelectionMode(NCalendarWidget::MultipleDate);
    QCOMPARE(picker.dateSelectionMode(), NCalendarWidget::MultipleDate);
    
    // 设置多个日期
    QList<QDate> dates;
    dates << QDate(2023, 10, 1) << QDate(2023, 10, 5) << QDate(2023, 10, 10);
    
    QSignalSpy spy(&picker, &NCalendarDatePicker::selectedDatesChanged);
    QSignalSpy spySelectedDateChanged(&picker, &NCalendarDatePicker::pSelectedDateChanged);
    
    picker.setSelectedDates(dates);
    
    QCOMPARE(picker.selectedDates(), dates);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spySelectedDateChanged.count(), 1);
    
    // 选定的日期应该是列表中的第一个日期
    QCOMPARE(picker.getSelectedDate(), dates.first());
}

void tst_NCalendarDatePicker::testDateFormat() {
    NCalendarDatePicker picker;
    
    // 测试设置日期格式
    QString customFormat = "dd/MM/yyyy";
    QSignalSpy spy(&picker, &NCalendarDatePicker::pDateFormatChanged);
    
    picker.setDateFormat(customFormat);
    
    QCOMPARE(picker.getDateFormat(), customFormat);
    QCOMPARE(spy.count(), 1);
    
    // 设置日期后检查显示格式
    QDate testDate(2023, 10, 15);
    picker.setSelectedDate(testDate);
    
    // 注意：由于无法直接访问内部Label，我们无法直接测试显示文本
    // 这部分在实际测试中可能需要UI自动化测试或特殊处理
}

void tst_NCalendarDatePicker::testPlaceholderText() {
    NCalendarDatePicker picker;
    
    // 测试设置占位文本
    QString customPlaceholder = "Select Date";
    QSignalSpy spy(&picker, &NCalendarDatePicker::pPlaceholderTextChanged);
    
    picker.setPlaceholderText(customPlaceholder);
    
    QCOMPARE(picker.getPlaceholderText(), customPlaceholder);
    QCOMPARE(spy.count(), 1);
}

void tst_NCalendarDatePicker::testLocale() {
    NCalendarDatePicker picker;
    
    // 测试设置语言环境
    QLocale frLocale(QLocale::French);
    
    picker.setLocale(frLocale);
    QCOMPARE(picker.locale(), frLocale);
    
    // 验证本地化设置对日期格式的影响
    QDate testDate(2023, 10, 15);
    picker.setSelectedDate(testDate);
    
    // 注：由于无法直接访问内部Label文本，无法直接验证格式化后的显示
}

void tst_NCalendarDatePicker::testBorderRadius() {
    NCalendarDatePicker picker;
    
    // 测试设置边框半径
    int customRadius = 10;
    QSignalSpy spy(&picker, &NCalendarDatePicker::pBorderRadiusChanged);
    
    picker.setBorderRadius(customRadius);
    
    QCOMPARE(picker.getBorderRadius(), customRadius);
    QCOMPARE(spy.count(), 1);
}

void tst_NCalendarDatePicker::testSelectionMode() {
    NCalendarDatePicker picker;
    
    // 测试设置选择模式
    QSignalSpy spy(&picker, &NCalendarDatePicker::dateSelectionModeChanged);
    
    picker.setDateSelectionMode(NCalendarWidget::MultipleDate);
    QCOMPARE(picker.dateSelectionMode(), NCalendarWidget::MultipleDate);
    QCOMPARE(spy.count(), 1);
    
    picker.setDateSelectionMode(NCalendarWidget::DateRange);
    QCOMPARE(picker.dateSelectionMode(), NCalendarWidget::DateRange);
    QCOMPARE(spy.count(), 2);
    
    picker.setDateSelectionMode(NCalendarWidget::SingleDate);
    QCOMPARE(picker.dateSelectionMode(), NCalendarWidget::SingleDate);
    QCOMPARE(spy.count(), 3);
}

void tst_NCalendarDatePicker::testSignals() {
    NCalendarDatePicker picker;
    
    // 测试popupOpened和popupClosed信号
    // 注意：这些信号通常在实际UI交互中触发，在单元测试中可能难以直接测试
    // 这里我们只验证信号的存在性
    
    // 以下代码仅用于验证信号连接正常，不会真正触发信号
    QSignalSpy spyOpened(&picker, &NCalendarDatePicker::popupOpened);
    QSignalSpy spyClosed(&picker, &NCalendarDatePicker::popupClosed);
    
    QVERIFY(connect(&picker, &NCalendarDatePicker::popupOpened, [&]() {}));
    QVERIFY(connect(&picker, &NCalendarDatePicker::popupClosed, [&]() {}));
}

QTEST_MAIN(tst_NCalendarDatePicker)
#include "tst_ncalendardatepicker.moc"
