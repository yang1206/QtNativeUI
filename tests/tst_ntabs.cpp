#include <QAbstractButton>
#include <QAccessible>
#include <QCoreApplication>
#include <QIcon>
#include <QImage>
#include <QKeyEvent>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSignalSpy>
#include <QStyleOptionTab>
#include <QStyleOptionTabWidgetFrame>
#include <QTest>
#include <QToolButton>
#include <QtMath>
#include <QtNativeUI/NTabWidget.h>
#include <QtNativeUI/NTheme.h>
#include "ntabbarstyle.h"

namespace {

class InspectableTabWidget : public NTabWidget {
  public:
    QRect contentRect() const {
        QStyleOptionTabWidgetFrame option;
        initStyleOption(&option);
        return style()->subElementRect(QStyle::SE_TabWidgetTabContents, &option, this);
    }
};

class InspectableTabBar : public NTabBar {
  public:
    using NTabBar::NTabBar;
    QSize   publicTabSizeHint(int index) const { return tabSizeHint(index); }
    QString publicPaintedTabText(int index) const {
        QStyleOptionTab option;
        initStyleOption(&option, index);
        return option.text;
    }
};

void settle() {
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
}

void configureSurface(NTabWidget& tabs) {
    tabs.setLightSelectedBackground(Qt::white);
    tabs.setDarkSelectedBackground(Qt::white);
    tabs.setLightBorderColor(Qt::black);
    tabs.setDarkBorderColor(Qt::black);
    for (int i = 0; i < 3; ++i)
        tabs.addTab(new QWidget, QStringLiteral("document %1").arg(i));
}

QImage snapshot(QWidget& host, qreal dpr) {
    QImage image(QSize(qCeil(host.width() * dpr), qCeil(host.height() * dpr)), QImage::Format_ARGB32_Premultiplied);
    image.setDevicePixelRatio(dpr);
    image.fill(Qt::transparent);
    host.render(&image);
    return image;
}

QColor stripSurfaceColor() {
    return NThemeColor(NFluentColorKey::SolidBackgroundFillColorBase,
                       nTheme->isDarkMode() ? NThemeType::Dark : NThemeType::Light);
}

QPoint outerFootSample(const QRect& bar, const QRect& selected, qreal dpr) {
    const int x = selected.left() == bar.left() ? bar.left() - 1 : bar.right() + 1;
    return QPoint(qFloor((x + 0.5) * dpr), qFloor(bar.bottom() * dpr) - 1);
}

void setHostBackground(QWidget& host) {
    QPalette palette = host.palette();
    palette.setColor(QPalette::Window, QColor(180, 70, 120));
    host.setPalette(palette);
    host.setAutoFillBackground(true);
    host.resize(600, 320);
}

} // namespace

class TestNTabs : public QObject {
    Q_OBJECT

  private slots:
    void nativeCloseSignalIsForwardedOnce();
    void closabilityFollowsTheTab();
    void closePolicyKeepsLayoutStable();
    void movingTabsKeepsPagesInSync();
    void explicitPagePaletteIsPreserved();
    void addButtonDoesNotOverlap_data();
    void addButtonDoesNotOverlap();
    void hiddenBarDoesNotReserveStrip();
    void customTabButtonKeepsNativeBehavior();
    void compactModeOnlyCollapsesIconTabs();
    void sizeToContentKeepsFullTextWhenTabsFit();
    void tabDragApiDelegatesToTabBar();
    void focusVisualRequiresKeyboardFocus();
    void explicitTabTextColorIsPainted();
    void shortcutsWorkInsideCurrentPage();
    void accessibilityExposesTabsAndActions();
    void documentModeKeepsQtSemantics();
    void transparentBackgroundIsOptIn();
    void selectedTabJoinsSurface_data();
    void selectedTabJoinsSurface();
    void outerTabFeetAreNotClipped_data();
    void outerTabFeetAreNotClipped();
    void tabStripSharesTheOuterFrame_data();
    void tabStripSharesTheOuterFrame();
};

