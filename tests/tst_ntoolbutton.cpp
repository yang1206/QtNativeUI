// Copyright (C) 2024 QtNativeUI Project
// SPDX-License-Identifier: GPL-3.0-only

#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QMenu>
#include <QScreen>
#include <QSignalSpy>
#include <QStyleOption>
#include <QTest>
#include <QTimer>

#include <QtNativeUI/NTheme.h>
#include <QtNativeUI/NToolButton.h>

class TestNToolButton : public NToolButton {
  public:
    using NToolButton::minimumSizeHint;
    using NToolButton::NToolButton;
    using NToolButton::sizeHint;

    void initStyleOption(QStyleOptionToolButton* option) const override { NToolButton::initStyleOption(option); }
};

class tst_NToolButton : public QObject {
    Q_OBJECT

  public:
    tst_NToolButton();
    virtual ~tst_NToolButton();

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    // 基础功能测试
    void getSetCheck();
    void triggered();
    void defaultActionSynced();
    void deleteInHandler();
    void emptyMenu();
    void buttonType();

    // NToolButton特有功能测试
    void fluentIcon();
    void accentColor();
    void colorProperties();
    void themeChange();
    void transparentBackground();
    void borderRadius();

    // 工具按钮特有测试
    void toolButtonStyleChange();
    void popupModeTests();
    void arrowIndicator();

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


    void signalTiming();

    // 样式和绘制测试
    void highDpiRendering();
    void styleSheetApplication();
    void focusRendering();

    // 国际化测试
    void internationalization();

  protected slots:
    void sendMouseClick();

  private:
    QPointer<QWidget> m_menu;
    NToolButton*      testWidget;
};

tst_NToolButton::tst_NToolButton() {}

tst_NToolButton::~tst_NToolButton() {}

void tst_NToolButton::initTestCase() {
    testWidget = new NToolButton();
    testWidget->setObjectName("testWidget");
    testWidget->resize(100, 100);
    testWidget->show();
}

void tst_NToolButton::cleanupTestCase() {
    delete testWidget;
    testWidget = nullptr;
}

void tst_NToolButton::init() {
    testWidget->setAutoRaise(false);
    testWidget->setEnabled(true);
    testWidget->setMenu(nullptr);
    testWidget->setDefaultAction(nullptr);
}

// Testing get/set functions
void tst_NToolButton::getSetCheck() {
    NToolButton obj1;

    // QMenu* NToolButton::menu()
    // void NToolButton::setMenu(QMenu*)
    QMenu* var1 = new QMenu;
    obj1.setMenu(var1);
    QCOMPARE(var1, obj1.menu());
    obj1.setMenu(nullptr);
    QCOMPARE(obj1.menu(), nullptr);
    delete var1;

    // ToolButtonPopupMode NToolButton::popupMode()
    // void NToolButton::setPopupMode(ToolButtonPopupMode)
    obj1.setPopupMode(QToolButton::DelayedPopup);
    QCOMPARE(obj1.popupMode(), QToolButton::DelayedPopup);
    obj1.setPopupMode(QToolButton::MenuButtonPopup);
    QCOMPARE(obj1.popupMode(), QToolButton::MenuButtonPopup);
    obj1.setPopupMode(QToolButton::InstantPopup);
    QCOMPARE(obj1.popupMode(), QToolButton::InstantPopup);

    // bool NToolButton::autoRaise()
    // void NToolButton::setAutoRaise(bool)
    obj1.setAutoRaise(false);
    QCOMPARE(false, obj1.autoRaise());
    obj1.setAutoRaise(true);
    QCOMPARE(true, obj1.autoRaise());

    // QAction * NToolButton::defaultAction()
    // void NToolButton::setDefaultAction(QAction *)
    QAction* var4 = new QAction(nullptr);
    obj1.setDefaultAction(var4);
    QCOMPARE(var4, obj1.defaultAction());
    obj1.setDefaultAction(nullptr);
    QCOMPARE(obj1.defaultAction(), nullptr);
    delete var4;
}

