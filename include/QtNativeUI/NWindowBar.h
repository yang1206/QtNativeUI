#ifndef QTNATIVEUI_NWINDOWBAR_H
#define QTNATIVEUI_NWINDOWBAR_H

#include <QFrame>
#include <QLabel>
#include <QMenuBar>

#include "NWindowButton.h"
#include "stdafx.h"

class NWindowBarPrivate;
class NWindowButton;

/**
 * @brief Window title bar component providing title, icon, system buttons and other functionality
 * 
 * NWindowBar is a complete window title bar implementation that integrates window icon, 
 * title text, menu bar, and system buttons. It deeply integrates with QWindowKit to 
 * provide native window operation experience, including:
 * 
 * - Window dragging: Empty areas of the title bar support dragging to move the window
 * - System buttons: Minimize, maximize, close, pin, theme toggle buttons
 * - Menu integration: Can embed QMenuBar into the title bar
 * - Custom widgets: Support adding custom widgets to the title bar
 * - Responsive layout: Automatically adapts to window size changes
 * 
 * The title bar layout from left to right is: [Icon] [Title] [Menu Bar] [Custom Widgets] [System Buttons]
 */
class QTNATIVEUI_EXPORT NWindowBar : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(NWindowBar)

public:
    explicit NWindowBar(QWidget* parent = nullptr);
    ~NWindowBar() override;

    /**
     * @brief Get host widget
     * 
     * Returns the window that the current title bar belongs to. The title bar monitors 
     * the host widget's state changes and automatically updates button states 
     * (such as maximize button icon) and window title.
     * 
     * @return Host widget pointer, returns nullptr if not set
     * @see setHostWidget()
     */
    QWidget* hostWidget() const;

    /**
     * @brief Set host widget
     * 
     * Sets the window that the title bar belongs to. The title bar will automatically 
     * connect to the widget's signals to monitor window state changes and update 
     * corresponding UI elements.
     * 
     * @param widget Host widget, usually an NMainWindow instance
     * @see hostWidget()
     */
    void setHostWidget(QWidget* widget);

    /**
     * @brief Get window title text
     * @return Currently displayed title text
     * @see setTitle()
     */
    QString title() const;

    /**
     * @brief Set window title text
     * 
     * Sets the title text displayed in the title bar. If a host widget is set, 
     * the title bar will automatically sync with the host widget's windowTitle property.
     * 
     * @param title Title text to display
     * @see title(), setTitleVisible()
     */
    void setTitle(const QString& title);

    /**
     * @brief Get window icon
     * @return Currently displayed window icon
     * @see setIcon()
     * @note Not supported on macOS
     */
    QIcon icon() const;

    /**
     * @brief Set window icon
     * 
     * Sets the window icon displayed on the left side of the title bar. If a host 
     * widget is set, the title bar will automatically sync with the host widget's 
     * windowIcon property.
     * 
     * @param icon Window icon to display
     * @see icon(), setIconVisible()
     * @note Not supported on macOS
     */
    void setIcon(const QIcon& icon);

    /**
     * @brief Get title text visibility state
     * @return true if title is visible, false if hidden
     * @see setTitleVisible()
     */
    bool titleVisible() const;

    /**
     * @brief Set title text visibility state
     * 
     * Controls the visibility of the title text in the title bar. When the title 
     * is hidden, the title area space will be used by other elements 
     * (such as menu bar or custom widgets).
     * 
     * @param visible true to show title, false to hide title
     * @see titleVisible(), setTitle()
     */
    void setTitleVisible(bool visible);

    /**
     * @brief Get window icon visibility state
     * @return true if icon is visible, false if hidden
     * @see setIconVisible()
     * @note Not supported on macOS
     */
    bool iconVisible() const;

    /**
     * @brief Set window icon visibility state
     * 
     * Controls the visibility of the window icon in the title bar. In some designs, 
     * the window icon may not need to be displayed to save space or simplify the interface.
     * 
     * @param visible true to show icon, false to hide icon
     * @see iconVisible(), setIcon()
     * @note Not supported on macOS
     */
    void setIconVisible(bool visible);

    /**
     * @brief Set system button visibility state
     * 
     * Controls the visibility of the specified type of system button. System buttons include:
     * - Minimize: Minimize button
     * - Maximize: Maximize/restore button
     * - Close: Close button
     * - Pin: Window stay-on-top button
     * - Theme: Theme toggle button
     * 
     * @param type Button type
     * @param visible true to show button, false to hide button
     * @see systemButtonVisible(), systemButton()
     * @note Not supported on macOS (uses native system buttons)
     */
    void setSystemButtonVisible(NWindowButton::SystemButtonType type, bool visible);

    /**
     * @brief Get system button visibility state
     * @param type Button type
     * @return true if button is visible, false if hidden
     * @see setSystemButtonVisible()
     * @note Not supported on macOS (uses native system buttons)
     */
    bool systemButtonVisible(NWindowButton::SystemButtonType type) const;

    /**
     * @brief Get system button of specified type
     * 
     * Returns the system button component of the specified type in the title bar, 
     * which can be used for further customization of button properties.
     * 
     * @param type Button type
     * @return Button component pointer, returns nullptr if button doesn't exist
     * @see setSystemButtonVisible(), NWindowButton
     * @note Not supported on macOS (uses native system buttons)
     */
    NWindowButton* systemButton(NWindowButton::SystemButtonType type) const;

    /**
     * @brief Set menu bar
     * 
     * Embeds a QMenuBar into the title bar, positioned between the title text and 
     * system buttons. This saves vertical space and provides a more compact interface layout.
     * 
     * @param menuBar Menu bar to embed, if nullptr the current menu bar is removed
     * @see menuBar()
     */
    void setMenuBar(QMenuBar* menuBar);

    /**
     * @brief Get current menu bar
     * @return Currently embedded menu bar, returns nullptr if not set
     * @see setMenuBar()
     */
    QMenuBar* menuBar() const;

    /**
     * @brief Add custom widget to title bar
     * 
     * Adds a custom widget between the menu bar and system buttons in the title bar. 
     * Widgets are arranged from left to right in the order they are added.
     * 
     * @param widget Widget to add
     * @param stretch Stretch factor, 0 means fixed size, greater than 0 means stretchable
     * @see insertWidget()
     */
    void addWidget(QWidget* widget, int stretch = 0);

    /**
     * @brief Insert custom widget at specified position
     * 
     * Inserts a custom widget at the specified position in the title bar.
     * 
     * @param index Insert position index
     * @param widget Widget to insert
     * @param stretch Stretch factor, 0 means fixed size, greater than 0 means stretchable
     * @see addWidget()
     */
    void insertWidget(int index, QWidget* widget, int stretch = 0);