void TestNTabs::nativeCloseSignalIsForwardedOnce() {
    NTabWidget tabs;
    tabs.addTab(new QWidget, QStringLiteral("first"));
    tabs.addTab(new QWidget, QStringLiteral("second"));
    tabs.resize(600, 240);
    tabs.show();
    QSignalSpy closeSpy(&tabs, &QTabWidget::tabCloseRequested);
    // 分别覆盖子类、Qt 基类和元对象属性三条设置路径。
    tabs.setTabsClosable(true);
    static_cast<QTabWidget*>(&tabs)->setTabsClosable(false);
    static_cast<QTabWidget*>(&tabs)->setTabsClosable(true);
    settle();
    auto* button = qobject_cast<QAbstractButton*>(tabs.tabBar()->tabButton(0, QTabBar::RightSide));
    QVERIFY(button);
    tabs.setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::Never);
    settle();
    QVERIFY(button->isHidden());
    tabs.setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::Always);
    settle();
    button->click();
    QCOMPARE(closeSpy.count(), 1);
    static_cast<QTabWidget*>(&tabs)->setTabsClosable(false);
    QVERIFY(tabs.setProperty("tabsClosable", true));
    settle();
    button = qobject_cast<QAbstractButton*>(tabs.tabBar()->tabButton(0, QTabBar::RightSide));
    QVERIFY(button);
    button->click();
    QCOMPARE(closeSpy.count(), 2);
    QCOMPARE(closeSpy.first().first().toInt(), 0);
    QCOMPARE(tabs.count(), 2); // 请求由业务处理，不自行销毁页面。
    tabs.setTabsClosable(false);
    settle();
    QVERIFY(!tabs.tabBar()->tabButton(0, QTabBar::RightSide));
}

void TestNTabs::closabilityFollowsTheTab() {
    NTabWidget tabs;
    tabs.addTab(new QWidget, QStringLiteral("locked"));
    tabs.addTab(new QWidget, QStringLiteral("normal"));
    tabs.setTabsClosable(true);
    tabs.setTabClosable(0, false);
    tabs.tabBar()->setTabData(0, QStringLiteral("business payload"));
    tabs.tabBar()->moveTab(0, 1);
    tabs.insertTab(0, new QWidget, QStringLiteral("inserted"));
    QCOMPARE(tabs.tabText(2), QStringLiteral("locked"));
    QVERIFY(!tabs.getTabClosable(2));
    QCOMPARE(tabs.tabBar()->tabData(2).toString(), QStringLiteral("business payload"));
    QSignalSpy closeSpy(&tabs, &QTabWidget::tabCloseRequested);
    tabs.tabBar()->requestTabClose(2);
    tabs.setCurrentIndex(2);
    QKeyEvent closeEvent(QEvent::KeyPress, Qt::Key_F4, Qt::ControlModifier);
    QCoreApplication::sendEvent(&tabs, &closeEvent);
    QCOMPARE(closeSpy.count(), 0);
    tabs.tabBar()->requestTabClose(1);
    QCOMPARE(closeSpy.count(), 1);
}

void TestNTabs::closePolicyKeepsLayoutStable() {
    NTabWidget tabs;
    tabs.addTab(new QWidget, QStringLiteral("first"));
    tabs.addTab(new QWidget, QStringLiteral("second"));
    tabs.setTabsClosable(true);
    tabs.setTabWidthMode(NTabBar::TabWidthMode::SizeToContent);
    tabs.setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::OnHover);
    tabs.resize(600, 240);
    tabs.show();
    settle();
    auto* bar           = tabs.tabBar();
    auto* selectedClose = bar->tabButton(0, QTabBar::RightSide);
    auto* otherClose    = bar->tabButton(1, QTabBar::RightSide);
    QVERIFY(selectedClose);
    QVERIFY(otherClose);
    QVERIFY(selectedClose->isVisible());
    QVERIFY(otherClose->isHidden());
    const QRect before = bar->tabRect(1);
    QTest::mouseMove(bar, before.center());
    settle();
    QCOMPARE(bar->tabRect(1), before);
    QVERIFY(otherClose->isVisible());
    tabs.setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::Never);
    settle();
    QVERIFY(selectedClose->isHidden());
    QVERIFY(otherClose->isHidden());
}

void TestNTabs::movingTabsKeepsPagesInSync() {
    NTabWidget tabs;
    auto*      first  = new QWidget;
    auto*      second = new QWidget;
    tabs.addTab(first, QStringLiteral("first"));
    tabs.addTab(second, QStringLiteral("second"));
    tabs.setCurrentWidget(first);
    tabs.tabBar()->moveTab(0, 1);
    QCOMPARE(tabs.widget(1), first);
    QCOMPARE(tabs.widget(0), second);
    QCOMPARE(tabs.currentWidget(), first);
}

