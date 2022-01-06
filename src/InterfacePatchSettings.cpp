#include "Interface.h"
#include <stdio.h>
#include <Arduino.h>



#define LOOPA_EN_STARTPOS           12
#define LOOPB_EN_STARTPOS           14
#define ON_OFF_STARTPOS             13
#define NAME_STARTPOS               3
#define CONFIRM_STARTPOS            3

#define MAX_LOOP_VAL                1

#define MENU_RESET                  0
#define MENU_LOOP_ENABLE            1
#define MENU_LOOP_MUTE              2
#define MENU_MIDI_OUT               3  
#define MENU_MIDI_IN                4
#define MENU_NAME                   5
#define MENU_WRITE_COPY             6
#define MENU_WRITE_CONFIRM          7


// Inspired by https://github.com/semibran/wrap-around
// min = range minimum value (included)
// max = range maximum value (included)
// Take care about having max >= min otherwise results are not meaningful!
int wrap(int val, int min, int max)
{
	int val0 = val - min;
	int max0 = max + 1 - min;
	
	return min + ((val0 >= 0) ? 
				  (val0 % max0) : 
				  ((val0 % max0 + max0) % max0));
}

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
                    else
                    {
                        // Do nothing - first entry
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
                        _menuState = MENU_MIDI_OUT;
                    }
                }
                else 
                {
                    // -1 --> go to LOOP_ENABLE
                    _menuState = MENU_LOOP_ENABLE;
                }
                break;

            case MENU_MIDI_OUT:
                if(deltaSign > 0) 
                {   
                    _midiOutIndx++;
                    if(_midiOutIndx >= MAX_NUM_MIDI_OUT)
                    {
                        _midiInIndx = 0;
                        _menuState = MENU_MIDI_IN;
                    }
                }
                else 
                {
                    _midiOutIndx--;
                    if (_midiOutIndx < 0)
                    {
                        // -1 && 1st MIDI OUT message --> go to LOOP_MUTE
                        _loopAB = MAX_LOOP_VAL;
                        _menuState = MENU_LOOP_MUTE;
                    }
                }
                break;

            case MENU_MIDI_IN:
                if(deltaSign > 0) 
                {   
                    _midiInIndx++;
                    if(_midiInIndx >= MAX_NUM_MIDI_IN)
                    {
                        _menuState = MENU_NAME;
                    }
                }
                else 
                {
                    _midiInIndx--;
                    if (_midiInIndx < 0)
                    {
                        // -1 && 1st MIDI IN message --> go to MIDI_OUT
                        _midiOutIndx = (MAX_NUM_MIDI_OUT - 1);
                        _menuState = MENU_MIDI_OUT;
                    }
                }
                break;

            case MENU_NAME:
                if(deltaSign > 0) 
                {   
                    _menuState = MENU_WRITE_COPY;
                }
                else
                {
                    _midiInIndx = (MAX_NUM_MIDI_IN - 1);
                    _menuState = MENU_MIDI_IN;
                }

                break;

            case MENU_WRITE_COPY:
                if(deltaSign > 0) 
                {   
                    _menuState = MENU_WRITE_CONFIRM;
                }
                else
                {
                    _menuState = MENU_NAME;
                }

                break;

            case MENU_WRITE_CONFIRM:
                if(deltaSign > 0) 
                {   
                    // Do nothing - last entry
                }
                else
                {
                    _menuState = MENU_WRITE_COPY;
                }

                break;                

        }

        Serial.print(" outState:");Serial.println(_menuState);        

        deltaAbs--;
    }
}


const char* strNumTabs[] = 
{
    "[1]23456",
    "1[2]3456",
    "12[3]456",
    "123[4]56",
    "1234[5]6",
    "12345[6]"
};

void Interface::_printMidiMsg(char* buf, int maxLen, MidiMsg_t msg)
{
    if(msg.type == MIDI_TYPE_NONE)
    {
        snprintf(buf, maxLen, "%-16s", "Off");
    }
    else if (msg.type == MIDI_TYPE_PC)
    {
        //snprintf(buf, maxLen, "PC#%03d      Ch%02d", msg.num, msg.chan);
        snprintf(buf, maxLen, "PC C%02d #%03d     ", msg.chan, msg.num);
    }
    else if (msg.type == MIDI_TYPE_CC)
    {
        //snprintf(buf, maxLen, "CC#%03d V%03d Ch%02d", msg.num, msg.val, msg.chan);
        snprintf(buf, maxLen, "CC C%02d #%03d V%03d", msg.chan, msg.num, msg.val);
    }
}

