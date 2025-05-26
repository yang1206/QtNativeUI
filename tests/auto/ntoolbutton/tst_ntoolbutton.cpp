#include <QtTest>
#include <QApplication>
#include <QSignalSpy>
#include <QtNativeUI/NToolButton.h>
#include <QtNativeUI/NTheme.h>

// 创建一个用于测试的NToolButton子类，使protected方法可访问
class TestToolButton : public NToolButton
{
public:
    using NToolButton::NToolButton;  // 继承构造函数
    
    // 公开protected方法
    QSize publicSizeHint() const { return sizeHint(); }
    QSize publicMinimumSizeHint() const { return minimumSizeHint(); }
};

class tst_NToolButton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 测试创建与默认属性
    void testConstruction();
    void testDefaultProperties();
    
    // 测试信号
    void testClickedSignal();
    
    // 测试图标功能
    void testSetFluentIcon();
    
    // 测试样式与颜色
    void testAccentColor();
};

void tst_NToolButton::initTestCase()
{
}

void tst_NToolButton::cleanupTestCase()
{
}

void tst_NToolButton::init()
{
}

void tst_NToolButton::cleanup()
{
}

void tst_NToolButton::testConstruction()
{
    // 测试默认构造函数
    NToolButton button1;
    QVERIFY(button1.isEnabled());
    
    // 测试带图标的构造函数（如果有）
    NToolButton button2;
    button2.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button2.icon().isNull());
}

void tst_NToolButton::testDefaultProperties()
{
    NToolButton button;
    
    // 检查默认属性 - 根据NToolButton的实际属性调整
    QVERIFY(button.isEnabled());
    QVERIFY(!button.autoRaise());  // NToolButton默认不是自动升起的
}

void tst_NToolButton::testClickedSignal()
{
    NToolButton button;
    QSignalSpy spy(&button, &NToolButton::clicked);
    
    // 显示按钮以确保鼠标事件能被处理
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));
    
    // 模拟点击按钮
    QTest::mouseClick(&button, Qt::LeftButton);
    
    // 验证信号是否被触发
    QCOMPARE(spy.count(), 1);
}

void tst_NToolButton::testSetFluentIcon()
{
    NToolButton button;
    
    // 初始状态应该没有图标
    QVERIFY(button.icon().isNull());
    
    // 设置Regular图标
    button.setFluentIcon(NRegularIconType::Add12Regular);
    QVERIFY(!button.icon().isNull());
    
    // 设置Filled图标
    button.setFluentIcon(NFilledIconType::Add12Filled);
    QVERIFY(!button.icon().isNull());
    
    // 自定义大小
    button.setFluentIcon(NRegularIconType::Add12Regular, 24);
    QCOMPARE(button.iconSize(), QSize(24, 24));
}

void tst_NToolButton::testAccentColor()
{
    NToolButton button;
    
    // 设置自定义AccentColor（如果NToolButton支持）
    QColor customColor(0, 128, 255);
    // 假设有setAccentColor方法
    // button.setAccentColor(customColor);
    
    // 这里添加与AccentColor相关的测试
    // 根据NToolButton的实际API调整
}

QTEST_MAIN(tst_NToolButton)
#include "tst_ntoolbutton.moc"