void TestNTabs::explicitPagePaletteIsPreserved() {
    NTabWidget tabs;
    auto*      page   = new QWidget;
    QPalette   custom = page->palette();
    custom.setColor(QPalette::Window, QColor(23, 45, 67));
    page->setPalette(custom);
    page->setAutoFillBackground(false);
    tabs.addTab(page, QStringLiteral("custom"));
    tabs.setLightSelectedBackground(QColor(210, 220, 230));
    tabs.setDarkSelectedBackground(QColor(30, 40, 50));
    QCOMPARE(page->palette().color(QPalette::Window), QColor(23, 45, 67));
    QVERIFY(!page->autoFillBackground());
}

void TestNTabs::addButtonDoesNotOverlap_data() {
    QTest::addColumn<int>("position");
    QTest::addColumn<bool>("rtl");
    QTest::addColumn<int>("tabCount");
    for (int position :
         {int(QTabWidget::North), int(QTabWidget::South), int(QTabWidget::West), int(QTabWidget::East)}) {
        for (bool rtl : {false, true}) {
            for (int count : {1, 12}) {
                const QByteArray name =
                    QByteArray::number(position) + "/" + QByteArray::number(rtl) + "/" + QByteArray::number(count);
                QTest::newRow(name.constData()) << position << rtl << count;
            }
        }
    }
}

void TestNTabs::addButtonDoesNotOverlap() {
    QFETCH(int, position);
    QFETCH(bool, rtl);
    QFETCH(int, tabCount);
    NTabWidget tabs;
    tabs.setTabPosition(static_cast<QTabWidget::TabPosition>(position));
    tabs.setLayoutDirection(rtl ? Qt::RightToLeft : Qt::LeftToRight);
    tabs.setAddButtonVisible(true);
    for (int i = 0; i < tabCount; ++i)
        tabs.addTab(new QWidget, QStringLiteral("document %1").arg(i));
    tabs.resize(480, 360);
    tabs.show();
    settle();
    auto* add = tabs.findChild<QToolButton*>(QStringLiteral("NTabWidgetAddButton"));
    QVERIFY(add);
    QVERIFY(add->isVisible());
    QVERIFY(tabs.rect().contains(add->geometry()));
    QVERIFY(!tabs.tabBar()->geometry().intersects(add->geometry()));
    QSignalSpy addSpy(&tabs, &NTabWidget::tabAddRequested);
    add->click();
    QCOMPARE(addSpy.count(), 1);
}

void TestNTabs::hiddenBarDoesNotReserveStrip() {
    InspectableTabWidget tabs;
    tabs.addTab(new QWidget, QStringLiteral("only"));
    tabs.setTabBarAutoHide(true);
    tabs.resize(400, 240);
    tabs.show();
    settle();
    QVERIFY(tabs.tabBar()->isHidden());
    QVERIFY(tabs.contentRect().top() <= tabs.getBorderRadius());
}

void TestNTabs::customTabButtonKeepsNativeBehavior() {
    InspectableTabBar bar;
    bar.addTab(QStringLiteral("custom action"));
    bar.setTabsClosable(true);
    auto* custom = new QPushButton(QStringLiteral("操作"));
    custom->setFixedSize(72, 28);
    custom->setFocusPolicy(Qt::StrongFocus);
    bar.setTabButton(0, QTabBar::RightSide, custom);
    bar.setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::Never);
    settle();
    QCOMPARE(bar.tabButton(0, QTabBar::RightSide), custom);
    QCOMPARE(custom->size(), QSize(72, 28));
    QCOMPARE(custom->focusPolicy(), Qt::StrongFocus);
    QVERIFY(!custom->isHidden());
    QVERIFY(bar.publicTabSizeHint(0).width() >= custom->width() + 16);
}