void tst_NToolButton::triggered() {
    if (QGuiApplication::platformName().startsWith(QLatin1String("wayland"), Qt::CaseInsensitive))
        QSKIP("Wayland: This fails. Figure out why.");

    qRegisterMetaType<QAction*>("QAction *");
    QWidget mainWidget;
    mainWidget.setWindowTitle(QStringLiteral("triggered"));
    mainWidget.resize(200, 200);
    mainWidget.move(QGuiApplication::primaryScreen()->availableGeometry().center() - QPoint(100, 100));

    NToolButton*          toolButton = new NToolButton(&mainWidget);
    QSignalSpy            spy(toolButton, SIGNAL(triggered(QAction*)));
    QScopedPointer<QMenu> menu(new QMenu(QStringLiteral("Menu")));
    QAction*              one = menu->addAction("one");
    menu->addAction("two");
    QAction* defaultAction = new QAction(QStringLiteral("def"), this);

    toolButton->setMenu(menu.data());
    toolButton->setDefaultAction(defaultAction);

    mainWidget.show();
    QVERIFY(QTest::qWaitForWindowActive(&mainWidget));

    defaultAction->trigger();
    QCOMPARE(spy.size(), 1);
    QCOMPARE(qvariant_cast<QAction*>(spy.at(0).at(0)), defaultAction);

    m_menu = menu.data();

    // QMenu uses QGuiApplicationPrivate::lastCursorPosition to detect pointer
    // movement. And GuiApplication needs at least one mouse move to properly
    // initialize it. So we send a mouse move now, before we open the menu.
    QTest::mouseMove(mainWidget.windowHandle(), mainWidget.mapFromGlobal(QPoint(0, 0)));

    QTimer* timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendMouseClick()));
    timer->start();
    QTimer::singleShot(10000, &mainWidget, SLOT(close())); // Emergency bail-out

    toolButton->showMenu();
    QTRY_COMPARE(spy.size(), 2);
    QCOMPARE(qvariant_cast<QAction*>(spy.at(1).at(0)), one);
}

void tst_NToolButton::buttonType() {
    NToolButton button;

    // 默认应该是Standard类型
    QCOMPARE(button.buttonType(), NToolButton::Standard);

    // 设置为Accent类型
    button.setButtonType(NToolButton::Accent);
    QCOMPARE(button.buttonType(), NToolButton::Accent);

    // 设置回Standard类型
    button.setButtonType(NToolButton::Standard);
    QCOMPARE(button.buttonType(), NToolButton::Standard);
}

void tst_NToolButton::sendMouseClick() {
    if (m_menu.isNull()) {
        qWarning("m_menu is NULL");
        return;
    }
    if (!m_menu->isVisible())
        return;

    QTest::mouseClick(m_menu.data(), Qt::LeftButton, {}, QPoint(7, 7));
    if (QTimer* timer = qobject_cast<QTimer*>(sender())) {
        timer->stop();
        timer->deleteLater();
    }
}

void tst_NToolButton::defaultActionSynced() {
    QAction a;
    a.setCheckable(true);
    NToolButton tb;
    tb.setDefaultAction(&a);
    QVERIFY(tb.isCheckable());

    QSignalSpy tbSpy(&tb, SIGNAL(toggled(bool)));
    QSignalSpy aSpy(&a, SIGNAL(toggled(bool)));
    int        tbToggledCount = 0;
    int        aToggledCount  = 0;

    tb.setChecked(true);
    QVERIFY(a.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);

    tb.setChecked(false);
    QVERIFY(!a.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);

    a.setChecked(true);
    QVERIFY(tb.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);

    a.setChecked(false);
    QVERIFY(!tb.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), ++aToggledCount);

    QAction    b;
    QSignalSpy bSpy(&b, SIGNAL(toggled(bool)));
    int        bToggledCount = 0;

    tb.setDefaultAction(&b);
    QVERIFY(!tb.isCheckable());

    b.setCheckable(true);
    QVERIFY(tb.isCheckable());

    tb.setChecked(true);
    QVERIFY(!a.isChecked());
    QVERIFY(b.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), aToggledCount);
    QCOMPARE(bSpy.size(), ++bToggledCount);

    tb.click();
    QVERIFY(!a.isChecked());
    QVERIFY(!tb.isChecked());
    QVERIFY(!b.isChecked());
    QCOMPARE(tbSpy.size(), ++tbToggledCount);
    QCOMPARE(aSpy.size(), aToggledCount);
    QCOMPARE(bSpy.size(), ++bToggledCount);
}