void Interface::_MenuShow(bool clearLcd)
{    

    if(clearLcd)
    {
        _lcd.clear();
    }

    unsigned long curTime = micros();

    switch(_menuState)
    {          
        case MENU_LOOP_ENABLE:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "Loop %-11s", (_loopAB == 0)?("A"):("B"));
                //snprintf(lcdLine0, LCD_LINE_LEN + 1, "%02d| Loop %-7s", curPatch.num, (_loopAB == 0)?("A"):("B"));
                char loopEnStr[5];
                int8_t startBit =  (_loopAB == 0)?(3):(6);
                int8_t stopBit =  (_loopAB == 0)?(0):(5);
                _loopEnableToStr(_curPatch.loopEnable, startBit, stopBit, loopEnStr);
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "Enable%+10s", loopEnStr);
            }
            break;

        case MENU_LOOP_MUTE:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "Loop %-11s", (_loopAB == 0)?("A"):("B"));
                //snprintf(lcdLine0, LCD_LINE_LEN + 1, "%02d| Loop %-7s", curPatch.num, (_loopAB == 0)?("A"):("B"));
                bool loopMuted = (_loopAB == 0)?(_curPatch.loopEnable & LOOPA_MUTE):(_curPatch.loopEnable & LOOPB_MUTE);
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "Mute%+12s", (loopMuted)?("On "):("Off"));
            }
            break;
        
        case MENU_MIDI_OUT:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "MIDI Out  %6s", strNumTabs[_midiOutIndx]);
                //snprintf(lcdLine0, LCD_LINE_LEN + 1, "%02d| MIDI Out [%d]", curPatch.num, _midiOutIndx);            
                _printMidiMsg(lcdLine1, LCD_LINE_LEN + 1, _curPatch.midiOut[_midiOutIndx]);
            }
            break;

        case MENU_MIDI_IN:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "MIDI In %8s", strNumTabs[_midiInIndx]);
                //snprintf(lcdLine0, LCD_LINE_LEN + 1, "%02d| MIDI In  [%d]", curPatch.num, _midiInIndx);
                _printMidiMsg(lcdLine1, LCD_LINE_LEN + 1, _curPatch.midiIn[_midiInIndx]);
            }
            break;

        case MENU_NAME:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "%-16s", "Name");
                //snprintf(lcdLine0, LCD_LINE_LEN + 1, "%02d| %12s", curPatch.num, "Name");
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "  [%-12s]", _curPatch.name);
            }
            break;

        case MENU_WRITE_COPY:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "%-16s", "Write/Copy to");
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "#%02d %-12s", _curPatch.num, _curPatch.name);
            }
            break;

        case MENU_WRITE_CONFIRM:
            {
                snprintf(lcdLine0, LCD_LINE_LEN + 1, "|Confirm write?|");
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "|NO>        YES|");
            }
            break;
    }    

    unsigned long elapsed = micros() - curTime;
    Serial.print("EL1:"); Serial.print(elapsed);

    // Print to screen
    curTime = micros();
    _lcd.setCursor(0, 0);
    _lcd.print(lcdLine0);

    elapsed = micros() - curTime;
    Serial.print(" EL2:"); Serial.print(elapsed);

    curTime = micros();
    _lcd.setCursor(0, 1);
    _lcd.print(lcdLine1);
    elapsed = micros() - curTime;
    Serial.print(" EL3:"); Serial.println(elapsed);
}



void Interface::_MenuCursorPos(int8_t delta)
{
    _selPosition += delta;

    switch(_menuState)
    {          
        case MENU_LOOP_ENABLE:
            {
                // _selPosition = constrain(_selPosition, 0, (_loopAB == 0)?(3):(1));
                _selPosition = wrap(_selPosition, 0, (_loopAB == 0)?(3):(1));
                Serial.print("SELPOS");Serial.println(_selPosition);
                _curPosition = _selPosition + ((_loopAB == 0)?(LOOPA_EN_STARTPOS):(LOOPB_EN_STARTPOS));
            }
            break;

        case MENU_LOOP_MUTE:
            {
                _selPosition = 0;
                _curPosition = ON_OFF_STARTPOS;
            }
            break;
        
        case MENU_MIDI_OUT:
        case MENU_MIDI_IN:        
            {
                int8_t fieldPos;                
                int8_t midiCurPos[] = {0, 3, 7, 12};


                MidiMsg_t msg = (_menuState == MENU_MIDI_OUT)?
                                (_curPatch.midiOut[_midiOutIndx]):
                                (_curPatch.midiIn[_midiInIndx]);

                if(msg.type == MIDI_TYPE_NONE)
                {
                    _selPosition = 0;
                }
                else if (msg.type == MIDI_TYPE_PC)
                {
                    // _selPosition = constrain(_selPosition, 0, 2);
                    _selPosition = wrap(_selPosition, 0, 2);
                }
                else if (msg.type == MIDI_TYPE_CC)
                {
                    // _selPosition = constrain(_selPosition, 0, 3);
                    _selPosition = wrap(_selPosition, 0, 3);
                }
                
                _curPosition = midiCurPos[_selPosition];
            }
            break;

        case MENU_NAME:
            {
                //_selPosition = constrain(_selPosition, 0, PATCH_NAME_LEN - 1);
                _selPosition = wrap(_selPosition, 0, PATCH_NAME_LEN - 1);
                _curPosition = _selPosition + NAME_STARTPOS;
            }
            break;

        case MENU_WRITE_COPY:
            {
                _selPosition = 0;
                _curPosition = 0;
            }
            break;

        case MENU_WRITE_CONFIRM:
            {
                _selPosition = constrain(_selPosition, -1, 9);
                _curPosition = _selPosition + CONFIRM_STARTPOS;
            }
            break;            
    }

    Serial.print("CURPOS");Serial.println(_curPosition);

    return;
}