void TestNTabs::compactModeOnlyCollapsesIconTabs() {
    InspectableTabBar bar;
    bar.addTab(QStringLiteral("文档一"));
    bar.addTab(QStringLiteral("文档二"));
    bar.setCurrentIndex(0);
    bar.setTabMinimumWidth(180);
    bar.setTabMaximumWidth(180);
    bar.setTabWidthMode(NTabBar::TabWidthMode::Compact);
    QVERIFY(bar.publicTabSizeHint(1).width() >= bar.getTabMinimumWidth());

    const QColor textColor(230, 20, 40);
    bar.setTabTextColor(1, textColor);
    bar.resize(400, bar.sizeHint().height());
    bar.show();
    settle();
    const QRect  unselectedTab         = bar.tabRect(1);
    const QImage image                 = bar.grab().toImage().convertToFormat(QImage::Format_ARGB32);
    const qreal  dpr                   = image.devicePixelRatio();
    bool         foundLeadingTextPixel = false;
    const int    leadingEnd            = unselectedTab.left() + unselectedTab.width() / 3;
    for (int y = qFloor(unselectedTab.top() * dpr); y < qCeil(unselectedTab.bottom() * dpr) && !foundLeadingTextPixel;
         ++y) {
        for (int x = qFloor(unselectedTab.left() * dpr); x < qCeil(leadingEnd * dpr); ++x) {
            const QColor color = image.pixelColor(x, y);
            if (color.red() > 180 && color.green() < 90 && color.blue() < 100 && color.alpha() > 150) {
                foundLeadingTextPixel = true;
                break;
            }
        }
    }
    QVERIFY(foundLeadingTextPixel);

    QPixmap icon(16, 16);
    icon.fill(Qt::red);
    bar.setTabIcon(1, QIcon(icon));
    QVERIFY(bar.publicTabSizeHint(1).width() < bar.getTabMinimumWidth());
    bar.setCurrentIndex(1);
    QVERIFY(bar.publicTabSizeHint(1).width() >= bar.getTabMinimumWidth());
}

void TestNTabs::sizeToContentKeepsFullTextWhenTabsFit() {
    InspectableTabBar bar;
    bar.setTabsClosable(true);
    bar.setCloseButtonDisplayMode(NTabBar::CloseButtonDisplayMode::OnHover);
    bar.setTabWidthMode(NTabBar::TabWidthMode::SizeToContent);
    QPixmap iconPixmap(16, 16);
    iconPixmap.fill(Qt::red);
    const QIcon icon(iconPixmap);
    for (int number = 9; number <= 12; ++number)
        bar.addTab(icon, QStringLiteral("文档 %1").arg(number));
    bar.setTabClosable(0, false);
    bar.setCurrentIndex(3);
    bar.resize(900, bar.sizeHint().height());
    bar.show();
    settle();

    int requiredWidth = 0;
    for (int index = 0; index < bar.count(); ++index)
        requiredWidth += bar.publicTabSizeHint(index).width();
    QVERIFY(requiredWidth < bar.width());
    for (int index = 0; index < bar.count(); ++index)
        QCOMPARE(bar.publicPaintedTabText(index), bar.tabText(index));
}

void TestNTabs::tabDragApiDelegatesToTabBar() {
    NTabWidget tabs;
    QVERIFY(!tabs.getTabDragEnabled());
    QVERIFY(!tabs.tabBar()->acceptDrops());
    tabs.setTabDragEnabled(true);
    QVERIFY(tabs.getTabDragEnabled());
    QVERIFY(tabs.tabBar()->acceptDrops());
    tabs.setTabDragEnabled(false);
    QVERIFY(!tabs.getTabDragEnabled());
    QVERIFY(!tabs.tabBar()->acceptDrops());

    tabs.tabBar()->setChangeCurrentOnDrag(true);
    tabs.setTabDragEnabled(true);
    tabs.setTabDragEnabled(false);
    QVERIFY(tabs.tabBar()->acceptDrops());
}

void TestNTabs::focusVisualRequiresKeyboardFocus() {
    QVERIFY(!nShouldPaintFocusVisual(QStyle::State_None));
    QVERIFY(!nShouldPaintFocusVisual(QStyle::State_HasFocus));
    QVERIFY(!nShouldPaintFocusVisual(QStyle::State_KeyboardFocusChange));
    QVERIFY(nShouldPaintFocusVisual(QStyle::State_HasFocus | QStyle::State_KeyboardFocusChange));
}

