#ifndef QTNATIVEUI_NWINDOWBUTTON_H
#define QTNATIVEUI_NWINDOWBUTTON_H

#include <QPushButton>
#include "stdafx.h"

class NWindowButtonPrivate;

/**
 * @brief Window title bar button for minimize, maximize, close and other operations
 * 
 * NWindowButton is a specialized button component designed for window title bars. 
 * It provides standard window operation buttons (minimize, maximize, close) as well as 
 * custom functionality buttons (pin, theme toggle). Key features include:
 * 
 * - Multiple button types: Minimize, Maximize, Close, Pin, Theme, Custom
 * - Automatic icon management: Icons are automatically selected based on button type and state
 * - State awareness: Maximize button automatically switches between maximize/restore icons
 * - Theme integration: Automatically adapts to light/dark themes through NTheme
 * - Hover effects: Provides visual feedback for mouse interactions
 * - Double-click support: Emits doubleClicked signal for custom handling
 * 
 * The button integrates with QtNativeUI's icon system and follows platform design guidelines.
 */
class QTNATIVEUI_EXPORT NWindowButton : public QPushButton {
    Q_OBJECT
    Q_DECLARE_PRIVATE(NWindowButton)

public:
    /**
     * @brief System button type enumeration
     * 
     * Defines the types of system buttons that can be created. Each type has 
     * predefined behavior, icons, and styling appropriate for its function.
     */
    enum SystemButtonType {
        Minimize,    ///< Minimize button - minimizes the window
        Maximize,    ///< Maximize/restore button - toggles window maximized state
        Close,       ///< Close button - closes the window
        Pin,         ///< Pin button - toggles window stay-on-top state
        Theme,       ///< Theme button - toggles between light/dark themes
        Custom       ///< Custom button - user-defined functionality
    };
    Q_ENUM(SystemButtonType)

    explicit NWindowButton(QWidget* parent = nullptr);
    explicit NWindowButton(SystemButtonType type, QWidget* parent = nullptr);
    ~NWindowButton() override;

    /**
     * @brief Set button type
     * 
     * Changes the button's type, which automatically updates its icon, tooltip, 
     * and behavior. The button will adopt the standard appearance and functionality 
     * associated with the specified type.
     * 
     * @param type Button type to set
     * @see buttonType(), SystemButtonType
     */
    void setButtonType(SystemButtonType type);

    /**
     * @brief Get current button type
     * @return Current button type
     * @see setButtonType()
     */
    SystemButtonType buttonType() const;

    /**
     * @brief Set maximized state for maximize button
     * 
     * For maximize-type buttons, this controls whether the button displays the 
     * maximize icon (when false) or restore icon (when true). This should be 
     * synchronized with the actual window state.
     * 
     * @param maximized true to show restore icon, false to show maximize icon
     * @note Only affects buttons of type Maximize
     * @see isMaximized()
     */
    void setMaximized(bool maximized);

    /**
     * @brief Get maximized state
     * @return true if button is in maximized state (showing restore icon)
     * @see setMaximized()
     */
    bool isMaximized() const;

Q_SIGNALS:
    /**
     * @brief Double-click signal
     * 
     * Emitted when the button is double-clicked. This can be used for custom 
     * functionality, such as maximizing the window when double-clicking the 
     * window icon button.
     */
    void doubleClicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QScopedPointer<NWindowButtonPrivate> d_ptr;
};

#endif // QTNATIVEUI_NWINDOWBUTTON_H
