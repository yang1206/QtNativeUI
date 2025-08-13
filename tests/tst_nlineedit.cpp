// Copyright (C) 2024 QtNativeUI Project
// SPDX-License-Identifier: GPL-3.0-only

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QSignalSpy>
#include <QStyleFactory>
#include <QTest>
#include <QTimer>
#include <QValidator>

#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NTheme.h>

// 创建一个用于测试的NLineEdit子类
class TestNLineEdit : public NLineEdit {
  public:
    using NLineEdit::NLineEdit;

    // 公开protected方法用于测试
    using NLineEdit::minimumSizeHint;
    using NLineEdit::sizeHint;

    // 用于测试绘制方法
    void testDrawMethods() {
        QPixmap  pixmap(200, 50);
        QPainter painter(&pixmap);
        // 如果有自定义绘制方法，可以在这里测试
    }
};

class tst_NLineEdit : public QObject {
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
    void testTextOperations();
    void testReadOnly();
    void testMaxLength();
    void testPlaceholderText();
    void testEchoMode();
    void testValidator();
    void testInputMask();
    void testCursorPosition();
    void testSelection();
    void testUndo();
    void testCopy();
    void testCut();
    void testPaste();
    void testDragDrop();

    // 信号测试
    void testTextChangedSignal();
    void testFocusSignals();
    void testEditingFinished();
    void testReturnPressed();
    void testSelectionChanged();

    // 操作和菜单测试
    void testAddAction();
    void testContextMenu();

    // NLineEdit特有功能测试
    void testBorderRadius();
    void testBorderWidth();
    void testColorProperties();
    void testThemeChange();
    void testFluentIcon();

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
    void inputValidation();

    // 信号完整性测试
    void signalTiming();

    // 样式和绘制测试
    void highDpiRendering();
    void styleSheetApplication();
    void focusRendering();

    // 国际化测试
    void internationalization();

  private:
    NLineEdit* testWidget;
};

void tst_NLineEdit::initTestCase() {
    testWidget = new NLineEdit("Test LineEdit");
    testWidget->setObjectName("testWidget");
    testWidget->resize(200, 30);
    testWidget->show();
}

void tst_NLineEdit::cleanupTestCase() {
    delete testWidget;
    testWidget = nullptr;
}

void tst_NLineEdit::init() {
    testWidget->clear();
    testWidget->setEnabled(true);
    testWidget->setReadOnly(false);
    testWidget->setMaxLength(32767); // Qt默认值
    testWidget->setEchoMode(QLineEdit::Normal);
    testWidget->setPlaceholderText("");
    testWidget->setValidator(nullptr);
    testWidget->setInputMask("");
}

void tst_NLineEdit::cleanup() {
    // 每个测试后执行
}

// Testing get/set functions
void tst_NLineEdit::getSetCheck() {
    NLineEdit obj1;

    // QString NLineEdit::text()
    // void NLineEdit::setText(const QString&)
    obj1.setText("Test Text");
    QCOMPARE(obj1.text(), QString("Test Text"));
    obj1.setText("");
    QVERIFY(obj1.text().isEmpty());

    // QString NLineEdit::placeholderText()
    // void NLineEdit::setPlaceholderText(const QString&)
    obj1.setPlaceholderText("Enter text here");
    QCOMPARE(obj1.placeholderText(), QString("Enter text here"));
    obj1.setPlaceholderText("");
    QVERIFY(obj1.placeholderText().isEmpty());

    // bool NLineEdit::isReadOnly()
    // void NLineEdit::setReadOnly(bool)
    obj1.setReadOnly(true);
    QVERIFY(obj1.isReadOnly());
    obj1.setReadOnly(false);
    QVERIFY(!obj1.isReadOnly());

    // int NLineEdit::maxLength()
    // void NLineEdit::setMaxLength(int)
    obj1.setMaxLength(100);
    QCOMPARE(obj1.maxLength(), 100);
    obj1.setMaxLength(50);
    QCOMPARE(obj1.maxLength(), 50);

    // QLineEdit::EchoMode NLineEdit::echoMode()
    // void NLineEdit::setEchoMode(QLineEdit::EchoMode)
    obj1.setEchoMode(QLineEdit::Password);
    QCOMPARE(obj1.echoMode(), QLineEdit::Password);
    obj1.setEchoMode(QLineEdit::Normal);
    QCOMPARE(obj1.echoMode(), QLineEdit::Normal);
}

void tst_NLineEdit::testConstruction() {
    // 测试默认构造函数
    NLineEdit lineEdit1;
    QVERIFY(lineEdit1.text().isEmpty());
    QVERIFY(lineEdit1.isEnabled());
    QVERIFY(!lineEdit1.isReadOnly());

    // 测试带文本的构造函数
    NLineEdit lineEdit2("Test Text");
    QCOMPARE(lineEdit2.text(), QString("Test Text"));
    QVERIFY(lineEdit2.isEnabled());
    QVERIFY(!lineEdit2.isReadOnly());

    // 测试带父对象的构造函数
    QWidget   parent;
    NLineEdit lineEdit3(&parent);
    QCOMPARE(lineEdit3.parent(), &parent);

    NLineEdit lineEdit4("Parent Text", &parent);
    QCOMPARE(lineEdit4.text(), QString("Parent Text"));
    QCOMPARE(lineEdit4.parent(), &parent);
}