void TestNTabs::explicitTabTextColorIsPainted() {
    InspectableTabBar bar;
    bar.addTab(QStringLiteral("Colored"));
    bar.setTabTextColor(0, QColor(230, 20, 40));
    bar.resize(240, bar.sizeHint().height());
    bar.show();
    settle();
    const QImage image              = bar.grab().toImage().convertToFormat(QImage::Format_ARGB32);
    bool         foundExplicitColor = false;
    for (int y = 0; y < image.height() && !foundExplicitColor; ++y) {
        for (int x = 0; x < image.width(); ++x) {
            const QColor color = image.pixelColor(x, y);
            if (color.red() > 180 && color.green() < 90 && color.blue() < 100 && color.alpha() > 150) {
                foundExplicitColor = true;
                break;
            }
        }
    }
    QVERIFY(foundExplicitColor);
}

void TestNTabs::shortcutsWorkInsideCurrentPage() {
    NTabWidget tabs;
    auto*      editor = new QPlainTextEdit;
    tabs.addTab(editor, QStringLiteral("editor"));
    tabs.setTabsClosable(true);
    tabs.setAddButtonVisible(true);
    tabs.resize(480, 260);
    tabs.show();
    editor->setFocus();
    settle();
    QSignalSpy closeSpy(&tabs, &QTabWidget::tabCloseRequested);
    QSignalSpy addSpy(&tabs, &NTabWidget::tabAddRequested);
    QTest::keyClick(editor, Qt::Key_W, Qt::ControlModifier);
    QTest::keyClick(editor, Qt::Key_T, Qt::ControlModifier);
    QCOMPARE(closeSpy.count(), 1);
    QCOMPARE(addSpy.count(), 1);
}

void TestNTabs::accessibilityExposesTabsAndActions() {
    NTabWidget tabs;
    tabs.addTab(new QWidget, QStringLiteral("&Home"));
    tabs.addTab(new QWidget, QStringLiteral("Settings"));
    tabs.tabBar()->setAccessibleTabName(0, QStringLiteral("首页标签"));
    tabs.setTabsClosable(true);
    tabs.setAddButtonVisible(true);
    tabs.resize(480, 260);
    tabs.show();
    settle();

    QAccessibleInterface* tabList = QAccessible::queryAccessibleInterface(tabs.tabBar());
    QVERIFY(tabList);
    QCOMPARE(tabList->role(), QAccessible::PageTabList);
    QVERIFY(tabList->childCount() >= tabs.count());
    QAccessibleInterface* firstTab  = tabList->child(0);
    QAccessibleInterface* secondTab = tabList->child(1);
    QVERIFY(firstTab);
    QVERIFY(secondTab);
    QCOMPARE(firstTab->role(), QAccessible::PageTab);
    QCOMPARE(firstTab->text(QAccessible::Name), QStringLiteral("首页标签"));
    QVERIFY(firstTab->state().selected);
    auto* action = static_cast<QAccessibleActionInterface*>(secondTab->interface_cast(QAccessible::ActionInterface));
    QVERIFY(action);
    QVERIFY(action->actionNames().contains(QAccessibleActionInterface::pressAction()));
    action->doAction(QAccessibleActionInterface::pressAction());
    QCOMPARE(tabs.currentIndex(), 1);

    auto* closeButton = qobject_cast<QAbstractButton*>(tabs.tabBar()->tabButton(1, QTabBar::RightSide));
    auto* addButton   = tabs.findChild<QToolButton*>(QStringLiteral("NTabWidgetAddButton"));
    QVERIFY(closeButton);
    QVERIFY(addButton);
    QVERIFY(!closeButton->accessibleName().isEmpty());
    QVERIFY(!closeButton->accessibleDescription().isEmpty());
    QCOMPARE(closeButton->focusPolicy(), Qt::StrongFocus);
    QCOMPARE(addButton->accessibleName(), QStringLiteral("添加标签"));
    QVERIFY(!addButton->accessibleDescription().isEmpty());
}

void TestNTabs::documentModeKeepsQtSemantics() {
    NTabWidget tabs;
    QVERIFY(!tabs.documentMode());
    tabs.setDocumentMode(true);
    QVERIFY(tabs.documentMode());
    QVERIFY(tabs.tabBar()->documentMode());
    QVERIFY(tabs.tabBar()->drawBase());
    tabs.setDocumentMode(false);
    QVERIFY(!tabs.tabBar()->documentMode());
    QVERIFY(!tabs.tabBar()->drawBase());
}