int8_t Interface::_MenuFieldUpdate(int8_t delta)
{
    switch(_menuState)
    {          
        case MENU_LOOP_ENABLE:
            {
                bool loopEnabled = (delta > 0);              
                
                // Change character shown
                _lcd.print((loopEnabled) ? ("+"):("-"));
                _lcd.setCursor(_curPosition, 1);
            }
            break;

        case MENU_LOOP_MUTE:
            {
                bool isOn = (delta > 0);
                
                // Change character shown
                _lcd.print((isOn) ? ("On "):("Off"));
                _lcd.setCursor(_curPosition, 1);
            }
            break;
        
        case MENU_MIDI_OUT:
        case MENU_MIDI_IN:        
            {
                MidiMsg_t tempMsg = (_menuState == MENU_MIDI_OUT)?
                                (_curPatch.midiOut[_midiOutIndx]):
                                (_curPatch.midiIn[_midiInIndx]);

                switch(_selPosition)
                {
                    case 0:
                        {
                            int8_t type = tempMsg.type + delta;
                            tempMsg.type = constrain(type, MIDI_TYPE_NONE, MIDI_TYPE_CC);
                        }
                        break;
                    case 1:
                        {
                            int8_t chan = tempMsg.chan + delta;
                            // tempMsg.chan = constrain(chan, 0, 15);
                            tempMsg.chan = wrap(chan, 0, 15);
                        }
                        break;
                    case 2:
                        {
                            int8_t num = tempMsg.num + delta;
                            // tempMsg.num = constrain(num, 0, 127);
                            tempMsg.num = wrap(num, 0, 127);
                        }
                        break;
                    case 3:
                        {
                            int8_t val = tempMsg.val + delta;
                            //tempMsg.val = constrain(val, 0, 127);
                            tempMsg.val = wrap(val, 0, 127);
                        }
                        break;
                }

                _curPatch.midiOut[_midiOutIndx] = tempMsg;
                _printMidiMsg(lcdLine1, LCD_LINE_LEN + 1, tempMsg);
                _lcd.setCursor(0, 1);
                _lcd.print(lcdLine1);
                _lcd.setCursor(_curPosition, 1);
            }
            break;

        case MENU_NAME:
            {
                int16_t newLetter = _curPatch.name[_selPosition] + delta;

                // newLetter = constrain(newLetter, 32, 127);
                newLetter = wrap(newLetter, 32, 127);
                _curPatch.name[_selPosition] = (char)newLetter;

                _lcd.print(_curPatch.name[_selPosition]);
                _lcd.setCursor(_curPosition, 1);
            }
            break;

        case MENU_WRITE_COPY:
            {
                Serial.print("WT1:");Serial.print(_writeTarget);

                if(_writeTarget < 0) _writeTarget = _curPatch.num;

                Serial.print(" WT2:");Serial.print(_writeTarget);
                _writeTarget += delta;

                Serial.print(" WT3:");Serial.print(_writeTarget);
                _writeTarget = wrap(_writeTarget, 0, PATCH_LIBRARY_LEN - 1);

                Serial.print(" WT4:");Serial.print(_writeTarget);

                char wtName[PATCH_NAME_LEN + 1];
                _patchMgr.getPatchName(_writeTarget, wtName);

                Serial.print(" WT5:");Serial.println(wtName);

                _lcd.setCursor(0, 1);
                snprintf(lcdLine1, LCD_LINE_LEN + 1, "#%02d %-12s", _writeTarget, wtName);
                _lcd.print(lcdLine1);
                _lcd.setCursor(_curPosition, 1);
            }
            break;

        case MENU_WRITE_CONFIRM:
            {
                if(delta < 0)
                {
                    // Cancel operation
                    while(delta < 0)
                    {
                        Serial.print("DELTA");Serial.print(delta);
                        Serial.print(" SEL1");Serial.print(_selPosition);
                        Serial.print(" CUR1");Serial.print(_curPosition);

                        if((_selPosition >= 0) && (_selPosition <= 8))
                        {
                            _lcd.print(" ");
                        }

                        _MenuCursorPos(-1);
                        Serial.print(" SEL2");Serial.print(_selPosition);
                        Serial.print(" CUR2");Serial.println(_curPosition);

                        _lcd.setCursor(_curPosition, 1);
                        delta++;

                        if(_selPosition < 0)
                        {
                            Serial.println("**** EXIT NO ****");
                        }
                    }

                }
                else
                {
                    while(delta > 0)
                    {
                        Serial.print("DELTA");Serial.print(delta);
                        Serial.print(" SEL1");Serial.print(_selPosition);
                        Serial.print(" CUR1");Serial.print(_curPosition);
                        _MenuCursorPos(1);
                        Serial.print(" SEL2");Serial.print(_selPosition);
                        Serial.print(" CUR2");Serial.println(_curPosition);
                        _lcd.setCursor(_curPosition, 1);

                        if(_selPosition <= 8)
                        {
                            _lcd.print(">");
                            _lcd.setCursor(_curPosition, 1);
                        }
                        else
                        {
                            Serial.println("**** EXIT YES ****");
                        }
                        delta--;
                    }
                }
            }
            break;
    }

    return 0;
}