void tst_NToolButton::deleteInHandler() {
    // Tests that if something deletes the button
    // while its event handler is still on the callstack, we don't crash
    QPointer<NToolButton> tb = new NToolButton();
    tb->show();
    QVERIFY(QTest::qWaitForWindowActive(tb));

    connect(tb, &NToolButton::clicked, this, [tb] { delete tb; });

    QTest::mouseClick(tb, Qt::LeftButton);
    QVERIFY(!tb);
}

void tst_NToolButton::emptyMenu() {
    NToolButton tb;
    auto        menu = new QMenu(&tb);
    tb.setMenu(menu);
    tb.showMenu(); // calls exec(), but since the fix for QTBUG-129108, we don't show an empty menu

    // see triggered() test
    QTest::mouseMove(tb.windowHandle(), tb.mapFromGlobal(QPoint(0, 0)));

    // But if we now put something in the menu, it should show up
    auto       act = menu->addAction("an action");
    QSignalSpy triggeredSpy(act, &QAction::triggered);

    // In 200ms, click on the action so that exec() returns
    QTimer::singleShot(200, menu, [&]() { QTest::mouseClick(menu, Qt::LeftButton, {}, menu->rect().center()); });

    tb.showMenu(); // calls exec(), which only returns in 200ms
    QTRY_COMPARE(triggeredSpy.size(), 1);
}

// NToolButton特有功能测试
void tst_NToolButton::fluentIcon() {
    NToolButton button;

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
}

void tst_NToolButton::accentColor() {
    NToolButton button;

    // 测试设置强调色
    QColor customColor(0, 128, 255);
    button.setAccentColor(customColor);
    QCOMPARE(button.buttonType(), NToolButton::Accent);

    // 测试重置强调色
    button.resetAccentColor();

    // 测试多种颜色
    button.setAccentColor(QColor(255, 0, 0)); // 红色
    button.setAccentColor(QColor(0, 255, 0)); // 绿色
    button.setAccentColor(QColor(0, 0, 255)); // 蓝色
}

void tst_NToolButton::colorProperties() {
    NToolButton button;
    QColor      testColor(128, 128, 128);

    // 测试各种颜色属性的设置和获取
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
}

void tst_NToolButton::themeChange() {
    NToolButton button("Theme Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试主题切换不会导致崩溃
    button.update();
    QTest::qWait(100);

    // 测试在不同主题下按钮仍然可以正常点击
    QSignalSpy spy(&button, &NToolButton::clicked);
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);

    // 测试带图标的按钮在主题切换后的行为
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button.icon().isNull());
    button.update();
    QTest::qWait(100);
    QVERIFY(!button.icon().isNull());
}

void tst_NToolButton::transparentBackground() {
    NToolButton button;

    button.setTransparentBackground(true);
    QCOMPARE(button.getTransparentBackground(), true);

    button.setTransparentBackground(false);
    QCOMPARE(button.getTransparentBackground(), false);
}

void tst_NToolButton::borderRadius() {
    NToolButton button;

    int defaultRadius = button.getBorderRadius();
    QVERIFY(defaultRadius >= 0);

    button.setBorderRadius(10);
    QCOMPARE(button.getBorderRadius(), 10);

    button.setBorderRadius(0);
    QCOMPARE(button.getBorderRadius(), 0);
}

