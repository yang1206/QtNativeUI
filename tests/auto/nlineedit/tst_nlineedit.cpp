#include <QtTest>
#include <QApplication>
#include <QSignalSpy>
#include <QtNativeUI/NLineEdit.h>
#include <QtNativeUI/NTheme.h>

// 创建一个用于测试的NLineEdit子类
class TestLineEdit : public NLineEdit {
public:
    using NLineEdit::NLineEdit;  // 继承构造函数
    
    // 如果需要访问protected方法，可以在这里添加代理方法
};

class tst_NLineEdit : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 测试创建与默认属性
    void testConstruction();
    void testDefaultProperties();
    
    // 测试基本功能
    void testTextOperations();
    void testReadOnly();
    void testMaxLength();
    
    // 测试信号
    void testTextChangedSignal();
    void testFocusSignals();
    
    // 测试添加操作
    void testAddAction();
    
    // 测试样式属性
    void testBorderRadius();
};

void tst_NLineEdit::initTestCase() {
    // 测试开始前执行一次
}

void tst_NLineEdit::cleanupTestCase() {
    // 测试结束后执行一次
}

void tst_NLineEdit::init() {
    // 每个测试前执行
}

void tst_NLineEdit::cleanup() {
    // 每个测试后执行
}

void tst_NLineEdit::testConstruction() {
    // 测试默认构造函数
    NLineEdit lineEdit1;
    QVERIFY(lineEdit1.text().isEmpty());
    QVERIFY(lineEdit1.isEnabled());
    
    // 测试带文本的构造函数
    NLineEdit lineEdit2("Test Text");
    QCOMPARE(lineEdit2.text(), QString("Test Text"));
    QVERIFY(lineEdit2.isEnabled());
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
    NLineEdit lineEdit;
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
    NLineEdit lineEdit("Test");
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
    QAction* regularAction = lineEdit.addAction(NRegularIconType::Search12Regular, NLineEdit::TrailingPosition);
    QVERIFY(regularAction != nullptr);
    QVERIFY(!regularAction->icon().isNull());
    
    // 测试添加Filled图标
    QAction* filledAction = lineEdit.addAction(NFilledIconType::Search12Filled, NLineEdit::LeadingPosition);
    QVERIFY(filledAction != nullptr);
    QVERIFY(!filledAction->icon().isNull());
    
    // 测试添加自定义QAction
    QAction* customAction = new QAction("Custom", &lineEdit);
    lineEdit.addAction(customAction, NLineEdit::TrailingPosition);
    
    // 检查操作数量 - 应该有3个操作
    QList<QAction*> actions = lineEdit.actions();
    QCOMPARE(actions.size(), 3);
}

void tst_NLineEdit::testBorderRadius() {
    NLineEdit lineEdit;
    
    // 获取默认边框半径
    int defaultRadius = lineEdit.getBorderRadius();
    
    // 设置新的边框半径
    lineEdit.setBorderRadius(10);
    QCOMPARE(lineEdit.getBorderRadius(), 10);
    
    // 设置另一个值
    lineEdit.setBorderRadius(20);
    QCOMPARE(lineEdit.getBorderRadius(), 20);
    
    // 恢复默认值
    lineEdit.setBorderRadius(defaultRadius);
    QCOMPARE(lineEdit.getBorderRadius(), defaultRadius);
}

QTEST_MAIN(tst_NLineEdit)
#include "tst_nlineedit.moc"
