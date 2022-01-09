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

// Forward declaration to allow setting screen parent
class Menu;

class MenuScreen
{
protected:
    Menu* _parent;
    int16_t _curTab = 0;
    int16_t _numTabs = 0;

public:
    virtual ~MenuScreen() {}

    void setParent(Menu* menu) 
    {
        _parent = menu;
    }

    void setNumTabs(int16_t numTabs) 
    {
        _numTabs = numTabs;
    }

    int16_t getNumTabs(void) 
    {
        return _numTabs;
    }    

    int16_t getCurTab(void) 
    {
        return _curTab;
    }

    void setCurTab(int16_t curTab) 
    {
        // Check if valid
        if(curTab < _numTabs)
        {
            _curTab = curTab;
        }
    }

    virtual void show() = 0;
    virtual void updateScreen(int16_t delta) = 0;
    virtual void updateCursor(int16_t delta) = 0;    
    virtual void updateValue(int16_t delta) = 0;
};

#endif // MENU_SCREEN_H

/****************************************************************************
 ****************************************************************************/


