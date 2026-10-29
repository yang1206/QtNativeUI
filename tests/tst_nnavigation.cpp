#include <QAbstractItemModelTester>
#include <QLabel>
#include <QListView>
#include <QPointer>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyledItemDelegate>
#include <QSignalSpy>
#include <QTest>
#include <QTreeView>
#include <QtNativeUI/NAutoSuggestBox.h>
#include <QtNativeUI/NMenu.h>
#include <QtNativeUI/NNavigationBar.h>
#include <QtNativeUI/NNavigationRouter.h>
#include <QtNativeUI/NNavigationView.h>
#include <QtNativeUI/NPageComponent.h>
#include <QtNativeUI/NStackedWidget.h>

namespace {
constexpr auto icon = NRegularIconType::Home16Regular;
QString key(QWidget* page) { return page->property("NPageKey").toString(); }
void configure(NNavigationView& view) {
    view.setPageTransitionType(NNavigationType::NoTransition);
    view.setDisplayMode(NNavigationType::Maximal);
    view.resize(900, 600);
}
class GuardedPage : public NPageComponent {
  public:
    bool enterAllowed{true}, leaveAllowed{true};
    int enters{0}, leaves{0}, leaveChecks{0};
    bool beforeRouteEnter(const QVariantMap&) override { return enterAllowed; }
    bool beforeRouteLeave(QVariantMap&) override { ++leaveChecks; return leaveAllowed; }
    void onRouteEnter(const QVariantMap&) override { ++enters; }
    void onRouteLeave(QVariantMap&) override { ++leaves; }
};

class ColorPage : public QWidget {
  public:
    QColor color;
    QRect mark;
    int paints{0};
    ColorPage(QColor color, QRect mark) : color(color), mark(mark) {}
    void paintEvent(QPaintEvent*) override {
        ++paints;
        QPainter painter(this);
        painter.fillRect(mark, color);
    }
};

QPropertyAnimation* transitionAnimation(QWidget& widget) {
    for (auto* animation : widget.findChildren<QPropertyAnimation*>())
        if (animation->propertyName() == "progress") return animation;
    return nullptr;
}

int colorPixels(const QImage& image, QColor color) {
    int count = 0;
    for (int y = 0; y < image.height(); ++y)
        for (int x = 0; x < image.width(); ++x) {
            const QColor pixel = image.pixelColor(x, y);
            if (color == Qt::red ? pixel.red() > pixel.blue() + 40 && pixel.red() > pixel.green() + 40
                                 : pixel.blue() > pixel.red() + 40 && pixel.blue() > pixel.green() + 40)
                ++count;
        }
    return count;
}

QImage navigationRow(QTreeView* tree, const QModelIndex& index) {
    QImage image(280, 40, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.setFont(tree->font());
    QStyleOptionViewItem option;
    option.initFrom(tree);
    option.index = index;
    option.rect = image.rect();
    option.state = QStyle::State_Enabled;
    tree->style()->drawControl(QStyle::CE_ItemViewItem, &option, &painter, tree);
    return image;
}
}

class TestNNavigation : public QObject {
    Q_OBJECT
  private slots:
    void instanceIsolationAndPlainPages();
    void guardsAndLifecycle();
    void mouseActivationHonorsGuards();
    void rejectedInitialPage();
    void destructionWhileAnimating();
    void subtreeRemovalAndPersistentIndexes();
    void footerMutationAndActions();
    void pageDestructionAndDuplicateRegistration();
    void minimalAndCompactPane();
    void interruptedTransitions_data();
    void interruptedTransitions();
    void compactMenuAndKeyboard();
    void footerPaintsTitleOnce_data();
    void footerPaintsTitleOnce();
    void scaleFramesDoNotOverlap();
    void paneAndContentMoveTogether();
    void transitionFramesAndResize_data();
    void transitionFramesAndResize();
    void rippleKeepsUnrevealedCorners();
    void expandedGroupNavigationKeepsRowsVisible_data();
    void expandedGroupNavigationKeepsRowsVisible();
    void expanderToggleKeepsChildPaint();
    void standaloneStackDestructionWhileAnimating();
};

void TestNNavigation::standaloneStackDestructionWhileAnimating() {
    for (int round = 0; round < 3; ++round) {
        for (int type = NNavigationType::PopupTransition; type <= NNavigationType::RippleTransition; ++type) {
            auto* stack = new NStackedWidget;
            stack->resize(320, 240);
            stack->addWidget(new QLabel("old"));
            stack->addWidget(new QLabel("new"));
            stack->show();
            QTest::qWait(1);
            stack->doPageSwitch(static_cast<NNavigationType::PageTransitionType>(type), 1, false, 300);
            delete stack; // 顶层窗口析构发出 Hide 时，不能再访问 QStackedLayout。
        }
    }
    QTest::qWait(20);
}

void TestNNavigation::expandedGroupNavigationKeepsRowsVisible_data() {
    QTest::addColumn<bool>("nested");
    QTest::newRow("group") << false;
    QTest::newRow("nested-group") << true;
}

void TestNNavigation::expandedGroupNavigationKeepsRowsVisible() {
    QFETCH(bool, nested);
    NNavigationView view;
    configure(view);
    view.addPageNode("Home", new QWidget, icon);
    QString group, parent;
    view.addExpanderNode("Navigation", group, icon);
    parent = group;
    if (nested) view.addExpanderNode("Nested", parent, group, icon);
    auto* a = new QWidget;
    auto* b = new QWidget;
    view.addPageNode("TabWidget", a, parent, icon);
    view.addPageNode("TabBar", b, parent, icon);
    view.addPageNode("Pivot", new QWidget, parent, icon);
    view.show();
    view.navigateTo(key(a));
    QTest::qWait(650);
    auto* tree = view.findChild<QTreeView*>();
    auto parentIndex = tree->model()->index(1, 0);
    if (nested) parentIndex = tree->model()->index(0, 0, parentIndex);
    const auto witness = tree->model()->index(2, 0, parentIndex);
    const QImage before = navigationRow(tree, witness);
    const QImage parentBefore = navigationRow(tree, parentIndex);
    QImage empty(before.size(), before.format());
    empty.fill(Qt::white);
    QVERIFY(before != empty);
    QSignalSpy expanded(tree, &QTreeView::expanded);
    for (int row : {1, 0, 1}) {
        const auto index = tree->model()->index(row, 0, parentIndex);
        QTest::mouseClick(tree->viewport(), Qt::LeftButton, {}, tree->visualRect(index).center());
        QCOMPARE(view.currentWidget(), row == 0 ? a : b);
        // 切换页面不能让同组内未选中的文字消失，也不能重播展开动画。
        QCOMPARE(navigationRow(tree, witness), before);
        QCOMPARE(navigationRow(tree, parentIndex), parentBefore);
    }
    QCOMPARE(expanded.size(), 0);
}

void TestNNavigation::expanderToggleKeepsChildPaint() {
    NNavigationView view;
    configure(view);
    view.addPageNode("Home", new QWidget, icon);
    QString group;
    view.addExpanderNode("Navigation", group, icon);
    view.addPageNode("TabWidget", new QWidget, group, icon);
    view.show();
    QTest::qWait(20);
    auto* tree = view.findChild<QTreeView*>();
    const auto parent = tree->model()->index(1, 0);
    const auto child = tree->model()->index(0, 0, parent);
    const QImage before = navigationRow(tree, child);
    // QTreeView 在展开过程中使用子项快照，快照里的文字必须保持可见。
    for (int i = 0; i < 3; ++i) {
        QTest::mouseClick(tree->viewport(), Qt::LeftButton, {}, tree->visualRect(parent).center());
        QCOMPARE(navigationRow(tree, child), before);
        QTest::qWait(30);
    }
    // 通过公开接口反向请求，覆盖树视图原生动画尚未结束的情况。
    view.collpaseNavigationNode(group);
    QVERIFY(!tree->isExpanded(parent));
    QCOMPARE(navigationRow(tree, child), before);
    view.expandNavigationNode(group);
    QVERIFY(tree->isExpanded(parent));
    QTest::qWait(650);
    QCOMPARE(navigationRow(tree, child), before);
}

void TestNNavigation::footerPaintsTitleOnce_data() {
    QTest::addColumn<bool>("dark");
    QTest::newRow("light") << false;
    QTest::newRow("dark") << true;
}

void TestNNavigation::footerPaintsTitleOnce() {
    QFETCH(bool, dark);
    const auto previous = nTheme->themeMode();
    nTheme->setThemeMode(dark ? NThemeType::Dark : NThemeType::Light);
    NNavigationView view;
    configure(view);
    view.addPageNode("home", new QWidget, icon);
    QString a, b;
    view.addFooterNode("Settings", a, 0, icon);
    view.addFooterNode("WWWWWW", b, 0, icon);
    view.show();
    QTest::qWait(20);
    auto* footer = view.findChild<QListView*>("navigationFooterView");
    auto render = [&](int row) {
        QImage image(295, 40, QImage::Format_ARGB32_Premultiplied);
        image.fill(QColor(128, 128, 128));
        QPainter painter(&image);
        QStyleOptionViewItem option;
        option.initFrom(footer);
        option.rect = image.rect();
        option.widget = footer;
        option.state = QStyle::State_Enabled;
        footer->itemDelegate()->paint(&painter, option, footer->model()->index(row, 0));
        return image;
    };
    const QImage first = render(0), second = render(1);
    const auto dir = qEnvironmentVariable("QTNATIVEUI_NAV_SNAPSHOT_DIR");
    if (!dir.isEmpty()) view.grab().save(dir + (dark ? "/footer-dark.png" : "/footer-light.png"));
    nTheme->setThemeMode(previous);
    // 相同图标的区域不应随标题变化；标题只能出现在图标右侧。
    QCOMPARE(first.copy(0, 3, 39, 34), second.copy(0, 3, 39, 34));
    QCOMPARE(footer->model()->index(0, 0).data(Qt::AccessibleTextRole).toString(), QString("Settings"));
}

void TestNNavigation::scaleFramesDoNotOverlap() {
    NStackedWidget stack;
    stack.resize(400, 300);
    stack.addWidget(new ColorPage(Qt::red, QRect(45, 50, 70, 70)));
    stack.addWidget(new ColorPage(Qt::blue, QRect(230, 150, 70, 70)));
    stack.show();
    QTest::qWait(20);
    stack.doPageSwitch(NNavigationType::ScaleTransition, 1, false, 300);
    auto* animation = transitionAnimation(stack);
    QVERIFY(animation);
    animation->pause();
    animation->setCurrentTime(210);
    const QImage incoming = stack.grab().toImage();
    QVERIFY(colorPixels(incoming, Qt::blue) > 500);
    QCOMPARE(colorPixels(incoming, Qt::red), 0);
    animation->setCurrentTime(300);
    QVERIFY(stack.currentWidget()->isVisible());
    QCOMPARE(colorPixels(stack.grab().toImage(), Qt::red), 0);
}

void TestNNavigation::paneAndContentMoveTogether() {
    NNavigationView view;
    configure(view);
    view.addPageNode("home", new QWidget, icon);
    view.show();
    QTest::qWait(20);
    auto* bar = view.findChild<NNavigationBar*>();
    auto* stack = view.findChild<NStackedWidget*>();
    view.setPaneOpen(false);
    QTest::qWait(60);
    QVERIFY(bar->width() > 47 && bar->width() < 300);
    QCOMPARE(stack->x(), bar->width());
    view.setPaneOpen(true);
    QTest::qWait(60);
    QCOMPARE(stack->x(), bar->width());
    QTRY_COMPARE(stack->x(), 300);
}

void TestNNavigation::transitionFramesAndResize_data() {
    QTest::addColumn<int>("type");
    QTest::addColumn<bool>("back");
    for (int type = NNavigationType::PopupTransition; type <= NNavigationType::RippleTransition; ++type) {
        QTest::newRow(qPrintable(QString("%1-forward").arg(type))) << type << false;
        QTest::newRow(qPrintable(QString("%1-back").arg(type))) << type << true;
    }
}

void TestNNavigation::transitionFramesAndResize() {
    QFETCH(int, type);
    QFETCH(bool, back);
    NStackedWidget stack;
    stack.resize(400, 300);
    auto* oldPage = new ColorPage(Qt::red, QRect(40, 50, 70, 70));
    auto* newPage = new ColorPage(Qt::blue, QRect(240, 150, 70, 70));
    stack.addWidget(oldPage);
    stack.addWidget(newPage);
    stack.show();
    QTest::qWait(20);
    stack.doPageSwitch(static_cast<NNavigationType::PageTransitionType>(type), 1, back, 300);
    auto* animation = transitionAnimation(stack);
    QVERIFY(animation);
    animation->pause();
    const int paints = newPage->paints;
    for (int time : {30, 90, 150, 240, 290}) {
        animation->setCurrentTime(time);
        const QImage frame = stack.grab().toImage();
        QVERIFY(!frame.isNull());
        if (type == NNavigationType::ScaleTransition && time >= 150) QCOMPARE(colorPixels(frame, Qt::red), 0);
    }
    // 动画帧使用缓存画面，不反复触发业务页面的绘制。
    QCOMPARE(newPage->paints, paints);
    if (type != NNavigationType::BlurTransition) QVERIFY(!stack.graphicsEffect());
    stack.resize(450, 350);
    QCoreApplication::processEvents();
    QCOMPARE(stack.currentWidget(), newPage);
    QVERIFY(newPage->isVisible());
    QVERIFY(!oldPage->isVisible());
    QCOMPARE(newPage->size(), stack.size());
    QVERIFY(!stack.graphicsEffect());
    QVERIFY(colorPixels(stack.grab().toImage(), Qt::blue) > 1000);
    QCOMPARE(colorPixels(stack.grab().toImage(), Qt::red), 0);
}

void TestNNavigation::rippleKeepsUnrevealedCorners() {
    NStackedWidget stack;
    stack.resize(400, 400);
    stack.addWidget(new ColorPage(Qt::red, QRect(20, 20, 30, 30)));
    stack.addWidget(new ColorPage(Qt::blue, QRect(185, 185, 30, 30)));
    stack.show();
    QTest::qWait(20);
    stack.doPageSwitch(NNavigationType::RippleTransition, 1, false, 300);
    auto* animation = transitionAnimation(stack);
    QVERIFY(animation);
    animation->pause();
    animation->setCurrentTime(100); // 圆已超过半宽，但还没有覆盖到四角。
    const QImage frame = stack.grab().toImage();
    QVERIFY(colorPixels(frame, Qt::red) > 500);
    QVERIFY(colorPixels(frame, Qt::blue) > 500);
}

void TestNNavigation::instanceIsolationAndPlainPages() {
    NNavigationView a, b;
    configure(a); configure(b);
    auto* a0 = new QWidget; auto* a1 = new QWidget; auto* b0 = new QWidget; auto* b1 = new QWidget;
    a.addPageNode("a0", a0, icon); a.addPageNode("a1", a1, icon);
    b.addPageNode("b0", b0, icon); b.addPageNode("b1", b1, icon);
    QCOMPARE(a.currentRouteKey(), key(a0));
    QCOMPARE(b.currentRouteKey(), key(b0));
    QSignalSpy changes(&a, &NNavigationView::currentChanged);
    a.navigateTo(key(a1), {{"id", 42}});
    QCOMPARE(a.currentWidget(), a1);
    QCOMPARE(a.historyCount(), 1);
    QCOMPARE(changes.size(), 1);
    QCOMPARE(b.currentWidget(), b0);
    QCOMPARE(b.historyCount(), 0);
    b.navigateTo(key(a1)); // 不得跳到其他实例的页面。
    QCOMPARE(b.currentWidget(), b0);
    a.navigateBack({{"back", true}});
    QCOMPARE(a.currentWidget(), a0);
    QCOMPARE(a.currentRouteParams().value("back").toBool(), true);
    QCOMPARE(a.historyCount(), 0);
    a.navigation(key(a1), false);
    QCOMPARE(a.currentWidget(), a1);
    QCOMPARE(a.currentRouteKey(), key(a1));
    QCOMPARE(a.historyCount(), 0);
    NNavigationRouter::getInstance()->navigateTo(key(b1));
    QCOMPARE(b.currentWidget(), b1);
    QCOMPARE(a.currentWidget(), a1);
    QSignalSpy legacyState(NNavigationRouter::getInstance(), &NNavigationRouter::navigationStateChanged);
    NNavigationRouter::getInstance()->clearHistory();
    QCOMPARE(b.historyCount(), 0);
    QCOMPARE(legacyState.size(), 1);
    QCOMPARE(legacyState.first().first().toBool(), false);
}

void TestNNavigation::mouseActivationHonorsGuards() {
    NNavigationView view;
    configure(view);
    auto* a = new GuardedPage; auto* b = new GuardedPage;
    view.addPageComponent(a, icon); view.addPageComponent(b, icon);
    view.show();
    QTest::qWait(10);
    auto* tree = view.findChild<QTreeView*>();
    const auto rect = tree->visualRect(tree->model()->index(1, 0));
    QVERIFY(!rect.isEmpty());
    QSignalSpy clicked(&view, &NNavigationView::navigationNodeClicked);
    b->enterAllowed = false;
    QTest::mouseClick(tree->viewport(), Qt::LeftButton, {}, rect.center());
    QCoreApplication::processEvents();
    QCOMPARE(view.currentWidget(), a);
    QCOMPARE(clicked.size(), 0);
    b->enterAllowed = true;
    QTest::mouseClick(tree->viewport(), Qt::LeftButton, {}, rect.center());
    QTRY_COMPARE(view.currentWidget(), b);
    QCOMPARE(clicked.size(), 1);
    QCOMPARE(view.historyCount(), 1);
}

void TestNNavigation::guardsAndLifecycle() {
    NNavigationView view;
    configure(view);
    auto* a = new GuardedPage; auto* b = new GuardedPage;
    view.addPageComponent(a, icon); view.addPageComponent(b, icon);
    b->enterAllowed = false;
    view.navigation(key(b));
    QCOMPARE(view.currentWidget(), a);
    QCOMPARE(view.currentRouteKey(), key(a));
    QCOMPARE(view.historyCount(), 0);
    QCOMPARE(a->leaves, 0);
    b->enterAllowed = true;
    view.navigation(key(b));
    QCOMPARE(view.currentWidget(), b);
    QCOMPARE(a->leaves, 1);
    QCOMPARE(b->enters, 1);
    b->leaveAllowed = false;
    b->navigateBack();
    QCOMPARE(view.currentWidget(), b);
    QCOMPARE(view.historyCount(), 1);
    QCOMPARE(b->leaves, 0);
    b->leaveAllowed = true;
    const int checks = b->leaveChecks;
    b->navigateBack();
    QCOMPARE(b->leaveChecks, checks + 1);
    QCOMPARE(b->leaves, 1);
    QCOMPARE(view.currentWidget(), a);
    QCOMPARE(view.historyCount(), 0);
}

void TestNNavigation::rejectedInitialPage() {
    NNavigationView view;
    configure(view);
    auto* page = new GuardedPage;
    page->enterAllowed = false;
    QSignalSpy changes(&view, &NNavigationView::currentChanged);
    view.addPageComponent(page, icon);
    QVERIFY(!view.currentWidget());
    QCOMPARE(view.currentIndex(), -1);
    QCOMPARE(changes.size(), 0);
    page->enterAllowed = true;
    view.navigateTo(key(page));
    QCOMPARE(view.currentWidget(), page);
    QCOMPARE(changes.size(), 1);
}

void TestNNavigation::destructionWhileAnimating() {
    auto* view = new NNavigationView;
    configure(*view);
    auto* a = new QWidget; auto* b = new QWidget;
    view->addPageNode("a", a, icon); view->addPageNode("b", b, icon);
    view->show();
    QTest::qWait(10);
    view->setPageTransitionType(NNavigationType::ScaleTransition);
    view->navigateTo(key(b));
    delete view;
    QTest::qWait(650); // 样式、指示器和页面动画均不能保留已销毁视图。
}

void TestNNavigation::subtreeRemovalAndPersistentIndexes() {
    NNavigationView view;
    configure(view);
    QString group, nested;
    view.addExpanderNode("group", group, icon);
    view.addExpanderNode("nested", nested, group, icon);
    auto* a = new QWidget; auto* b = new QWidget; auto* survivor = new QWidget;
    view.addPageNode("a", a, nested, icon); view.addPageNode("b", b, group, icon);
    view.addPageNode("survivor", survivor, icon);
    auto* tree = view.findChild<QTreeView*>();
    QVERIFY(tree);
    QAbstractItemModelTester tester(tree->model(), QAbstractItemModelTester::FailureReportingMode::QtTest);
    QPersistentModelIndex survivorIndex(tree->model()->index(1, 0));
    QPointer<QObject> removedNode(static_cast<QObject*>(tree->model()->index(0, 0).internalPointer()));
    view.navigateTo(key(b));
    QSignalSpy removed(&view, &NNavigationView::navigationNodeRemoved);
    view.removeNavigationNode(group);
    QCOMPARE(view.count(), 1);
    QCOMPARE(view.currentWidget(), survivor);
    QCOMPARE(view.currentRouteKey(), key(survivor));
    QCOMPARE(view.historyCount(), 0);
    QCOMPARE(removed.size(), 4);
    QVERIFY(removedNode.isNull());
    QVERIFY(survivorIndex.isValid());
    QCOMPARE(survivorIndex.row(), 0);
    QCOMPARE(survivorIndex.data().toString(), QString("survivor"));
    view.removeNavigationNode(key(survivor));
    QCOMPARE(view.count(), 0);
    QVERIFY(view.currentRouteKey().isEmpty());
}

void TestNNavigation::footerMutationAndActions() {
    NNavigationView view;
    configure(view);
    view.addPageNode("home", new QWidget, icon);
    QStringList footerKeys;
    for (int i = 0; i < 5; ++i) {
        QString footer;
        QCOMPARE(view.addFooterNode(QString::number(i), new QWidget, footer, 0, icon), NNavigationType::Success);
        footerKeys.append(footer);
    }
    auto* footer = view.findChild<QListView*>("navigationFooterView");
    QVERIFY(footer);
    QAbstractItemModelTester tester(footer->model(), QAbstractItemModelTester::FailureReportingMode::QtTest);
    QPersistentModelIndex kept(footer->model()->index(3, 0));
    view.navigateTo(footerKeys.at(3));
    view.removeNavigationNode(footerKeys.first());
    QCOMPARE(kept.row(), 2);
    QCOMPARE(kept.data().toString(), QString("3"));
    QCOMPARE(view.currentRouteKey(), footerKeys.at(3));
    QString action;
    view.addFooterNode("action", action, 0, icon);
    const QString before = view.currentRouteKey();
    const int history = view.historyCount();
    QSignalSpy clicks(&view, &NNavigationView::navigationNodeClicked);
    view.navigation(action);
    QCOMPARE(view.currentRouteKey(), before);
    QCOMPARE(view.historyCount(), history);
    QCOMPARE(clicks.size(), 1);
    QCOMPARE(clicks.first().at(0).value<NNavigationType::NavigationNodeType>(), NNavigationType::FooterNode);
    view.setNodeKeyPoints(action, -1);
    QCOMPARE(view.getNodeKeyPoints(action), 0);
}

void TestNNavigation::pageDestructionAndDuplicateRegistration() {
    NNavigationView view, other;
    configure(view); configure(other);
    auto* a = new QWidget; auto* b = new GuardedPage;
    view.addPageNode("a", a, icon); view.addPageComponent(b, icon);
    QCOMPARE(view.addPageNode("duplicate", a, icon), NNavigationType::PageInvalid);
    QCOMPARE(other.addPageNode("duplicate", a, icon), NNavigationType::PageInvalid);
    view.navigateTo(key(b));
    const QString removed = key(b);
    delete b;
    QCOMPARE(view.count(), 1);
    QCOMPARE(view.currentRouteKey(), key(a));
    QCOMPARE(view.router()->navigateTo(removed), NNavigationRouterType::PageNotFound);
}

void TestNNavigation::minimalAndCompactPane() {
    NNavigationView view;
    configure(view);
    auto* a = new QWidget; auto* b = new QWidget;
    view.addPageNode("a", a, icon); view.addPageNode("b", b, icon);
    view.setDisplayMode(NNavigationType::Minimal);
    view.show();
    QTest::qWait(10);
    auto* bar = view.findChild<NNavigationBar*>();
    auto* stack = view.findChild<NStackedWidget*>();
    QVERIFY(bar && stack);
    QTRY_COMPARE(bar->width(), 47);
    QVERIFY(!view.isPaneOpen());
    QCOMPARE(stack->x(), 0);
    const int width = stack->width();
    view.setPaneOpen(true);
    QTRY_COMPARE(bar->width(), 300);
    const auto snapshotDir = qEnvironmentVariable("QTNATIVEUI_NAV_SNAPSHOT_DIR");
    if (!snapshotDir.isEmpty()) view.grab().save(snapshotDir + "/navigation-minimal-open.png");
    QCOMPARE(stack->width(), width); // 覆盖式展开不挤压内容。
    view.navigateTo(key(b));
    QVERIFY(!view.isPaneOpen());
    QTRY_COMPARE(bar->width(), 47);
    view.setDisplayMode(NNavigationType::Compact);
    QTRY_COMPARE(stack->x(), 47);
    view.setPaneOpen(true);
    auto* scrim = view.findChild<QWidget*>("navigationPaneScrim");
    QVERIFY(scrim && scrim->isVisible());
    QTest::mouseClick(scrim, Qt::LeftButton, {}, QPoint(view.width() - 5, 100));
    QVERIFY(!view.isPaneOpen());
    // 连续请求必须以最后一次为准，不能因动画未完成而丢失。
    view.setDisplayMode(NNavigationType::Minimal);
    view.setDisplayMode(NNavigationType::Maximal);
    view.setDisplayMode(NNavigationType::Compact);
    QTRY_COMPARE(bar->width(), 47);
    QCOMPARE(view.effectiveDisplayMode(), NNavigationType::Compact);
}

void TestNNavigation::interruptedTransitions_data() {
    QTest::addColumn<int>("type");
    for (int i = NNavigationType::NoTransition; i <= NNavigationType::RippleTransition; ++i)
        QTest::newRow(qPrintable(QString::number(i))) << i;
}

void TestNNavigation::interruptedTransitions() {
    QFETCH(int, type);
    NNavigationView view;
    configure(view);
    auto* a = new QLabel("a"); auto* b = new QLabel("b"); auto* c = new QLabel("c");
    view.addPageNode("a", a, icon); view.addPageNode("b", b, icon); view.addPageNode("c", c, icon);
    view.setPageTransitionType(static_cast<NNavigationType::PageTransitionType>(type));
    view.setPageTransitionDuration(30);
    view.show();
    QTest::qWait(10);
    view.navigateTo(key(b));
    view.navigateTo(key(c));
    view.removeNavigationNode(key(b));
    view.navigateTo(key(a));
    QTest::qWait(100);
    QCOMPARE(view.currentWidget(), a);
    QVERIFY(a->isVisible());
    QVERIFY(!b->isVisible());
    QVERIFY(!c->isVisible());
    view.setPageTransitionDuration(0);
    view.navigateTo(key(c));
    QCOMPARE(view.currentWidget(), c);
    QVERIFY(c->isVisible());
}

void TestNNavigation::compactMenuAndKeyboard() {
    NNavigationView view;
    configure(view);
    QString group, nested;
    view.addExpanderNode("group", group, icon); view.addExpanderNode("nested", nested, group, icon);
    auto* page = new NPageComponent;
    page->setPageTitle("component");
    view.addPageComponent(page, nested, icon);
    auto* sibling = new QWidget;
    view.addPageNode("sibling", sibling, icon);
    view.setDisplayMode(NNavigationType::Compact);
    view.show();
    view.expandNavigationNode(group);
    auto* menu = view.findChild<NMenu*>();
    QVERIFY(menu);
    QCOMPARE(menu->actions().size(), 1);
    auto* submenu = menu->actions().first()->menu();
    QVERIFY(submenu);
    QCOMPARE(submenu->actions().size(), 1);
    const QString removed = key(page);
    view.removeNavigationNode(removed);
    submenu->actions().first()->trigger(); // 旧菜单只保留键，不解引用已删除节点。
    QCOMPARE(view.currentWidget(), sibling);
    menu->close();
    view.setDisplayMode(NNavigationType::Maximal);
    auto* tree = view.findChild<QTreeView*>();
    tree->setCurrentIndex(tree->model()->index(1, 0));
    QTest::keyClick(tree, Qt::Key_Return);
    QCOMPARE(view.currentRouteKey(), key(sibling));
}

QTEST_MAIN(TestNNavigation)
#include "tst_nnavigation.moc"
