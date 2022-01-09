#include "ScreenPsWriteConfirm.h"
#include "../Display.h"
#include "../Menu.h"
#include "../PatchManager.h"
#include "../InterfaceUtils.h"

#define CONFIRM_STARTPOS            3

void ScreenPsWriteConfirm::show(void)
{
    Display* lcd = _parent->_lcd;

    snprintf(lcd->line0, LCD_LINE_LEN + 1, "|Confirm write?|");
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "|NO>        YES|");
}

void ScreenPsWriteConfirm::updateCursor(int16_t delta)
{
    _parent->_selPosition += delta;

    _parent->_selPosition = constrain(_parent->_selPosition, -1, 9);
    _parent->_curPosition = _parent->_selPosition + CONFIRM_STARTPOS;
}

void ScreenPsWriteConfirm::updateValue(int16_t delta)
{
    Display* lcd = _parent->_lcd;
    Patch_t* curPatch = &_parent->_owner->_curPatch;
    int8_t* writeTarget = &_parent->_owner->_writeTarget;
    PatchManager* patchMgr = _parent->_owner->_patchMgr;

    if(delta < 0)
    {
        // Cancel operation
        while(delta < 0)
        {
            Serial.print("DELTA");Serial.print(delta);
            Serial.print(" SEL1");Serial.print(_parent->_selPosition);
            Serial.print(" CUR1");Serial.print(_parent->_curPosition);

            if((_parent->_selPosition >= 0) && (_parent->_selPosition <= 8))
            {
                lcd->print(" ");
            }

            updateCursor(-1);
            Serial.print(" SEL2");Serial.print(_parent->_selPosition);
            Serial.print(" CUR2");Serial.println(_parent->_curPosition);

            lcd->setCursor(_parent->_curPosition, 1);
            delta++;

            if(_parent->_selPosition < 0)
            {
                Serial.println("**** EXIT NO ****");
            }
        }

    }
    else
    {
        while(delta > 0)
        {
            Serial.print("DELTA");Serial.print(delta);
            Serial.print(" SEL1");Serial.print(_parent->_selPosition);
            Serial.print(" CUR1");Serial.print(_parent->_curPosition);
            updateCursor(1);
            Serial.print(" SEL2");Serial.print(_parent->_selPosition);
            Serial.print(" CUR2");Serial.println(_parent->_curPosition);
            lcd->setCursor(_parent->_curPosition, 1);

            if(_parent->_selPosition <= 8)
            {
                lcd->print(">");
                lcd->setCursor(_parent->_curPosition, 1);
            }
            else
            {
                Serial.println("**** EXIT YES ****");
            }
            delta--;
        }
    }
}


