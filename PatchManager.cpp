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
int8_t PatchManager::begin(void)
{
    // Load patch library from EEPROM
    _loadLibrary();
    return PATCHMGR_OK;
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
        _library[i].num = i;
        snprintf(buf, PATCH_NAME_LEN + 1, "PATCH%d", i);    
        strncpy(_library[i].name, buf, PATCH_NAME_LEN + 1);
        _library[i].loopEnable = i;
    }

    return PATCHMGR_OK;
}


/****************************************************************************
 ****************************************************************************/
