#include "Menu.h"
#include <stdio.h>
#include <Arduino.h>

static Patch_t _curPatch;


static void loopEnableToStr(uint8_t loopEnable, int8_t startBit, int8_t stopBit, char* str)
{
    uint8_t indx = 0;

    while(startBit >= stopBit)
    {
        str[indx++] = (loopEnable & (1 << startBit--))?('+'):('-');
    }

    // Add null termination
    str[indx] = '\0';
}


void MenuInit(Menu_t* menu, LiquidCrystal_I2C* lcdInstance, Patch_t patch)
{
    menu->lcd = lcdInstance;
    menu->selItem = 0;
    
    // Count actual number of items, looking for terminator item
    menu->numItems = 0;
    while(menu->items[menu->numItems].type != ENTRY_TYPE_TERM)
    {
        menu->numItems++;
    }

    Serial.print("NUMITEMS");
    Serial.println(menu->numItems);

    _curPatch = patch;
}

void MenuShow(Menu_t* menu, bool clearLcd)
{    
    char lcdLine[LCD_LINE_LEN + 1];    // Accounts for null

    if(clearLcd)
    {
        menu->lcd->clear();
    }

    // First line = menu entry name
    menu->lcd->setCursor(0, 0);
    snprintf(lcdLine, 16, "%-14s", menu->items[menu->selItem].name);
    menu->lcd->print(lcdLine);

    // Second line : it depends on entry type
    menu->lcd->setCursor(0, 1);

    if(menu->items[menu->selItem].onPrintHandler != NULL)
    {
        // Invoke requested action
        menu->items[menu->selItem].onPrintHandler(lcdLine);
    }
    else
    {
        // Do nothing --> print blank line
        snprintf(lcdLine, 16, "%-16s", "");
    }

    menu->lcd->print(lcdLine);    
}

void MenuUpdate(Menu_t* menu, uint8_t action, int8_t actionParm)
{
    if(action == MENU_ACTION_NEWSEL)
    {
        int8_t newSelItem = actionParm;

        // Limit requested input to maximum selectable item in menu 
        newSelItem = constrain(newSelItem, 0, menu->numItems - 1);
        
        if(newSelItem != menu->selItem)
        {
            menu->selItem = newSelItem;
            Serial.print("SEL");
            Serial.print(menu->selItem);

            // Update visualized menu
            MenuShow(menu);
        }
    }
    else if (action == MENU_ACTION_ENTER)
    {
/*        
        if(menu->items[menu->selItem].onEnterHandler != NULL)
        {
            // Invoke requested action
            menu->items[menu->selItem].onEnterHandler(NULL);
        }
*/        
    }
}

void MenuControl(Menu_t* menu, UiEvents_t events)
{
    if(events.EncDelta != 0)
    {
        int8_t newSelItem = menu->selItem + events.EncDelta;        
        MenuUpdate(menu, MENU_ACTION_NEWSEL, newSelItem);
    }
}