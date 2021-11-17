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
#include <JC_Button.h>          // https://github.com/JChristensen/JC_Button
#include <Rotary.h>				// https://github.com/buxtronix/arduino/tree/master/libraries/Rotary

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
#define DEBUG_PRINT_ENC			0
 
#define PIN_ENC_A				2
#define PIN_ENC_B				3
#define PIN_BTN_ENC				5
#define PIN_BTN_1				6
#define PIN_BTN_2				7

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
static Button _Button1(PIN_BTN_1, BTN_1_DEBOUNCE_MS, false);       
static Button _Button2(PIN_BTN_2, BTN_2_DEBOUNCE_MS, false);       
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
    _Button1.begin();
    _Button2.begin();

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
		
#if DEBUG_PRINT_ENC		
        Serial.print("ENC POS");
        Serial.print(_EncoderPos);
        Serial.print(" DELTA");
        Serial.println(events.EncDelta);
#endif 

		// Save position to detect next rotation event
        _EncoderPrevPos = _EncoderPos;
    }  	
	
	// *** Process encoder button *** 
    events.ButtonEnc = BTN_IDLE;	
    _ButtonEnc.read();    
    if (_ButtonEnc.wasReleased())
    {
        if(_ButtonEncLongPress)
        {	
			// Button was released after a long press event - do nothing
            _ButtonEncLongPress = false;
        }   
        else
        {     
			// Button was released before long press triggered - generate click event
            events.ButtonEnc = BTN_CLICK;
#if DEBUG_PRINT_ENC					
            Serial.println("BTN ENC CLICK");
#endif			
        }        
    }

    if(!_ButtonEncLongPress)
    {
        if(_ButtonEnc.pressedFor(BTN_ENC_LONG_PRESS_MS))
        {
			// Generate long press event
            events.ButtonEnc = BTN_LONG_PRESS;

#if DEBUG_PRINT_ENC			
            Serial.println("BTN ENC LONGPRESS");
#endif
			// Keep track that a long press is in progress, to ignore subsequent release event
			_ButtonEncLongPress = true;
        }
    }
	
	// *** Process button 1 *** 	
    events.Button1 = BTN_IDLE;
    _Button1.read();
    if (_Button1.wasReleased())
    {
        if(_Button1LongPress)
        {	
			// Button was released after a long press event - do nothing
            _Button1LongPress = false;
        }   
        else
        {     
			// Button was released before long press triggered - generate click event
            events.Button1 = BTN_CLICK;

#if DEBUG_PRINT_ENC					
            Serial.println("BTN 1 CLICK");
#endif			
        }        
    }

    if(!_Button1LongPress)
    {
        if(_Button1.pressedFor(BTN_1_LONG_PRESS_MS))
        {
			// Generate long press event
            events.Button1 = BTN_LONG_PRESS;

#if DEBUG_PRINT_ENC			
            Serial.println("BTN 1 LONGPRESS");
#endif
			// Keep track that a long press is in progress, to ignore subsequent release event
			_Button1LongPress = true;
        }
    }
	

	// *** Process button 2 *** 
    events.Button2 = BTN_IDLE;
    _Button2.read();	
    if (_Button2.wasReleased())
    {
        if(_Button2LongPress)
        {	
			// Button was released after a long press event - do nothing
            _Button2LongPress = false;
        }   
        else
        {     
			// Button was released before long press triggered - generate click event
            events.Button2 = BTN_CLICK;
#if DEBUG_PRINT_ENC					
            Serial.println("BTN 2 CLICK");
#endif		
        }        
    }

    if(!_Button2LongPress)
    {
        if(_Button2.pressedFor(BTN_2_LONG_PRESS_MS))
        {
			// Generate long press event
            events.Button2 = BTN_LONG_PRESS;

#if DEBUG_PRINT_ENC			
            Serial.println("BTN 2 LONGPRESS");
#endif
			// Keep track that a long press is in progress, to ignore subsequent release event
			_Button2LongPress = true;
        }
    }		

    return events;
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
            status = BTN_LONG_PRESS;

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
