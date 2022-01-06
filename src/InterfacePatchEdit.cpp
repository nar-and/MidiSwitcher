#include "Interface.h"
#include "Menu.h"
#include <stdio.h>
#include <Arduino.h>

/*
void OnPatchEditExit(void* ptrObj)
{
    _moveToState(INT_STATE_PATCH_SEL);
}
*/
Patch_t curPatch;

void onLoopAEnablePrint(char* line)
{   
    snprintf(line, 16, "%-16s", "++++");
}

void onLoopBEnablePrint(char* line)
{
    snprintf(line, 16, "%-16s", "--");    
}


void onLoopAMutePrint(char* line)
{
    snprintf(line, 16, "%-16s", "On");
}

void onLoopBMutePrint(char* line)
{
    snprintf(line, 16, "%-16s", "Off");
}

void onMidiOutEnablePrint(char* line)
{
    snprintf(line, 16, "%-16s", "On");
}


void onMidiInEnablePrint(char* line)
{
    snprintf(line, 16, "%-16s", "Off");
}

void onPatchNamePrint(char* line)
{
    snprintf(line, 16, "%-16s", curPatch.name);
}


Menu_t _patchEditMenu = 
{
    NULL,   // lcd
    0,      // selItem
    0,      // numItems
            // items
    {       
        {"Loop A", "Enable", ENTRY_TYPE_LOOPEN, onLoopAEnablePrint, NULL},
        {"Loop A", "Mute", ENTRY_TYPE_ON_OFF, onLoopAEnablePrint, NULL},
        {"Loop B", "Enable", ENTRY_TYPE_LOOPEN, onLoopBEnablePrint, NULL},
        {"Loop B", "Mute", ENTRY_TYPE_ON_OFF, onLoopBEnablePrint, NULL},
        {"MIDI Out", "Enable", ENTRY_TYPE_ON_OFF, onLoopBEnablePrint, NULL},
    }
};



#if 0
#define NAME_CURSOR_START           4
#define LOOPA_CURSOR_START          5
#define LOOPB_CURSOR_START          11
#define MIDI_OUT_CURSOR_START       15


#define TEST    0


Patch_t patch;
int8_t  selPosition = 0;

int8_t LoopSelToCursor(int8_t sel)
{
    int8_t cur;

    if(sel < 4)
    {
        cur = sel + LOOPA_CURSOR_START;
    }
    else if (sel < 6)
    {
        cur = sel - 4 + LOOPB_CURSOR_START;
    }
    else
    {
        cur = sel - 6 + MIDI_OUT_CURSOR_START;
    }

    return cur;
}



int8_t NameSelToCursor(int8_t sel)
{
    return (sel + NAME_CURSOR_START);
}

#define BLINK_INTERVAL_MS       500

uint32_t blinkTime;
bool     blinkStatus;
bool     blinkEnabled = false;

void BlinkPatchNumberStart()
{    
    blinkTime = millis();
    blinkStatus = false;
    blinkEnabled = true;
}

void BlinkPatchNumberStop()
{    
    blinkEnabled = true;
}


void BlinkPatchNumberExec(LiquidCrystal_I2C* lcd)
{
    if(!blinkEnabled)
    {
        return;
    }

    uint32_t curTime = millis();
    if((curTime - blinkTime) >= BLINK_INTERVAL_MS)
    {
        // Toggle blink
        blinkTime = curTime;
        blinkStatus = !blinkStatus;

         char tmpA[3], tmpB[3];        
        if(blinkStatus)
        {
            sprintf(tmpA, "%02d", patch.num);    
            sprintf(tmpB, " \x7E");    
        }
        else
        {
            sprintf(tmpA, "  ");    
            sprintf(tmpB, "  ");    
        }
        
        lcd->noCursor();

        lcd->setCursor(0, 0);
        lcd->print(tmpA);
        
        lcd->setCursor(0, 1);
        lcd->print(tmpB);

        lcd->setCursor(NameSelToCursor(selPosition), 0);
        
#if TEST
        lcd->setCursor(LoopSelToCursor(selPosition), 1);
#endif

        lcd->cursor();

    }
}


bool letterSelected;

