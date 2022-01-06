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
#include "libs/LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "UserInput.h"
#include "PatchManager.h"

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
#define     LCD_NUM_LINES       2
#define     LCD_LINE_LEN        16

#define     MENU_ACTION_NEWSEL      0
#define     MENU_ACTION_ENTER       1

#define     ENTRY_TYPE_TERM         0
#define     ENTRY_TYPE_ITEM         1
#define     ENTRY_TYPE_LOOPEN       2
#define     ENTRY_TYPE_ON_OFF       3
#define     ENTRY_TYPE_NAME         4

/*-----------------------------------*
 * PUBLIC MACROS
 *-----------------------------------*/
// None

/*-----------------------------------*
 * PUBLIC TYPEDEFS
 *-----------------------------------*/
typedef void (*OnEnterHandler_t)(void* ptrObj);
typedef void (*OnPrintHandler_t)(char* line);
typedef void (*OnEditHandler_t)(void);


typedef struct
{
    char                title[LCD_LINE_LEN + 1];
    char                name[LCD_LINE_LEN + 1];
    uint8_t             type;
    OnPrintHandler_t    onPrintHandler;
    OnEditHandler_t     onEditHandler;
//    OnEnterHandler_t    onEnterHandler;
} MenuItem_t;

typedef struct
{
    LiquidCrystal_I2C*  lcd;
    int8_t      selItem;                // Currently selected item
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
void MenuInit(Menu_t* menu, LiquidCrystal_I2C* lcdInstance, Patch_t patch);
void MenuShow(Menu_t* menu, bool clearLcd = false);
void MenuUpdate(Menu_t* menu, uint8_t action, int8_t actionParm);
void MenuControl(Menu_t* menu, UiEvents_t events);

#endif // MENU_H

/****************************************************************************
 ****************************************************************************/



