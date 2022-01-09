#include "ScreenPsName.h"
#include "../Display.h"
#include "../Menu.h"
#include "../PatchManager.h"
#include "../InterfaceUtils.h"

#define NAME_STARTPOS               3

void ScreenPsName::show(void)
{
    Display* lcd = _parent->_lcd;    
    snprintf(lcd->line0, LCD_LINE_LEN + 1, "%-16s", "Name");
    //snprintf(_lcd.line0, LCD_LINE_LEN + 1, "%02d| %12s", curPatch.num, "Name");
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "  [%-12s]", _parent->_owner->_curPatch.name);
}

void ScreenPsName::updateCursor(int16_t delta)
{
    _parent->_selPosition += delta;

    _parent->_selPosition = wrap(_parent->_selPosition, 0, PATCH_NAME_LEN - 1);
    _parent->_curPosition = _parent->_selPosition + NAME_STARTPOS;
}

void ScreenPsName::updateValue(int16_t delta)
{
    Display* lcd = _parent->_lcd;
    Patch_t* curPatch = &_parent->_owner->_curPatch;

    int16_t newLetter = curPatch->name[_parent->_selPosition] + delta;

    // newLetter = constrain(newLetter, 32, 127);
    newLetter = wrap(newLetter, 32, 127);
    curPatch->name[_parent->_selPosition] = (char)newLetter;

    lcd->print(curPatch->name[_parent->_selPosition]);
    lcd->setCursor(_parent->_curPosition, 1);
}


