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

#ifndef DISPLAY_H
#define DISPLAY_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include "libs/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

#define     LCD_ADDRESS             0x27
#define     LCD_LINE_LEN            16
#define     LCD_NUM_LINES           2

class Display : public LiquidCrystal_I2C
{
public:
    Display(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);

    char line0[LCD_LINE_LEN + 1];    // Accounts for null
    char line1[LCD_LINE_LEN + 1];    // Accounts for null
};

#endif // DISPLAY_H

/****************************************************************************
 ****************************************************************************/


