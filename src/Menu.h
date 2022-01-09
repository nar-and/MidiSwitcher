/****************************************************************************
 ****************************************************************************
 *
 *                              MODULE TITLE
 *
 * Author(s):
 * 
 * 
 * Description:
 * 
 *
 * Usage notes:
 * 
 *
 ****************************************************************************
 ****************************************************************************/

#ifndef MENU_H
#define MENU_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "MenuScreen.h"
#include "UserInterface.h"
#include "Display.h"


// Max number of screens in menu
#define MAX_NUM_SCREENS     10

// Error codes for class methods
#define MENU_OK                     0
#define MENU_ERROR_GENERIC          -1


typedef struct
{
    uint16_t    id;
    MenuScreen* screen;    
} MenuEntry_t;

// Forward declaration of owner - needed to avoid circular dependencies
class UserInterface;

/**
 * @brief Manage a menu (i.e. a collection of screens that can be navigated for viewing & editing info)
 * 
 */
class Menu 
{
private: 
    MenuEntry_t _screens[MAX_NUM_SCREENS];      // Deck of screens owned by menu    
    uint16_t    _numScreens;                    // Total number of registered screens 
    uint16_t    _curScreen;                     // Index of currently selected screen

    /**
     * @brief Lookup screen index in deck by screen id
     * 
     * @param id       : requested screen identifier
     * @return uint16_t: screen index in deck, -1 if not found
     */
    int16_t _lookup(uint16_t id);              

public:
    /**
     * @brief Menu constructor
     * 
     * @param ui        : pointer to owner UserInterface instance
     * @param lcd       : pointer to display (used by menu for printing)
     */
    Menu(UserInterface* ui);

    /**
     * @brief Register menu screen and add it to deck
     * 
     * @param id        : screen identifier
     * @param screen    : pointer to screen instance
     * @param numTabs   : for tabbed screens, number of tabs (default = 0, i.e. screen is not tabbed)
     * @return int8_t   : error code (see definitions above)
     */
    int8_t registerScreen(uint16_t id, MenuScreen* screen, uint16_t numTabs = 1);

    /**
     * @brief Get the number of registered screens
     * 
     * @return uint16_t : number of registered screens
     */
    uint16_t getNumScreens(void);

    /**
     * @brief Move to selected screen & tab
     * 
     * @param id        : target screen identifier
     * @param tab       : target tab identifier
     * @return int8_t   : error code (see definitions above)
     */
    int8_t setScreen(uint16_t id, uint16_t tab = 0);
    int8_t setNextScreen(void);
    int8_t setPrevScreen(void);


    /**
     * @brief 
     * 
     */
    void show(void);

    /**
     * @brief 
     * 
     * @param delta
     */
    void updateScreen(int16_t delta);

    /**
     * @brief 
     * 
     * @param delta
     */
    void updateCursor(int16_t delta);

    /**
     * @brief 
     * 
     * @param delta 
     */
    void updateValue(int16_t delta);



    int8_t _selPosition;
    int8_t _curPosition;
    Display* _lcd;
    UserInterface* _owner;
};

#endif // MENU_H

/****************************************************************************
 ****************************************************************************/