#define NAVI_STYLE      1

void Interface::_statePatchSettings(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;
        _menuSubstViewEdit = MENU_SUBST_VIEW;
        _patchMgr.getActivePatch(&_curPatch);
        _MenuInit();
        _MenuShow(true);

    }

    if(_menuSubstViewEdit == MENU_SUBST_VIEW)
    {
        if(events.EncDelta != 0)
        {
            _MenuUpdate(events.EncDelta);
            _MenuShow(false);
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // Move to EDIT substate
            Serial.println("EDIT");
            _menuSubstViewEdit = MENU_SUBST_EDIT;
            _writeTarget = -1;
            _selPosition = 0;

#if (NAVI_STYLE == 0)
            _isSelected = false;
#else
            _isSelected = true;
#endif            
            _MenuCursorPos(0);
            _lcd.setCursor(_curPosition, 1);
            // _lcd.cursor();
            _lcd.blink();
        }

        if(events.ButtonEnc == BTN_LONG_PRESS)
        {        
            _moveToState(INT_STATE_PATCH_SEL);        
        }
    }
    else
    {

#if (NAVI_STYLE == 0)

        if(_isSelected == false)
        {
            // Move around
            if(events.EncDelta != 0)
            {
                Serial.println("MOVE");
                _MenuCursorPos(events.EncDelta);
                _lcd.setCursor(_curPosition, 1);
            }
        }
        else
        {
            if(events.EncDelta != 0)
            {
                Serial.println("UPDATE");            
                _MenuFieldUpdate(events.EncDelta);
            }
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // Toggle selection state
            _isSelected = !_isSelected;
            Serial.print("SELECTED");Serial.println(_isSelected);
        }

#else
        if(_menuState != MENU_WRITE_CONFIRM)
        {
            if(events.ButtonEnc == BTN_CLICK)
            {
                Serial.println("MOVE");
                _MenuCursorPos(1);
                _lcd.setCursor(_curPosition, 1);
            }

            if(events.EncDelta != 0)
            {
                Serial.println("UPDATE");            
                _MenuFieldUpdate(events.EncDelta);
            }
        }
        else
        {
            if(events.EncDelta != 0)
            {
                Serial.println("UPDATE"); 
                _MenuFieldUpdate(events.EncDelta);
            }
        }
#endif
        if(events.ButtonEnc == BTN_LONG_PRESS)
        {        
            Serial.println("VIEW");
            _menuSubstViewEdit = MENU_SUBST_VIEW;
            // _lcd.noCursor();
            _lcd.noBlink();
        }
    }
}




#if 0
#define NAME_CURSOR_START           4
#define LOOPA_CURSOR_START          5
#define LOOPB_CURSOR_START          11
#define MIDI_OUT_CURSOR_START       15


#define TEST    0

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

void Interface::_statePatchSettings(UiEvents_t events)
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