void TestNTabs::transparentBackgroundIsOptIn() {
    QWidget host;
    setHostBackground(host);
    InspectableTabWidget tabs(&host);
    configureSurface(tabs);
    tabs.setGeometry(12, 12, 576, 296);

    QVERIFY(!tabs.getTransparentBackground());
    QCOMPARE(tabs.palette().color(QPalette::Window), QColor(Qt::white));
    tabs.setTransparentBackground(true);
    QCOMPARE(tabs.palette().color(QPalette::Window), QColor(Qt::transparent));
    QCOMPARE(tabs.palette().color(QPalette::Base), QColor(Qt::transparent));

    host.show();
    settle();
    const QPoint paneSample = tabs.mapTo(&host, tabs.contentRect().center());
    QCOMPARE(snapshot(host, 1.0).pixelColor(paneSample), QColor(180, 70, 120));

    tabs.setTransparentBackground(false);
    settle();
    QCOMPARE(tabs.palette().color(QPalette::Window), QColor(Qt::white));
    QCOMPARE(snapshot(host, 1.0).pixelColor(paneSample), QColor(Qt::white));
}

void TestNTabs::selectedTabJoinsSurface_data() {
    QTest::addColumn<int>("position");
    QTest::addColumn<qreal>("dpr");
    for (int position :
         {int(QTabWidget::North), int(QTabWidget::South), int(QTabWidget::West), int(QTabWidget::East)}) {
        for (qreal dpr : {1.0, 1.25, 1.5, 1.75, 2.0}) {
            const QByteArray name = QByteArray::number(position) + "/" + QByteArray::number(dpr);
            QTest::newRow(name.constData()) << position << dpr;
        }
    }
}

void TestNTabs::selectedTabJoinsSurface() {
    QFETCH(int, position);
    QFETCH(qreal, dpr);
    QWidget host;
    setHostBackground(host);
    NTabWidget tabs(&host);
    configureSurface(tabs);
    tabs.setTabPosition(static_cast<QTabWidget::TabPosition>(position));
    tabs.setGeometry(12, 12, 576, 296);
    host.show();
    settle();
    const auto* bar = tabs.tabBar();
    const QRect barRect(bar->mapTo(&host, QPoint()), bar->size());
    const QRect selected   = bar->tabRect(tabs.currentIndex()).translated(barRect.topLeft());
    const bool  horizontal = position == QTabWidget::North || position == QTabWidget::South;
    int         boundary   = 0;
    switch (position) {
        case QTabWidget::North:
            boundary = barRect.bottom() + 1;
            break;
        case QTabWidget::South:
            boundary = barRect.top();
            break;
        case QTabWidget::West:
            boundary = barRect.right() + 1;
            break;
        case QTabWidget::East:
            boundary = barRect.left();
            break;
    }
    const int    middle = qFloor(((horizontal ? selected.center().x() : selected.center().y()) + 0.5) * dpr);
    const QImage image  = snapshot(host, dpr);
    // 标签与页面接缝两侧都应完全覆盖背景，包含 125%/150%/175% 下的物理像素。
    for (int axis = qFloor((boundary - 2) * dpr); axis < qCeil((boundary + 2) * dpr); ++axis) {
        const QPoint pixel = horizontal ? QPoint(middle, axis) : QPoint(axis, middle);
        QCOMPARE(image.pixelColor(pixel), QColor(Qt::white));
    }
}

void TestNTabs::outerTabFeetAreNotClipped_data() {
    QTest::addColumn<bool>("rtl");
    QTest::addColumn<int>("index");
    QTest::addColumn<qreal>("dpr");
    for (bool rtl : {false, true}) {
        for (int index : {0, 2}) {
            for (qreal dpr : {1.0, 1.25, 1.5, 1.75, 2.0}) {
                const QByteArray name =
                    QByteArray::number(rtl) + "/" + QByteArray::number(index) + "/" + QByteArray::number(dpr);
                QTest::newRow(name.constData()) << rtl << index << dpr;
            }
        }
    }
}

