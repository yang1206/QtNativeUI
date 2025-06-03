#include <QtNativeUI/NContentDialog.h>
#include "../private/ncontentdialog_p.h"
#include "../private/nmaskwidget_p.h"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QScreen>
#include <QStyle>
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/NTheme.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

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
#ifdef Q_OS_WIN
    createWinId();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    window()->setWindowFlags((window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
    window()->setWindowFlags((window()->windowFlags()) | Qt::FramelessWindowHint);
    setStyleSheet("#NContentDialog{background-color:transparent;}");
#endif

    d->initialize();
}

NContentDialog::~NContentDialog() {
    Q_D(NContentDialog);
    d->_maskWidget->deleteLater();
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
    if (d->_maskWidget && activeWindow) {
        d->_maskWidget->setParent(activeWindow);
        d->_maskWidget->setVisible(true);
        d->_maskWidget->raise();
        d->_maskWidget->setFixedSize(activeWindow->size());
        d->_maskWidget->doMaskAnimation(90);
    }

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    HWND hwnd = (HWND) d->_currentWinID;
    ElaWinShadowHelper::getInstance()->setWindowShadow(d->_currentWinID);
    DWORD style      = ::GetWindowLongPtr(hwnd, GWL_STYLE);
    bool  hasCaption = (style & WS_CAPTION) == WS_CAPTION;
    if (!hasCaption) {
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
    }
#endif
#endif

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
    painter.drawRect(rect());

    painter.setBrush(NThemeColor(NFluentColorKey::SolidBackgroundFillColorBase, nTheme->themeMode()));
    painter.setPen(NThemeColor(NFluentColorKey::DividerStrokeColorDefault, d->_themeMode));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    QDialog::paintEvent(event);
}

void NContentDialog::keyPressEvent(QKeyEvent* event) { event->accept(); }

void NContentDialog::resizeEvent(QResizeEvent* event) { QDialog::resizeEvent(event); }

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool NContentDialog::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool ElaContentDialog::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_D(NContentDialog);
    if ((eventType != "windows_generic_MSG") || !message) {
        return false;
    }
    const auto msg  = static_cast<const MSG*>(message);
    const HWND hwnd = msg->hwnd;
    if (!hwnd || !msg) {
        return false;
    }
    d->_currentWinID    = (qint64) hwnd;
    const UINT   uMsg   = msg->message;
    const WPARAM wParam = msg->wParam;
    const LPARAM lParam = msg->lParam;
    switch (uMsg) {
        case WM_WINDOWPOSCHANGING: {
            WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
            if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0) {
                wp->flags |= SWP_NOCOPYBITS;
                *result = ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
                return true;
            }
            return false;
        }
        case WM_NCACTIVATE: {
            *result = TRUE;
            return true;
        }
        case WM_NCCALCSIZE: {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
            if (wParam == FALSE) {
                return false;
            }
            if (::IsZoomed(hwnd)) {
                setContentsMargins(8, 8, 8, 8);
            } else {
                setContentsMargins(0, 0, 0, 0);
            }
            *result = 0;
            return true;
#else
            if (wParam == FALSE) {
                return false;
            }
            RECT* clientRect = &((NCCALCSIZE_PARAMS*) (lParam))->rgrc[0];
            if (!::IsZoomed(hwnd)) {
                clientRect->top -= 1;
                clientRect->bottom -= 1;
            }
            *result = WVR_REDRAW;
            return true;
#endif
        }
    }
    return QDialog::nativeEvent(eventType, message, result);
}
#endif