/****************************************************************************
 ****************************************************************************
 *
 *                              MODULE TITLE
 *
 * Author(s):
 *
 *
 * Implementation notes:
 * 
 *
 ****************************************************************************
 ****************************************************************************/

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "PatchManager.h"
#include <stdio.h>          // NULL, sprintf definitions
#include <Arduino.h>        // All Arduino functions (Serial etc.) 

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PRIVATE MACROS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PRIVATE TYPEDEFS
 *-----------------------------------*/
// None 

/*-----------------------------------*
 * PRIVATE FUNCTION PROTOTYPES
 *-----------------------------------*/
/*--------------------------------------------------------------------------*
 * Function name - Function description
 *
 * Arguments:
 * None
 *
 * Returned value:
 * None
 *
 * Usage notes:
 * None
 *--------------------------------------------------------------------------*/


/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/


/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
/*--------------------------------------------------------------------------*
 * Function name - Function description
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
int8_t PatchManager::init(void)
{
    // Load patch library from EEPROM
    _loadLibrary();
    return PATCHMGR_OK;
}

int8_t PatchManager::selectPatch(uint16_t indx)
{
    if(indx >= PATCH_LIBRARY_LEN)
    {
        return PATCHMGR_ERROR_GENERIC;
    }

    _selectedIndx = indx;

    return PATCHMGR_OK;   
}

int8_t PatchManager::updateSelection(int16_t delta)
{
    _selectedIndx += delta;
    _selectedIndx = constrain(_selectedIndx, 0, PATCH_LIBRARY_LEN - 1);
    return PATCHMGR_OK;   
}

int8_t PatchManager::activatePatch(uint16_t indx)
{
    if(indx >= PATCH_LIBRARY_LEN)
    {
        return PATCHMGR_ERROR_GENERIC;
    }

    _activeIndx = indx;

    return PATCHMGR_OK;   
}


int8_t PatchManager::activateSelectedPatch(void)
{
    return activatePatch(_selectedIndx);
}


uint16_t PatchManager::getSelectedPatchIndx(void)
{
    return _selectedIndx;
}

uint16_t PatchManager::getActivePatchIndx(void)
{
    return _activeIndx;
}

int8_t PatchManager::getPatch(uint16_t indx, Patch_t* patch)
{
    if(indx >= PATCH_LIBRARY_LEN)
    {
        return PATCHMGR_ERROR_GENERIC;
    }

    *patch = _library[indx];

    return PATCHMGR_OK;
}

int8_t PatchManager::getPatchName(uint16_t indx, char* name)
{
    if(indx >= PATCH_LIBRARY_LEN)
    {
        return PATCHMGR_ERROR_GENERIC;
    }

    strcpy(name, _library[indx].name);

    return PATCHMGR_OK;
}


int8_t PatchManager::getSelectedPatch(Patch_t* patch)
{
    return getPatch(_selectedIndx, patch);
}

int8_t PatchManager::getActivePatch(Patch_t* patch)
{
    return getPatch(_activeIndx, patch);
}

bool PatchManager::isSelActive(void)
{
    return (_selectedIndx == _activeIndx);
}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
/*--------------------------------------------------------------------------*
 * Function name - Function description
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
int8_t PatchManager::_loadLibrary(void)
{
    char buf[PATCH_NAME_LEN + 1];
    // TODO: load from EEPROM
    for(int i = 0; i < PATCH_LIBRARY_LEN; i++)
    {   
        // INITIALIZE WITH DUMMY DATA FOR TESTING
        _library[i].num = i;

        // Patch name must be blank-filled!!
        snprintf(_library[i].name, PATCH_NAME_LEN + 1, "PATCH%02d     ", i);    
        _library[i].loopEnable = ((i & 0x1) << 7) | 
                                 ((i & 0x3) << 5) | 
                                 ((~i & 0x1) << 4) | 
                                 (i & 0xF);
        char tmp[16];
        snprintf(tmp, 16, "%d LPEN%02x", i, _library[i].loopEnable);
        Serial.println(tmp);

        for(int j = 0; j < MAX_NUM_MIDI_OUT; j++)
        {
            _library[i].midiOut[j] = {j % 3, j + 1, j + 2, j + 3};
            Serial.print("O");Serial.print(j);Serial.print(":");
            Serial.print(_library[i].midiOut[j].type);Serial.println(_library[i].midiOut[j].chan);
        }

        for(int j = 0; j < MAX_NUM_MIDI_IN; j++)
        {
            _library[i].midiIn[j] = {j % 2, j + 4, j + 5, j + 6};
            Serial.print("I");Serial.print(j);Serial.print(":");
            Serial.print(_library[i].midiIn[j].type);Serial.println(_library[i].midiIn[j].chan);
        }
    }

    return PATCHMGR_OK;
}


/****************************************************************************
 ****************************************************************************/
