#include "Interface.h"
#include <stdio.h>
#include <Arduino.h>

#define     LCD_NUM_LINES       2
#define     LCD_LINE_LEN        16


Menu_t _patchEditMenu = 
{
    0,      // curLevel
    0,      // selItem
    0,      // firstItemShown
    5,      // numItems
            // items
    {       
        {0, "Loop enable"},
        {0, "MIDI OUT en."},
        {0, "MIDI IN en."},
        {0, "Patch name"},
        {0, "Exit"},
    }    
};

void Interface::_statePatchEdit(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;
        _lcd.clear();
        _showEditMenu(&_patchEditMenu);
    }

    if(events.EncDelta != 0)
    {
        _patchEditMenu.selItem += events.EncDelta;
        _patchEditMenu.selItem = constrain(_patchEditMenu.selItem, 0, _patchEditMenu.numItems - 1);

        // Check if new selection is still visible
        if((_patchEditMenu.selItem < _patchEditMenu.firstItemShown) || 
           (_patchEditMenu.selItem > (_patchEditMenu.firstItemShown + 1)))
        {
            // Not visible --> update text
            _patchEditMenu.firstItemShown = _patchEditMenu.selItem;
        }

        Serial.print("SEL");
        Serial.print(_patchEditMenu.selItem);
        Serial.print(" FSW");
        Serial.println(_patchEditMenu.firstItemShown);

        _showEditMenu(&_patchEditMenu);
    }

    if(events.ButtonEnc == BTN_LONG_PRESS)
    {
        _moveToState(INT_STATE_PATCH_SEL);
    }
}


void Interface::_showEditMenu(Menu_t* menu)
{
    char lcdLine[16+1];    // Accounts for null

    for (int8_t line = 0; line < 2; line++)
    {        
        if((menu->firstItemShown + line) <= menu->numItems)
        {
            _lcd.setCursor(0,line);
            snprintf(lcdLine, 16, "%-2s%-14s", (menu->selItem == (menu->firstItemShown + line))?("->"):(""), 
                                                menu->items[menu->firstItemShown + line].name);
            _lcd.print(lcdLine);

            Serial.print("LINE");
            Serial.print(line);
            Serial.println(" PRINTED");
        }
        else
        {
            Serial.print("LINE");
            Serial.print(line);
            Serial.println(" SKIPPED");
        }
    }
}