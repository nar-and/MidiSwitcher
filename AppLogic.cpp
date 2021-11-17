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
#include "AppLogic.h"
#include <stdio.h>          // NULL, sprintf definitions
#include <Arduino.h>        // All Arduino functions (Serial etc.) 
#include "UserInput.h"


/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
// Used to print structured messages with sprintf()
char msgString[128];

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
// None

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
/*--------------------------------------------------------------------------*
 * AppSetup - Initialize application logic
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
void AppSetup(void)
{
#if DEBUG_PRINT
    // Initialize USB serial (debug printouts)
    Serial.begin(115200);
#endif

    // Initialize user input management
    UserInputInit();
}

/*--------------------------------------------------------------------------*
 * AppLoop - Application loop 
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
void AppLoop(void)
{
    // Process user inputs
    UiEvents_t events = UserInputRead();

#if DEBUG_PRINT		
    if(events.EncDelta != 0)
    {
        Serial.print("ENC DELTA:");
        Serial.println(events.EncDelta);
    }

    if(events.ButtonEnc != BTN_IDLE)
    {
        Serial.print("BTNE:");
        Serial.println(events.ButtonEnc);
    }

    if(events.Button1 != BTN_IDLE)
    {
        Serial.print("BTN1:");
        Serial.println(events.Button1);
    }

    if(events.Button2 != BTN_IDLE)
    {
        Serial.print("BTN2:");
        Serial.println(events.Button2);
    }

#endif     
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
