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

#ifndef MENU_H
#define MENU_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h>
#include <LiquidCrystal_I2C.h>

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
#define     LCD_NUM_LINES       2
#define     LCD_LINE_LEN        16

#define     MENU_ACTION_NEWSEL      0
#define     MENU_ACTION_ENTER       1


/*-----------------------------------*
 * PUBLIC MACROS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC TYPEDEFS
 *-----------------------------------*/
typedef void (*OnEnterHandler_t)(void* ptrObj);


typedef struct
{
    int8_t              level;
    char                name[LCD_LINE_LEN + 1];
    OnEnterHandler_t    onEnterHandler;
} MenuItem_t;

typedef struct
{
    LiquidCrystal_I2C*  lcd;
    int8_t      curLevel;
    int8_t      selItem;
    int8_t      firstItemShown;
    int8_t      numItems;
    MenuItem_t  items[];
} Menu_t;

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
void MenuInit(Menu_t* menu, LiquidCrystal_I2C* lcdInstance);
void MenuShow(Menu_t* menu, bool clearLcd = false);
void MenuUpdate(Menu_t* menu, uint8_t action, int8_t actionParm);

#endif // MENU_H

/****************************************************************************
 ****************************************************************************/


