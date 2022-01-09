#include "ScreenPsLoopMute.h"
#include "../Display.h"
#include "../Menu.h"
#include "../PatchManager.h"

#define ON_OFF_STARTPOS             13

ScreenPsLoopMute::ScreenPsLoopMute(uint8_t id)
{
    _loopId = id;
}

void ScreenPsLoopMute::show(void)
{
    Display* lcd = _parent->_lcd;
    uint8_t loopEn = _parent->_owner->_curPatch.loopEnable;

    snprintf(lcd->line0, LCD_LINE_LEN + 1, "Loop %-11s", (_loopId == LOOP_ID_A)?("A"):("B"));
    //snprintf(lcd->line0, LCD_LINE_LEN + 1, "%02d| Loop %-7s", curPatch.num, (_loopAB == 0)?("A"):("B"));
    bool loopMuted = (_loopId == LOOP_ID_A)?(loopEn & LOOPA_MUTE):(loopEn & LOOPB_MUTE);
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "Mute%+12s", (loopMuted)?("On "):("Off"));
}

void ScreenPsLoopMute::updateCursor(int16_t delta)
{
    _parent->_selPosition = 0;
    _parent->_curPosition = ON_OFF_STARTPOS;
}

void ScreenPsLoopMute::updateValue(int16_t delta)
{
    Display* lcd = _parent->_lcd;
    bool isOn = (delta > 0);
    
    // Change character shown
    lcd->print((isOn) ? ("On "):("Off"));
    lcd->setCursor(_parent->_curPosition, 1);
}


