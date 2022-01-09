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

#ifndef USER_INPUT_H
#define USER_INPUT_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
// Return codes
#define UI_OK                       0
#define UI_ERROR_GENERIC            -1

// Button events
#define BTN_IDLE                    0
#define BTN_CLICK                   1
#define BTN_LONG                    2

/*-----------------------------------*
 * PUBLIC MACROS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC TYPEDEFS
 *-----------------------------------*/
typedef struct
{
    int16_t EncDelta;        // Encoder rotations since last read (>0 = CW)
    uint8_t ButtonEnc;       // Encoder button event
    uint8_t Button1;         // Button 1 event
    uint8_t Button2;         // Button 2 event
} UiEvents_t;

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
int8_t UserInputInit(void);
UiEvents_t UserInputRead(void);
bool UserInputIsAnyActive(UiEvents_t events);

#endif // USER_INPUT_H

/****************************************************************************
 ****************************************************************************/


