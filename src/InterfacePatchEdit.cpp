#include "Interface.h"
#include "Menu.h"
#include <stdio.h>
#include <Arduino.h>

/*
void OnPatchEditExit(void* ptrObj)
{
    _moveToState(INT_STATE_PATCH_SEL);
}
*/

Menu_t _patchEditMenu = 
{
    NULL,   // lcd
    0,      // curLevel
    0,      // selItem
    0,      // firstItemShown
    0,      // numItems
            // items
    {       
        {0, "Loop enable", NULL},
        {0, "MIDI OUT en.", NULL},
        {0, "MIDI IN en.", NULL},
        {0, "Patch name", NULL},
        {0, "Exit", NULL},
        {-1, "", NULL},   // Terminator item
    }
};

#define NAME_CURSOR_START           4
#define LOOPA_CURSOR_START          5
#define LOOPB_CURSOR_START          11
#define MIDI_OUT_CURSOR_START       15


#define TEST    0


Patch_t patch;
int8_t  selPosition = 0;

int8_t LoopSelToCursor(int8_t sel)
{
    int8_t cur;

    if(sel < 4)
    {
        cur = sel + LOOPA_CURSOR_START;
    }
    else if (sel < 6)
    {
        cur = sel - 4 + LOOPB_CURSOR_START;
    }
    else
    {
        cur = sel - 6 + MIDI_OUT_CURSOR_START;
    }

    return cur;
}



int8_t NameSelToCursor(int8_t sel)
{
    return (sel + NAME_CURSOR_START);
}

#define BLINK_INTERVAL_MS       500

uint32_t blinkTime;
bool     blinkStatus;
bool     blinkEnabled = false;

void BlinkPatchNumberStart()
{    
    blinkTime = millis();
    blinkStatus = false;
    blinkEnabled = true;
}

void BlinkPatchNumberStop()
{    
    blinkEnabled = true;
}


void BlinkPatchNumberExec(LiquidCrystal_I2C* lcd)
{
    if(!blinkEnabled)
    {
        return;
    }

    uint32_t curTime = millis();
    if((curTime - blinkTime) >= BLINK_INTERVAL_MS)
    {
        // Toggle blink
        blinkTime = curTime;
        blinkStatus = !blinkStatus;

         char tmpA[3], tmpB[3];        
        if(blinkStatus)
        {
            sprintf(tmpA, "%02d", patch.num);    
            sprintf(tmpB, " \x7E");    
        }
        else
        {
            sprintf(tmpA, "  ");    
            sprintf(tmpB, "  ");    
        }
        
        lcd->noCursor();

        lcd->setCursor(0, 0);
        lcd->print(tmpA);
        
        lcd->setCursor(0, 1);
        lcd->print(tmpB);

        lcd->setCursor(NameSelToCursor(selPosition), 0);
        
#if TEST
        lcd->setCursor(LoopSelToCursor(selPosition), 1);
#endif

        lcd->cursor();

    }
}


bool letterSelected;

void Interface::_statePatchEdit(UiEvents_t events)
{  
    if(_onEnter)
    {        
        _onEnter = false;
        
        _patchMgr.getPatch(_activePatchIndx, &patch);   // Read active patch
        _printPatchInfo(patch, true);
        
        // Initialize patch number blinking
        BlinkPatchNumberStart();

        selPosition = 0;

        letterSelected = false;
        _lcd.setCursor(NameSelToCursor(selPosition), 0);

#if TEST        
        _lcd.setCursor(LoopSelToCursor(selPosition), 1);
#endif        
        _lcd.cursor();      // Turn on cursor

    }

    // Manage patch number blinking
    BlinkPatchNumberExec(&_lcd);


    if(events.EncDelta != 0)
    {   
        if(letterSelected == false)
        {
            selPosition += events.EncDelta;
            selPosition = constrain(selPosition, 0, 11);

            _lcd.setCursor(NameSelToCursor(selPosition), 0);
        }
        else
        {
            // Change letter
            char newLetter = patch.name[selPosition] + events.EncDelta;
            newLetter = constrain(newLetter, 33, 127);
            patch.name[selPosition] = newLetter;

            _lcd.print(newLetter);
            _lcd.setCursor(NameSelToCursor(selPosition), 0);        
        }
    }

    if(events.ButtonEnc == BTN_CLICK)
    {   
        // Toggle letter selection status
        letterSelected = !letterSelected;
    }    



#if TEST
    if(events.EncDelta != 0)
    {   
        selPosition += events.EncDelta;
        selPosition = constrain(selPosition, 0, 6);

        _lcd.setCursor(LoopSelToCursor(selPosition), 1);       
    }

    if(events.ButtonEnc == BTN_CLICK)
    {   
        Serial.print("SEL");
        Serial.print(selPosition);

        Serial.print(" LPEN");
        Serial.print(patch.loopEnable, HEX);

        // Toggle loop enable 
        uint8_t toggleMask = (1 << (7 - selPosition));
        patch.loopEnable ^= toggleMask;

        Serial.print(" TGL");
        Serial.print(toggleMask, HEX);
        Serial.print(" LPEN2");
        Serial.print(patch.loopEnable, HEX);
        Serial.print( " RES");
        Serial.println(patch.loopEnable & toggleMask, HEX);

        // Change character shown
        _lcd.print((patch.loopEnable & toggleMask) ? ("+"):("-"));
        _lcd.setCursor(LoopSelToCursor(selPosition), 1);
    }    
#endif 


    if(events.ButtonEnc == BTN_LONG_PRESS)
    {
        BlinkPatchNumberStop();
        _moveToState(INT_STATE_PATCH_SEL);
    }    
}


/*
void Interface::_statePatchEdit(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;

        MenuInit(&_patchEditMenu, &_lcd);     // TODO: move away and separate init from reset (i.e. reset indexes to move to beginning)
        MenuShow(&_patchEditMenu, true);
    }

    if(events.EncDelta != 0)
    {
        int8_t newSelItem = _patchEditMenu.selItem + events.EncDelta;        
        MenuUpdate(&_patchEditMenu, MENU_ACTION_NEWSEL, newSelItem);
    }

    if(events.ButtonEnc == BTN_LONG_PRESS)
    {        
        _moveToState(INT_STATE_PATCH_SEL);
    }
}
*/
