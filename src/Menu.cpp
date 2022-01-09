#include "Menu.h"

Menu::Menu(UserInterface* ui, Display* lcd)
{
    _lcd = lcd;
    _owner = ui;
    _numScreens = 0;
    _curScreen = 0;
}

int8_t Menu::registerScreen(uint16_t id, MenuScreen* screen, uint16_t numTabs)
{
    Serial.print("register > "); Serial.print(id);Serial.print("/"); Serial.println((unsigned long)screen, HEX);
    Serial.print("_numScreens: ");Serial.println(_numScreens); 
    // Check if screen deck is already filled
    if(_numScreens >= MAX_NUM_SCREENS)
    {
        return MENU_ERROR_GENERIC;
    }

    Serial.println("pre-lookup");
    // Does the screen already exist?
    if(_lookup(id) >= 0)
    {
        return MENU_ERROR_GENERIC;
    }

    // Screen does not exists: add it to deck
    screen->setParent(this);
    screen->setNumTabs(numTabs);
    screen->setCurTab(0);
    _screens[_numScreens].id = id;
    _screens[_numScreens].screen = screen;
    Serial.print("after: "); Serial.println((unsigned long)_screens[_numScreens].screen, HEX);

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
    if(_curScreen == (_numScreens - 1))
    {
        // Last screen in deck - do nothing! (no wrapping policy)
        return MENU_OK;
    }


    // Get info for current screen
    MenuScreen* scr = _screens[_curScreen].screen;
    int16_t nt = scr->getNumTabs();
    int16_t ct = scr->getCurTab();

    if(nt > 0)
    {
        // Screen is tabbed
        if(ct < (nt - 1))
        {
            // Not the last tab: change tab but not screen    
            scr->setCurTab(++ct);
        }
        else
        {
            // Last tab: move to next screen (force first tab)
            _curScreen++;
            _screens[_curScreen].screen->setCurTab(0);
        }
    }
    else
    {
        // Screen is not tabbed --> go to next
        _curScreen++;
    }

    return MENU_OK;
}

int8_t Menu::setPrevScreen(void)
{
    if(_curScreen == 0)
    {
        // First screen in deck - do nothing! (no wrapping policy)
        return MENU_OK;
    }

    // Get info for current screen
    MenuScreen* scr = _screens[_curScreen].screen;
    int16_t nt = scr->getNumTabs();
    int16_t ct = scr->getCurTab();

    if(nt > 0)
    {
        // Screen is tabbed
        if(ct > 0)
        {
            // Not the last tab: change tab but not screen    
            scr->setCurTab(--ct);
        }
        else
        {
            // Last tab: move to previous screen (force first tab)
            _curScreen--;
            _screens[_curScreen].screen->setCurTab(0);
        }
    }
    else
    {
        // Screen is not tabbed --> go to previous
        _curScreen--;
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

    Serial.print("lookup: "); Serial.println(scrIndx);

    return scrIndx;
}

void Menu::show(void)
{
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

    while (deltaAbs > 0)
    {
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

