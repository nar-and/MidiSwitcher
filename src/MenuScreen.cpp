#include "MenuScreen.h"
#include "Menu.h"

void MenuScreen::updateScreen(int16_t delta)
{
    if(delta > 0) 
    {   
        // Do nothing - last entry
        _parent->setNextScreen();
    }
    else 
    {
        _parent->setPrevScreen();
    }
}

