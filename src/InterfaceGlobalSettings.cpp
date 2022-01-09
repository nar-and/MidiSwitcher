#include "UserInterface.h"



void UserInterface::_stateGlobalSettings(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;
        _menuSubstViewEdit = MENU_SUBST_VIEW;

        _globalSettingsMenu->setScreen(MENU_GS_MIDI_IN);
        _lcd.clear();
        _globalSettingsMenu->show();
    }

    if(_menuSubstViewEdit == MENU_SUBST_VIEW)
    {
        if(events.EncDelta != 0)
        {
            _globalSettingsMenu->updateScreen(events.EncDelta);
            _globalSettingsMenu->show();
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // Move to EDIT substate
            Serial.println("EDIT");
            _menuSubstViewEdit = MENU_SUBST_EDIT;
            _globalSettingsMenu->_selPosition = 0;

            _globalSettingsMenu->updateCursor(0);
            _lcd.setCursor(_globalSettingsMenu->_curPosition, 1);
            _lcd.blink();
        }

        if(events.ButtonEnc == BTN_LONG)
        {   
            _moveToState(INT_STATE_PATCH_SEL);        
        }
    }
    else
    {
        if(events.ButtonEnc == BTN_CLICK)
        {
            Serial.println("MOVE");
            _globalSettingsMenu->updateCursor(1);
            _lcd.setCursor(_curPosition, 1);
        }

        if(events.EncDelta != 0)
        {
            Serial.println("UPDATE");  
            _globalSettingsMenu->updateValue(events.EncDelta);
        }

        if(events.ButtonEnc == BTN_LONG)
        {        
            Serial.println("VIEW");
            _menuSubstViewEdit = MENU_SUBST_VIEW;
            _lcd.noBlink();
        }
    }
}

