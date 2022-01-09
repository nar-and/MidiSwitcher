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

#ifndef SCREEN_PS_MIDI_H
#define SCREEN_PS_MIDI_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "../MenuScreen.h"

class ScreenPsMidi : public MenuScreen
{
public:
    ScreenPsMidi(uint8_t dir);

    void show();
    void updateCursor(int16_t delta);    
    void updateValue(int16_t delta);

private:
    uint8_t _midiDir;
};

#endif // SCREEN_PS_MIDI_H

/****************************************************************************
 ****************************************************************************/


