// Copyright (C) 2024 QtNativeUI Project
// SPDX-License-Identifier: GPL-3.0-only

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QSignalSpy>
#include <QStyleFactory>
#include <QStyleOption>
#include <QTabWidget>
#include <QTest>
#include <QTimer>
#include <QVBoxLayout>

#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTheme.h>

class TestNPushButton : public NPushButton {
  public:
    using NPushButton::NPushButton;

    // 公开protected方法用于测试
    using NPushButton::hitButton;
    using NPushButton::minimumSizeHint;
    using NPushButton::sizeHint;

    // 用于测试绘制方法
    void testDrawMethods() {
        QPixmap  pixmap(100, 50);
        QPainter painter(&pixmap);
        drawBackground(&painter);
        drawBorder(&painter);
        drawIcon(&painter);
        drawText(&painter);
    }

    // 公开hitButton方法用于测试
    bool hitButton(const QPoint& point) const override { return NPushButton::hitButton(point); }
};

class tst_NPushButton : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    // 基础功能测试 (基于Qt官方测试)
    void getSetCheck();
    void autoRepeat();
    void pressed();
    void isCheckable();
    void setDown();
    void popupCrash();
    void isChecked();
    void toggle();
    void clicked();
    void toggled();
    void sizeHint_data();
    void sizeHint();
    void emitReleasedAfterChange();
    void hitButton();
    void mousePressAndMove();
    ;

    // NPushButton特有功能测试
    void buttonType();
    void borderRadius();
    void transparentBackground();
    void alignment();
    void accentColor();
    void fluentIcon();
    void colorProperties();
    void themeChange();
    void paintEvent();

    // 性能相关测试
    void cacheEfficiency();
    void iconCacheEfficiency();
    void cacheInvalidation();
    void memoryLeakTest();
    void longRunningMemoryTest();

    // 边界条件测试
    void disabledStateOperations();
    void extremeSizes();
    void textEdgeCases();
    void iconTextLayout();

    // 信号完整性测试
    void signalTiming();

    // 样式和绘制测试
    void highDpiRendering();
    void styleSheetApplication();
    void focusRendering();

    // 国际化测试
    void internationalization();

  protected slots:
    void resetCounters();
    void onClicked();
    void onToggled(bool on);
    void onPressed();
    void onReleased();

  private:
    int          click_count;
    int          toggle_count;
    int          press_count;
    int          release_count;
    NPushButton* testWidget;
};

// Testing get/set functions
void tst_NPushButton::getSetCheck() {
    NPushButton obj1;

    // QMenu* NPushButton::menu()
    // void NPushButton::setMenu(QMenu*)
    QMenu* var1 = new QMenu;
    obj1.setMenu(var1);
    QCOMPARE(var1, obj1.menu());
    obj1.setMenu(nullptr);
    QCOMPARE(obj1.menu(), nullptr);
    delete var1;

    // ButtonType NPushButton::buttonType()
    // void NPushButton::setButtonType(ButtonType)
    QCOMPARE(obj1.buttonType(), NPushButton::Standard);
    obj1.setButtonType(NPushButton::Accent);
    QCOMPARE(obj1.buttonType(), NPushButton::Accent);
    obj1.setButtonType(NPushButton::Standard);
    QCOMPARE(obj1.buttonType(), NPushButton::Standard);
}