void Interface::_statePatchEdit(UiEvents_t events)
{  
    if(_onEnter)
    {        
        _onEnter = false;
        
        _patchMgr.getPatch(_activePatchIndx, &patch);   // Read active patch
        _printPatchInfo(patch, true);
        
        // Initialize patch number blinking
        BlinkPatchNumberStart();

        selPosition = 0;

        letterSelected = false;
        _lcd.setCursor(NameSelToCursor(selPosition), 0);

#if TEST        
        _lcd.setCursor(LoopSelToCursor(selPosition), 1);
#endif        
        _lcd.cursor();      // Turn on cursor

    }

    // Manage patch number blinking
    BlinkPatchNumberExec(&_lcd);


    if(events.EncDelta != 0)
    {   
        if(letterSelected == false)
        {
            selPosition += events.EncDelta;
            selPosition = constrain(selPosition, 0, 11);

            _lcd.setCursor(NameSelToCursor(selPosition), 0);
        }
        else
        {
            // Change letter
            char newLetter = patch.name[selPosition] + events.EncDelta;
            newLetter = constrain(newLetter, 33, 127);
            patch.name[selPosition] = newLetter;

            _lcd.print(newLetter);
            _lcd.setCursor(NameSelToCursor(selPosition), 0);        
        }
    }

    if(events.ButtonEnc == BTN_CLICK)
    {   
        // Toggle letter selection status
        letterSelected = !letterSelected;
    }    



#if TEST
    if(events.EncDelta != 0)
    {   
        selPosition += events.EncDelta;
        selPosition = constrain(selPosition, 0, 6);

        _lcd.setCursor(LoopSelToCursor(selPosition), 1);       
    }

    if(events.ButtonEnc == BTN_CLICK)
    {   
        Serial.print("SEL");
        Serial.print(selPosition);

        Serial.print(" LPEN");
        Serial.print(patch.loopEnable, HEX);

        // Toggle loop enable 
        uint8_t toggleMask = (1 << (7 - selPosition));
        patch.loopEnable ^= toggleMask;

        Serial.print(" TGL");
        Serial.print(toggleMask, HEX);
        Serial.print(" LPEN2");
        Serial.print(patch.loopEnable, HEX);
        Serial.print( " RES");
        Serial.println(patch.loopEnable & toggleMask, HEX);

        // Change character shown
        _lcd.print((patch.loopEnable & toggleMask) ? ("+"):("-"));
        _lcd.setCursor(LoopSelToCursor(selPosition), 1);
    }    
#endif 


    if(events.ButtonEnc == BTN_LONG_PRESS)
    {
        BlinkPatchNumberStop();
        _moveToState(INT_STATE_PATCH_SEL);
    }    
}

#endif


#define MAX_LOOP_VAL            1
#define MAX_MIDI_OUT_VAL        3

static uint8_t _loopAB;                 // 0 = A, 1 = B
static uint8_t _midiOutIndx;
static bool _midiOutEn[] = {true, false, false, true};

static uint8_t _menuState;

#define MENU_RESET                  0
#define MENU_LOOP_ENABLE            1
#define MENU_LOOP_MUTE              2
#define MENU_MIDI_OUT_EN            3
#define MENU_MIDI_OUT_TYPE          4
#define MENU_MIDI_OUT_CHAN          5
#define MENU_MIDI_OUT_NUM           6
#define MENU_MIDI_OUT_VAL           7
#define MENU_NAME                   8


char _strMenuTitle[LCD_LINE_LEN + 1];

void Interface::_MenuInit(void)
{
    _menuState = MENU_LOOP_ENABLE;
    _loopAB = 0;
}


