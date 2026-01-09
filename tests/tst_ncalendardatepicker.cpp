// Copyright (C) 2024 QtNativeUI Project
// SPDX-License-Identifier: GPL-3.0-only

#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QLocale>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QSignalSpy>
#include <QStyleFactory>
#include <QTest>
#include <QTimer>

#include <QtNativeUI/NCalendarDatePicker.h>
#include <QtNativeUI/NTheme.h>

// 创建一个用于测试的NCalendarDatePicker子类
class TestNCalendarDatePicker : public NCalendarDatePicker {
  public:
    using NCalendarDatePicker::NCalendarDatePicker;

    // 公开protected方法用于测试
    using NCalendarDatePicker::changeEvent;
    using NCalendarDatePicker::eventFilter;
    using NCalendarDatePicker::paintEvent;
    using NCalendarDatePicker::resizeEvent;
};

class tst_NCalendarDatePicker : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 基础功能测试
    void getSetCheck();
    void testConstruction();
    void testDefaultProperties();

    // 日期操作测试
    void testSelectedDate();
    void testDateRange();
    void testMultipleDates();
    void testMinMaxDate();

    // 格式和显示测试
    void testDateFormat();
    void testPlaceholderText();
    void testLocale();

    // 属性测试
    void testBorderRadius();
    void testSelectionMode();

    // 信号测试
    void testSignals();
    void testDateSelectionSignals();

    // 性能相关测试
    void cacheEfficiency();
    void memoryLeakTest();
    void longRunningMemoryTest();

    // 边界条件测试
    void disabledStateOperations();
    void extremeSizes();
    void dateEdgeCases();
    void invalidDateHandling();

    // 样式和绘制测试
    void highDpiRendering();
    void styleSheetApplication();
    void focusRendering();
    void paintEventTest();

    // 国际化测试
    void internationalization();
    void localeSpecificFormatting();

    // 交互测试
    void keyboardNavigation();
    void mouseInteraction();

  private:
    NCalendarDatePicker* testWidget;
};

void tst_NCalendarDatePicker::initTestCase() {
    testWidget = new NCalendarDatePicker();
    testWidget->setObjectName("testWidget");
    testWidget->resize(200, 35);
    testWidget->show();
}

void tst_NCalendarDatePicker::cleanupTestCase() {
    delete testWidget;
    testWidget = nullptr;
}

void tst_NCalendarDatePicker::init() {
    testWidget->setSelectedDate(QDate());
    testWidget->setMinimumDate(QDate(1601, 1, 1));
    testWidget->setMaximumDate(QDate(9999, 12, 31));
    testWidget->setPlaceholderText(tr("Pick a date"));
    testWidget->setDateFormat("yyyy-MM-dd");
    testWidget->setBorderRadius(4);
    testWidget->setDateSelectionMode(NCalendarWidget::SingleDate);
    testWidget->setEnabled(true);
}

void tst_NCalendarDatePicker::cleanup() {
    // 每个测试后执行
}

