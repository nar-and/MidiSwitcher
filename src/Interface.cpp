#include "Interface.h"
#include "PatchManager.h"
#include <Arduino.h>

#define DEBUG_PRINT_INTERFACE       1

Interface::Interface() : _lcd(0x27, 16, 2)
{

}

void Interface::init(void)
{
    Wire.begin();
    Wire.setClock(400000);

    // Initialize LCD
    _lcd.init();
    _lcd.clear();
    _lcd.backlight();

    // Initialize patch manager
    _patchMgr.begin();

    // Update screen with currently active patch
    _curState = INT_STATE_INIT;
    _onEnter = false;
}

void Interface::refresh(UiEvents_t events)
{
    switch(_curState)
    {
        case INT_STATE_INIT:
            _moveToState(INT_STATE_PATCH_SEL);
            break;

        case INT_STATE_PATCH_SEL:
            _statePatchSelect(events);
            break;

        case INT_STATE_PATCH_SETTINGS:
            _statePatchSettings(events);
            break;

        case INT_STATE_GLOBAL_SETTINGS:
            _stateGlobalSettings(events);
            break;

        default:
            break;
    }
}

void Interface::_moveToState(uint8_t state)
{
    _curState = state;
    _onEnter = true;
}