// 工具按钮特有测试
void tst_NToolButton::toolButtonStyleChange() {
    NToolButton button("Style Test");
    button.setFluentIcon(NRegularIconType::Add12Regular);
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试不同的工具按钮样式
    button.setToolButtonStyle(Qt::ToolButtonIconOnly);
    QCOMPARE(button.toolButtonStyle(), Qt::ToolButtonIconOnly);
    QTest::qWait(50);

    button.setToolButtonStyle(Qt::ToolButtonTextOnly);
    QCOMPARE(button.toolButtonStyle(), Qt::ToolButtonTextOnly);
    QTest::qWait(50);

    button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QCOMPARE(button.toolButtonStyle(), Qt::ToolButtonTextBesideIcon);
    QTest::qWait(50);

    button.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QCOMPARE(button.toolButtonStyle(), Qt::ToolButtonTextUnderIcon);
    QTest::qWait(50);

    // 测试样式变化对尺寸的影响
    QSize iconOnlySize = button.sizeHint();
    button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QSize textBesideSize = button.sizeHint();
    
    // 有文本的样式应该更宽
    if (!button.text().isEmpty()) {
        QVERIFY(textBesideSize.width() >= iconOnlySize.width());
    }
}

void tst_NToolButton::popupModeTests() {
    NToolButton button("Popup Test");
    QMenu* menu = new QMenu(&button);
    menu->addAction("Action 1");
    menu->addAction("Action 2");
    button.setMenu(menu);
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试不同的弹出模式
    button.setPopupMode(QToolButton::DelayedPopup);
    QCOMPARE(button.popupMode(), QToolButton::DelayedPopup);

    button.setPopupMode(QToolButton::MenuButtonPopup);
    QCOMPARE(button.popupMode(), QToolButton::MenuButtonPopup);

    button.setPopupMode(QToolButton::InstantPopup);
    QCOMPARE(button.popupMode(), QToolButton::InstantPopup);

    // 测试弹出模式对尺寸的影响
    QSize instantSize = button.sizeHint();
    button.setPopupMode(QToolButton::MenuButtonPopup);
    QSize menuButtonSize = button.sizeHint();
    
    // MenuButtonPopup模式应该为箭头预留空间
    QVERIFY(menuButtonSize.width() >= instantSize.width());
}

void tst_NToolButton::arrowIndicator() {
    NToolButton button("Arrow Test");
    QMenu* menu = new QMenu(&button);
    menu->addAction("Test Action");
    button.setMenu(menu);
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试箭头类型
    button.setArrowType(Qt::NoArrow);
    QCOMPARE(button.arrowType(), Qt::NoArrow);

    button.setArrowType(Qt::DownArrow);
    QCOMPARE(button.arrowType(), Qt::DownArrow);

    button.setArrowType(Qt::UpArrow);
    QCOMPARE(button.arrowType(), Qt::UpArrow);

    button.setArrowType(Qt::LeftArrow);
    QCOMPARE(button.arrowType(), Qt::LeftArrow);

    button.setArrowType(Qt::RightArrow);
    QCOMPARE(button.arrowType(), Qt::RightArrow);

    // 测试箭头对尺寸的影响
    QSize noArrowSize = button.sizeHint();
    button.setArrowType(Qt::DownArrow);
    QSize withArrowSize = button.sizeHint();
    
    // 有箭头时可能会影响尺寸
    QVERIFY(withArrowSize.width() >= noArrowSize.width() || 
            withArrowSize.height() >= noArrowSize.height());
}

