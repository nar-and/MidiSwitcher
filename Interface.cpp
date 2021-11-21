#include "Interface.h"
#include "PatchManager.h"
#include <Arduino.h>

#define DEBUG_PRINT_INTERFACE       1

Interface::Interface() : _lcd(0x27, 16, 2)
{

}

void Interface::init(void)
{
    // Initialize LCD
    _lcd.init();
    _lcd.clear();
    _lcd.backlight();

    // Initialize patch manager
    _patchMgr.begin();
    _activePatchIndx = 0;
    _selectedPatchIndx = 0;

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

        case INT_STATE_PATCH_EDIT:
            _statePatchEdit(events);
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


void Interface::_loopEnableToStr(uint8_t loopEnable, int8_t startBit, int8_t stopBit, char* str)
{
    uint8_t indx = 0;

    while(startBit >= stopBit)
    {
        str[indx++] = (loopEnable & (1 << startBit--))?('+'):('-');
    }

    // Add null termination
    str[indx] = '\0';
}

void Interface::_printPatchInfo(Patch_t patch, bool isActive)
{
    // Print first line
    _lcd.setCursor(0,0);
    sprintf(_msgString, "%02d| %-12s", patch.num, patch.name);
    _lcd.print(_msgString);

    // Print second line
    _lcd.setCursor(0,1);
    
    char tempA[5];
    char tempB[3];
    _loopEnableToStr(patch.loopEnable, 5, 2, tempA);
    _loopEnableToStr(patch.loopEnable, 1, 0, tempB);
    sprintf(_msgString, "%s| A%s B%s M-", (isActive)?("->"):("  "), tempA, tempB);
    _lcd.print(_msgString);

#if DEBUG_PRINT_INTERFACE    

    Serial.print((isActive)?("->"):("  "));
    Serial.print(patch.name);

    Serial.print("  A");
    Serial.print(tempA);

    Serial.print("  B");
    Serial.print(tempB);

    Serial.println();
#endif
}