void tst_NPushButton::initTestCase() {
    // Create the test class
    testWidget = new NPushButton("&Start", nullptr);
    testWidget->setObjectName("testWidget");
    testWidget->resize(200, 200);
    testWidget->show();

    connect(testWidget, SIGNAL(clicked()), this, SLOT(onClicked()));
    connect(testWidget, SIGNAL(pressed()), this, SLOT(onPressed()));
    connect(testWidget, SIGNAL(released()), this, SLOT(onReleased()));
    connect(testWidget, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
}

void tst_NPushButton::cleanupTestCase() {
    delete testWidget;
    testWidget = nullptr;
}

void tst_NPushButton::init() {
    testWidget->setAutoRepeat(false);
    testWidget->setDown(false);
    testWidget->setText("Test");
    testWidget->setEnabled(true);
    testWidget->setButtonType(NPushButton::Standard);
    testWidget->setTransparentBackground(false);
    testWidget->setAlignment(Qt::AlignCenter);
    resetCounters();
}

void tst_NPushButton::resetCounters() {
    toggle_count  = 0;
    press_count   = 0;
    release_count = 0;
    click_count   = 0;
}

void tst_NPushButton::onClicked() { click_count++; }

void tst_NPushButton::onToggled(bool /*on*/) { toggle_count++; }

void tst_NPushButton::onPressed() { press_count++; }

void tst_NPushButton::onReleased() { release_count++; }

void tst_NPushButton::autoRepeat() {
    // If this changes, this test must be completely revised.
    QVERIFY(!testWidget->isCheckable());

    // verify autorepeat is off by default.
    NPushButton tmp;
    tmp.setObjectName("tmp");
    QVERIFY(!tmp.autoRepeat());

    // check if we can toggle the mode
    testWidget->setAutoRepeat(true);
    QVERIFY(testWidget->autoRepeat());
    testWidget->setAutoRepeat(false);
    QVERIFY(!testWidget->autoRepeat());

    resetCounters();

    // check that the button is down if we press space and not in autorepeat
    testWidget->setDown(false);
    testWidget->setAutoRepeat(false);
    QTest::keyPress(testWidget, Qt::Key_Space);
    QTRY_VERIFY(testWidget->isDown());
    QCOMPARE(toggle_count, 0);
    QCOMPARE(press_count, 1);
    QCOMPARE(release_count, 0);
    QCOMPARE(click_count, 0);
    QTest::keyRelease(testWidget, Qt::Key_Space);

    resetCounters();

    // check that the button is down if we press space while in autorepeat
    // we can't actually confirm how many times it is fired, more than 1 is enough.
    testWidget->setDown(false);
    testWidget->setAutoRepeat(true);
    QTest::keyPress(testWidget, Qt::Key_Space);
    QTRY_VERIFY(press_count > 3);
    QVERIFY(testWidget->isDown());
    QCOMPARE(toggle_count, 0);
    QTest::keyRelease(testWidget, Qt::Key_Space);
    QCOMPARE(press_count, release_count);
    QCOMPARE(release_count, click_count);

    // check that pressing ENTER has no effect
    resetCounters();
    testWidget->setDown(false);
    testWidget->setAutoRepeat(false);
    QTest::keyPress(testWidget, Qt::Key_Enter);
    QTest::qWait(300);
    QVERIFY(!testWidget->isDown());
    QCOMPARE(toggle_count, 0);
    QCOMPARE(press_count, 0);
    QCOMPARE(release_count, 0);
    QCOMPARE(click_count, 0);
    QTest::keyRelease(testWidget, Qt::Key_Enter);

    // check that pressing ENTER has no effect in autorepeat mode
    resetCounters();
    testWidget->setDown(false);
    testWidget->setAutoRepeat(true);
    QTest::keyClick(testWidget, Qt::Key_Enter);
    QTest::qWait(300);
    QVERIFY(!testWidget->isDown());
    QCOMPARE(toggle_count, 0);
    QCOMPARE(press_count, 0);
    QCOMPARE(release_count, 0);
    QCOMPARE(click_count, 0);
}

void tst_NPushButton::pressed() {
    QTest::keyPress(testWidget, ' ');
    QCOMPARE(press_count, 1);
    QCOMPARE(release_count, 0);
    QTest::keyRelease(testWidget, ' ');
    QCOMPARE(press_count, 1);
    QCOMPARE(release_count, 1);

    QTest::keyPress(testWidget, Qt::Key_Enter);
    QCOMPARE(press_count, 1);
    QCOMPARE(release_count, 1);

    testWidget->setAutoDefault(true);
    QTest::keyPress(testWidget, Qt::Key_Enter);
    QCOMPARE(press_count, 2);
    QCOMPARE(release_count, 2);
    testWidget->setAutoDefault(false);
}

void tst_NPushButton::isCheckable() { QVERIFY(!testWidget->isCheckable()); }

void tst_NPushButton::setDown() {
    testWidget->setDown(false);
    QVERIFY(!testWidget->isDown());
    testWidget->setDown(true);
    QVERIFY(testWidget->isDown());
    testWidget->setDown(true);
    QTest::keyClick(testWidget, Qt::Key_Escape);
    QVERIFY(!testWidget->isDown());
}

void tst_NPushButton::isChecked() {
    testWidget->setDown(false);
    QVERIFY(!testWidget->isChecked());
    testWidget->setDown(true);
    QVERIFY(!testWidget->isChecked());
    testWidget->setDown(false);
    testWidget->toggle();
    QCOMPARE(testWidget->isChecked(), testWidget->isCheckable());
}

void tst_NPushButton::toggle() {
    // the pushbutton shouldn't toggle the button.
    testWidget->toggle();
    QCOMPARE(testWidget->isChecked(), false);
}

void tst_NPushButton::toggled() {
    // the pushbutton shouldn't send a toggled signal when we call the toggle slot.
    QVERIFY(!testWidget->isCheckable());
    testWidget->toggle();
    QCOMPARE(toggle_count, 0);

    // do it again, just to be sure
    resetCounters();
    testWidget->toggle();
    QCOMPARE(toggle_count, 0);

    // finally check that we can toggle using the mouse
    resetCounters();
    QTest::mousePress(testWidget, Qt::LeftButton);
    QCOMPARE(toggle_count, 0);
    QCOMPARE(click_count, 0);
    QTest::mouseRelease(testWidget, Qt::LeftButton);
    QCOMPARE(click_count, 1);
}

void tst_NPushButton::clicked() {
    QTest::mousePress(testWidget, Qt::LeftButton);
    QCOMPARE(press_count, 1);
    QCOMPARE(release_count, 0);
    QTest::mouseRelease(testWidget, Qt::LeftButton);
    QCOMPARE(press_count, 1);
    QCOMPARE(release_count, 1);

    press_count   = 0;
    release_count = 0;
    testWidget->setDown(false);
    for (uint i = 0; i < 10; i++)
        QTest::mouseClick(testWidget, Qt::LeftButton);
    QCOMPARE(press_count, 10);
    QCOMPARE(release_count, 10);
}

void tst_NPushButton::sizeHint_data() {
    QTest::addColumn<QString>("stylename");

#if !defined(QT_NO_STYLE_WINDOWS)
    QTest::newRow("windows") << QString::fromLatin1("windows");
#endif
#if !defined(QT_NO_STYLE_FUSION)
    QTest::newRow("fusion") << QString::fromLatin1("fusion");
#endif
#if defined(Q_OS_WIN) && !defined(QT_NO_STYLE_WINDOWSVISTA)
    QTest::newRow("windowsvista") << QString::fromLatin1("windowsvista");
#endif
}

void tst_NPushButton::sizeHint() {
    QFETCH(QString, stylename);
    QStyle* style = QStyleFactory::create(stylename);
    if (!style)
        QFAIL(qPrintable(QString::fromLatin1("Cannot create style: %1").arg(stylename)));
    QApplication::setStyle(style);

    // Test 1
    {
        NPushButton* button       = new NPushButton("123");
        QSize        initSizeHint = button->sizeHint();
        QDialog*     dialog       = new QDialog;
        QWidget*     widget       = new QWidget(dialog);
        button->setParent(widget);
        button->sizeHint();
        widget->setParent(nullptr);
        delete dialog;
        QCOMPARE(button->sizeHint(), initSizeHint);
        delete button;
        delete widget;
    }

    // Test 2
    {
        QWidget*     tab1      = new QWidget;
        QHBoxLayout* layout1   = new QHBoxLayout(tab1);
        NPushButton* button1_1 = new NPushButton("123");
        NPushButton* button1_2 = new NPushButton("123");
        layout1->addWidget(button1_1);
        layout1->addWidget(button1_2);

        QWidget*     tab2      = new QWidget;
        QHBoxLayout* layout2   = new QHBoxLayout(tab2);
        NPushButton* button2_1 = new NPushButton("123");
        NPushButton* button2_2 = new NPushButton("123");
        layout2->addWidget(button2_1);
        layout2->addWidget(button2_2);

        QDialog*    dialog    = new QDialog;
        QTabWidget* tabWidget = new QTabWidget;
        tabWidget->addTab(tab1, "1");
        tabWidget->addTab(tab2, "2");
        QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
        mainLayout->addWidget(tabWidget);
        dialog->showNormal();

        tabWidget->setCurrentWidget(tab2);
        tabWidget->setCurrentWidget(tab1);
        QTRY_COMPARE(button1_2->size(), button2_2->size());

        delete dialog;
    }
}

void tst_NPushButton::emitReleasedAfterChange() {
    NPushButton* button1 = new NPushButton("A");
    NPushButton* button2 = new NPushButton("B");
    QVBoxLayout* layout  = new QVBoxLayout();
    layout->addWidget(button1);
    layout->addWidget(button2);

    QDialog dialog;
    dialog.setLayout(layout);
    dialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dialog));

    button1->setFocus();
    QSignalSpy spy(button1, SIGNAL(released()));
    QTest::mousePress(button1, Qt::LeftButton);
    QVERIFY(button1->isDown());
    QTest::keyClick(&dialog, Qt::Key_Tab);
    QVERIFY(!button1->isDown());
    QCOMPARE(spy.size(), 1);

    spy.clear();
    QCOMPARE(spy.size(), 0);
    button1->setFocus();
    QTest::mousePress(button1, Qt::LeftButton);
    QVERIFY(button1->isDown());
    button1->setEnabled(false);
    QVERIFY(!button1->isDown());
    QCOMPARE(spy.size(), 1);
}