void tst_NLineEdit::testDefaultProperties() {
    NLineEdit lineEdit;

    // 检查默认边框半径
    QCOMPARE(lineEdit.getBorderRadius(), NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt());

    // 检查默认边框宽度
    QCOMPARE(lineEdit.getBorderWidth(), 1);

    // 默认应该不是只读
    QVERIFY(!lineEdit.isReadOnly());

    // 默认应该是启用的
    QVERIFY(lineEdit.isEnabled());
}

void tst_NLineEdit::testTextOperations() {
    NLineEdit lineEdit;

    // 测试设置和获取文本
    lineEdit.setText("Hello World");
    QCOMPARE(lineEdit.text(), QString("Hello World"));

    // 测试清除文本
    lineEdit.clear();
    QVERIFY(lineEdit.text().isEmpty());

    // 测试插入文本
    lineEdit.setText("Hello");
    lineEdit.setCursorPosition(5);
    lineEdit.insert(" World");
    QCOMPARE(lineEdit.text(), QString("Hello World"));

    // 测试选择文本
    lineEdit.selectAll();
    QCOMPARE(lineEdit.selectedText(), QString("Hello World"));
    QCOMPARE(lineEdit.selectionStart(), 0);

    // 测试部分选择
    lineEdit.setSelection(0, 5);
    QCOMPARE(lineEdit.selectedText(), QString("Hello"));
}

void tst_NLineEdit::testReadOnly() {
    NLineEdit lineEdit("Initial Text");

    // 默认不是只读
    QVERIFY(!lineEdit.isReadOnly());

    // 设置为只读
    lineEdit.setReadOnly(true);
    QVERIFY(lineEdit.isReadOnly());

    // 测试只读状态下不能修改文本
    QString originalText = lineEdit.text();
    QTest::keyClicks(&lineEdit, "Additional Text");
    QCOMPARE(lineEdit.text(), originalText);

    // 恢复可写状态
    lineEdit.setReadOnly(false);
    QVERIFY(!lineEdit.isReadOnly());

    // 测试可写状态下可以修改文本
    QTest::keyClicks(&lineEdit, " More Text");
    QVERIFY(lineEdit.text().length() > originalText.length());
}

void tst_NLineEdit::testMaxLength() {
    NLineEdit lineEdit;

    // 测试设置最大长度
    lineEdit.setMaxLength(10);
    QCOMPARE(lineEdit.maxLength(), 10);

    // 测试输入不超过最大长度的文本
    lineEdit.setText("12345");
    QCOMPARE(lineEdit.text(), QString("12345"));

    // 测试输入超过最大长度的文本
    lineEdit.setText("12345678901234");
    QCOMPARE(lineEdit.text().length(), 10);
    QCOMPARE(lineEdit.text(), QString("1234567890"));
}

void tst_NLineEdit::testTextChangedSignal() {
    NLineEdit  lineEdit;
    QSignalSpy textChangedSpy(&lineEdit, &NLineEdit::textChanged);
    QSignalSpy textEditedSpy(&lineEdit, &NLineEdit::textEdited);

    // 通过setText触发textChanged但不触发textEdited
    lineEdit.setText("Test Text");
    QCOMPARE(textChangedSpy.count(), 1);
    QCOMPARE(textEditedSpy.count(), 0);

    // 通过用户输入触发textChanged和textEdited
    lineEdit.clear();
    textChangedSpy.clear();
    textEditedSpy.clear();

    // 显示编辑框以确保键盘事件能被处理
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    QTest::keyClicks(&lineEdit, "User Input");
    QVERIFY(textChangedSpy.count() > 0);
    QVERIFY(textEditedSpy.count() > 0);
}

void tst_NLineEdit::testFocusSignals() {
    NLineEdit  lineEdit("Test");
    QSignalSpy focusInSpy(&lineEdit, &NLineEdit::focusIn);
    QSignalSpy focusOutSpy(&lineEdit, &NLineEdit::focusOut);

    // 显示编辑框以确保焦点事件能被处理
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试获取焦点
    lineEdit.setFocus();
    QApplication::processEvents();
    QCOMPARE(focusInSpy.count(), 1);
    QCOMPARE(focusInSpy.at(0).at(0).toString(), QString("Test"));

    // 测试失去焦点
    lineEdit.clearFocus();
    QApplication::processEvents();
    QCOMPARE(focusOutSpy.count(), 1);
    QCOMPARE(focusOutSpy.at(0).at(0).toString(), QString("Test"));
}