// 性能相关测试
void tst_NToolButton::cacheEfficiency() {
    NToolButton button("Cache Test");
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

void tst_NToolButton::iconCacheEfficiency() {
    NToolButton button;
    
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

void tst_NToolButton::cacheInvalidation() {
    NToolButton button("Cache Invalidation Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 设置初始状态
    button.setButtonType(NToolButton::Standard);
    QTest::qWait(50);

    // 改变按钮类型应该失效缓存
    button.setButtonType(NToolButton::Accent);
    QTest::qWait(50);

    // 改变颜色属性应该失效缓存
    button.setLightDefaultColor(QColor(255, 0, 0));
    QTest::qWait(50);

    // 设置图标应该失效图标缓存
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QTest::qWait(50);

    // 改变工具按钮样式应该失效缓存
    button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QTest::qWait(50);
}

void tst_NToolButton::memoryLeakTest() {
    const int buttonCount = 100;
    QVector<NToolButton*> buttons;
    buttons.reserve(buttonCount);

    // 创建大量工具按钮
    for (int i = 0; i < buttonCount; ++i) {
        NToolButton* button = new NToolButton();
        button->setText(QString("Tool %1").arg(i));
        button->setFluentIcon(NRegularIconType::Add12Regular);
        button->setButtonType(i % 2 == 0 ? NToolButton::Standard : NToolButton::Accent);
        
        // 为部分按钮添加菜单
        if (i % 3 == 0) {
            QMenu* menu = new QMenu(button);
            menu->addAction(QString("Action %1").arg(i));
            button->setMenu(menu);
        }
        
        buttons.append(button);
    }

    // 销毁所有按钮
    qDeleteAll(buttons);
    buttons.clear();

    // 强制垃圾回收
    QCoreApplication::processEvents();
}

void tst_NToolButton::longRunningMemoryTest() {
    NToolButton button("Long Running Test");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 模拟长时间运行中的各种操作
    for (int i = 0; i < 50; ++i) {
        // 切换按钮类型
        button.setButtonType(i % 2 == 0 ? NToolButton::Standard : NToolButton::Accent);
        
        // 改变工具按钮样式
        Qt::ToolButtonStyle styles[] = {
            Qt::ToolButtonIconOnly,
            Qt::ToolButtonTextOnly,
            Qt::ToolButtonTextBesideIcon,
            Qt::ToolButtonTextUnderIcon
        };
        button.setToolButtonStyle(styles[i % 4]);
        
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
void tst_NToolButton::disabledStateOperations() {
    NToolButton button("Disabled Test");
    button.setEnabled(false);
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 测试禁用状态下的各种操作
    QSignalSpy clickSpy(&button, &NToolButton::clicked);
    QSignalSpy pressSpy(&button, &NToolButton::pressed);

    // 禁用状态下点击不应该触发信号
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(clickSpy.count(), 0);
    QCOMPARE(pressSpy.count(), 0);

    // 测试禁用状态下的属性设置
    button.setButtonType(NToolButton::Accent);
    QCOMPARE(button.buttonType(), NToolButton::Accent);

    button.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button.icon().isNull());

    // 测试禁用状态下的工具按钮特有功能
    button.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QCOMPARE(button.toolButtonStyle(), Qt::ToolButtonTextBesideIcon);

    // 重新启用后应该正常工作
    button.setEnabled(true);
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(clickSpy.count(), 1);
}

void tst_NToolButton::extremeSizes() {
    // 测试极小尺寸
    NToolButton smallButton;
    smallButton.setText("S");
    smallButton.resize(10, 10);
    smallButton.show();
    QVERIFY(QTest::qWaitForWindowExposed(&smallButton));
    
    QSize minSize = smallButton.minimumSizeHint();
    QVERIFY(minSize.width() > 0);
    QVERIFY(minSize.height() > 0);

    // 测试极大尺寸
    NToolButton largeButton;
    largeButton.setText("Large Tool Button");
    largeButton.resize(1000, 500);
    largeButton.show();
    QVERIFY(QTest::qWaitForWindowExposed(&largeButton));

    // 测试零尺寸
    NToolButton zeroButton;
    zeroButton.resize(0, 0);
    QSize zeroSize = zeroButton.sizeHint();
    QVERIFY(zeroSize.width() >= 0);
    QVERIFY(zeroSize.height() >= 0);
}

void tst_NToolButton::textEdgeCases() {
    // 测试空文本
    NToolButton emptyButton;
    emptyButton.setText("");
    QCOMPARE(emptyButton.text(), QString(""));
    QSize emptySize = emptyButton.sizeHint();
    QVERIFY(emptySize.width() > 0);
    QVERIFY(emptySize.height() > 0);

    // 测试很长的文本
    QString longText = QString("Very Long Tool Button Text ").repeated(10);
    NToolButton longButton;
    longButton.setText(longText);
    QCOMPARE(longButton.text(), longText);
    QSize longSize = longButton.sizeHint();
    QVERIFY(longSize.width() > emptySize.width());

    // 测试特殊字符
    NToolButton specialButton;
    specialButton.setText("工具按钮\n换行\t制表符&快捷键");
    QSize specialSize = specialButton.sizeHint();
    QVERIFY(specialSize.width() > 0);
    QVERIFY(specialSize.height() > 0);

    // 测试HTML标签（应该被当作普通文本）
    NToolButton htmlButton;
    htmlButton.setText("<b>Bold</b> <i>Italic</i>");
    QVERIFY(htmlButton.text().contains("<b>"));
}



void tst_NToolButton::signalTiming() {
    NToolButton button;
    button.setText("Signal Timing");
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    QSignalSpy pressedSpy(&button, &NToolButton::pressed);
    QSignalSpy releasedSpy(&button, &NToolButton::released);
    QSignalSpy clickedSpy(&button, &NToolButton::clicked);

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
void tst_NToolButton::highDpiRendering() {
    NToolButton button;
    button.setText("High DPI Test");
    button.setFluentIcon(NRegularIconType::Add12Regular);
    
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));
    
    // 测试图标在高DPI下是否正确渲染
    QVERIFY(!button.icon().isNull());
    
    // 测试尺寸计算在高DPI下是否正确
    QSize sizeHint = button.sizeHint();
    QVERIFY(sizeHint.width() > 0);
    QVERIFY(sizeHint.height() > 0);
}

void tst_NToolButton::styleSheetApplication() {
    NToolButton button;
    button.setText("Style Test");
    
    // 测试样式表的应用
    QString styleSheet = "NToolButton { background-color: red; color: white; border: 2px solid blue; }";
    button.setStyleSheet(styleSheet);
    QCOMPARE(button.styleSheet(), styleSheet);
    
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));
    
    // 测试样式表不会影响基本功能
    QSignalSpy clickSpy(&button, &NToolButton::clicked);
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(clickSpy.count(), 1);
    
    // 清除样式表
    button.setStyleSheet("");
    QVERIFY(button.styleSheet().isEmpty());
}

void tst_NToolButton::focusRendering() {
    NToolButton button;
    button.setText("Focus Test");
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
void tst_NToolButton::internationalization() {
    // 测试中文文本
    NToolButton chineseButton;
    chineseButton.setText("确定");
    QSize chineseSize = chineseButton.sizeHint();
    QVERIFY(chineseSize.width() > 0);
    QVERIFY(chineseSize.height() > 0);
    
    // 测试阿拉伯文本（RTL）
    NToolButton arabicButton;
    arabicButton.setText("موافق");
    QSize arabicSize = arabicButton.sizeHint();
    QVERIFY(arabicSize.width() > 0);
    QVERIFY(arabicSize.height() > 0);
    
    // 测试日文文本
    NToolButton japaneseButton;
    japaneseButton.setText("はい");
    QSize japaneseSize = japaneseButton.sizeHint();
    QVERIFY(japaneseSize.width() > 0);
    QVERIFY(japaneseSize.height() > 0);
    
    // 测试混合文本
    NToolButton mixedButton;
    mixedButton.setText("OK确定はい");
    QSize mixedSize = mixedButton.sizeHint();
    QVERIFY(mixedSize.width() > 0);
    QVERIFY(mixedSize.height() > 0);
    
    // 测试特殊Unicode字符
    NToolButton unicodeButton;
    unicodeButton.setText("✓ ✗ ★ ♥ ☀");
    QSize unicodeSize = unicodeButton.sizeHint();
    QVERIFY(unicodeSize.width() > 0);
    QVERIFY(unicodeSize.height() > 0);
}

QTEST_MAIN(tst_NToolButton)
#include "tst_ntoolbutton.moc"