// Testing get/set functions
void tst_NCalendarDatePicker::getSetCheck() {
    NCalendarDatePicker obj1;

    // QDate NCalendarDatePicker::selectedDate()
    // void NCalendarDatePicker::setSelectedDate(const QDate&)
    QDate testDate(2023, 10, 15);
    obj1.setSelectedDate(testDate);
    QCOMPARE(obj1.getSelectedDate(), testDate);

    // 设置无效日期
    obj1.setSelectedDate(QDate());
    QCOMPARE(obj1.getSelectedDate(), testDate); // 应该保持之前的有效日期

    // QString NCalendarDatePicker::placeholderText()
    // void NCalendarDatePicker::setPlaceholderText(const QString&)
    obj1.setPlaceholderText("Select a date");
    QCOMPARE(obj1.getPlaceholderText(), QString("Select a date"));
    obj1.setPlaceholderText("");
    QCOMPARE(obj1.getPlaceholderText(), QString(""));

    // QString NCalendarDatePicker::dateFormat()
    // void NCalendarDatePicker::setDateFormat(const QString&)
    obj1.setDateFormat("dd/MM/yyyy");
    QCOMPARE(obj1.getDateFormat(), QString("dd/MM/yyyy"));
    obj1.setDateFormat("yyyy-MM-dd");
    QCOMPARE(obj1.getDateFormat(), QString("yyyy-MM-dd"));

    // int NCalendarDatePicker::borderRadius()
    // void NCalendarDatePicker::setBorderRadius(int)
    obj1.setBorderRadius(10);
    QCOMPARE(obj1.getBorderRadius(), 10);
    obj1.setBorderRadius(0);
    QCOMPARE(obj1.getBorderRadius(), 0);

    // QDate NCalendarDatePicker::minimumDate()
    // void NCalendarDatePicker::setMinimumDate(const QDate&)
    QDate minDate(2020, 1, 1);
    obj1.setMinimumDate(minDate);
    QCOMPARE(obj1.getMinimumDate(), minDate);

    // QDate NCalendarDatePicker::maximumDate()
    // void NCalendarDatePicker::setMaximumDate(const QDate&)
    QDate maxDate(2030, 12, 31);
    obj1.setMaximumDate(maxDate);
    QCOMPARE(obj1.getMaximumDate(), maxDate);

    // NCalendarWidget::DateSelectionMode NCalendarDatePicker::dateSelectionMode()
    // void NCalendarDatePicker::setDateSelectionMode(NCalendarWidget::DateSelectionMode)
    obj1.setDateSelectionMode(NCalendarWidget::MultipleDate);
    QCOMPARE(obj1.dateSelectionMode(), NCalendarWidget::MultipleDate);
    obj1.setDateSelectionMode(NCalendarWidget::DateRange);
    QCOMPARE(obj1.dateSelectionMode(), NCalendarWidget::DateRange);
    obj1.setDateSelectionMode(NCalendarWidget::SingleDate);
    QCOMPARE(obj1.dateSelectionMode(), NCalendarWidget::SingleDate);
}

void tst_NCalendarDatePicker::testConstruction() {
    // 测试默认构造函数
    NCalendarDatePicker picker1;
    QVERIFY(picker1.getSelectedDate().isNull());
    QVERIFY(picker1.isEnabled());
    QCOMPARE(picker1.dateSelectionMode(), NCalendarWidget::SingleDate);

    // 测试带父对象的构造函数
    QWidget             parent;
    NCalendarDatePicker picker2(&parent);
    QCOMPARE(picker2.parent(), &parent);
    QVERIFY(picker2.getSelectedDate().isNull());
    QVERIFY(picker2.isEnabled());
}

void tst_NCalendarDatePicker::testDefaultProperties() {
    NCalendarDatePicker picker;

    // 检查默认边框半径
    int defaultRadius = picker.getBorderRadius();
    QVERIFY(defaultRadius >= 0);

    // 默认占位文本
    QString defaultPlaceholder = picker.getPlaceholderText();
    QVERIFY(!defaultPlaceholder.isEmpty());

    // 默认日期格式
    QString defaultFormat = picker.getDateFormat();
    QVERIFY(!defaultFormat.isEmpty());

    // 默认最小日期
    QDate minDate = picker.getMinimumDate();
    QVERIFY(minDate.isValid());
    QVERIFY(minDate.year() >= 1601);

    // 默认最大日期
    QDate maxDate = picker.getMaximumDate();
    QVERIFY(maxDate.isValid());
    QVERIFY(maxDate.year() <= 9999);

    // 默认选择模式
    QCOMPARE(picker.dateSelectionMode(), NCalendarWidget::SingleDate);

    // 默认语言环境
    QLocale defaultLocale = picker.locale();
    QVERIFY(defaultLocale != QLocale::c());
}