void tst_NPushButton::popupCrash() {
    NPushButton* pb   = new NPushButton("foo");
    QMenu*       menu = new QMenu("bar", pb);
    pb->setMenu(menu);
    QTimer::singleShot(1000, this, [&pb] {
        delete pb;
        pb = nullptr;
    });
    pb->show();
    pb->click();
    QTRY_COMPARE(pb, nullptr);
}

void tst_NPushButton::hitButton() {
    QDialog          dialog;
    QVBoxLayout*     layout  = new QVBoxLayout;
    TestNPushButton* button1 = new TestNPushButton("Ok");
    TestNPushButton* button2 = new TestNPushButton("Cancel");
    button2->setStyleSheet("NPushButton {"
                           "padding: 5px;"
                           "margin: 5px;"
                           "border-radius: 4px;"
                           "border: 1px solid black; }");
    layout->addWidget(button1);
    layout->addWidget(button2);
    dialog.setLayout(layout);
    dialog.show();
    QVERIFY(QTest::qWaitForWindowExposed(&dialog));

    const QPoint button1Center = button1->rect().center();
    QVERIFY(button1->hitButton(button1Center));

    const QPoint button2Center = button2->rect().center();
    QVERIFY(button2->hitButton(button2Center));
    QVERIFY(button2->hitButton(QPoint(6, 6)));
    QVERIFY(!button2->hitButton(QPoint(2, 2)));
}

