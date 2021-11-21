#include "Menu.h"
#include <stdio.h>
#include <Arduino.h>


void MenuInit(Menu_t* menu, LiquidCrystal_I2C* lcdInstance)
{
    menu->lcd = lcdInstance;
    menu->curLevel = 0;
    menu->selItem = 0;
    menu->firstItemShown = 0;
    
    // Count actual number of items, looking for terminator item
    menu->numItems = 0;
    while(menu->items[menu->numItems].level >= 0)
    {
        menu->numItems++;
    }

    Serial.print("NUMITEMS");
    Serial.println(menu->numItems);
}

void MenuShow(Menu_t* menu, bool clearLcd)
{    
    char lcdLine[LCD_LINE_LEN + 1];    // Accounts for null

    if(clearLcd)
    {
        menu->lcd->clear();
    }

    for (int8_t line = 0; line < LCD_NUM_LINES; line++)
    {        
        if((menu->firstItemShown + line) <= menu->numItems)
        {
            menu->lcd->setCursor(0,line);
            snprintf(lcdLine, 16, "%-2s%-14s", (menu->selItem == (menu->firstItemShown + line))?("->"):(""), 
                                                menu->items[menu->firstItemShown + line].name);
            menu->lcd->print(lcdLine);

            Serial.print("LINE");
            Serial.print(line);
            Serial.println(" PRINTED");
        }
        else
        {
            Serial.print("LINE");
            Serial.print(line);
            Serial.println(" SKIPPED");
        }
    }
}

void MenuUpdate(Menu_t* menu, int8_t newSelItem)
{
    // Limit requested input to maximum selectable item in menu 
    newSelItem = constrain(newSelItem, 0, menu->numItems - 1);
    
    if(newSelItem != menu->selItem)
    {
        // New item selected, visualization must actually change
        int8_t selOffset = newSelItem - menu->selItem;
        menu->selItem = newSelItem;
    
        if(selOffset > 0)
        {
            // Selection going up --> move visible items window only if exceeding upper window limit
            if(menu->selItem > (menu->firstItemShown + (LCD_NUM_LINES - 1)))
            {
                menu->firstItemShown = (menu->selItem - (LCD_NUM_LINES - 1));
            }
        }
        else
        {
            // Selection going down (cannot be the same as offset = 0 is ruled out by previous checks)
            // --> move visible items window only if exceeding lower window limit
            if(menu->selItem < menu->firstItemShown)
            {
                menu->firstItemShown = menu->selItem;
            }
        }

        Serial.print("SEL");
        Serial.print(menu->selItem);
        Serial.print(" FSW");
        Serial.println(menu->firstItemShown);

        // Update visualized menu
        MenuShow(menu);
    }
}
