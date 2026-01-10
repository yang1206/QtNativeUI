#include <QtNativeUI/NContentDialog.h>
#include "../private/ncontentdialog_p.h"

#include <QApplication>
#include <QPainterPath>
#include <QResizeEvent>
#include <QScreen>
#include <QStyle>
#include "QtNativeUI/NOverlay.h"
#include "QtNativeUI/NTheme.h"

Q_PROPERTY_CREATE_Q_CPP(NContentDialog, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, QString, Title)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, QString, Content)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, QString, PrimaryButtonText)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, QString, SecondaryButtonText)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, QString, CloseButtonText)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, bool, IsPrimaryButtonEnabled)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, bool, IsSecondaryButtonEnabled)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, bool, IsCloseButtonEnabled)
Q_PROPERTY_CREATE_Q_CPP(NContentDialog, bool, DefaultButton)

NContentDialog::NContentDialog(QWidget* parent) : QDialog(parent), d_ptr(new NContentDialogPrivate()) {
    Q_D(NContentDialog);
    d->q_ptr = this;
    resize(400, height());
    setWindowModality(Qt::ApplicationModal);
    setObjectName("NContentDialog");

    d->initialize();
}

NContentDialog::~NContentDialog() {
}

void NContentDialog::setContentWidget(QWidget* widget) {
    Q_D(NContentDialog);
    if (!widget)
        return;

    d->_mainLayout->takeAt(0);
    d->_mainLayout->takeAt(0);
    delete d->_contentWidget;
    d->_mainLayout->addWidget(widget);
    d->_mainLayout->addWidget(d->_buttonWidget);
}

NContentDialog::DialogResult NContentDialog::showDialog() {
    Q_D(NContentDialog);

    d->_dialogResult = ResultNone;

    d->setupContentLayout();
    d->updateButtonsLayout();

    QDialog::exec();

    return d->_dialogResult;
}

void NContentDialog::closeDialog() {
    Q_D(NContentDialog);
    d->doCloseAnimation();
}

NContentDialog::DialogResult NContentDialog::result() const {
    Q_D(const NContentDialog);
    return d->_dialogResult;
}

void NContentDialog::showEvent(QShowEvent* event) {
    Q_D(NContentDialog);

    QWidget* activeWindow = QApplication::activeWindow();
    if (d->_overlay && activeWindow) {
        d->_overlay->setParent(activeWindow);
        d->_overlay->showOverlay();
    }

    emit dialogOpened();

    QDialog::showEvent(event);
}

void NContentDialog::paintEvent(QPaintEvent* event) {
    Q_D(NContentDialog);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    nTheme->drawEffectShadow(
        &painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius, NDesignTokenKey::ElevationRest);

    QColor bgColor;
    if (d->_isDark) {
        bgColor = NThemeColor(NFluentColorKey::SolidBackgroundFillColorQuarternary, NThemeType::Dark);
    } else {
        bgColor = NThemeColor(NFluentColorKey::SolidBackgroundFillColorQuarternary, NThemeType::Light);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
#ifdef Q_OS_WIN
    painter.drawRect(rect());
#endif

#ifdef Q_OS_MACOS
    QPainterPath path;
    path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    painter.drawPath(path);
#endif

    painter.setPen(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, d->_themeMode));
    painter.drawLine(rect().x(), height() - 60, rect().right(), height() - 60);

    painter.setBrush(NThemeColor(NFluentColorKey::SolidBackgroundFillColorBase, nTheme->themeMode()));
    painter.setPen(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, d->_themeMode));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    QDialog::paintEvent(event);
}

void NContentDialog::keyPressEvent(QKeyEvent* event) { event->accept(); }

void NContentDialog::resizeEvent(QResizeEvent* event) { QDialog::resizeEvent(event); }