Q_SIGNALS:
    /**
     * @brief 最小化按钮点击信号
     * 
     * 当用户点击最小化按钮时发射此信号。通常连接到窗口的 showMinimized() 槽。
     */
    void minimizeRequested();

    /**
     * @brief 最大化按钮点击信号
     * 
     * 当用户点击最大化/恢复按钮时发射此信号。通常连接到窗口的 
     * showMaximized() 或 showNormal() 槽，根据当前窗口状态决定。
     */
    void maximizeRequested();

    /**
     * @brief 关闭按钮点击信号
     * 
     * 当用户点击关闭按钮时发射此信号。通常连接到窗口的 close() 槽。
     */
    void closeRequested();

    /**
     * @brief 置顶按钮状态切换信号
     * 
     * 当用户点击置顶按钮切换窗口置顶状态时发射此信号。
     * 
     * @param pinned true 表示请求置顶窗口，false 表示请求取消置顶
     */
    void pinRequested(bool pinned);

    /**
     * @brief 主题切换按钮点击信号
     * 
     * 当用户点击主题切换按钮时发射此信号。通常连接到主题管理器
     * 来切换应用程序的明暗主题。
     */
    void themeRequested();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void setupUI();
    void updateButtonStates();

    QScopedPointer<NWindowBarPrivate> d_ptr;
};

#endif // QTNATIVEUI_NWINDOWBAR_H
