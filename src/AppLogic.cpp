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
#include "SwitcherLogic.h"
#include "libs/MIDI_Library/MIDI.h"
#include "InterfaceUtils.h"
#include "LoopSwitch.h"

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
static MidiMsg_t MidiMsgRead(void);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
static PatchManager patchMgr;
static SwitcherLogic swLogic;
static LoopSwitch loopSwitch;
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

    // Initialize user input management
    UserInputInit();

    patchMgr.init();
    swLogic.init(&patchMgr);
    loopSwitch.init();

    // Initialize MIDI communications, listen to all channels
    midiInMsgReceived = false;

    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleControlChange(handleControlChange);

    // Read channel filter from settings
    GlobalSettings_t gs;
    patchMgr.getGlobalSettings(&gs);

    // TODO: align definitions between global settings and MIDI library
    uint8_t inChan = (gs.midiInChannel == MIDI_IN_CHAN_OMNI) ? 
                     (MIDI_CHANNEL_OMNI):
                     (gs.midiInChannel + 1);
    MIDI.begin(inChan);
    MIDI.turnThruOff();     // Disable soft thru
}

/*--------------------------------------------------------------------------*
 * AppLoop - Application loop 
 *
 * Implementation notes:
 * None
 *--------------------------------------------------------------------------*/
void AppLoop(void)
{
    // Read external inputs 
    MidiMsg_t rxMsg = MidiMsgRead();        // MIDI messages    
    UiEvents_t events = UserInputRead();    // User inputs on HMI

    // DEBUG
    if(rxMsg.type != MIDI_TYPE_NONE)
    {
#if DEBUG_PRINT        
        printMidiMsg(_msgString, 20, lastMidiInMsg);
        Serial.println(_msgString);
#endif        
    }

    if(UserInputIsAnyActive(events))
    {        
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

    // Process external inputs
    swLogic.refresh(events, rxMsg);
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
MidiMsg_t MidiMsgRead(void)
{
    // Call MIDI.read the fastest you can for real-time performance.
    MIDI.read();
    
    if(midiInMsgReceived)
    {        
        midiInMsgReceived = false;
        return lastMidiInMsg;
    }
    else
    {
        return {MIDI_TYPE_NONE, 0, 0, 0};
    }
}


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