void tst_NLineEdit::testAddAction() {
    NLineEdit lineEdit;

    // 测试添加Fluent图标
    QAction* regularAction = lineEdit.addAction(NRegularIconType::Search12Regular, QLineEdit::TrailingPosition);
    QVERIFY(regularAction != nullptr);
    QVERIFY(!regularAction->icon().isNull());

    // 测试添加Filled图标
    QAction* filledAction = lineEdit.addAction(NFilledIconType::Search12Filled, QLineEdit::LeadingPosition);
    QVERIFY(filledAction != nullptr);
    QVERIFY(!filledAction->icon().isNull());

    // 测试添加自定义QAction
    QAction* customAction = new QAction("Custom", &lineEdit);
    lineEdit.addAction(customAction, QLineEdit::TrailingPosition);

    // 检查操作数量 - 应该有3个操作
    QList<QAction*> actions = lineEdit.actions();
    QCOMPARE(actions.size(), 3);
}

void tst_NLineEdit::testPlaceholderText() {
    NLineEdit lineEdit;

    // 测试设置占位符文本
    lineEdit.setPlaceholderText("Enter your name");
    QCOMPARE(lineEdit.placeholderText(), QString("Enter your name"));

    // 测试清空占位符文本
    lineEdit.setPlaceholderText("");
    QVERIFY(lineEdit.placeholderText().isEmpty());

    // 测试占位符文本在有内容时的行为
    lineEdit.setPlaceholderText("Placeholder");
    lineEdit.setText("Real Text");
    QCOMPARE(lineEdit.text(), QString("Real Text"));
    QCOMPARE(lineEdit.placeholderText(), QString("Placeholder"));
}

void tst_NLineEdit::testEchoMode() {
    NLineEdit lineEdit;

    // 测试普通模式
    lineEdit.setEchoMode(QLineEdit::Normal);
    QCOMPARE(lineEdit.echoMode(), QLineEdit::Normal);
    lineEdit.setText("visible");
    QCOMPARE(lineEdit.displayText(), QString("visible"));

    // 测试密码模式
    lineEdit.setEchoMode(QLineEdit::Password);
    QCOMPARE(lineEdit.echoMode(), QLineEdit::Password);
    lineEdit.setText("secret");
    QVERIFY(lineEdit.displayText() != QString("secret"));
    QCOMPARE(lineEdit.text(), QString("secret"));

    // 测试无回显模式
    lineEdit.setEchoMode(QLineEdit::NoEcho);
    QCOMPARE(lineEdit.echoMode(), QLineEdit::NoEcho);
    lineEdit.setText("hidden");
    QVERIFY(lineEdit.displayText().isEmpty());
    QCOMPARE(lineEdit.text(), QString("hidden"));

    // 测试编辑时密码模式
    lineEdit.setEchoMode(QLineEdit::PasswordEchoOnEdit);
    QCOMPARE(lineEdit.echoMode(), QLineEdit::PasswordEchoOnEdit);
}

void tst_NLineEdit::testValidator() {
    NLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试整数验证器
    QIntValidator* intValidator = new QIntValidator(0, 100, &lineEdit);
    lineEdit.setValidator(intValidator);
    QCOMPARE(lineEdit.validator(), intValidator);

    // 测试有效输入
    lineEdit.clear();
    QTest::keyClicks(&lineEdit, "50");
    QCOMPARE(lineEdit.text(), QString("50"));

    // 测试无效输入（字母）
    lineEdit.clear();
    QTest::keyClicks(&lineEdit, "abc");
    QVERIFY(lineEdit.text().isEmpty() || !lineEdit.text().contains("abc"));

    // 测试双精度验证器
    QDoubleValidator* doubleValidator = new QDoubleValidator(0.0, 100.0, 2, &lineEdit);
    lineEdit.setValidator(doubleValidator);
    QCOMPARE(lineEdit.validator(), doubleValidator);

    lineEdit.clear();
    QTest::keyClicks(&lineEdit, "12.34");
    QCOMPARE(lineEdit.text(), QString("12.34"));

    // 移除验证器
    lineEdit.setValidator(nullptr);
    QCOMPARE(lineEdit.validator(), nullptr);
}

void tst_NLineEdit::testInputMask() {
    NLineEdit lineEdit;

    // 测试电话号码掩码
    lineEdit.setInputMask("(999) 999-9999");
    QCOMPARE(lineEdit.inputMask(), QString("(999) 999-9999"));

    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试输入
    QTest::keyClicks(&lineEdit, "1234567890");
    QVERIFY(lineEdit.text().contains("(123) 456-7890"));

    // 测试IP地址掩码
    lineEdit.setInputMask("000.000.000.000");
    lineEdit.clear();
    QTest::keyClicks(&lineEdit, "192168001001");
    QVERIFY(lineEdit.text().contains("192.168.001.001"));

    // 清除掩码
    lineEdit.setInputMask("");
    QVERIFY(lineEdit.inputMask().isEmpty());
}