void tst_NCalendarDatePicker::testSelectedDate() {
    NCalendarDatePicker picker;

    // 初始状态下无选定日期
    QVERIFY(picker.getSelectedDate().isNull());

    // 设置选定日期
    QDate      testDate(2023, 10, 15);
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
    QString    customFormat = "dd/MM/yyyy";
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
    QString    customPlaceholder = "Select Date";
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
    int        customRadius = 10;
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

void tst_NCalendarDatePicker::testMinMaxDate() {
    NCalendarDatePicker picker;

    // 测试设置最小日期
    QDate      minDate(2020, 1, 1);
    QSignalSpy minSpy(&picker, &NCalendarDatePicker::pMinimumDateChanged);
    picker.setMinimumDate(minDate);
    QCOMPARE(picker.getMinimumDate(), minDate);
    QCOMPARE(minSpy.count(), 1);

    // 测试设置最大日期
    QDate      maxDate(2030, 12, 31);
    QSignalSpy maxSpy(&picker, &NCalendarDatePicker::pMaximumDateChanged);
    picker.setMaximumDate(maxDate);
    QCOMPARE(picker.getMaximumDate(), maxDate);
    QCOMPARE(maxSpy.count(), 1);

    // 测试设置无效日期不会改变值
    picker.setMinimumDate(QDate());
    QCOMPARE(picker.getMinimumDate(), minDate);
    QCOMPARE(minSpy.count(), 1);

    picker.setMaximumDate(QDate());
    QCOMPARE(picker.getMaximumDate(), maxDate);
    QCOMPARE(maxSpy.count(), 1);
}

void tst_NCalendarDatePicker::testDateSelectionSignals() {
    NCalendarDatePicker picker;

    // 测试单日期选择信号
    QSignalSpy dateSelectedSpy(&picker, &NCalendarDatePicker::dateSelected);
    QSignalSpy selectedDateChangedSpy(&picker, &NCalendarDatePicker::pSelectedDateChanged);

    QDate testDate(2023, 10, 15);
    picker.setSelectedDate(testDate);
    QCOMPARE(dateSelectedSpy.count(), 1);
    QCOMPARE(selectedDateChangedSpy.count(), 1);

    // 测试多日期选择信号
    picker.setDateSelectionMode(NCalendarWidget::MultipleDate);
    QSignalSpy selectedDatesSpy(&picker, &NCalendarDatePicker::selectedDatesChanged);

    QList<QDate> dates;
    dates << QDate(2023, 10, 1) << QDate(2023, 10, 5) << QDate(2023, 10, 10);
    picker.setSelectedDates(dates);
    QCOMPARE(selectedDatesSpy.count(), 1);

    // 测试日期范围选择信号
    picker.setDateSelectionMode(NCalendarWidget::DateRange);
    QSignalSpy dateRangeSpy(&picker, &NCalendarDatePicker::selectedDateRangeChanged);

    QDate startDate(2023, 10, 1);
    QDate endDate(2023, 10, 7);
    picker.setDateRange(startDate, endDate);
    QCOMPARE(dateRangeSpy.count(), 1);
}

// 性能相关测试
void tst_NCalendarDatePicker::cacheEfficiency() {
    NCalendarDatePicker picker;
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 测试重复设置相同日期的性能
    QDate testDate(2023, 10, 15);
    for (int i = 0; i < 100; ++i) {
        picker.setSelectedDate(testDate);
    }

    // 测试重复设置相同属性的性能
    for (int i = 0; i < 100; ++i) {
        picker.setBorderRadius(10);
        picker.setDateFormat("yyyy-MM-dd");
        picker.setPlaceholderText("Pick a date");
    }
}

void tst_NCalendarDatePicker::memoryLeakTest() {
    const int                     pickerCount = 50;
    QVector<NCalendarDatePicker*> pickers;
    pickers.reserve(pickerCount);

    // 创建大量日期选择器
    for (int i = 0; i < pickerCount; ++i) {
        NCalendarDatePicker* picker = new NCalendarDatePicker();
        picker->setSelectedDate(QDate(2023, 10, i % 28 + 1));
        picker->setPlaceholderText(QString("Picker %1").arg(i));
        picker->setDateFormat(i % 2 == 0 ? "yyyy-MM-dd" : "dd/MM/yyyy");
        pickers.append(picker);
    }

    // 销毁所有日期选择器
    qDeleteAll(pickers);
    pickers.clear();

    // 强制垃圾回收
    QCoreApplication::processEvents();
}

void tst_NCalendarDatePicker::longRunningMemoryTest() {
    NCalendarDatePicker picker;
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 模拟长时间运行中的各种操作
    for (int i = 0; i < 100; ++i) {
        // 改变日期
        picker.setSelectedDate(QDate(2023, 10, (i % 28) + 1));

        // 改变属性
        picker.setBorderRadius(i % 20);
        picker.setDateFormat(i % 2 == 0 ? "yyyy-MM-dd" : "dd/MM/yyyy");

        // 改变选择模式
        NCalendarWidget::DateSelectionMode modes[] = {
            NCalendarWidget::SingleDate, NCalendarWidget::MultipleDate, NCalendarWidget::DateRange};
        picker.setDateSelectionMode(modes[i % 3]);

        QTest::qWait(10);
    }
}

// 边界条件测试
void tst_NCalendarDatePicker::disabledStateOperations() {
    NCalendarDatePicker picker;
    picker.setEnabled(false);
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 测试禁用状态下的属性设置仍然有效
    QDate testDate(2023, 10, 15);
    picker.setSelectedDate(testDate);
    QCOMPARE(picker.getSelectedDate(), testDate);

    picker.setBorderRadius(15);
    QCOMPARE(picker.getBorderRadius(), 15);

    picker.setPlaceholderText("Disabled Picker");
    QCOMPARE(picker.getPlaceholderText(), QString("Disabled Picker"));

    // 重新启用后应该正常工作
    picker.setEnabled(true);
    QVERIFY(picker.isEnabled());
}

void tst_NCalendarDatePicker::extremeSizes() {
    // 测试极小尺寸
    NCalendarDatePicker smallPicker;
    smallPicker.resize(10, 10);
    smallPicker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&smallPicker));

    // 测试极大尺寸
    NCalendarDatePicker largePicker;
    largePicker.resize(1000, 100);
    largePicker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&largePicker));

    // 测试零尺寸
    NCalendarDatePicker zeroPicker;
    zeroPicker.resize(0, 0);
    // 零尺寸下仍应该能设置属性
    zeroPicker.setSelectedDate(QDate(2023, 10, 15));
    QCOMPARE(zeroPicker.getSelectedDate(), QDate(2023, 10, 15));
}

