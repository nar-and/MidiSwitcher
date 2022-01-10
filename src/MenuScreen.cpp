#include "MenuScreen.h"
#include "Menu.h"


void MenuScreen::setParent(Menu* menu) 
{ 
    _parent = menu; 
}

void MenuScreen::setNumTabs(int16_t numTabs) 
{ 
    _numTabs = numTabs; 
}

int16_t MenuScreen::getNumTabs(void) 
{ 
    return _numTabs; 
}    

void MenuScreen::setCurTab(int16_t curTab) 
{
    // Only set if valid, ignore otherwise
    if(curTab < _numTabs)
    {        
        _curTab = curTab;
    }
}

void MenuScreen::setFirstTab(void) 
{
    _curTab = 0;
}

void MenuScreen::setLastTab() 
{
    _curTab = (_numTabs - 1);
}

int16_t MenuScreen::getCurTab(void) 
{
    return _curTab;
}

void MenuScreen::updateScreen(int16_t delta)
{
    if(delta > 0) 
    {   
        // Go to next screen in menu
        _parent->setNextScreen();
    }
    else if (delta < 0) 
    {
        // Go to previous screen in menu
        _parent->setPrevScreen();
    }
    else
    {
        // Do nothing
    }
}
