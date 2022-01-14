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

#ifndef PATCH_MANAGER_H
#define PATCH_MANAGER_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "KeyValueTable.h"

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
// Return codes
#define PATCHMGR_OK                       0
#define PATCHMGR_ERROR_GENERIC            -1
#define PATCHMGR_ERROR_NO_PATCH           -2

// Size of patch library (i.e. collection of all the patches)
#define PATCH_LIBRARY_LEN   64

// Patch name length (excluding null termination)
#define PATCH_NAME_LEN      12 

// Loop enable bitmasks
#define LOOPA_1_EN          0x01
#define LOOPA_2_EN          0x02
#define LOOPA_3_EN          0x04
#define LOOPA_4_EN          0x08
#define LOOPA_MUTE          0x10

#define LOOPB_1_EN          0x20
#define LOOPB_2_EN          0x40
#define LOOPB_MUTE          0x80

// Allowed midi types
#define MIDI_TYPE_NONE      0
#define MIDI_TYPE_PC        1
#define MIDI_TYPE_CC        2

#define MAX_NUM_MIDI_OUT    4
#define MAX_NUM_MIDI_IN     4

/*-----------------------------------*
 * PUBLIC MACROS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC TYPEDEFS
 *-----------------------------------*/

typedef struct
{
    uint8_t type : 4;       // We will only use NONE/PC/CC but keep 4 bits for additional messages if needed 
    uint8_t chan : 4;       // 0...15 according to MIDI format definition
    uint8_t num;
    uint8_t val;
} MidiMsg_t;


typedef struct
{
    uint8_t num;
    char name[PATCH_NAME_LEN + 1];      // add space for NULL termination
    uint8_t loopEnable;                 // Loop activation control
    MidiMsg_t midiOut[MAX_NUM_MIDI_OUT];
    MidiMsg_t midiIn[MAX_NUM_MIDI_IN];
} Patch_t;

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

class PatchManager 
{
public: 
    int8_t init(void);
    int8_t selectPatch(uint16_t indx);
    int8_t updateSelection(int16_t delta);

    int8_t activatePatch(uint16_t indx);
    int8_t activateSelectedPatch(void);
    
    uint16_t getSelectedPatchIndx(void);
    uint16_t getActivePatchIndx(void);

    int8_t getSelectedPatch(Patch_t* patch);
    int8_t getActivePatch(Patch_t* patch);
    int8_t getPatch(uint16_t indx, Patch_t* patch);
    int8_t getPatchName(uint16_t indx, char* name);

    bool isSelActive(void);


private:
    int16_t _selectedIndx = 0;
    int16_t _activeIndx = 0;
    uint16_t _libraryIndx = 0;
    Patch_t  _library[PATCH_LIBRARY_LEN];
    KeyValueTable _midiInTriggers;

    int8_t _loadLibrary();    
};
#endif // PATCH_MANAGER_H

/****************************************************************************
 ****************************************************************************/