void TestNTabs::outerTabFeetAreNotClipped() {
    QFETCH(bool, rtl);
    QFETCH(int, index);
    QFETCH(qreal, dpr);
    QWidget host;
    setHostBackground(host);
    NTabWidget tabs(&host);
    configureSurface(tabs);
    tabs.setLayoutDirection(rtl ? Qt::RightToLeft : Qt::LeftToRight);
    tabs.setCurrentIndex(index);
    tabs.setGeometry(12, 12, 576, 296);
    host.show();
    settle();
    const auto* bar = tabs.tabBar();
    const QRect barRect(bar->mapTo(&host, QPoint()), bar->size());
    const QRect selected = bar->tabRect(index).translated(barRect.topLeft());
    QVERIFY(selected.left() == barRect.left() || selected.right() == barRect.right());
    const QPoint sample = outerFootSample(barRect, selected, dpr);
    // 外扩弧应覆盖标签栏底色，不受子控件裁剪限制。
    const QImage image = snapshot(host, dpr);
    QVERIFY(image.pixelColor(sample) != stripSurfaceColor());
    tabs.setBorderWidth(0);
    settle();
    const QRect  framelessBar(bar->mapTo(&host, QPoint()), bar->size());
    const QRect  framelessSelected = bar->tabRect(index).translated(framelessBar.topLeft());
    const QPoint framelessSample   = outerFootSample(framelessBar, framelessSelected, dpr);
    QVERIFY(snapshot(host, dpr).pixelColor(framelessSample) != stripSurfaceColor());

    // 切到中间标签后，原来的栏外圆弧也必须被清除。
    tabs.setCurrentIndex(1);
    settle();
    const QImage moved = snapshot(host, dpr);
    QCOMPARE(moved.pixelColor(framelessSample), stripSurfaceColor());
}

void TestNTabs::tabStripSharesTheOuterFrame_data() {
    QTest::addColumn<int>("position");
    QTest::addColumn<bool>("rtl");
    QTest::addColumn<qreal>("dpr");
    for (int position :
         {int(QTabWidget::North), int(QTabWidget::South), int(QTabWidget::West), int(QTabWidget::East)}) {
        for (bool rtl : {false, true}) {
            for (qreal dpr : {1.0, 1.25, 1.5, 1.75, 2.0}) {
                const QByteArray name =
                    QByteArray::number(position) + "/" + QByteArray::number(rtl) + "/" + QByteArray::number(dpr);
                QTest::newRow(name.constData()) << position << rtl << dpr;
            }
        }
    }
}

void TestNTabs::tabStripSharesTheOuterFrame() {
    QFETCH(int, position);
    QFETCH(bool, rtl);
    QFETCH(qreal, dpr);
    QWidget host;
    setHostBackground(host);
    NTabWidget tabs(&host);
    configureSurface(tabs);
    tabs.setTabPosition(static_cast<QTabWidget::TabPosition>(position));
    tabs.setLayoutDirection(rtl ? Qt::RightToLeft : Qt::LeftToRight);
    tabs.setGeometry(12, 12, 576, 296);
    host.show();
    settle();
    const QRect frame      = tabs.geometry();
    const bool  horizontal = position == QTabWidget::North || position == QTabWidget::South;
    for (int index : {0, 1, 2}) {
        tabs.setCurrentIndex(index);
        settle();
        const auto*  bar    = tabs.tabBar();
        const QPoint center = bar->mapTo(&host, bar->rect().center());
        const QPoint first  = horizontal ? QPoint(frame.left() + 2, center.y()) : QPoint(center.x(), frame.top() + 2);
        const QPoint last = horizontal ? QPoint(frame.right() - 2, center.y()) : QPoint(center.x(), frame.bottom() - 2);
        const QPoint firstEdge = horizontal ? QPoint(frame.left(), center.y()) : QPoint(center.x(), frame.top());
        const QPoint lastEdge  = horizontal ? QPoint(frame.right(), center.y()) : QPoint(center.x(), frame.bottom());
        const QImage image     = snapshot(host, dpr);
        auto         pixel     = [dpr](const QPoint& point) {
            return QPoint(qFloor((point.x() + 0.5) * dpr), qFloor((point.y() + 0.5) * dpr));
        };
        // 标签栏两端必须覆盖宿主背景，外框边线也应贯穿整个标签栏高度。
        QCOMPARE(image.pixelColor(pixel(first)), stripSurfaceColor());
        QCOMPARE(image.pixelColor(pixel(last)), stripSurfaceColor());
        QCOMPARE(image.pixelColor(pixel(firstEdge)), QColor(Qt::black));
        QCOMPARE(image.pixelColor(pixel(lastEdge)), QColor(Qt::black));
    }
}

QTEST_MAIN(TestNTabs)
#include "tst_ntabs.moc"
