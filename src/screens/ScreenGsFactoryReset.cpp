#include "ScreenGsFactoryReset.h"
#include "../Display.h"
#include "../Menu.h"

void ScreenGsFactoryReset::show(void)
{
    Display* lcd = _parent->_lcd;
    snprintf(lcd->line0, LCD_LINE_LEN + 1, "Factory Reset   ");
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "Test            ");
}

void ScreenGsFactoryReset::updateCursor(int16_t delta)
{
    _parent->_selPosition = 0;
    _parent->_curPosition = 0;
}

void ScreenGsFactoryReset::updateValue(int16_t delta)
{
}