void tst_NCalendarDatePicker::dateEdgeCases() {
    NCalendarDatePicker picker;

    // 测试边界日期
    QDate minDate(1601, 1, 1);
    QDate maxDate(9999, 12, 31);

    picker.setSelectedDate(minDate);
    QCOMPARE(picker.getSelectedDate(), minDate);

    picker.setSelectedDate(maxDate);
    QCOMPARE(picker.getSelectedDate(), maxDate);

    // 测试闰年日期
    QDate leapDate(2024, 2, 29);
    picker.setSelectedDate(leapDate);
    QCOMPARE(picker.getSelectedDate(), leapDate);

    // 测试非闰年的2月29日（无效日期）
    QDate invalidLeapDate(2023, 2, 29);
    QVERIFY(!invalidLeapDate.isValid());
    QDate previousDate = picker.getSelectedDate();
    picker.setSelectedDate(invalidLeapDate);
    QCOMPARE(picker.getSelectedDate(), previousDate); // 应该保持之前的日期

    // 测试月末日期
    picker.setSelectedDate(QDate(2023, 1, 31));
    QCOMPARE(picker.getSelectedDate(), QDate(2023, 1, 31));

    picker.setSelectedDate(QDate(2023, 4, 30));
    QCOMPARE(picker.getSelectedDate(), QDate(2023, 4, 30));
}

void tst_NCalendarDatePicker::invalidDateHandling() {
    NCalendarDatePicker picker;

    // 设置一个有效日期
    QDate validDate(2023, 10, 15);
    picker.setSelectedDate(validDate);
    QCOMPARE(picker.getSelectedDate(), validDate);

    // 尝试设置无效日期
    QDate invalidDate;
    QVERIFY(!invalidDate.isValid());
    picker.setSelectedDate(invalidDate);
    QCOMPARE(picker.getSelectedDate(), validDate); // 应该保持之前的有效日期

    // 测试无效的最小/最大日期
    picker.setMinimumDate(QDate());
    QVERIFY(picker.getMinimumDate().isValid()); // 应该保持之前的有效值

    picker.setMaximumDate(QDate());
    QVERIFY(picker.getMaximumDate().isValid()); // 应该保持之前的有效值
}

// 样式和绘制测试
void tst_NCalendarDatePicker::highDpiRendering() {
    NCalendarDatePicker picker;
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 测试在高DPI下的基本功能
    picker.setSelectedDate(QDate(2023, 10, 15));
    QCOMPARE(picker.getSelectedDate(), QDate(2023, 10, 15));

    // 测试尺寸计算
    QSize sizeHint = picker.sizeHint();
    QVERIFY(sizeHint.width() > 0);
    QVERIFY(sizeHint.height() > 0);
}

void tst_NCalendarDatePicker::styleSheetApplication() {
    NCalendarDatePicker picker;

    // 测试样式表的应用
    QString styleSheet = "NCalendarDatePicker { background-color: yellow; border: 2px solid red; }";
    picker.setStyleSheet(styleSheet);
    QCOMPARE(picker.styleSheet(), styleSheet);

    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 测试样式表不会影响基本功能
    QDate testDate(2023, 10, 15);
    picker.setSelectedDate(testDate);
    QCOMPARE(picker.getSelectedDate(), testDate);

    // 清除样式表
    picker.setStyleSheet("");
    QVERIFY(picker.styleSheet().isEmpty());
}

