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

#ifndef SCREEN_GS_MIDI_IN_H
#define SCREEN_GS_MIDI_IN_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "../MenuScreen.h"

class ScreenGsMidiIn : public MenuScreen
{
    void show();
    void updateScreen(int16_t delta);
    void updateCursor(int16_t delta);    
    void updateValue(int16_t delta);
};

#endif // SCREEN_GS_MIDI_IN_H

/****************************************************************************
 ****************************************************************************/