void Interface::_MenuUpdate(int16_t delta)
{
    int16_t deltaAbs = (delta >= 0)?(delta):(-delta);
    int16_t deltaSign = (delta >= 0)?(+1):(-1);


    while (deltaAbs > 0)
    {
        Serial.print("deltaAbs:");Serial.print(deltaAbs);
        Serial.print(" deltaSign:");Serial.println(deltaSign);
        Serial.print("inState:");Serial.print(_menuState);


        switch(_menuState)
        {
            case MENU_LOOP_ENABLE:
                if(deltaSign > 0) 
                {   
                    // +1 --> move to next state
                    _menuState = MENU_LOOP_MUTE;
                }
                else 
                {
                    // -1 --> go back to MUTE only if processing loop B
                    if (_loopAB == 1)
                    {
                        _loopAB = 0;
                        _menuState = MENU_LOOP_MUTE;
                    }
                }
                break;

            case MENU_LOOP_MUTE:
                if(deltaSign > 0) 
                {   
                    // +1 --> go to LOOP_ENABLE only if processing loop A, otherwise MIDI OUT
                    if (_loopAB == 0)
                    {
                        _loopAB = 1;
                        _menuState = MENU_LOOP_ENABLE;
                    }
                    else
                    {
                        _midiOutIndx = 0;
                        _menuState = MENU_MIDI_OUT_EN;
                    }
                }
                else 
                {
                    // -1 --> go to LOOP_ENABLE
                    _menuState = MENU_LOOP_ENABLE;
                }
                break;

            case MENU_MIDI_OUT_EN:
                if(deltaSign > 0) 
                {   
                    if(_midiOutEn[_midiOutIndx])
                    {
                        // Message enabled --> move to 
                        _menuState = MENU_MIDI_OUT_TYPE;
                    }
                    else
                    {
                        _midiOutIndx++;
                        if(_midiOutIndx > MAX_MIDI_OUT_VAL)
                        {
                            _menuState = MENU_NAME;
                        }
                    }
                }
                else 
                {
                    if (_midiOutIndx == 0)
                    {
                        // -1 && 1st MIDI OUT message --> go to LOOP_MUTE
                        _menuState = MENU_LOOP_MUTE;
                        _loopAB = 1;
                    }
                    else
                    {
                        // -1 && >1st MIDI OUT message --> go to previous MIDI OUT message
                        _midiOutIndx--;
                        if(_midiOutEn[_midiOutIndx])
                        {
                            _menuState = MENU_MIDI_OUT_VAL;
                        }
                        else
                        {
                            _menuState = MENU_MIDI_OUT_EN;
                        }
                    }
                }

                break;

            case MENU_MIDI_OUT_TYPE:
            case MENU_MIDI_OUT_CHAN:
            case MENU_MIDI_OUT_NUM:
                if(deltaSign > 0) 
                {   
                    _menuState++;
                }
                else
                {
                    _menuState--;
                }
                break;

            case MENU_MIDI_OUT_VAL:
                if(deltaSign > 0) 
                {   
                    _midiOutIndx++;
                    if(_midiOutIndx > MAX_MIDI_OUT_VAL)
                    {
                        _menuState = MENU_NAME;
                    }
                    else
                    {
                        _menuState = MENU_MIDI_OUT_EN;
                    }
                }
                else
                {
                    _menuState--;
                }
                break;

            case MENU_NAME:
                if(deltaSign > 0) 
                {   

                }
                else
                {
                    _midiOutIndx = 3;
                    if(_midiOutEn[_midiOutIndx])
                    {
                        _menuState = MENU_MIDI_OUT_VAL;
                    }
                    else
                    {
                        _menuState = MENU_MIDI_OUT_EN;
                    }
                }

                break;
        }

        Serial.print(" outState:");Serial.println(_menuState);        

        deltaAbs--;
    }
}


char* strNumTabs[] = 
{
    "[1]23456",
    "1[2]3456",
    "12[3]456",
    "123[4]56",
    "1234[5]6",
    "12345[6]"
};

void Interface::_MenuShow(bool clearLcd)
{    
    char lcdLine0[LCD_LINE_LEN + 1];    // Accounts for null
    char lcdLine1[LCD_LINE_LEN + 1];    // Accounts for null

    if(clearLcd)
    {
        _lcd.clear();
    }

    switch(_menuState)
    {          
        case MENU_LOOP_ENABLE:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "Loop %-11s", (_loopAB == 0)?("A"):("B"));
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "Enable%+10s", "++-+");                
            break;

        case MENU_LOOP_MUTE:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "Loop %-11s", (_loopAB == 0)?("A"):("B"));
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "Mute%+12s", "Off");                
            break;
        
        case MENU_MIDI_OUT_EN:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "MIDI Out  %6s", strNumTabs[_midiOutIndx]);
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "#%d Enable%+7s", _midiOutIndx, (_midiOutEn[_midiOutIndx])?("On"):("Off"));                
            break;

        case MENU_MIDI_OUT_TYPE:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "%s", "MIDI Out");
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "#%d Type%+9s", _midiOutIndx, "CC");
            break;

        case MENU_MIDI_OUT_CHAN:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "%s", "MIDI Out");
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "#%d Channel%+6s", _midiOutIndx, "01");
            break;

        case MENU_MIDI_OUT_NUM:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "%s", "MIDI Out");
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "#%d Number%+7s", _midiOutIndx, "012");
            break;

        case MENU_MIDI_OUT_VAL:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "%s", "MIDI Out");
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "#%d Value%+8s", _midiOutIndx, "123");
            break;

        case MENU_NAME:
            snprintf(lcdLine0, LCD_LINE_LEN + 1, "%-16s", "Name");
            snprintf(lcdLine1, LCD_LINE_LEN + 1, "%-16s", "PATCH01");
            break;

    }    

    // Print
    _lcd.setCursor(0, 0);
    _lcd.print(lcdLine0);

    _lcd.setCursor(0, 1);
    _lcd.print(lcdLine1);
}


void Interface::_statePatchEdit(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;
        
        _patchMgr.getActivePatch(&curPatch);
        _MenuInit();
        _MenuShow(true);
        //MenuInit(&_patchEditMenu, &_lcd, curPatch);     // TODO: move away and separate init from reset (i.e. reset indexes to move to beginning)
        // MenuShow(&_patchEditMenu, true);
    }

    if(events.EncDelta != 0)
    {
        _MenuUpdate(events.EncDelta);
        _MenuShow(false);
    }

    // MenuControl(&_patchEditMenu, events);

    if(events.ButtonEnc == BTN_LONG_PRESS)
    {        
        _moveToState(INT_STATE_PATCH_SEL);
    }
}