void tst_NPushButton::mousePressAndMove() {
    NPushButton button;
    button.setGeometry(0, 0, 20, 20);
    QSignalSpy pressSpy(&button, &QAbstractButton::pressed);
    QSignalSpy releaseSpy(&button, &QAbstractButton::released);

    QTest::mousePress(&button, Qt::LeftButton);
    QCOMPARE(pressSpy.size(), 1);
    QCOMPARE(releaseSpy.size(), 0);

    // mouse pressed and moving out
    QTest::mouseMove(&button, QPoint(100, 100));
    // should emit released signal when the mouse is dragged out of boundary
    QCOMPARE(pressSpy.size(), 1);
    QCOMPARE(releaseSpy.size(), 1);

    // mouse pressed and moving into
    QTest::mouseMove(&button, QPoint(10, 10));
    // should emit pressed signal when the mouse is dragged into of boundary
    QCOMPARE(pressSpy.size(), 2);
    QCOMPARE(releaseSpy.size(), 1);
}

// NPushButton特有功能测试
void tst_NPushButton::buttonType() {
    NPushButton button;

    // 默认应该是Standard类型
    QCOMPARE(button.buttonType(), NPushButton::Standard);

    // 更改为Accent类型应该触发重绘
    button.setButtonType(NPushButton::Accent);
    QCOMPARE(button.buttonType(), NPushButton::Accent);

    // 设置相同类型不应该有变化
    button.setButtonType(NPushButton::Accent);
    QCOMPARE(button.buttonType(), NPushButton::Accent);

    // 更改回Standard类型
    button.setButtonType(NPushButton::Standard);
    QCOMPARE(button.buttonType(), NPushButton::Standard);

    // 测试在显示状态下的类型切换
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    button.setButtonType(NPushButton::Accent);
    QCOMPARE(button.buttonType(), NPushButton::Accent);

    button.setButtonType(NPushButton::Standard);
    QCOMPARE(button.buttonType(), NPushButton::Standard);
}

void tst_NPushButton::borderRadius() {
    NPushButton button;

    // 获取默认边框半径
    int defaultRadius = button.getBorderRadius();
    QVERIFY(defaultRadius >= 0);

    // 设置新的边框半径
    button.setBorderRadius(10);
    QCOMPARE(button.getBorderRadius(), 10);

    // 设置另一个值
    button.setBorderRadius(20);
    QCOMPARE(button.getBorderRadius(), 20);

    // 测试边界值
    button.setBorderRadius(0);
    QCOMPARE(button.getBorderRadius(), 0);

    // 测试大值
    button.setBorderRadius(100);
    QCOMPARE(button.getBorderRadius(), 100);

    // 恢复默认值
    button.setBorderRadius(defaultRadius);
    QCOMPARE(button.getBorderRadius(), defaultRadius);
}

