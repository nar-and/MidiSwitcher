/****************************************************************************
 ****************************************************************************
 *
 *                              MODULE TITLE
 *
 * Author(s):
 * 
 * 
 * Description:
 * 
 *
 * Usage notes:
 * 
 *
 ****************************************************************************
 ****************************************************************************/

#ifndef INTERFACE_UTILS_H
#define INTERFACE_UTILS_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "PatchManager.h"

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC MACROS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC TYPEDEFS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC VARIABLE DECLARATIONS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
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
int wrap(int val, int min, int max);
void loopEnableToStr(uint8_t loopEnable, int8_t startBit, int8_t stopBit, char* str);
void printMidiMsg(char* buf, int maxLen, MidiMsg_t msg);
void getTabStr(uint16_t curTab, uint16_t numTabs, char* buf);
void printMidiInChan(char* buf, int maxLen, uint8_t chan);

#endif // INTERFACE_UTILS_H

/****************************************************************************
 ****************************************************************************/