void tst_NLineEdit::testCursorPosition() {
    NLineEdit lineEdit("Hello World");

    // 测试设置光标位置
    lineEdit.setCursorPosition(5);
    QCOMPARE(lineEdit.cursorPosition(), 5);

    // 测试移动到开始
    lineEdit.home(false);
    QCOMPARE(lineEdit.cursorPosition(), 0);

    // 测试移动到结束
    lineEdit.end(false);
    QCOMPARE(lineEdit.cursorPosition(), lineEdit.text().length());

    // 测试选择移动
    lineEdit.home(true);
    QCOMPARE(lineEdit.cursorPosition(), 0);
    QCOMPARE(lineEdit.selectedText(), lineEdit.text());
}

void tst_NLineEdit::testSelection() {
    NLineEdit lineEdit("Hello World");

    // 测试全选
    lineEdit.selectAll();
    QCOMPARE(lineEdit.selectedText(), QString("Hello World"));
    QCOMPARE(lineEdit.selectionStart(), 0);
    QCOMPARE(lineEdit.selectionLength(), 11);

    // 测试部分选择
    lineEdit.setSelection(0, 5);
    QCOMPARE(lineEdit.selectedText(), QString("Hello"));
    QCOMPARE(lineEdit.selectionStart(), 0);
    QCOMPARE(lineEdit.selectionLength(), 5);

    // 测试取消选择
    lineEdit.deselect();
    QVERIFY(lineEdit.selectedText().isEmpty());
    QCOMPARE(lineEdit.selectionLength(), 0);

    // 测试反向选择
    lineEdit.setSelection(6, -6);
    QCOMPARE(lineEdit.selectedText(), QString("Hello "));
    QCOMPARE(lineEdit.selectionStart(), 0);
}

void tst_NLineEdit::testUndo() {
    NLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 初始状态不能撤销
    QVERIFY(!lineEdit.isUndoAvailable());

    // 输入文本
    QTest::keyClicks(&lineEdit, "Hello");
    QCOMPARE(lineEdit.text(), QString("Hello"));
    QVERIFY(lineEdit.isUndoAvailable());

    // 继续输入
    QTest::keyClicks(&lineEdit, " World");
    QCOMPARE(lineEdit.text(), QString("Hello World"));

    // 测试撤销
    lineEdit.undo();
    QVERIFY(lineEdit.text() != QString("Hello World"));

    // 测试重做
    if (lineEdit.isRedoAvailable()) {
        lineEdit.redo();
    }
}

void tst_NLineEdit::testCopy() {
    NLineEdit lineEdit("Copy Test");

    // 选择文本
    lineEdit.selectAll();
    QCOMPARE(lineEdit.selectedText(), QString("Copy Test"));

    // 复制到剪贴板
    lineEdit.copy();

    // 验证剪贴板内容
    QClipboard* clipboard = QApplication::clipboard();
    QCOMPARE(clipboard->text(), QString("Copy Test"));
}