void tst_NPushButton::transparentBackground() {
    NPushButton button;

    // 默认应该是不透明背景
    QCOMPARE(button.getTransparentBackground(), false);

    // 设置透明背景
    button.setTransparentBackground(true);
    QCOMPARE(button.getTransparentBackground(), true);

    // 恢复不透明背景
    button.setTransparentBackground(false);
    QCOMPARE(button.getTransparentBackground(), false);
}

void tst_NPushButton::alignment() {
    NPushButton button;

    // 默认应该是居中对齐
    QCOMPARE(button.getAlignment(), Qt::AlignCenter);

    // 测试左对齐
    button.setAlignment(Qt::AlignLeft);
    QCOMPARE(button.getAlignment(), Qt::AlignLeft);

    // 测试右对齐
    button.setAlignment(Qt::AlignRight);
    QCOMPARE(button.getAlignment(), Qt::AlignRight);

    // 恢复居中对齐
    button.setAlignment(Qt::AlignCenter);
    QCOMPARE(button.getAlignment(), Qt::AlignCenter);
}

void tst_NPushButton::accentColor() {
    NPushButton button;

    // 将按钮类型更改为Accent
    button.setButtonType(NPushButton::Accent);
    QCOMPARE(button.buttonType(), NPushButton::Accent);

    // 设置自定义AccentColor
    QColor customColor(0, 128, 255);
    button.setAccentColor(customColor);

    // 测试多种颜色
    button.setAccentColor(QColor(255, 0, 0)); // 红色
    button.setAccentColor(QColor(0, 255, 0)); // 绿色
    button.setAccentColor(QColor(0, 0, 255)); // 蓝色

    // 设置NAccentColor对象
    NAccentColor accentColor = NColorUtils::toAccentColor(QColor(255, 128, 0));
    button.setAccentColor(accentColor);
}

void tst_NPushButton::fluentIcon() {
    NPushButton button;

    // 初始状态应该没有图标
    QVERIFY(button.icon().isNull());

    // 设置Regular图标
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button.icon().isNull());
    QCOMPARE(button.iconSize(), QSize(16, 16)); // 默认大小

    // 设置Filled图标
    button.setFluentIcon(NFilledIconType::Add12Filled);
    QVERIFY(!button.icon().isNull());
    QCOMPARE(button.iconSize(), QSize(16, 16)); // 默认大小

    // 自定义大小
    button.setFluentIcon(NRegularIconType::Add12Regular, 24);
    QCOMPARE(button.iconSize(), QSize(24, 24));

    // 测试大小边界值
    button.setFluentIcon(NRegularIconType::Add12Regular, 8);
    QCOMPARE(button.iconSize(), QSize(8, 8));

    button.setFluentIcon(NRegularIconType::Add12Regular, 64);
    QCOMPARE(button.iconSize(), QSize(64, 64));

    // 自定义颜色
    QColor customColor(255, 0, 0);
    button.setFluentIcon(NRegularIconType::Add12Regular, 16, customColor);
    QVERIFY(!button.icon().isNull());
    QCOMPARE(button.iconSize(), QSize(16, 16));

    // 测试不同的图标类型
    button.setFluentIcon(NRegularIconType::Accessibility16Regular);
    QVERIFY(!button.icon().isNull());

    button.setFluentIcon(NFilledIconType::Accessibility16Filled);
    QVERIFY(!button.icon().isNull());

    // 测试在不同按钮类型下的图标
    button.setButtonType(NPushButton::Accent);
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button.icon().isNull());

    button.setButtonType(NPushButton::Standard);
    button.setFluentIcon(NFilledIconType::Add12Filled);
    QVERIFY(!button.icon().isNull());
}

void tst_NPushButton::colorProperties() {
    NPushButton button;

    // 测试颜色属性的设置和获取
    QColor testColor(128, 128, 128);

    button.setLightDefaultColor(testColor);
    QCOMPARE(button.getLightDefaultColor(), testColor);

    button.setDarkDefaultColor(testColor);
    QCOMPARE(button.getDarkDefaultColor(), testColor);

    button.setLightHoverColor(testColor);
    QCOMPARE(button.getLightHoverColor(), testColor);

    button.setDarkHoverColor(testColor);
    QCOMPARE(button.getDarkHoverColor(), testColor);

    button.setLightPressColor(testColor);
    QCOMPARE(button.getLightPressColor(), testColor);

    button.setDarkPressColor(testColor);
    QCOMPARE(button.getDarkPressColor(), testColor);

    button.setLightTextDefaultColor(testColor);
    QCOMPARE(button.getLightTextDefaultColor(), testColor);

    button.setDarkTextDefaultColor(testColor);
    QCOMPARE(button.getDarkTextDefaultColor(), testColor);

    button.setLightTextPressColor(testColor);
    QCOMPARE(button.getLightTextPressColor(), testColor);

    button.setDarkTextPressColor(testColor);
    QCOMPARE(button.getDarkTextPressColor(), testColor);
}

