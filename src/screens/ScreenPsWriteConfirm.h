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

#ifndef SCREEN_PS_WRITE_CONFIRM_H
#define SCREEN_PS_WRITE_CONFIRM_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "../MenuScreen.h"

class ScreenPsWriteConfirm : public MenuScreen
{
public:  
    void show();
    void updateCursor(int16_t delta);    
    void updateValue(int16_t delta);
};

#endif // SCREEN_PS_WRITE_CONFIRM_H

/****************************************************************************
 ****************************************************************************/


