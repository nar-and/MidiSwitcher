#include "Interface.h"
#include "Menu.h"
#include <stdio.h>
#include <Arduino.h>


Menu_t _patchEditMenu = 
{
    NULL,   // lcd
    0,      // curLevel
    0,      // selItem
    0,      // firstItemShown
    0,      // numItems
            // items
    {       
        {0, "Loop enable"},
        {0, "MIDI OUT en."},
        {0, "MIDI IN en."},
        {0, "Patch name"},
        {0, "Exit"},
        {-1, ""},   // Terminator item
    }    
};


void Interface::_statePatchEdit(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;

        MenuInit(&_patchEditMenu, &_lcd);
        MenuShow(&_patchEditMenu, true);
    }

    if(events.EncDelta != 0)
    {
        int8_t newSelItem = _patchEditMenu.selItem + events.EncDelta;        
        MenuUpdate(&_patchEditMenu, newSelItem);
    }

    if(events.ButtonEnc == BTN_LONG_PRESS)
    {
        _moveToState(INT_STATE_PATCH_SEL);
    }
}
