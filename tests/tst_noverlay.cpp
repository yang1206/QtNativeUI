#include <memory>

#include <QApplication>
#include <QTest>

#include <QtNativeUI/NContentDialog.h>
#include <QtNativeUI/NOverlay.h>

namespace {

void showHost(QWidget& host) {
    host.resize(640, 480);
    host.show();
    QCoreApplication::processEvents();
}

void verifyCenteredContent(NOverlay& overlay) {
    QWidget* content = overlay.contentWidget();
    QVERIFY(content);
    QCOMPARE(content->pos(), QPoint((overlay.width() - content->width()) / 2,
                                    (overlay.height() - content->height()) / 2));
}

} // namespace

class tst_NOverlay : public QObject {
    Q_OBJECT

  private slots:
    void followsParentGeometry_data();
    void followsParentGeometry();
    void switchesParent();
    void resynchronizesAfterHiddenResize();
    void loadingFollowsParentDuringAnimation();
    void contentDialogOverlayFollowsWindow();
};

void tst_NOverlay::followsParentGeometry_data() {
    QTest::addColumn<bool>("nested");
    QTest::newRow("window") << false;
    QTest::newRow("container") << true;
}

void tst_NOverlay::followsParentGeometry() {
    QFETCH(bool, nested);
    QWidget host;
    QWidget container(&host);
    container.setGeometry(40, 50, 360, 280);
    showHost(host);
    QWidget* parent = nested ? &container : &host;
    NOverlay overlay(parent);
    overlay.setAnimationDuration(0);
    auto* content = new QWidget;
    content->setFixedSize(120, 60);
    overlay.setContentWidget(content);
    overlay.showOverlay();

    QCOMPARE(overlay.geometry(), parent->rect());
    for (const QSize& size : {QSize(800, 600), QSize(240, 180), QSize(470, 350)}) {
        parent->resize(size);
        QCOMPARE(overlay.geometry(), parent->rect());
        verifyCenteredContent(overlay);
    }

    parent->move(parent->pos() + QPoint(20, 30));
    QCoreApplication::processEvents();
    QCOMPARE(overlay.geometry(), parent->rect());
    QCOMPARE(overlay.mapToGlobal(QPoint()), parent->mapToGlobal(QPoint()));
}

void tst_NOverlay::switchesParent() {
    auto first = std::make_unique<QWidget>();
    QWidget second;
    showHost(*first);
    showHost(second);
    second.resize(360, 240);
    NOverlay overlay(first.get());
    overlay.setAnimationDuration(0);
    overlay.showOverlay();

    overlay.showOverlay(&second);
    QCOMPARE(overlay.geometry(), second.rect());
    first->resize(900, 700);
    QCOMPARE(overlay.geometry(), second.rect());
    first.reset();
    second.resize(500, 320);
    QCOMPARE(overlay.geometry(), second.rect());

    overlay.setParent(nullptr);
    const QRect detachedGeometry = overlay.geometry();
    second.resize(600, 400);
    QCOMPARE(overlay.geometry(), detachedGeometry);

    overlay.setParent(&second);
    overlay.show();
    second.resize(420, 260);
    QCOMPARE(overlay.geometry(), second.rect());
}

void tst_NOverlay::resynchronizesAfterHiddenResize() {
    QWidget host;
    showHost(host);
    NOverlay overlay(&host);
    overlay.setAnimationDuration(0);
    overlay.showOverlay();

    host.hide();
    host.resize(820, 610);
    host.show();
    QCoreApplication::processEvents();
    QCOMPARE(overlay.geometry(), host.rect());

    overlay.hideOverlay();
    host.resize(310, 230);
    overlay.show();
    QCOMPARE(overlay.geometry(), host.rect());
}

void tst_NOverlay::loadingFollowsParentDuringAnimation() {
    QWidget host;
    showHost(host);
    NOverlay overlay;
    overlay.setAnimationDuration(200);
    overlay.showLoading(&host, QStringLiteral("Loading"));
    QVERIFY(overlay.isAnimating());
    host.resize(760, 540);
    QCOMPARE(overlay.geometry(), host.rect());
    verifyCenteredContent(overlay);

    overlay.hideOverlay();
    host.resize(400, 300);
    QCOMPARE(overlay.geometry(), host.rect());
    verifyCenteredContent(overlay);
    QTRY_VERIFY(!overlay.isVisible());
}

void tst_NOverlay::contentDialogOverlayFollowsWindow() {
    QWidget host;
    QWidget page(&host);
    page.setGeometry(20, 30, 400, 300);
    showHost(host);
    QApplication::setActiveWindow(&host);
    NContentDialog dialog(&page);
    dialog.show();

    auto* overlay = host.findChild<NOverlay*>(QString(), Qt::FindDirectChildrenOnly);
    QVERIFY(overlay);
    QVERIFY(overlay->isVisible());
    host.resize(900, 680);
    QCOMPARE(overlay->geometry(), host.rect());
    host.resize(500, 360);
    QCOMPARE(overlay->geometry(), host.rect());
    dialog.closeDialog();
}

QTEST_MAIN(tst_NOverlay)
#include "tst_noverlay.moc"
