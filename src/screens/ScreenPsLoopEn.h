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

#ifndef SCREEN_PS_LOOP_EN_H
#define SCREEN_PS_LOOP_EN_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "../MenuScreen.h"

class ScreenPsLoopEn : public MenuScreen
{
public:  
    ScreenPsLoopEn(uint8_t id);

    void show();
    void updateCursor(int16_t delta);    
    void updateValue(int16_t delta);

private:
    uint8_t _loopId;    
};

#endif // SCREEN_PS_LOOP_EN_H

/****************************************************************************
 ****************************************************************************/


