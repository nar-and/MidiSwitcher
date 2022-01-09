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

#ifndef SCREEN_PS_LOOP_MUTE_H
#define SCREEN_PS_LOOP_MUTE_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "../MenuScreen.h"

class ScreenPsLoopMute : public MenuScreen
{
public:  
    ScreenPsLoopMute(uint8_t id);

    void show();
    void updateCursor(int16_t delta);    
    void updateValue(int16_t delta);

private:
    uint8_t _loopId;    
};

#endif // SCREEN_PS_LOOP_MUTE_H

/****************************************************************************
 ****************************************************************************/


