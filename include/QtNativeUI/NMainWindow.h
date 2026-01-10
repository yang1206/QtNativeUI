#ifndef QTNATIVEUI_NMAINWINDOW_H
#define QTNATIVEUI_NMAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <functional>

#include "NWindowButton.h"
#include "stdafx.h"

class QAbstractButton;
namespace QWK {
class WidgetWindowAgent;
}

class NWindowBar;
class NMainWindowPrivate;

class QTNATIVEUI_EXPORT NMainWindow : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(BackdropType backdropType READ backdropType WRITE setBackdropType NOTIFY backdropTypeChanged)

  public:
    /**
     * @brief Window backdrop effect type enumeration
     * 
     * Defines the window backdrop effect types supported on different platforms. 
     * These effects are implemented through the QWindowKit library, which calls 
     * native window APIs on each platform.
     */
    enum BackdropType { 
        None = 0,    ///< No effect, uses standard window background
        Blur,        ///< Blur effect
        Acrylic,     ///< Acrylic material effect
        Mica,        ///< Mica material effect
        MicaAlt      ///< Mica alternative material effect
    };
    Q_ENUM(BackdropType)

    /**
     * @brief System button type enumeration
     * 
     * Defines the system button types in the window title bar. These buttons 
     * integrate with the system window manager through QWindowKit, providing 
     * native window operation behavior.
     */
    enum SystemButtonType { 
        WindowIcon,  ///< Window icon button (Windows only, for system menu)
        Theme,       ///< Theme toggle button (custom functionality)
        Pin,         ///< Window pin/stay-on-top button (custom functionality)
        Minimize,    ///< Minimize button
        Maximize,    ///< Maximize/restore button
        Close        ///< Close button
    };
    Q_ENUM(SystemButtonType)

    explicit NMainWindow(QWidget* parent = nullptr);
    ~NMainWindow() override;

    /**
     * @brief Set window backdrop effect type
     * 
     * Sets the window backdrop effect through the QWindowKit library. Different platforms 
     * have varying implementations and support:
     * 
     * **Windows Platform:**
     * - None: Standard window background, no effects
     * - Blur: DWM blur effect, calls DwmEnableBlurBehindWindow API
     * - Acrylic: Windows 10+ acrylic material, calls SetWindowCompositionAttribute API
     * - Mica: Windows 11+ mica material, calls DwmSetWindowAttribute(DWMWA_SYSTEMBACKDROP_TYPE, DWMSBT_MAINWINDOW)
     * - MicaAlt: Windows 11+ mica alternative material, calls DwmSetWindowAttribute(DWMWA_SYSTEMBACKDROP_TYPE, DWMSBT_TABBEDWINDOW)
     * 
     * **macOS Platform:**
     * - None: Standard window background
     * - Blur: NSVisualEffectView blur effect, supports automatic light/dark mode switching
     * - Other types: Fall back to Blur or None
     * 
     * **Linux Platform:**
     * - All effect types fall back to None due to lack of unified window effect APIs in X11/Wayland
     * 
     * @param type The backdrop effect type to set
     * @note After setting effects, the window background becomes transparent to allow effects to show through. 
     *       If the platform doesn't support the specified effect, it will automatically fall back to a supported type
     * @see backdropType(), BackdropType
     */
    void setBackdropType(BackdropType type);

    /**
     * @brief Get current window backdrop effect type
     * @return Currently set backdrop effect type
     * @see setBackdropType()
     */
    BackdropType backdropType() const;

    /**
     * @brief Get window border thickness
     * 
     * Returns the current window's border thickness in pixels. This value is obtained 
     * from the system by QWindowKit and is used to correctly calculate the window's 
     * draggable area and layout.
     * 
     * @return Border thickness in pixels, returns 0 if unable to obtain
     */
    int borderThickness() const;

    /**
     * @brief Get title bar height
     * 
     * Returns the recommended height of the current window's title bar in pixels. 
     * This value is obtained from the system by QWindowKit to ensure the title bar 
     * height conforms to platform specifications.
     * 
     * @return Title bar height in pixels, defaults to 32 pixels
     */
    int titleBarHeight() const;

