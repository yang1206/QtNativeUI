#ifndef QTNATIVEUI_NCONTENTDIALOG_H
#define QTNATIVEUI_NCONTENTDIALOG_H

#include <QDialog>

#include "stdafx.h"

class NContentDialogPrivate;
class QTNATIVEUI_EXPORT NContentDialog : public QDialog {
    Q_OBJECT
    Q_Q_CREATE(NContentDialog)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(QString, Title)
    Q_PROPERTY_CREATE_Q_H(QString, Content)
    Q_PROPERTY_CREATE_Q_H(QString, PrimaryButtonText)
    Q_PROPERTY_CREATE_Q_H(QString, SecondaryButtonText)
    Q_PROPERTY_CREATE_Q_H(QString, CloseButtonText)
    Q_PROPERTY_CREATE_Q_H(bool, IsPrimaryButtonEnabled)
    Q_PROPERTY_CREATE_Q_H(bool, IsSecondaryButtonEnabled)
    Q_PROPERTY_CREATE_Q_H(bool, IsCloseButtonEnabled)
    Q_PROPERTY_CREATE_Q_H(bool, DefaultButton)

  public:
    enum DialogButton { None, Primary, Secondary, Close };
    Q_ENUM(DialogButton)

    enum DialogResult { ResultNone, ResultPrimary, ResultSecondary, ResultClose };
    Q_ENUM(DialogResult)

    explicit NContentDialog(QWidget* parent = nullptr);
    ~NContentDialog() override;

    void setContentWidget(QWidget* widget);

    DialogResult showDialog();
    void         closeDialog();
    DialogResult result() const;

  signals:
    void primaryButtonClicked();
    void secondaryButtonClicked();
    void closeButtonClicked();
    void dialogOpened();
    void dialogClosed(DialogResult result);

  protected:
    void showEvent(QShowEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif

    virtual void onPrimaryButtonClicked() {}
    virtual void onSecondaryButtonClicked() {}
    virtual void onCloseButtonClicked() {}
};

#endif // QTNATIVEUI_NCONTENTDIALOG_H