#ifndef QTNATIVEUI_NDIALOG_H
#define QTNATIVEUI_NDIALOG_H

#include <QDialog>
#include <QVariant>

#include "stdafx.h"

class QAbstractButton;
namespace QWK {
class WidgetWindowAgent;
}

class NDialogPrivate;
class NWindowBar;

/**
 * @brief Frameless dialog with native window effects
 *
 * NDialog provides a QDialog with frameless window support through QWindowKit,
 * enabling backdrop effects like Mica, Acrylic, and Blur on supported platforms.
 */
class QTNATIVEUI_EXPORT NDialog : public QDialog {
    Q_OBJECT
    Q_PROPERTY(BackdropType backdropType READ backdropType WRITE setBackdropType NOTIFY backdropTypeChanged)

public:
    enum BackdropType {
        None = 0,
        Blur,
        Acrylic,
        Mica,
        MicaAlt
    };
    Q_ENUM(BackdropType)

    enum SystemButtonType {
        Minimize,
        Maximize,
        Close
    };
    Q_ENUM(SystemButtonType)

    explicit NDialog(QWidget* parent = nullptr);
    ~NDialog() override;

    void setBackdropType(BackdropType type);
    BackdropType backdropType() const;

    int borderThickness() const;
    int titleBarHeight() const;

#ifdef Q_OS_MAC
    void setNativeSystemButtonsVisible(bool visible);
    bool nativeSystemButtonsVisible() const;
    void setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback);
#endif

    bool setWindowAttribute(const QString& key, const QVariant& value);
    QVariant windowAttribute(const QString& key) const;

    void setTitleBar(QWidget* titleBar);
    QWidget* titleBar() const;

    NWindowBar* windowBar() const;

    void setWindowBarVisible(bool visible);
    bool windowBarVisible() const;

    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const;

    void setHitTestVisible(QWidget* widget, bool visible = true);
    void setSystemButton(SystemButtonType type, QAbstractButton* button);

    QWK::WidgetWindowAgent* windowAgent() const;

Q_SIGNALS:
    void backdropTypeChanged(BackdropType type);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    NDialogPrivate* d_ptr;
    Q_DECLARE_PRIVATE(NDialog)
};

#endif // QTNATIVEUI_NDIALOG_H
