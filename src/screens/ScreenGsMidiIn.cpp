#include "ScreenGsMidiIn.h"
#include "../Display.h"
#include "../Menu.h"

void ScreenGsMidiIn::show(void)
{
    Display* lcd = _parent->_lcd;
    snprintf(lcd->line0, LCD_LINE_LEN + 1, "GS MIDI In      ");
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "Test            ");
}

void ScreenGsMidiIn::updateScreen(int16_t delta)
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

void ScreenGsMidiIn::updateCursor(int16_t delta)
{
    _parent->_selPosition = 0;
    _parent->_curPosition = 0;
}

void ScreenGsMidiIn::updateValue(int16_t delta)
{
}

