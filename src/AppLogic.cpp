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
#include "UserInterface.h"
#include "libs/MIDI_Library/MIDI.h"
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
static void handleControlChange(byte channel, byte number, byte value);
static void handleProgramChange(byte channel, byte number);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
static PatchManager patchMgr;
static UserInterface interface;
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

static MidiMsg_t lastMidiInMsg;
static bool midiInMsgReceived;

// Used to print structured messages with sprintf()
static char _msgString[50];


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


    Serial.print("PORTB");Serial.println(VPORTB.OUT, HEX);
    Serial.print("DDRB");Serial.println(VPORTB.DIR, HEX);
    Serial.print("PINB");Serial.println(VPORTB.IN, HEX);

//    VPORTA.OUT &= 0xFE;         // A1
    VPORTB.OUT &= 0xFC;         // B0, B1
    VPORTE.OUT &= 0xF4;         // E0, E1, E3
//    VPORTF.OUT &= 0xEF;         // F4
    //VPORTB.OUT |= 02;
//    VPORTA.DIR |= 0x01;
    VPORTB.DIR |= 0x03;
    VPORTE.DIR |= 0x0B;
//    VPORTF.DIR |= 0x10;

    Serial.print("*PORTB");Serial.println(VPORTB.OUT, HEX);
    Serial.print("*DDRB");Serial.println(VPORTB.DIR, HEX);
    Serial.print("*PINB");Serial.println(VPORTB.IN, HEX);

    // Initialize user input management
    UserInputInit();

    patchMgr.init();
    interface.init(&patchMgr);

    // Initialize MIDI communications, listen to all channels
    midiInMsgReceived = false;

    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleControlChange(handleControlChange);

    // TODO: add input channel filtering based on global settings
    //MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.begin(1);
    MIDI.turnThruOff();     // Disable soft thru
}

bool testStatus = false;

/*--------------------------------------------------------------------------*
 * AppLoop - Application loop 
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
void AppLoop(void)
{
    // Call MIDI.read the fastest you can for real-time performance.
    MIDI.read();
    
    if(midiInMsgReceived)
    {        
        midiInMsgReceived = false;
        // TODO: deal with MIDI IN messages        
        printMidiMsg(_msgString, 20, lastMidiInMsg);
        Serial.println(_msgString);
    }

    // Read user inputs
    UiEvents_t events = UserInputRead();

    if(UserInputIsAnyActive(events))
    {        
        if(events.ButtonEnc)
        {
            testStatus = !testStatus;
//            VPORTA.OUT ^= 0x01;
            VPORTB.OUT ^= 0x03;
             VPORTE.OUT ^= 0x0B;
//            VPORTF.OUT ^= 0x10;
            // digitalWrite(10, testStatus);
            Serial.print("*PORTB");Serial.println(VPORTB.OUT, HEX);

        }

#if DEBUG_PRINT
        Serial.print("ENC DELTA:");
        Serial.print(events.EncDelta);
        Serial.print(" BTNE:");
        Serial.print(events.ButtonEnc);
        Serial.print(" BTN1:");
        Serial.print(events.Button1);
        Serial.print(" BTN2:");
        Serial.println(events.Button2);
#endif
    }


    // Process user inputs
    interface.refresh(events);
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
void handleControlChange(byte channel, byte number, byte value)
{
    lastMidiInMsg = {MIDI_TYPE_CC, channel, number, value};
    midiInMsgReceived = true;
}

void handleProgramChange(byte channel, byte number)
{
    lastMidiInMsg = {MIDI_TYPE_PC, channel, number, 0};
    midiInMsgReceived = true;
}

/****************************************************************************
 ****************************************************************************/
