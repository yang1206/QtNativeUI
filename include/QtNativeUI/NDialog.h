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
    Q_PROPERTY(WindowEffectType windowEffect READ windowEffect WRITE setWindowEffect NOTIFY windowEffectChanged)

  public:
    enum WindowEffectType { None = 0, Blur, Acrylic, Mica, MicaAlt };
    Q_ENUM(WindowEffectType)

    enum SystemButtonType { Minimize, Maximize, Close };
    Q_ENUM(SystemButtonType)

    explicit NDialog(QWidget* parent = nullptr);
    ~NDialog() override;

    void         setWindowEffect(WindowEffectType type);
    WindowEffectType windowEffect() const;

    int borderThickness() const;
    int titleBarHeight() const;

#ifdef Q_OS_MAC
    /**
     * @brief Set macOS system button area callback
     * @param callback Function that returns system button area
     * @note Must be called after the dialog is shown
     */
    void setSystemButtonAreaCallback(const std::function<QRect(const QSize&)>& callback);

    /**
     * @brief Set native system buttons visibility (macOS only)
     *
     * Controls the visibility of macOS traffic light buttons. When set to false,
     * this method calls both setWindowAttribute("no-system-buttons", true) and
     * setSystemButtonAreaCallback to move buttons off-screen.
     *
     * @param visible Whether system buttons should be visible
     * @note This method only works on macOS and must be called after the dialog is shown
     * @note For custom positioning, use setSystemButtonAreaCallback() directly
     */
    void setNativeSystemButtonsVisible(bool visible);
#endif

    bool     setWindowAttribute(const QString& key, const QVariant& value);
    QVariant windowAttribute(const QString& key) const;

    /**
     * @brief Set custom title bar widget
     * @param titleBar Custom title bar widget, replaces the default NWindowBar
     * @note On macOS, native system buttons (traffic lights) remain visible by default.
     *       To hide them, call setNativeSystemButtonsVisible(false) in showEvent.
     * @see titleBar(), setNativeSystemButtonsVisible()
     */
    void     setTitleBar(QWidget* titleBar);
    QWidget* titleBar() const;

    NWindowBar* windowBar() const;

    void setWindowBarVisible(bool visible);
    bool windowBarVisible() const;

    void     setContentWidget(QWidget* widget);
    QWidget* contentWidget() const;

    void setHitTestVisible(QWidget* widget, bool visible = true);
    void setSystemButton(SystemButtonType type, QAbstractButton* button);

    QWK::WidgetWindowAgent* windowAgent() const;

  Q_SIGNALS:
    void windowEffectChanged(WindowEffectType type);

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    NDialogPrivate* d_ptr;
    Q_DECLARE_PRIVATE(NDialog)
};

#endif // QTNATIVEUI_NDIALOG_H