#ifdef Q_OS_MAC
    /**
     * @brief Set macOS system button area callback
     * 
     * Sets a callback function to customize the position of macOS native system buttons
     * (traffic light buttons). The callback receives the window size and should return
     * the rectangle area where system buttons should be placed.
     * 
     * To hide the traffic light buttons, move them off-screen:
     * @code
     * // In showEvent or after window is shown
     * mainWindow->setSystemButtonAreaCallback([](const QSize& size) {
     *     constexpr int width = -75;
     *     return QRect(QPoint(size.width() - width, 0), QSize(width, size.height()));
     * });
     * @endcode
     * 
     * To move buttons to right side:
     * @code
     * mainWindow->setSystemButtonAreaCallback([](const QSize& size) {
     *     constexpr int width = 75;
     *     return QRect(QPoint(size.width() - width, 0), QSize(width, size.height()));
     * });
     * @endcode
     * 
     * @param callback Function that returns system button area, pass nullptr to reset to default (top-left)
     * @note Must be called after the window is shown for the callback to work properly
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
     * @note This method only works on macOS and must be called after the window is shown
     * @note For custom positioning, use setSystemButtonAreaCallback() directly
     */
    void setNativeSystemButtonsVisible(bool visible);
#endif

    /**
     * @brief Set window attribute
     * 
     * Directly sets QWindowKit window attributes. This is a low-level API that allows 
     * access to all window attributes provided by QWindowKit, including platform-specific 
     * functionality.
     * 
     * Common attributes include:
     * - "dark-mode": Set window dark mode state (Windows)
     * - "blur-effect": Set blur effect type (macOS)
     * - "border-thickness": Window border thickness
     * - "title-bar-height": Title bar height
     * 
     * @param key Attribute name
     * @param value Attribute value
     * @return true if set successfully, false if attribute is not supported or setting failed
     * @see windowAttribute(), QWK::WidgetWindowAgent::setWindowAttribute()
     */
    bool setWindowAttribute(const QString& key, const QVariant& value);

    /**
     * @brief Get window attribute
     * @param key Attribute name
     * @return Attribute value, returns invalid QVariant if attribute doesn't exist
     * @see setWindowAttribute()
     */
    QVariant windowAttribute(const QString& key) const;

    /**
     * @brief Get window title bar component
     * 
     * Returns the title bar component currently used by the window. NMainWindow 
     * automatically creates a default NWindowBar that includes window icon, title, 
     * theme button, pin button, and standard minimize/maximize/close buttons.
     * 
     * @return Current title bar component pointer, never nullptr
     * @see setWindowBar(), NWindowBar
     */
    NWindowBar* windowBar() const;

    /**
     * @brief Set custom window title bar component
     * 
     * Replaces the default title bar component. The new title bar will automatically 
     * integrate with QWindowKit, providing correct window dragging, system buttons, 
     * and hit testing functionality.
     * 
     * @param bar New title bar component, if nullptr the current title bar is removed
     * @note The old title bar component will be automatically deleted
     * @note On macOS, native system buttons (traffic lights) remain visible by default.
     *       To hide them, call setNativeSystemButtonsVisible(false) in showEvent.
     * @see windowBar(), setNativeSystemButtonsVisible()
     */
    void setWindowBar(NWindowBar* bar);

    /**
     * @brief Set custom system button implementation
     * 
     * Replaces the specified type of system button with a custom button component. 
     * This allows complete customization of button appearance and behavior while 
     * maintaining integration with QWindowKit to ensure correct window operation functionality.
     * 
     * For WindowIcon, Minimize, Maximize, Close types, buttons are registered with 
     * QWindowKit as system buttons, gaining native window operation behavior 
     * (such as Windows 11 Snap Layout support).
     * 
     * For Theme, Pin types, buttons are set as clickable areas but don't interfere 
     * with window dragging.
     * 
     * @param type System button type to replace
     * @param button Custom button component, if nullptr the custom button is removed
     * @see SystemButtonType, setSystemButtonVisible()
     */
    void setSystemButton(SystemButtonType type, QAbstractButton* button);

    /**
     * @brief Get system button component of specified type
     * 
     * Returns the button component of the specified type in the title bar, which can 
     * be used for further customization of button properties and behavior. This is a 
     * convenience method that internally forwards to windowBar()->systemButton().
     * 
     * @param type Button type
     * @return Button component pointer, returns nullptr if button doesn't exist
     * @see setSystemButtonVisible(), NWindowButton
     */
    NWindowButton* systemButton(SystemButtonType type) const;
    /**
     * @brief Set widget hit test visibility
     * 
     * Controls whether the specified widget participates in window hit testing. 
     * Widgets set to visible can receive mouse events, while widgets set to invisible 
     * are ignored by hit testing and mouse events are passed to the window for dragging.
     * 
     * This functionality is implemented through QWindowKit to ensure custom widgets 
     * integrate correctly with frameless window dragging behavior.
     * 
     * @param widget Widget to configure
     * @param visible true for widget to receive mouse events, false to ignore in hit testing
     * @see QWK::WidgetWindowAgent::setHitTestVisible()
     */
    void setHitTestVisible(QWidget* widget, bool visible = true);

    /**
     * @brief Set title bar widget
     * 
     * Sets a custom widget as the window's title bar. This widget is registered with 
     * QWindowKit as the title bar area and set as the window's menu widget to properly 
     * integrate with QMainWindow's layout.
     * 
     * @param widget Title bar widget
     * @see titleBarWidget(), setMenuWidget()
     */
    void setTitleBarWidget(QWidget* widget);

    /**
     * @brief Get current title bar widget
     * @return Current title bar widget, equivalent to menuWidget()
     * @see setTitleBarWidget()
     */
    QWidget* titleBarWidget() const;

    /**
     * @brief Get QWindowKit window agent object
     * 
     * Returns the underlying QWindowKit window agent object, providing direct access 
     * to all QWindowKit functionality. This is an advanced API for scenarios that 
     * require direct manipulation of QWindowKit features.
     * 
     * @return QWindowKit window agent object pointer
     * @see QWK::WidgetWindowAgent
     */
    QWK::WidgetWindowAgent* windowAgent() const;

    /**
     * @brief Set menu bar for the window
     * 
     * Sets a menu bar for the window, which will be embedded into the title bar 
     * between the title and system buttons. This is a convenience method that 
     * automatically handles the menu bar integration with QWindowKit.
     * 
     * The menu bar will be automatically configured for hit testing, allowing 
     * proper mouse interaction while maintaining window dragging functionality 
     * in empty areas of the title bar.
     * 
     * @param menuBar Menu bar to set, if nullptr the current menu bar is removed
     * @see menuBar(), QMenuBar
     */
    void setMenuBar(QMenuBar* menuBar);

    /**
     * @brief Get current menu bar
     * 
     * Returns the menu bar currently embedded in the title bar.
     * 
     * @return Current menu bar, returns nullptr if no menu bar is set
     * @see setMenuBar()
     */
    QMenuBar* menuBar() const;

  Q_SIGNALS:
    /**
     * @brief Window backdrop effect type changed signal
     * @param type New backdrop effect type
     * @see setBackdropType()
     */
    void backdropTypeChanged(BackdropType type);

    /**
     * @brief Theme toggle signal
     * 
     * Emitted when the user clicks the theme button to switch themes. 
     * The signal parameter indicates the theme state after switching.
     * 
     * @param isDark true indicates switch to dark theme, false indicates switch to light theme
     * @see systemButton(), NTheme
     */
    void themeToggled(bool isDark);

    /**
     * @brief Window pin state toggle signal
     * 
     * Emitted when the user clicks the pin button to change the window's stay-on-top state.
     * 
     * @param pinned true indicates window is pinned, false indicates pin is cancelled
     * @see Qt::WindowStaysOnTopHint
     */
    void pinButtonToggled(bool pinned);

    /**
     * @brief Window maximize state toggle signal
     * 
     * Emitted when the user clicks the maximize button to change the window's maximize state.
     * 
     * @param maximized true indicates window is maximized, false indicates window is restored
     * @see showMaximized(), showNormal()
     */
    void maximizeButtonToggled(bool maximized);

    /**
     * @brief Window close signal
     * 
     * Emitted when the user clicks the close button to close the window. 
     * This signal is emitted after the close() call.
     * 
     * @see close()
     */
    void windowClosed();

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    void setupDefaultWindowBar();
    void connectWindowBarSignals();

    NMainWindowPrivate* d_ptr;
    Q_DECLARE_PRIVATE(NMainWindow)
};

#endif // QTNATIVEUI_NMAINWINDOW_H
