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
#include <stdio.h>          // NULL, sprintf definitions
#include <string.h>
#include "InterfaceUtils.h"

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
#define DEBUG_PRINT		            1

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
// None

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
#define MAX_NUM_TABS    6

static const char* strNumTabs[MAX_NUM_TABS] = 
{
    "[1]23456",
    "1[2]3456",
    "12[3]456",
    "123[4]56",
    "1234[5]6",
    "12345[6]"
};

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
/*--------------------------------------------------------------------------*
 * AppSetup - Initialize application logic
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/


// Inspired by https://github.com/semibran/wrap-around
// min = range minimum value (included)
// max = range maximum value (included)
// Take care about having max >= min otherwise results are not meaningful!
int wrap(int val, int min, int max)
{
	int val0 = val - min;
	int max0 = max + 1 - min;
	
	return min + ((val0 >= 0) ? 
				  (val0 % max0) : 
				  ((val0 % max0 + max0) % max0));
}

void loopEnableToStr(uint8_t loopEnable, int8_t startBit, int8_t stopBit, char* str)
{
    uint8_t indx = 0;

    while(startBit >= stopBit)
    {
        str[indx++] = (loopEnable & (1 << startBit--))?('+'):('-');
    }

    // Add null termination
    str[indx] = '\0';
}

void printMidiMsg(char* buf, int maxLen, MidiMsg_t msg)
{
    if(msg.type == MIDI_TYPE_NONE)
    {
        snprintf(buf, maxLen, "%-16s", "Off");
    }
    else if (msg.type == MIDI_TYPE_PC)
    {
        //snprintf(buf, maxLen, "PC#%03d      Ch%02d", msg.num, msg.chan);
        snprintf(buf, maxLen, "PC C%02d #%03d     ", msg.chan, msg.num);
    }
    else if (msg.type == MIDI_TYPE_CC)
    {
        //snprintf(buf, maxLen, "CC#%03d V%03d Ch%02d", msg.num, msg.val, msg.chan);
        snprintf(buf, maxLen, "CC C%02d #%03d V%03d", msg.chan, msg.num, msg.val);
    }
}

void getTabStr(uint16_t curTab, uint16_t numTabs, char* buf)
{   
    // Number of characters for N tabs is N + 2
    strncpy(buf, strNumTabs[curTab], numTabs + 2);
    buf[numTabs + 2] = '\0';    // Add null termination
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

/****************************************************************************
 ****************************************************************************/