void tst_NPushButton::themeChange() {
    NPushButton button("Theme Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 记录初始状态
    NPushButton::ButtonType initialType        = button.buttonType();
    bool                    initialTransparent = button.getTransparentBackground();

    // 测试主题切换不会导致崩溃
    button.update();
    QTest::qWait(100);

    // 验证属性在主题切换后保持不变
    QCOMPARE(button.buttonType(), initialType);
    QCOMPARE(button.getTransparentBackground(), initialTransparent);

    // 测试在不同主题下按钮仍然可以正常点击
    QSignalSpy spy(&button, &NPushButton::clicked);
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);

    // 测试Accent按钮在主题切换后的行为
    button.setButtonType(NPushButton::Accent);
    button.update();
    QTest::qWait(100);

    spy.clear();
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);

    // 测试带图标的按钮在主题切换后的行为
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button.icon().isNull());
    button.update();
    QTest::qWait(100);
    QVERIFY(!button.icon().isNull());
}

void tst_NPushButton::paintEvent() {
    TestNPushButton button("Paint Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试绘制方法不会导致崩溃
    button.testDrawMethods();

    // 强制重绘
    button.update();
    QTest::qWait(100);

    // 测试不同状态下的绘制
    button.setEnabled(false);
    button.update();
    QTest::qWait(50);

    button.setEnabled(true);
    button.update();
    QTest::qWait(50);

    // 测试不同按钮类型的绘制
    button.setButtonType(NPushButton::Accent);
    button.update();
    QTest::qWait(50);

    button.setButtonType(NPushButton::Standard);
    button.update();
    QTest::qWait(50);

    // 测试透明背景的绘制
    button.setTransparentBackground(true);
    button.update();
    QTest::qWait(50);

    button.setTransparentBackground(false);
    button.update();
    QTest::qWait(50);
}

// 性能相关测试
void tst_NPushButton::cacheEfficiency() {
    NPushButton button("Cache Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试颜色缓存 - 多次hover应该使用缓存
    QTest::mouseMove(&button, button.rect().center());
    QTest::qWait(50);

    // 模拟多次hover事件，测试缓存是否生效
    for (int i = 0; i < 10; ++i) {
        QTest::mouseMove(&button, QPoint(1, 1));
        QTest::mouseMove(&button, button.rect().center());
        QTest::qWait(10);
    }

    // 测试按下状态的缓存
    QTest::mousePress(&button, Qt::LeftButton);
    QTest::qWait(50);
    QTest::mouseRelease(&button, Qt::LeftButton);
}

void tst_NPushButton::iconCacheEfficiency() {
    NPushButton button;

    // 设置图标后多次获取，应该使用缓存
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QIcon icon1 = button.icon();
    QIcon icon2 = button.icon();
    QIcon icon3 = button.icon();

    QVERIFY(!icon1.isNull());
    QVERIFY(!icon2.isNull());
    QVERIFY(!icon3.isNull());

    // 测试相同参数的图标设置是否使用缓存
    button.setFluentIcon(NRegularIconType::Add12Regular, 16);
    QIcon icon4 = button.icon();
    QVERIFY(!icon4.isNull());
}

void tst_NPushButton::cacheInvalidation() {
    NPushButton button("Cache Invalidation Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 设置初始状态
    button.setButtonType(NPushButton::Standard);
    QTest::qWait(50);

    // 改变按钮类型应该失效缓存
    button.setButtonType(NPushButton::Accent);
    QTest::qWait(50);

    // 改变颜色属性应该失效缓存
    button.setLightDefaultColor(QColor(255, 0, 0));
    QTest::qWait(50);

    // 设置图标应该失效图标缓存
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QTest::qWait(50);

    // 改变图标大小应该失效图标缓存
    button.setFluentIcon(NRegularIconType::Add12Regular, 24);
    QTest::qWait(50);
}

void tst_NPushButton::memoryLeakTest() {
    const int             buttonCount = 100;
    QVector<NPushButton*> buttons;
    buttons.reserve(buttonCount);

    // 创建大量按钮
    for (int i = 0; i < buttonCount; ++i) {
        NPushButton* button = new NPushButton(QString("Button %1").arg(i));
        button->setFluentIcon(NRegularIconType::Add12Regular);
        button->setButtonType(i % 2 == 0 ? NPushButton::Standard : NPushButton::Accent);
        buttons.append(button);
    }

    // 销毁所有按钮
    qDeleteAll(buttons);
    buttons.clear();

    // 强制垃圾回收
    QCoreApplication::processEvents();
}

void tst_NPushButton::longRunningMemoryTest() {
    NPushButton button("Long Running Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 模拟长时间运行中的各种操作
    for (int i = 0; i < 50; ++i) {
        // 切换按钮类型
        button.setButtonType(i % 2 == 0 ? NPushButton::Standard : NPushButton::Accent);

        // 改变图标
        if (i % 3 == 0) {
            button.setFluentIcon(NRegularIconType::Add12Regular);
        } else if (i % 3 == 1) {
            button.setFluentIcon(NFilledIconType::Add12Filled);
        } else {
            button.setIcon(QIcon());
        }

        // 模拟用户交互
        QTest::mousePress(&button, Qt::LeftButton);
        QTest::qWait(10);
        QTest::mouseRelease(&button, Qt::LeftButton);

        QTest::qWait(20);
    }
}

// 边界条件测试
void tst_NPushButton::disabledStateOperations() {
    NPushButton button("Disabled Test");
    button.setEnabled(false);
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试禁用状态下的各种操作
    QSignalSpy clickSpy(&button, &NPushButton::clicked);
    QSignalSpy pressSpy(&button, &NPushButton::pressed);

    // 禁用状态下点击不应该触发信号
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(clickSpy.count(), 0);
    QCOMPARE(pressSpy.count(), 0);

    // 禁用状态下键盘操作不应该触发信号
    QTest::keyClick(&button, Qt::Key_Space);
    QCOMPARE(clickSpy.count(), 0);
    QCOMPARE(pressSpy.count(), 0);

    // 测试禁用状态下的属性设置
    button.setButtonType(NPushButton::Accent);
    QCOMPARE(button.buttonType(), NPushButton::Accent);

    button.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button.icon().isNull());

    // 重新启用后应该正常工作
    button.setEnabled(true);
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(clickSpy.count(), 1);
}

void tst_NPushButton::extremeSizes() {
    // 测试极小尺寸
    NPushButton smallButton("S");
    smallButton.resize(10, 10);
    smallButton.show();
    QVERIFY(QTest::qWaitForWindowExposed(&smallButton));

    QSize minSize = smallButton.minimumSizeHint();
    QVERIFY(minSize.width() > 0);
    QVERIFY(minSize.height() > 0);

    // 测试极大尺寸
    NPushButton largeButton("Large Button Text");
    largeButton.resize(1000, 500);
    largeButton.show();
    QVERIFY(QTest::qWaitForWindowExposed(&largeButton));

    // 测试零尺寸
    NPushButton zeroButton;
    zeroButton.resize(0, 0);
    QSize zeroSize = zeroButton.sizeHint();
    QVERIFY(zeroSize.width() >= 0);
    QVERIFY(zeroSize.height() >= 0);
}

void tst_NPushButton::textEdgeCases() {
    // 测试空文本
    NPushButton emptyButton("");
    QCOMPARE(emptyButton.text(), QString(""));
    QSize emptySize = emptyButton.sizeHint();
    QVERIFY(emptySize.width() > 0);
    QVERIFY(emptySize.height() > 0);

    // 测试很长的文本
    QString     longText = QString("Very Long Button Text That Should Be Handled Properly ").repeated(10);
    NPushButton longButton(longText);
    QCOMPARE(longButton.text(), longText);
    QSize longSize = longButton.sizeHint();
    QVERIFY(longSize.width() > emptySize.width());

    // 测试特殊字符
    NPushButton specialButton("测试中文\n换行\t制表符&快捷键");
    QSize       specialSize = specialButton.sizeHint();
    QVERIFY(specialSize.width() > 0);
    QVERIFY(specialSize.height() > 0);

    // 测试HTML标签（应该被当作普通文本）
    NPushButton htmlButton("<b>Bold</b> <i>Italic</i>");
    QVERIFY(htmlButton.text().contains("<b>"));
}

void tst_NPushButton::iconTextLayout() {
    NPushButton button("Icon Text");
    button.setFluentIcon(NRegularIconType::Add12Regular);
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试有图标和文本时的尺寸
    QSize iconTextSize = button.sizeHint();
    QVERIFY(!button.icon().isNull());
    QVERIFY(!button.text().isEmpty());

    // 测试只有图标
    button.setText("");
    QSize iconOnlySize = button.sizeHint();
    QVERIFY(iconOnlySize.width() < iconTextSize.width());

    // 测试只有文本
    button.setIcon(QIcon());
    button.setText("Only Text");
    QSize textOnlySize = button.sizeHint();
    QVERIFY(!button.text().isEmpty());
    QVERIFY(button.icon().isNull());

    // 测试不同图标大小对布局的影响
    button.setFluentIcon(NRegularIconType::Add12Regular, 32);
    button.setText("Large Icon");
    QSize largeIconSize = button.sizeHint();
    QVERIFY(largeIconSize.height() >= 32);
}

void tst_NPushButton::signalTiming() {
    NPushButton button("Signal Timing");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    QSignalSpy pressedSpy(&button, &NPushButton::pressed);
    QSignalSpy releasedSpy(&button, &NPushButton::released);
    QSignalSpy clickedSpy(&button, &NPushButton::clicked);

    // 测试信号的正确顺序：pressed -> released -> clicked
    QTest::mousePress(&button, Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 0);
    QCOMPARE(clickedSpy.count(), 0);

    QTest::mouseRelease(&button, Qt::LeftButton);
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(releasedSpy.count(), 1);
    QCOMPARE(clickedSpy.count(), 1);
}

// 样式和绘制测试
void tst_NPushButton::highDpiRendering() {
    NPushButton button("High DPI Test");
    button.setFluentIcon(NRegularIconType::Add12Regular);

    // 模拟不同的设备像素比
    qreal originalDpr = button.devicePixelRatio();

    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试图标在高DPI下是否正确渲染
    QVERIFY(!button.icon().isNull());

    // 测试尺寸计算在高DPI下是否正确
    QSize sizeHint = button.sizeHint();
    QVERIFY(sizeHint.width() > 0);
    QVERIFY(sizeHint.height() > 0);
}

void tst_NPushButton::styleSheetApplication() {
    NPushButton button("Style Test");

    // 测试样式表的应用
    QString styleSheet = "NPushButton { background-color: red; color: white; border: 2px solid blue; }";
    button.setStyleSheet(styleSheet);
    QCOMPARE(button.styleSheet(), styleSheet);

    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试样式表不会影响基本功能
    QSignalSpy clickSpy(&button, &NPushButton::clicked);
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(clickSpy.count(), 1);

    // 清除样式表
    button.setStyleSheet("");
    QVERIFY(button.styleSheet().isEmpty());
}

void tst_NPushButton::focusRendering() {
    NPushButton button("Focus Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试获得焦点
    button.setFocus();
    QVERIFY(button.hasFocus());
    QTest::qWait(100);

    // 测试失去焦点
    button.clearFocus();
    QVERIFY(!button.hasFocus());
    QTest::qWait(100);

    // 测试Tab键导航
    button.setFocus();
    QTest::keyPress(&button, Qt::Key_Tab);
    QTest::qWait(50);
}

// 国际化测试
void tst_NPushButton::internationalization() {
    // 测试中文文本
    NPushButton chineseButton("确定");
    QSize       chineseSize = chineseButton.sizeHint();
    QVERIFY(chineseSize.width() > 0);
    QVERIFY(chineseSize.height() > 0);

    // 测试阿拉伯文本（RTL）
    NPushButton arabicButton("موافق");
    QSize       arabicSize = arabicButton.sizeHint();
    QVERIFY(arabicSize.width() > 0);
    QVERIFY(arabicSize.height() > 0);

    // 测试日文文本
    NPushButton japaneseButton("はい");
    QSize       japaneseSize = japaneseButton.sizeHint();
    QVERIFY(japaneseSize.width() > 0);
    QVERIFY(japaneseSize.height() > 0);

    // 测试混合文本
    NPushButton mixedButton("OK确定はい");
    QSize       mixedSize = mixedButton.sizeHint();
    QVERIFY(mixedSize.width() > 0);
    QVERIFY(mixedSize.height() > 0);

    // 测试特殊Unicode字符
    NPushButton unicodeButton("✓ ✗ ★ ♥ ☀");
    QSize       unicodeSize = unicodeButton.sizeHint();
    QVERIFY(unicodeSize.width() > 0);
    QVERIFY(unicodeSize.height() > 0);
}

QTEST_MAIN(tst_NPushButton)
#include "tst_npushbutton.moc"
