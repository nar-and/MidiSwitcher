#include "Interface.h"
#include <Arduino.h>

void Interface::_statePatchSelect(UiEvents_t events)
{
    Patch_t patch;

    if(_onEnter)
    {
        _onEnter = false;
        _patchMgr.getPatch(_selectedPatchIndx, &patch);
        _printPatchInfo(patch, (_selectedPatchIndx == _activePatchIndx));
    }

    if(events.EncDelta != 0)
    {   
        // Read new patch
        _selectedPatchIndx += events.EncDelta;
        _selectedPatchIndx = constrain(_selectedPatchIndx, 0, PATCH_LIBRARY_LEN - 1);

        _patchMgr.getPatch(_selectedPatchIndx, &patch);
        _printPatchInfo(patch, (_selectedPatchIndx == _activePatchIndx));
    }

    if(events.ButtonEnc == BTN_CLICK)
    {
        // Activate selected patch (if different than currently active patch)
        if(_activePatchIndx != _selectedPatchIndx)
        {
            _activePatchIndx = _selectedPatchIndx;
            _patchMgr.getPatch(_activePatchIndx, &patch);
            _printPatchInfo(patch, (_selectedPatchIndx == _activePatchIndx));
        }
    }

    if((events.ButtonEnc == BTN_LONG_PRESS) && 
        (_selectedPatchIndx == _activePatchIndx))
    {   
        // Enable edit mode only if currently shown patch is the active one
        _moveToState(INT_STATE_PATCH_EDIT);
    }
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
//    unsigned long time = millis();

    // Print first line: patch number + name
    _lcd.setCursor(0,0);
    sprintf(_msgString, "%02d| %-12s", patch.num, patch.name);
    _lcd.print(_msgString);

    // Print second line: patch enable indicator, patch loopA/B & midi enable
    _lcd.setCursor(0,1);
    
    char tempA[5];
    char tempB[3];
    char tempC[2];
    _loopEnableToStr(patch.loopEnable, 7, 4, tempA);
    _loopEnableToStr(patch.loopEnable, 3, 2, tempB);
    _loopEnableToStr(patch.loopEnable, 1, 1, tempC);
    sprintf(_msgString, "%s| A%s B%s M%s", (isActive)?(" \x7E"):("  "), tempA, tempB, tempC);
    _lcd.print(_msgString);

//    unsigned long elapsed = millis() - time;
//    Serial.println(elapsed);

#if DEBUG_PRINT_INTERFACE    

    Serial.print((isActive)?("->"):("  "));
    Serial.print(patch.name);

    Serial.print(" A");
    Serial.print(tempA);

    Serial.print(" B");
    Serial.print(tempB);

    Serial.print(" M");
    Serial.print(tempC);

    Serial.println();
#endif
}