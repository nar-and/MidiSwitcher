#include "ScreenPsWriteCopy.h"
#include "../Display.h"
#include "../Menu.h"
#include "../PatchManager.h"
#include "../InterfaceUtils.h"


void ScreenPsWriteCopy::show(void)
{
    Display* lcd = _parent->_lcd;    
    Patch_t* curPatch = &_parent->_owner->_curPatch;

    snprintf(lcd->line0, LCD_LINE_LEN + 1, "%-16s", "Write/Copy to");
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "#%02d %-12s", curPatch->num, curPatch->name);
}

void ScreenPsWriteCopy::updateCursor(int16_t delta)
{
    // _parent->_selPosition += delta;

    _parent->_selPosition = 0;
    _parent->_curPosition = 0;
}

void ScreenPsWriteCopy::updateValue(int16_t delta)
{
    Display* lcd = _parent->_lcd;
    Patch_t* curPatch = &_parent->_owner->_curPatch;
    int8_t* writeTarget = &_parent->_owner->_writeTarget;
    PatchManager* patchMgr = _parent->_owner->_patchMgr;


    Serial.print("WT1:");Serial.print(*writeTarget);

    if(*writeTarget < 0) *writeTarget = curPatch->num;

    Serial.print(" WT2:");Serial.print(*writeTarget);
    *writeTarget += delta;

    Serial.print(" WT3:");Serial.print(*writeTarget);
    *writeTarget = wrap(*writeTarget, 0, PATCH_LIBRARY_LEN - 1);

    Serial.print(" WT4:");Serial.print(*writeTarget);

    char wtName[PATCH_NAME_LEN + 1];
    patchMgr->getPatchName(*writeTarget, wtName);

    Serial.print(" WT5:");Serial.println(wtName);

    lcd->setCursor(0, 1);
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "#%02d %-12s", *writeTarget, wtName);
    lcd->print(lcd->line1);
    lcd->setCursor(_parent->_curPosition, 1);
}