void tst_NCalendarDatePicker::focusRendering() {
    NCalendarDatePicker picker;
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 测试获得焦点
    picker.setFocus();
    QVERIFY(picker.hasFocus());
    QTest::qWait(100);

    // 测试失去焦点
    picker.clearFocus();
    QVERIFY(!picker.hasFocus());
    QTest::qWait(100);
}

void tst_NCalendarDatePicker::paintEventTest() {
    TestNCalendarDatePicker picker;
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 强制重绘
    picker.update();
    QTest::qWait(100);

    // 测试不同状态下的绘制
    picker.setEnabled(false);
    picker.update();
    QTest::qWait(50);

    picker.setEnabled(true);
    picker.update();
    QTest::qWait(50);
}

// 国际化测试
void tst_NCalendarDatePicker::internationalization() {
    // 测试中文环境
    NCalendarDatePicker chinesePicker;
    chinesePicker.setLocale(QLocale(QLocale::Chinese, QLocale::China));
    chinesePicker.setPlaceholderText("选择日期");
    QCOMPARE(chinesePicker.getPlaceholderText(), QString("选择日期"));

    // 测试法文环境
    NCalendarDatePicker frenchPicker;
    frenchPicker.setLocale(QLocale(QLocale::French, QLocale::France));
    frenchPicker.setPlaceholderText("Choisir une date");
    QCOMPARE(frenchPicker.getPlaceholderText(), QString("Choisir une date"));

    // 测试阿拉伯文环境
    NCalendarDatePicker arabicPicker;
    arabicPicker.setLocale(QLocale(QLocale::Arabic, QLocale::SaudiArabia));
    arabicPicker.setPlaceholderText("اختر تاريخ");
    QCOMPARE(arabicPicker.getPlaceholderText(), QString("اختر تاريخ"));
}

void tst_NCalendarDatePicker::localeSpecificFormatting() {
    NCalendarDatePicker picker;
    QDate               testDate(2023, 10, 15);
    picker.setSelectedDate(testDate);

    // 测试不同语言环境下的日期格式
    QLocale usLocale(QLocale::English, QLocale::UnitedStates);
    picker.setLocale(usLocale);
    picker.setDateFormat("MM/dd/yyyy");
    QCOMPARE(picker.getDateFormat(), QString("MM/dd/yyyy"));

    QLocale cnLocale(QLocale::Chinese, QLocale::China);
    picker.setLocale(cnLocale);
    picker.setDateFormat("yyyy年MM月dd日");
    QCOMPARE(picker.getDateFormat(), QString("yyyy年MM月dd日"));

    QLocale frLocale(QLocale::French, QLocale::France);
    picker.setLocale(frLocale);
    picker.setDateFormat("dd/MM/yyyy");
    QCOMPARE(picker.getDateFormat(), QString("dd/MM/yyyy"));
}

// 交互测试
void tst_NCalendarDatePicker::keyboardNavigation() {
    NCalendarDatePicker picker;
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 测试Tab键导航
    picker.setFocus();
    QVERIFY(picker.hasFocus());

    QTest::keyPress(&picker, Qt::Key_Tab);
    QTest::qWait(50);

    // 测试空格键（可能触发弹出）
    picker.setFocus();
    QTest::keyPress(&picker, Qt::Key_Space);
    QTest::qWait(50);

    // 测试回车键
    QTest::keyPress(&picker, Qt::Key_Return);
    QTest::qWait(50);
}

void tst_NCalendarDatePicker::mouseInteraction() {
    NCalendarDatePicker picker;
    picker.show();
    QVERIFY(QTest::qWaitForWindowExposed(&picker));

    // 测试鼠标点击
    QTest::mouseClick(&picker, Qt::LeftButton, Qt::NoModifier, picker.rect().center());
    QTest::qWait(100);

    // 测试鼠标悬停
    QTest::mouseMove(&picker, picker.rect().center());
    QTest::qWait(50);

    // 测试鼠标离开
    QTest::mouseMove(&picker, QPoint(-1, -1));
    QTest::qWait(50);
}

QTEST_MAIN(tst_NCalendarDatePicker)
#include "tst_ncalendardatepicker.moc"
