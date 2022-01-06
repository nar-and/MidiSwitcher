#include "Interface.h"


#define MENU_GS_MIDI_IN             0
#define MENU_GS_FACTORY_RESET       1

void Interface::_stateGlobalSettings(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;
        _menuSubstViewEdit = MENU_SUBST_VIEW;
        _MenuGsInit();
        _MenuGsShow(true);
    }

    if(_menuSubstViewEdit == MENU_SUBST_VIEW)
    {
        if(events.EncDelta != 0)
        {
            _MenuGsUpdate(events.EncDelta);
            _MenuGsShow(false);
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // Move to EDIT substate
            Serial.println("EDIT");
            _menuSubstViewEdit = MENU_SUBST_EDIT;
            _selPosition = 0;

            _MenuGsCursorPos(0);
            _lcd.setCursor(_curPosition, 1);
            _lcd.blink();
        }

        if(events.ButtonEnc == BTN_LONG_PRESS)
        {        
            _moveToState(INT_STATE_PATCH_SEL);        
        }
    }
    else
    {
        if(events.ButtonEnc == BTN_CLICK)
        {
            Serial.println("MOVE");
            _MenuGsCursorPos(1);
            _lcd.setCursor(_curPosition, 1);
        }

        if(events.EncDelta != 0)
        {
            Serial.println("UPDATE");            
            _MenuGsFieldUpdate(events.EncDelta);
        }

        if(events.ButtonEnc == BTN_LONG_PRESS)
        {        
            Serial.println("VIEW");
            _menuSubstViewEdit = MENU_SUBST_VIEW;
            _lcd.noBlink();
        }
    }
}

void Interface::_MenuGsInit(void)
{
    _menuState = MENU_GS_MIDI_IN;
}


void Interface::_MenuGsUpdate(int16_t delta)
{
    int16_t deltaAbs = (delta >= 0)?(delta):(-delta);
    int16_t deltaSign = (delta >= 0)?(+1):(-1);

    while (deltaAbs > 0)
    {
        switch(_menuState)
        {
            case MENU_GS_MIDI_IN:
                if(deltaSign > 0) 
                {   
                    // +1 --> move to next state
                    _menuState = MENU_GS_FACTORY_RESET;
                }
                else 
                {
                    // Do nothing - first entry
                }
                break;

            case MENU_GS_FACTORY_RESET:
                if(deltaSign > 0) 
                {   
                    // Do nothing - last entry
                }
                else 
                {
                    // -1 --> go to LOOP_ENABLE
                    _menuState = MENU_GS_MIDI_IN;
                }
                break;
        }

        deltaAbs--;
    }

}


void Interface::_MenuGsShow(bool clearLcd)
{    
    if(clearLcd)
    {
        _lcd.clear();
    }

    switch(_menuState)
    {          
        case MENU_GS_MIDI_IN:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "GS MIDI In      ");
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "Test            ");
            }
            break;

        case MENU_GS_FACTORY_RESET:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "Factory Reset   ");
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "Test            ");
            }
            break;        
    }    

    // Print to screen
    _lcd.setCursor(0, 0);
    _lcd.print(lcdLine0);

    _lcd.setCursor(0, 1);
    _lcd.print(lcdLine1);
}

void Interface::_MenuGsCursorPos(int8_t delta)
{
    _selPosition += delta;

    switch(_menuState)
    {          
        case MENU_GS_MIDI_IN:
            {
                // _selPosition = constrain(_selPosition, 0, (_loopAB == 0)?(3):(1));
                _selPosition = 0;
                _curPosition = 0;
            }
            break;

        case MENU_GS_FACTORY_RESET:
            {
                _selPosition = 0;
                _curPosition = 0;
            }
            break;        
    }

    Serial.print("CURPOS");Serial.println(_curPosition);

    return;
}


int8_t Interface::_MenuGsFieldUpdate(int8_t delta)
{
    switch(_menuState)
    {          
        case MENU_GS_MIDI_IN:
            {
            }
            break;

        case MENU_GS_FACTORY_RESET:
            {
            }
            break;
    }

    return 0;
}
