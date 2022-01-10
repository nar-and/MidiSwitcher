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

#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>

// Forward declaration of parent Menu class - needed to avoid circular dependencies
class Menu;

class MenuScreen
{
protected:
    Menu*   _parent;            // Parent menu to which this screen belongs
    int16_t _curTab = 0;        // Current tab (if tabbed) 
    int16_t _numTabs = 1;       // Number of tabs in screen (if tabbed, 1 = no tabs)

public:
    virtual ~MenuScreen() {}

    // Setters & getters
    void setParent(Menu* menu);
    void setNumTabs(int16_t numTabs);
    int16_t getNumTabs(void);    

    void setCurTab(int16_t curTab); 
    void setFirstTab(void); 
    void setLastTab(void);
    int16_t getCurTab(void);

    // Show screen on display (prints starting appearance)
    virtual void show() = 0;

    // Move to different screen in menu
    // Default implementation moves to following screens
    // - delta > 0: next screen
    // - delta < 0: previous screen
    // - delta = 0: no changes, keep current screen
    virtual void updateScreen(int16_t delta);

    // Update cursor position on screen based on user inputs
    // Used to print a value in a specific position or to 
    // highlight a field to be edited
    virtual void updateCursor(int16_t delta) = 0;

    // Update value of currently selected field based on user inputs    
    virtual void updateValue(int16_t delta) = 0;
};

#endif // MENU_SCREEN_H

/****************************************************************************
 ****************************************************************************/


