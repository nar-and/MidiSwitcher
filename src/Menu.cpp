#include "Menu.h"

Menu::Menu(UserInterface* ui)
{
    _owner = ui;
    _lcd = &_owner->_lcd;
    _numScreens = 0;
    _curScreen = 0;
}

int8_t Menu::registerScreen(uint16_t id, MenuScreen* screen, uint16_t numTabs)
{
    // Check if screen deck is already filled
    if(_numScreens >= MAX_NUM_SCREENS)
    {
        return MENU_ERROR_GENERIC;
    }

    // Does the screen already exist?
    if(_lookup(id) >= 0)
    {
        // Yes: return error (cannot replace an existing screen)
        return MENU_ERROR_GENERIC;
    }

    // Screen does not exists: add it to deck
    screen->setParent(this);
    screen->setNumTabs(numTabs);
    screen->setFirstTab();
    _screens[_numScreens].id = id;
    _screens[_numScreens].screen = screen;

    _numScreens++;

    return MENU_OK;
}

int8_t Menu::setScreen(uint16_t id, uint16_t tab)
{
    // Lookup requested screen
    int16_t scrIndx = _lookup(id);
    
    // If valid, use it as new screen, otherwise ignore 
    if(scrIndx < 0)
    {
        return MENU_ERROR_GENERIC;
    }

    _curScreen = scrIndx;
    _screens[_curScreen].screen->setCurTab(tab); 

    return MENU_OK;
}

int8_t Menu::setNextScreen(void)
{
    // Get info for current screen
    MenuScreen* scr = _screens[_curScreen].screen;
    int16_t nt = scr->getNumTabs();
    int16_t ct = scr->getCurTab();

    if(ct < (nt - 1))    
    {
        // Not the last tab: change tab but not screen    
        scr->setCurTab(++ct);
    }
    else
    {
        // Last tab -> move to next screen
        if(_curScreen < (_numScreens - 1))
        {            
            // If this is not the last screen, go to next
            _curScreen++;
            _screens[_curScreen].screen->setFirstTab();                
        }            
    }

    return MENU_OK;
}

int8_t Menu::setPrevScreen(void)
{
    // Get info for current screen
    MenuScreen* scr = _screens[_curScreen].screen;
    int16_t nt = scr->getNumTabs();
    int16_t ct = scr->getCurTab();

    if(ct > 0)
    {
        // Not the last tab: change tab but not screen    
        scr->setCurTab(--ct);
    }
    else
    {
        // First tab -> move to previous screen
        if(_curScreen > 0)
        {            
            // Not the first screen already --> move to previous screen
            _curScreen--;
            _screens[_curScreen].screen->setLastTab();                
        }                    
    }

    return MENU_OK;
}

uint16_t Menu::getNumScreens(void)
{
    return _numScreens;
}

int16_t Menu::_lookup(uint16_t id)
{
    int16_t scrIndx = -1;

    // Lookup menu screen
    int16_t indx = 0;
    while(indx < _numScreens)
    {
        if(_screens[indx].id == id)
        {
            // Desired screen ID found
            scrIndx = indx;
            break;
        }
        indx++;
    }

    return scrIndx;
}

void Menu::show(void)
{
    // Ask current menu screen to fill LCD lines to be printed
    _screens[_curScreen].screen->show();

    // Print to screen
    _lcd->setCursor(0, 0);
    _lcd->print(_lcd->line0);

    _lcd->setCursor(0, 1);
    _lcd->print(_lcd->line1);    
  
}

void Menu::updateScreen(int16_t delta)
{
    int16_t deltaAbs = (delta >= 0)?(delta):(-delta);
    int16_t deltaSign = (delta >= 0)?(+1):(-1);

    // For each single user input request...
    while (deltaAbs > 0)
    {
        // ... propagate it to current menu screen so that next screen 
        // can be appropriately selected
        _screens[_curScreen].screen->updateScreen(deltaSign);   
        deltaAbs--;
    }
}

void Menu::updateCursor(int16_t delta)
{
    _screens[_curScreen].screen->updateCursor(delta);
}

void Menu::updateValue(int16_t delta)
{
    _screens[_curScreen].screen->updateValue(delta);
}

