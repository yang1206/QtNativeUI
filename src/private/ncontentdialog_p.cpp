#include "ncontentdialog_p.h"
#include "nframelesshelper_p.h"

#include <QApplication>
#include <QLabel>
#include <QScreen>
#include <QStyle>
#include <QTimer>

#include "QtNativeUI/NOverlay.h"
#include "QtNativeUI/NPushButton.h"
#include "QtNativeUI/NTheme.h"

NContentDialogPrivate::NContentDialogPrivate(QObject* parent) : QObject(parent) {
    _pBorderRadius             = NDesignToken(NDesignTokenKey::CornerRadiusMedium).toInt();
    _pTitle                    = QString();
    _pContent                  = QString();
    _pPrimaryButtonText        = QObject::tr("OK");
    _pSecondaryButtonText      = QString();
    _pCloseButtonText          = QObject::tr("Cancel");
    _pIsPrimaryButtonEnabled   = true;
    _pIsSecondaryButtonEnabled = true;
    _pIsCloseButtonEnabled     = true;
    _pDefaultButton            = true;

    _themeMode = nTheme->themeMode();
    _isDark    = nTheme->isDarkMode();
}

NContentDialogPrivate::~NContentDialogPrivate() {
    if (_overlay) {
        delete _overlay;
    }
}

void NContentDialogPrivate::initialize() {
    Q_Q(NContentDialog);

    _frameless = new NFramelessHelper(q, q);
    _frameless->setup();

    if (q->parentWidget()) {
        _overlay = new NOverlay(q->parentWidget());
        _overlay->setOpacity(90);
    }

    q->resize(400, 250);
    q->setWindowModality(Qt::ApplicationModal);

    _mainLayout = new QVBoxLayout(q);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(0);

    _titleBarWidget          = new QWidget(q);
    QVBoxLayout* titleLayout = new QVBoxLayout(_titleBarWidget);
    titleLayout->setContentsMargins(24, 24, 24, 12);

    _titleLabel     = new QLabel(q);
    QFont titleFont = _titleLabel->font();
    titleFont.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeTitle).toInt());
    titleFont.setWeight(static_cast<QFont::Weight>(NFontWeightToken(NDesignTokenKey::FontWeightBold).toInt()));
    titleFont.setBold(true);
    _titleLabel->setFont(titleFont);

    _contentLabel     = new QLabel(q);
    QFont contentFont = _contentLabel->font();
    contentFont.setPixelSize(NFontSizeToken(NDesignTokenKey::FontSizeBody).toInt());
    _contentLabel->setFont(contentFont);
    _contentLabel->setWordWrap(true);

    titleLayout->addWidget(_titleLabel);
    titleLayout->addWidget(_contentLabel);
    titleLayout->addStretch();

    _buttonWidget = new QWidget(q);
    _buttonWidget->setFixedHeight(60);
    _buttonLayout = new QHBoxLayout(_buttonWidget);
    _buttonLayout->setSpacing(NSpacingToken(NDesignTokenKey::SpacingM).toInt());

    _leftButton = new NPushButton("cancel", q);
    QObject::connect(_leftButton, &NPushButton::clicked, q, [this, q]() {
        _dialogResult = NContentDialog::ResultClose;
        q->closeButtonClicked();
        q->onCloseButtonClicked();
        doCloseAnimation();
    });
    _leftButton->setMinimumSize(0, 0);
    _leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    _leftButton->setFixedHeight(38);

    _middleButton = new NPushButton("minimum", q);
    QObject::connect(_middleButton, &NPushButton::clicked, q, [this, q]() {
        _dialogResult = NContentDialog::ResultSecondary;
        q->secondaryButtonClicked();
        q->onSecondaryButtonClicked();
    });
    _middleButton->setMinimumSize(0, 0);
    _middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    _middleButton->setFixedHeight(38);

    _rightButton = new NPushButton("exit", q);
    QObject::connect(_rightButton, &NPushButton::clicked, q, [this, q]() {
        _dialogResult = NContentDialog::ResultPrimary;
        q->primaryButtonClicked();
        q->onPrimaryButtonClicked();
        doCloseAnimation();
    });
    _rightButton->setButtonType(NPushButton::Accent);
    _rightButton->setMinimumSize(0, 0);
    _rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    _rightButton->setFixedHeight(38);

    _contentWidget              = new QWidget(q);
    QVBoxLayout* centralVLayout = new QVBoxLayout(_contentWidget);
    centralVLayout->setContentsMargins(15, 25, 15, 10);
    centralVLayout->addStretch();

    _mainLayout->addWidget(_titleBarWidget);
    _mainLayout->addWidget(_contentWidget, 1);
    _mainLayout->addWidget(_buttonWidget);

    QObject::connect(nTheme, &NTheme::themeModeChanged, q, [this, q](NThemeType::ThemeMode mode) {
        _themeMode = mode;
        _isDark    = nTheme->isDarkMode();
        q->update();
    });

    updateButtonsLayout();
}

void NContentDialogPrivate::updateButtonsLayout() {
    QLayoutItem* item;
    while ((item = _buttonLayout->takeAt(0)) != nullptr) {
        delete item;
    }

    _buttonLayout->addWidget(_leftButton);
    _buttonLayout->addWidget(_middleButton);
    _buttonLayout->addWidget(_rightButton);

    _leftButton->setText(_pCloseButtonText);
    _middleButton->setText(_pSecondaryButtonText);
    _rightButton->setText(_pPrimaryButtonText);

    _leftButton->setVisible(!_pCloseButtonText.isEmpty());
    _middleButton->setVisible(!_pSecondaryButtonText.isEmpty());
    _rightButton->setVisible(!_pPrimaryButtonText.isEmpty());

    _leftButton->setEnabled(_pIsCloseButtonEnabled);
    _middleButton->setEnabled(_pIsSecondaryButtonEnabled);
    _rightButton->setEnabled(_pIsPrimaryButtonEnabled);
}

void NContentDialogPrivate::setupContentLayout() {
    _titleLabel->setText(_pTitle);
    _titleLabel->setVisible(!_pTitle.isEmpty());

    _contentLabel->setText(_pContent);
    _contentLabel->setVisible(!_pContent.isEmpty());

    _titleBarWidget->setVisible(!_pTitle.isEmpty() || !_pContent.isEmpty());
}

void NContentDialogPrivate::doCloseAnimation() {
    Q_Q(NContentDialog);
    if (_overlay) {
        _overlay->hideOverlay();
    }

    q->dialogClosed(_dialogResult);

    q->QDialog::close();
}