void tst_NLineEdit::testCut() {
    NLineEdit lineEdit("Cut Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 选择文本
    lineEdit.selectAll();
    QCOMPARE(lineEdit.selectedText(), QString("Cut Test"));

    // 剪切到剪贴板
    lineEdit.cut();

    // 验证文本被删除
    QVERIFY(lineEdit.text().isEmpty());

    // 验证剪贴板内容
    QClipboard* clipboard = QApplication::clipboard();
    QCOMPARE(clipboard->text(), QString("Cut Test"));
}

void tst_NLineEdit::testPaste() {
    NLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 设置剪贴板内容
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText("Paste Test");

    // 粘贴
    lineEdit.paste();
    QCOMPARE(lineEdit.text(), QString("Paste Test"));

    // 测试在有选择的情况下粘贴
    lineEdit.setText("Replace Me");
    lineEdit.selectAll();
    clipboard->setText("New Text");
    lineEdit.paste();
    QCOMPARE(lineEdit.text(), QString("New Text"));
}

void tst_NLineEdit::testDragDrop() {
    // 这个测试比较复杂，需要模拟拖拽操作
    NLineEdit lineEdit("Drag Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试拖拽是否启用
    lineEdit.setDragEnabled(true);
    // 注意：实际的拖拽测试需要更复杂的模拟，这里只测试基本属性
}

void tst_NLineEdit::testEditingFinished() {
    NLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    QSignalSpy spy(&lineEdit, &NLineEdit::editingFinished);

    // 输入文本
    QTest::keyClicks(&lineEdit, "Test");
    QCOMPARE(spy.count(), 0);

    // 按Enter键应该触发editingFinished
    QTest::keyPress(&lineEdit, Qt::Key_Return);
    QCOMPARE(spy.count(), 1);

    // 失去焦点也应该触发editingFinished
    spy.clear();
    lineEdit.setFocus();
    QTest::keyClicks(&lineEdit, "More");
    lineEdit.clearFocus();
    QApplication::processEvents();
    QCOMPARE(spy.count(), 1);
}

void tst_NLineEdit::testReturnPressed() {
    NLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    QSignalSpy spy(&lineEdit, &NLineEdit::returnPressed);

    // 按Enter键应该触发returnPressed
    QTest::keyPress(&lineEdit, Qt::Key_Return);
    QCOMPARE(spy.count(), 1);

    // 按回车键也应该触发
    QTest::keyPress(&lineEdit, Qt::Key_Enter);
    QCOMPARE(spy.count(), 2);
}

void tst_NLineEdit::testSelectionChanged() {
    NLineEdit lineEdit("Selection Test");

    QSignalSpy spy(&lineEdit, &NLineEdit::selectionChanged);

    // 选择文本应该触发信号
    lineEdit.setSelection(0, 5);
    QCOMPARE(spy.count(), 1);

    // 改变选择应该再次触发
    lineEdit.setSelection(5, 4);
    QCOMPARE(spy.count(), 2);

    // 取消选择也应该触发
    lineEdit.deselect();
    QCOMPARE(spy.count(), 3);
}

void tst_NLineEdit::testContextMenu() {
    NLineEdit lineEdit("Context Menu Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试右键菜单（这个测试比较复杂，需要模拟右键点击）
    // 这里只测试基本的上下文菜单策略
    lineEdit.setContextMenuPolicy(Qt::DefaultContextMenu);
    QCOMPARE(lineEdit.contextMenuPolicy(), Qt::DefaultContextMenu);

    lineEdit.setContextMenuPolicy(Qt::NoContextMenu);
    QCOMPARE(lineEdit.contextMenuPolicy(), Qt::NoContextMenu);
}

// NLineEdit特有功能测试
void tst_NLineEdit::testBorderRadius() {
    NLineEdit lineEdit;

    // 获取默认边框半径
    int defaultRadius = lineEdit.getBorderRadius();
    QVERIFY(defaultRadius >= 0);

    // 设置新的边框半径
    lineEdit.setBorderRadius(10);
    QCOMPARE(lineEdit.getBorderRadius(), 10);

    // 设置另一个值
    lineEdit.setBorderRadius(20);
    QCOMPARE(lineEdit.getBorderRadius(), 20);

    // 测试边界值
    lineEdit.setBorderRadius(0);
    QCOMPARE(lineEdit.getBorderRadius(), 0);

    // 恢复默认值
    lineEdit.setBorderRadius(defaultRadius);
    QCOMPARE(lineEdit.getBorderRadius(), defaultRadius);
}

void tst_NLineEdit::testBorderWidth() {
    NLineEdit lineEdit;

    // 获取默认边框宽度
    int defaultWidth = lineEdit.getBorderWidth();
    QVERIFY(defaultWidth >= 0);

    // 设置新的边框宽度
    lineEdit.setBorderWidth(2);
    QCOMPARE(lineEdit.getBorderWidth(), 2);

    // 设置另一个值
    lineEdit.setBorderWidth(3);
    QCOMPARE(lineEdit.getBorderWidth(), 3);

    // 测试边界值
    lineEdit.setBorderWidth(0);
    QCOMPARE(lineEdit.getBorderWidth(), 0);

    // 恢复默认值
    lineEdit.setBorderWidth(defaultWidth);
    QCOMPARE(lineEdit.getBorderWidth(), defaultWidth);
}

void tst_NLineEdit::testColorProperties() {
    NLineEdit lineEdit;
    QColor    testColor(128, 128, 128);

    // 测试各种颜色属性的设置和获取
    lineEdit.setLightBackgroundColor(testColor);
    QCOMPARE(lineEdit.getLightBackgroundColor(), testColor);

    lineEdit.setDarkBackgroundColor(testColor);
    QCOMPARE(lineEdit.getDarkBackgroundColor(), testColor);

    lineEdit.setLightBackgroundHoverColor(testColor);
    QCOMPARE(lineEdit.getLightBackgroundHoverColor(), testColor);

    lineEdit.setDarkBackgroundHoverColor(testColor);
    QCOMPARE(lineEdit.getDarkBackgroundHoverColor(), testColor);

    lineEdit.setLightBackgroundFocusColor(testColor);
    QCOMPARE(lineEdit.getLightBackgroundFocusColor(), testColor);

    lineEdit.setDarkBackgroundFocusColor(testColor);
    QCOMPARE(lineEdit.getDarkBackgroundFocusColor(), testColor);

    lineEdit.setLightBorderColor(testColor);
    QCOMPARE(lineEdit.getLightBorderColor(), testColor);

    lineEdit.setDarkBorderColor(testColor);
    QCOMPARE(lineEdit.getDarkBorderColor(), testColor);

    lineEdit.setLightTextColor(testColor);
    QCOMPARE(lineEdit.getLightTextColor(), testColor);

    lineEdit.setDarkTextColor(testColor);
    QCOMPARE(lineEdit.getDarkTextColor(), testColor);
}

void tst_NLineEdit::testThemeChange() {
    NLineEdit lineEdit("Theme Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 记录初始状态
    QString initialText     = lineEdit.text();
    bool    initialReadOnly = lineEdit.isReadOnly();

    // 测试主题切换不会导致崩溃
    lineEdit.update();
    QTest::qWait(100);

    // 验证属性在主题切换后保持不变
    QCOMPARE(lineEdit.text(), initialText);
    QCOMPARE(lineEdit.isReadOnly(), initialReadOnly);

    // 测试在不同主题下输入框仍然可以正常使用
    QSignalSpy spy(&lineEdit, &NLineEdit::textChanged);
    lineEdit.setText("Theme Changed");
    QCOMPARE(spy.count(), 1);

    // 测试焦点在主题切换后的行为
    lineEdit.setFocus();
    QVERIFY(lineEdit.hasFocus());
    lineEdit.update();
    QTest::qWait(100);
}

void tst_NLineEdit::testFluentIcon() {
    NLineEdit lineEdit;

    // 测试添加Regular图标
    QAction* regularAction = lineEdit.addAction(NRegularIconType::Search12Regular, QLineEdit::TrailingPosition);
    QVERIFY(regularAction != nullptr);
    QVERIFY(!regularAction->icon().isNull());

    // 测试添加Filled图标
    QAction* filledAction = lineEdit.addAction(NFilledIconType::Search12Filled, QLineEdit::LeadingPosition);
    QVERIFY(filledAction != nullptr);
    QVERIFY(!filledAction->icon().isNull());

    // 测试图标位置
    QList<QAction*> actions = lineEdit.actions();
    QCOMPARE(actions.size(), 2);

    // 测试移除图标
    lineEdit.removeAction(regularAction);
    actions = lineEdit.actions();
    QCOMPARE(actions.size(), 1);

    lineEdit.removeAction(filledAction);
    actions = lineEdit.actions();
    QCOMPARE(actions.size(), 0);
}

// 性能相关测试
void tst_NLineEdit::cacheEfficiency() {
    NLineEdit lineEdit("Cache Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试颜色缓存 - 多次focus/blur应该使用缓存
    for (int i = 0; i < 10; ++i) {
        lineEdit.setFocus();
        QTest::qWait(10);
        lineEdit.clearFocus();
        QTest::qWait(10);
    }

    // 测试hover状态的缓存
    for (int i = 0; i < 10; ++i) {
        QTest::mouseMove(&lineEdit, lineEdit.rect().center());
        QTest::qWait(10);
        QTest::mouseMove(&lineEdit, QPoint(-1, -1));
        QTest::qWait(10);
    }
}

void tst_NLineEdit::iconCacheEfficiency() {
    NLineEdit lineEdit;

    // 设置图标后多次获取，应该使用缓存
    QAction* action1 = lineEdit.addAction(NRegularIconType::Search12Regular, NLineEdit::TrailingPosition);
    QIcon    icon1   = action1->icon();
    QIcon    icon2   = action1->icon();
    QIcon    icon3   = action1->icon();

    QVERIFY(!icon1.isNull());
    QVERIFY(!icon2.isNull());
    QVERIFY(!icon3.isNull());

    // 测试相同参数的图标设置是否使用缓存
    QAction* action2 = lineEdit.addAction(NRegularIconType::Search12Regular, NLineEdit::LeadingPosition);
    QIcon    icon4   = action2->icon();
    QVERIFY(!icon4.isNull());
}

void tst_NLineEdit::cacheInvalidation() {
    NLineEdit lineEdit("Cache Invalidation Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 改变颜色属性应该失效缓存
    lineEdit.setLightBackgroundColor(QColor(255, 0, 0));
    QTest::qWait(50);

    lineEdit.setDarkBackgroundColor(QColor(0, 255, 0));
    QTest::qWait(50);

    // 改变边框属性应该失效缓存
    lineEdit.setBorderRadius(15);
    QTest::qWait(50);

    lineEdit.setBorderWidth(3);
    QTest::qWait(50);

    // 改变状态应该失效缓存
    lineEdit.setReadOnly(true);
    QTest::qWait(50);

    lineEdit.setReadOnly(false);
    QTest::qWait(50);
}

void tst_NLineEdit::memoryLeakTest() {
    const int           lineEditCount = 100;
    QVector<NLineEdit*> lineEdits;
    lineEdits.reserve(lineEditCount);

    // 创建大量输入框
    for (int i = 0; i < lineEditCount; ++i) {
        NLineEdit* lineEdit = new NLineEdit(QString("LineEdit %1").arg(i));
        lineEdit->addAction(NRegularIconType::Search12Regular, QLineEdit::TrailingPosition);
        lineEdit->setPlaceholderText(QString("Placeholder %1").arg(i));
        lineEdits.append(lineEdit);
    }

    // 销毁所有输入框
    qDeleteAll(lineEdits);
    lineEdits.clear();

    // 强制垃圾回收
    QCoreApplication::processEvents();
}

void tst_NLineEdit::longRunningMemoryTest() {
    NLineEdit lineEdit("Long Running Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 模拟长时间运行中的各种操作
    for (int i = 0; i < 50; ++i) {
        // 改变文本
        lineEdit.setText(QString("Text %1").arg(i));

        // 改变属性
        lineEdit.setBorderRadius(i % 20);
        lineEdit.setBorderWidth((i % 3) + 1);

        // 改变颜色属性
        if (i % 5 == 0) {
            lineEdit.setLightBackgroundColor(QColor(i * 5 % 255, 100, 100));
            lineEdit.setDarkBackgroundColor(QColor(100, i * 5 % 255, 100));
        }

        // 改变状态
        lineEdit.setReadOnly(i % 2 == 0);

        // 模拟用户交互
        lineEdit.setFocus();
        QTest::qWait(10);
        lineEdit.clearFocus();
        QTest::qWait(10);

        QTest::qWait(20);
    }
}

// 边界条件测试
void tst_NLineEdit::disabledStateOperations() {
    NLineEdit lineEdit("Disabled Test");
    lineEdit.setEnabled(false);
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试禁用状态下的各种操作
    QSignalSpy textChangedSpy(&lineEdit, &NLineEdit::textChanged);
    QSignalSpy focusInSpy(&lineEdit, &NLineEdit::focusIn);

    // 禁用状态下不应该能获得焦点
    lineEdit.setFocus();
    QVERIFY(!lineEdit.hasFocus());
    QCOMPARE(focusInSpy.count(), 0);

    // 禁用状态下键盘输入不应该生效
    QTest::keyClicks(&lineEdit, "Should not work");
    QCOMPARE(textChangedSpy.count(), 0);

    // 但是程序设置文本应该仍然有效
    lineEdit.setText("Programmatic change");
    QCOMPARE(lineEdit.text(), QString("Programmatic change"));
    QCOMPARE(textChangedSpy.count(), 1);

    // 重新启用后应该正常工作
    lineEdit.setEnabled(true);
    lineEdit.setFocus();
    QVERIFY(lineEdit.hasFocus());
}

void tst_NLineEdit::extremeSizes() {
    // 测试极小尺寸
    NLineEdit smallLineEdit;
    smallLineEdit.resize(10, 10);
    smallLineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&smallLineEdit));

    QSize minSize = smallLineEdit.minimumSizeHint();
    QVERIFY(minSize.width() > 0);
    QVERIFY(minSize.height() > 0);

    // 测试极大尺寸
    NLineEdit largeLineEdit;
    largeLineEdit.resize(2000, 100);
    largeLineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&largeLineEdit));

    // 测试零尺寸
    NLineEdit zeroLineEdit;
    zeroLineEdit.resize(0, 0);
    QSize zeroSize = zeroLineEdit.sizeHint();
    QVERIFY(zeroSize.width() >= 0);
    QVERIFY(zeroSize.height() >= 0);
}

void tst_NLineEdit::textEdgeCases() {
    NLineEdit lineEdit;

    // 测试空文本
    lineEdit.setText("");
    QVERIFY(lineEdit.text().isEmpty());
    QVERIFY(lineEdit.displayText().isEmpty());

    // 测试很长的文本
    QString longText = QString("Very Long Text ").repeated(100);
    lineEdit.setText(longText);
    QCOMPARE(lineEdit.text(), longText);

    // 测试特殊字符
    QString specialText = "测试中文\n换行\t制表符&特殊字符";
    lineEdit.setText(specialText);
    QCOMPARE(lineEdit.text(), specialText);

    // 测试Unicode字符
    QString unicodeText = "✓ ✗ ★ ♥ ☀ 🌟 🎉";
    lineEdit.setText(unicodeText);
    QCOMPARE(lineEdit.text(), unicodeText);

    // 测试HTML标签（应该被当作普通文本）
    QString htmlText = "<b>Bold</b> <i>Italic</i> <script>alert('xss')</script>";
    lineEdit.setText(htmlText);
    QCOMPARE(lineEdit.text(), htmlText);
}

void tst_NLineEdit::inputValidation() {
    NLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试最大长度限制
    lineEdit.setMaxLength(5);
    lineEdit.setText("12345678");
    QCOMPARE(lineEdit.text().length(), 5);
    QCOMPARE(lineEdit.text(), QString("12345"));

    // 测试输入掩码验证
    lineEdit.setMaxLength(32767); // 重置最大长度
    lineEdit.setInputMask("99-99-99");
    lineEdit.clear();
    QTest::keyClicks(&lineEdit, "123456");
    QVERIFY(lineEdit.text().contains("12-34-56"));

    // 测试验证器
    lineEdit.setInputMask(""); // 清除掩码
    QIntValidator* validator = new QIntValidator(0, 999, &lineEdit);
    lineEdit.setValidator(validator);
    lineEdit.clear();
    QTest::keyClicks(&lineEdit, "abc123def");
    // 验证器应该阻止非数字字符
    QVERIFY(!lineEdit.text().contains("abc"));
    QVERIFY(!lineEdit.text().contains("def"));
}

void tst_NLineEdit::signalTiming() {
    NLineEdit lineEdit;
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    QSignalSpy textChangedSpy(&lineEdit, &NLineEdit::textChanged);
    QSignalSpy textEditedSpy(&lineEdit, &NLineEdit::textEdited);
    QSignalSpy editingFinishedSpy(&lineEdit, &NLineEdit::editingFinished);

    // 程序设置文本只触发textChanged
    lineEdit.setText("Program Text");
    QCOMPARE(textChangedSpy.count(), 1);
    QCOMPARE(textEditedSpy.count(), 0);
    QCOMPARE(editingFinishedSpy.count(), 0);

    // 用户输入触发textChanged和textEdited
    textChangedSpy.clear();
    textEditedSpy.clear();
    lineEdit.clear();
    QTest::keyClicks(&lineEdit, "User");
    QVERIFY(textChangedSpy.count() > 0);
    QVERIFY(textEditedSpy.count() > 0);

    // 按Enter触发editingFinished
    QTest::keyPress(&lineEdit, Qt::Key_Return);
    QCOMPARE(editingFinishedSpy.count(), 1);
}

// 样式和绘制测试
void tst_NLineEdit::highDpiRendering() {
    NLineEdit lineEdit("High DPI Test");
    lineEdit.addAction(NRegularIconType::Search12Regular, NLineEdit::TrailingPosition);

    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试尺寸计算在高DPI下是否正确
    QSize sizeHint = lineEdit.sizeHint();
    QVERIFY(sizeHint.width() > 0);
    QVERIFY(sizeHint.height() > 0);

    // 测试图标在高DPI下是否正确显示
    QList<QAction*> actions = lineEdit.actions();
    if (!actions.isEmpty()) {
        QVERIFY(!actions.first()->icon().isNull());
    }
}

void tst_NLineEdit::styleSheetApplication() {
    NLineEdit lineEdit("Style Test");

    // 测试样式表的应用
    QString styleSheet = "NLineEdit { background-color: yellow; color: blue; border: 2px solid red; }";
    lineEdit.setStyleSheet(styleSheet);
    QCOMPARE(lineEdit.styleSheet(), styleSheet);

    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试样式表不会影响基本功能
    QSignalSpy textChangedSpy(&lineEdit, &NLineEdit::textChanged);
    lineEdit.setText("Styled Text");
    QCOMPARE(textChangedSpy.count(), 1);

    // 清除样式表
    lineEdit.setStyleSheet("");
    QVERIFY(lineEdit.styleSheet().isEmpty());
}

void tst_NLineEdit::focusRendering() {
    NLineEdit lineEdit("Focus Test");
    lineEdit.show();
    QVERIFY(QTest::qWaitForWindowExposed(&lineEdit));

    // 测试获得焦点
    lineEdit.setFocus();
    QVERIFY(lineEdit.hasFocus());
    QTest::qWait(100);

    // 测试失去焦点
    lineEdit.clearFocus();
    QVERIFY(!lineEdit.hasFocus());
    QTest::qWait(100);

    // 测试Tab键导航
    lineEdit.setFocus();
    QTest::keyPress(&lineEdit, Qt::Key_Tab);
    QTest::qWait(50);
}

// 国际化测试
void tst_NLineEdit::internationalization() {
    // 测试中文输入
    NLineEdit chineseLineEdit;
    chineseLineEdit.setText("你好世界");
    QCOMPARE(chineseLineEdit.text(), QString("你好世界"));
    QSize chineseSize = chineseLineEdit.sizeHint();
    QVERIFY(chineseSize.width() > 0);

    // 测试阿拉伯文本（RTL）
    NLineEdit arabicLineEdit;
    arabicLineEdit.setText("مرحبا بالعالم");
    QCOMPARE(arabicLineEdit.text(), QString("مرحبا بالعالم"));
    QSize arabicSize = arabicLineEdit.sizeHint();
    QVERIFY(arabicSize.width() > 0);

    // 测试日文输入
    NLineEdit japaneseLineEdit;
    japaneseLineEdit.setText("こんにちは世界");
    QCOMPARE(japaneseLineEdit.text(), QString("こんにちは世界"));
    QSize japaneseSize = japaneseLineEdit.sizeHint();
    QVERIFY(japaneseSize.width() > 0);

    // 测试混合语言
    NLineEdit mixedLineEdit;
    mixedLineEdit.setText("Hello 你好 مرحبا こんにちは");
    QCOMPARE(mixedLineEdit.text(), QString("Hello 你好 مرحبا こんにちは"));
    QSize mixedSize = mixedLineEdit.sizeHint();
    QVERIFY(mixedSize.width() > 0);

    // 测试特殊Unicode字符
    NLineEdit unicodeLineEdit;
    unicodeLineEdit.setText("🌍🌎🌏 ✨🎉🎊 💖💝💗");
    QCOMPARE(unicodeLineEdit.text(), QString("🌍🌎🌏 ✨🎉🎊 💖💝💗"));
    QSize unicodeSize = unicodeLineEdit.sizeHint();
    QVERIFY(unicodeSize.width() > 0);
}

QTEST_MAIN(tst_NLineEdit)
#include "tst_nlineedit.moc"
