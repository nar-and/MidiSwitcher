#include "ScreenPsLoopEn.h"
#include "../Display.h"
#include "../Menu.h"
#include "../InterfaceUtils.h"

#define LOOPA_EN_STARTPOS           12
#define LOOPB_EN_STARTPOS           14

ScreenPsLoopEn::ScreenPsLoopEn(uint8_t id)
{
    _loopId = id;
}

void ScreenPsLoopEn::show(void)
{
    Display* lcd = _parent->_lcd;
    snprintf(lcd->line0, LCD_LINE_LEN + 1, "Loop %-11s", (_loopId == LOOP_ID_A)?("A"):("B"));

    uint8_t loopEn = _parent->_owner->_curPatch.loopEnable;

    char loopEnStr[5];
    int8_t startBit =  (_loopId == LOOP_ID_A)?(3):(6);
    int8_t stopBit =  (_loopId == LOOP_ID_A)?(0):(5);
    loopEnableToStr(loopEn, startBit, stopBit, loopEnStr);
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "Enable%+10s", loopEnStr);
}

void ScreenPsLoopEn::updateCursor(int16_t delta)
{
    _parent->_selPosition += delta;

    // _selPosition = constrain(_selPosition, 0, (_loopAB == 0)?(3):(1));
    _parent->_selPosition = wrap(_parent->_selPosition, 0, (_loopId == LOOP_ID_A)?(3):(1));
    _parent->_curPosition = _parent->_selPosition + ((_loopId == LOOP_ID_A)?(LOOPA_EN_STARTPOS):(LOOPB_EN_STARTPOS));
}

void ScreenPsLoopEn::updateValue(int16_t delta)
{
    Display* lcd = _parent->_lcd;
    bool loopEnabled = (delta > 0);              
    
    // Change character shown
    lcd->print((loopEnabled) ? ("+"):("-"));
    lcd->setCursor(_parent->_curPosition, 1);
}

