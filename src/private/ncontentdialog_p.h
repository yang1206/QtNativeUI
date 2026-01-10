#ifndef QTNATIVEUI_NCONTENTDIALOG_P_H
#define QTNATIVEUI_NCONTENTDIALOG_P_H

#include <QLabel>
#include <QObject>
#include <QPointer>
#include <QVBoxLayout>
#include "QtNativeUI/NContentDialog.h"
#include "QtNativeUI/NEnums.h"

class NPushButton;
class NOverlay;
class NFramelessHelper;

class NContentDialogPrivate : public QObject {
    Q_OBJECT
    Q_D_CREATE(NContentDialog)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title)
    Q_PROPERTY_CREATE_D(QString, Content)
    Q_PROPERTY_CREATE_D(QString, PrimaryButtonText)
    Q_PROPERTY_CREATE_D(QString, SecondaryButtonText)
    Q_PROPERTY_CREATE_D(QString, CloseButtonText)
    Q_PROPERTY_CREATE_D(bool, IsPrimaryButtonEnabled)
    Q_PROPERTY_CREATE_D(bool, IsSecondaryButtonEnabled)
    Q_PROPERTY_CREATE_D(bool, IsCloseButtonEnabled)
    Q_PROPERTY_CREATE_D(bool, DefaultButton)

  public:
    explicit NContentDialogPrivate(QObject* parent = nullptr);
    ~NContentDialogPrivate();

    void initialize();
    void updateButtonsLayout();
    void setupContentLayout();
    void doCloseAnimation();

    NThemeType::ThemeMode        _themeMode;
    bool                         _isDark;
    NContentDialog::DialogResult _dialogResult{NContentDialog::ResultNone};

    NFramelessHelper*    _frameless{nullptr};
    QPointer<NOverlay>   _overlay;
    QWidget*          _contentWidget{nullptr};
    QWidget*          _titleBarWidget{nullptr};
    QWidget*          _buttonWidget{nullptr};
    QLabel*           _titleLabel{nullptr};
    QLabel*           _contentLabel{nullptr};
    QVBoxLayout*      _mainLayout{nullptr};
    QHBoxLayout*      _buttonLayout{nullptr};

    NPushButton* _leftButton{nullptr};
    NPushButton* _middleButton{nullptr};
    NPushButton* _rightButton{nullptr};

    int _shadowBorderWidth{3};
};

#endif // QTNATIVEUI_NCONTENTDIALOG_P_H