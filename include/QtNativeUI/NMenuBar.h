#ifndef QTNATIVEUI_NMENUBAR_H
#define QTNATIVEUI_NMENUBAR_H

#include <QMenuBar>
#include "NIconEnums.h"
#include "NMenu.h"
#include "stdafx.h"

/**
 * @brief Custom menu bar that creates NMenu instances instead of QMenu
 * 
 * NMenuBar is a drop-in replacement for QMenuBar that automatically creates 
 * NMenu instances when adding menus. This ensures consistent styling across 
 * the application by using the library's custom menu implementation.
 * 
 * Usage is identical to QMenuBar:
 * @code
 * auto menuBar = new NMenuBar(this);
 * auto fileMenu = menuBar->addMenu("File");  // Returns NMenu*
 * fileMenu->addItem("New");
 * @endcode
 */
class QTNATIVEUI_EXPORT NMenuBar : public QMenuBar {
    Q_OBJECT

public:
    explicit NMenuBar(QWidget* parent = nullptr);
    ~NMenuBar() override;

    /**
     * @brief Add a menu with the given title
     * 
     * Creates and adds a new NMenu to the menu bar. This overrides QMenuBar's 
     * addMenu() to return NMenu instead of QMenu, ensuring consistent styling.
     * 
     * @param title Menu title
     * @return Pointer to the created NMenu
     */
    NMenu* addMenu(const QString& title);

    /**
     * @brief Add a menu with regular icon and title
     * 
     * Creates and adds a new NMenu with a regular icon from the library's icon system.
     * 
     * @param icon Regular icon type from NRegularIconType
     * @param title Menu title
     * @return Pointer to the created NMenu
     */
    NMenu* addMenu(NRegularIconType::Icon icon, const QString& title);

    /**
     * @brief Add a menu with filled icon and title
     * 
     * Creates and adds a new NMenu with a filled icon from the library's icon system.
     * 
     * @param icon Filled icon type from NFilledIconType
     * @param title Menu title
     * @return Pointer to the created NMenu
     */
    NMenu* addMenu(NFilledIconType::Icon icon, const QString& title);

    /**
     * @brief Add a menu with the given QIcon and title
     * 
     * Creates and adds a new NMenu with a QIcon to the menu bar.
     * 
     * @param icon Menu icon
     * @param title Menu title
     * @return Pointer to the created NMenu
     */
    NMenu* addMenu(const QIcon& icon, const QString& title);

    /**
     * @brief Add an existing menu to the menu bar
     * 
     * Adds a pre-created menu to the menu bar. The menu bar takes ownership.
     * 
     * @param menu Menu to add
     * @return The action associated with the menu
     */
    QAction* addMenu(QMenu* menu);
};

#endif // QTNATIVEUI_NMENUBAR_H
