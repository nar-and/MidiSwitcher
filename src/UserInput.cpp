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
#include "UserInput.h"
#include <Arduino.h>        	// All Arduino functions (Serial etc.) 
#include "libs/JC_Button/JC_Button.h"          // https://github.com/JChristensen/JC_Button
#include "libs/Rotary/Rotary.h"				// https://github.com/buxtronix/arduino/tree/master/libraries/Rotary

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
#define PIN_ENC_A				2
#define PIN_ENC_B				3
#define PIN_BTN_ENC				5
//#define PIN_BTN_1				6
//#define PIN_BTN_2				7

#define	BTN_ENC_DEBOUNCE_MS			10
#define	BTN_1_DEBOUNCE_MS			10
#define	BTN_2_DEBOUNCE_MS			10

#define	BTN_ENC_LONG_PRESS_MS		1000
#define	BTN_1_LONG_PRESS_MS			1000
#define	BTN_2_LONG_PRESS_MS			1000

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
static uint8_t ButtonRead(Button* btn, bool* isLongPress, uint16_t longPressMs);
static void EncoderRotate(void);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
static Button _ButtonEnc(PIN_BTN_ENC, BTN_ENC_DEBOUNCE_MS, false);   // false = disable internal pullups 
//static Button _Button1(PIN_BTN_1, BTN_1_DEBOUNCE_MS, false);       
//static Button _Button2(PIN_BTN_2, BTN_2_DEBOUNCE_MS, false);       
static Rotary _Encoder(PIN_ENC_B, PIN_ENC_A);

static volatile uint16_t _EncoderPos = 0;
static uint16_t _EncoderPrevPos = 0;
static bool _ButtonEncLongPress = false;
static bool _Button1LongPress = false;
static bool _Button2LongPress = false;

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
/*--------------------------------------------------------------------------*
 * Function name - Function description
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
int8_t UserInputInit(void)
{   
	// Attach ISR to encoder quadrature output pins
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), EncoderRotate, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), EncoderRotate, CHANGE);
	
	// Start processing buttons
    _ButtonEnc.begin();
//    _Button1.begin();
//    _Button2.begin();

    return UI_OK;
}

/*--------------------------------------------------------------------------*
 * Function name - Function description 
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
UiEvents_t UserInputRead(void)
{
    UiEvents_t events;

	// *** Process encoder ***
    events.EncDelta = 0; 
    if(_EncoderPos != _EncoderPrevPos)
    {
		// Encoder position changed: generate delta event
		events.EncDelta = (int16_t)(_EncoderPos - _EncoderPrevPos);

		// Save position to detect next rotation event
        _EncoderPrevPos = _EncoderPos;
    }  
	
	// *** Process buttons *** 
    events.ButtonEnc = ButtonRead(&_ButtonEnc, &_ButtonEncLongPress, BTN_ENC_LONG_PRESS_MS);	
//    events.Button1 = ButtonRead(&_Button1, &_Button1LongPress, BTN_1_LONG_PRESS_MS);	
//    events.Button2 = ButtonRead(&_Button2, &_Button2LongPress, BTN_2_LONG_PRESS_MS);

    return events;
}

bool UserInputIsAnyActive(UiEvents_t events)
{
/*    
    return ((events.EncDelta != 0) || 
            (events.ButtonEnc != BTN_IDLE) ||
            (events.Button1 != BTN_IDLE) ||
            (events.Button2 != BTN_IDLE)); 
*/            
    return ((events.EncDelta != 0) || 
            (events.ButtonEnc != BTN_IDLE));
}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
uint8_t ButtonRead(Button* btn, bool* isLongPress, uint16_t longPressMs)
{
    uint8_t status = BTN_IDLE;
    btn->read();    
    if (btn->wasReleased())
    {
        if(*isLongPress)
        {	
			// Button was released after a long press event - do nothing
            *isLongPress = false;
        }   
        else
        {     
			// Button was released before long press triggered - generate click event
            status = BTN_CLICK;
        }        
    }

    if(*isLongPress == false)
    {
        if(btn->pressedFor(longPressMs))
        {
			// Generate long press event
            status = BTN_LONG;

			// Keep track that a long press is in progress, to ignore subsequent release event
			*isLongPress = true;
        }
    }

    return status;
}



/*--------------------------------------------------------------------------*
 * Function name - Function description
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
void EncoderRotate(void) 
{
	uint8_t result = _Encoder.process();
	if (result == DIR_CW) 
	{
		_EncoderPos++;
	} 
	else if (result == DIR_CCW) 
	{
		_EncoderPos--;
	}
	else
	{
		// result == DIR_NONE --> do nothing
	}
}


/****************************************************************************
 ****************************************************************************/
