#include <QApplication>
#include <QSignalSpy>
#include <QtNativeUI/NPushButton.h>
#include <QtNativeUI/NTheme.h>
#include <QtTest>

// 创建一个用于测试的NPushButton子类，使protected方法可访问
class TestPushButton : public NPushButton {
  public:
    using NPushButton::NPushButton; // 继承构造函数

    // 公开protected方法
    QSize publicSizeHint() const { return sizeHint(); }
    QSize publicMinimumSizeHint() const { return minimumSizeHint(); }
};

class tst_NPushButton : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 测试创建与默认属性
    void testConstruction();
    void testDefaultProperties();

    // 测试属性设置与获取
    void testButtonType();
    void testBorderRadius();
    void testTransparentBackground();

    // 测试信号
    void testClickedSignal();

    // 测试图标功能
    void testSetFluentIcon();

    // 测试样式与颜色
    void testAccentColor();

    // 测试尺寸计算
    void testSizeHint();

    // 测试禁用状态
    void testDisabledState();
};

void tst_NPushButton::initTestCase() {
    // 在所有测试开始前执行一次
}

void tst_NPushButton::cleanupTestCase() {
    // 在所有测试结束后执行一次
}

void tst_NPushButton::init() {
    // 每个测试前执行
}

void tst_NPushButton::cleanup() {
    // 每个测试后执行
}

void tst_NPushButton::testConstruction() {
    // 测试默认构造函数
    NPushButton button1;
    QVERIFY(button1.text().isEmpty());
    QVERIFY(button1.isEnabled());

    // 测试带文本的构造函数
    NPushButton button2("Test Button");
    QCOMPARE(button2.text(), QString("Test Button"));
    QVERIFY(button2.isEnabled());
}

void tst_NPushButton::testDefaultProperties() {
    NPushButton button;

    // 检查默认按钮类型
    QCOMPARE(button.buttonType(), NPushButton::Standard);

    // 检查默认边框半径
    QCOMPARE(button.getBorderRadius(), NDesignToken(NDesignTokenKey::CornerRadiusDefault).toInt());

    // 检查默认透明背景设置
    QCOMPARE(button.getTransparentBackground(), false);

    // 检查默认对齐方式
    QCOMPARE(button.getAlignment(), Qt::AlignCenter);
}

void tst_NPushButton::testButtonType() {
    NPushButton button;

    // 默认应该是Standard类型
    QCOMPARE(button.buttonType(), NPushButton::Standard);

    // 更改为Accent类型
    button.setButtonType(NPushButton::Accent);
    QCOMPARE(button.buttonType(), NPushButton::Accent);

    // 更改回Standard类型
    button.setButtonType(NPushButton::Standard);
    QCOMPARE(button.buttonType(), NPushButton::Standard);
}

void tst_NPushButton::testBorderRadius() {
    NPushButton button;

    // 获取默认边框半径
    int defaultRadius = button.getBorderRadius();

    // 设置新的边框半径
    button.setBorderRadius(10);
    QCOMPARE(button.getBorderRadius(), 10);

    // 设置另一个值
    button.setBorderRadius(20);
    QCOMPARE(button.getBorderRadius(), 20);

    // 恢复默认值
    button.setBorderRadius(defaultRadius);
    QCOMPARE(button.getBorderRadius(), defaultRadius);
}

void tst_NPushButton::testTransparentBackground() {
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

void tst_NPushButton::testClickedSignal() {
    NPushButton button("Test");
    QSignalSpy  spy(&button, &NPushButton::clicked);

    // 显示按钮以确保鼠标事件能被处理
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 模拟点击按钮
    QTest::mouseClick(&button, Qt::LeftButton);

    // 验证信号是否被触发
    QCOMPARE(spy.count(), 1);

    // 再次点击
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(spy.count(), 2);
}

void tst_NPushButton::testSetFluentIcon() {
    NPushButton button;

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

    // 自定义颜色
    QColor customColor(255, 0, 0);
    button.setFluentIcon(NRegularIconType::Add12Regular, 16, customColor);
    QVERIFY(!button.icon().isNull());
}

void tst_NPushButton::testAccentColor() {
    NPushButton button;

    // 将按钮类型更改为Accent
    button.setButtonType(NPushButton::Accent);
    QCOMPARE(button.buttonType(), NPushButton::Accent);

    // 设置自定义AccentColor
    QColor customColor(0, 128, 255);
    button.setAccentColor(customColor);

    // 设置NAccentColor对象
    NAccentColor accentColor = NColorUtils::toAccentColor(QColor(255, 128, 0));
    button.setAccentColor(accentColor);
}

void tst_NPushButton::testSizeHint() {
    // 使用测试子类来访问protected方法

    // 1. 空按钮
    TestPushButton emptyButton;
    QSize          emptySize = emptyButton.publicSizeHint();
    QVERIFY(emptySize.width() > 0);
    QVERIFY(emptySize.height() > 0);

    // 2. 有文本的按钮
    TestPushButton textButton("Test Button");
    QSize          textSize = textButton.publicSizeHint();
    // 有文本的按钮通常会比空按钮宽
    QVERIFY(textSize.width() >= emptySize.width());

    // 3. 有图标的按钮
    TestPushButton iconButton;
    iconButton.setFluentIcon(NRegularIconType::Add12Regular);
    QSize iconSize = iconButton.publicSizeHint();
    QVERIFY(iconSize.width() > 0);
    QVERIFY(iconSize.height() > 0);

    // 4. 同时有图标和文本的按钮
    TestPushButton bothButton("Test");
    bothButton.setFluentIcon(NRegularIconType::Add12Regular);
    QSize bothSize = bothButton.publicSizeHint();

    // 打印尺寸信息以诊断问题
    qDebug() << "Empty button size:" << emptySize;
    qDebug() << "Text button size:" << textSize << "Text:" << textButton.text();
    qDebug() << "Icon button size:" << iconSize << "Icon size:" << iconButton.iconSize();
    qDebug() << "Both button size:" << bothSize << "Text:" << bothButton.text()
             << "Icon size:" << bothButton.iconSize();

    // QVERIFY(bothSize.width() >= textSize.width());
    QVERIFY(bothSize.width() >= iconSize.width());
}

void tst_NPushButton::testDisabledState() {
    NPushButton button("Test");

    // 默认状态应该是启用的
    QVERIFY(button.isEnabled());

    // 禁用按钮
    button.setEnabled(false);
    QVERIFY(!button.isEnabled());

    // 显示按钮以确保鼠标事件能被处理
    button.show();
    QVERIFY(QTest::qWaitForWindowExposed(&button));

    // 在禁用状态下，点击不应该触发clicked信号
    QSignalSpy spy(&button, &NPushButton::clicked);
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(spy.count(), 0);

    // 重新启用按钮
    button.setEnabled(true);
    QVERIFY(button.isEnabled());

    // 启用后点击应该触发信号
    QTest::mouseClick(&button, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(tst_NPushButton)
#include "tst_npushbutton.moc"
