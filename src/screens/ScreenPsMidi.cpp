#include "ScreenPsMidi.h"
#include "../Display.h"
#include "../Menu.h"
#include "../PatchManager.h"
#include "../InterfaceUtils.h"

ScreenPsMidi::ScreenPsMidi(uint8_t dir)
{
    _midiDir = dir;
}

void ScreenPsMidi::show(void)
{
    Display* lcd = _parent->_lcd;
    Patch_t* curPatch = &_parent->_owner->_curPatch;

    char tabStr[9];
    getTabStr(_curTab, _numTabs, tabStr);

    snprintf(lcd->line0, LCD_LINE_LEN + 1, "MIDI %-3s%+8s", (_midiDir == MIDI_DIR_OUT)?("Out"):("In "), tabStr);
    //snprintf(_lcd.line0, LCD_LINE_LEN + 1, "%02d| MIDI Out [%d]", curPatch.num, _midiOutIndx);         

    MidiMsg_t msg = (_midiDir == MIDI_DIR_OUT)?
                            (curPatch->midiOut[_curTab]):
                            (curPatch->midiIn[_curTab]);
    printMidiMsg(lcd->line1, LCD_LINE_LEN + 1, msg);
}

void ScreenPsMidi::updateCursor(int16_t delta)
{
    int8_t fieldPos;                
    int8_t midiCurPos[] = {0, 3, 7, 12};
    Patch_t curPatch = _parent->_owner->_curPatch;

    _parent->_selPosition += delta;

    MidiMsg_t msg = (_midiDir == MIDI_DIR_OUT)?
                            (curPatch.midiOut[_curTab]):
                            (curPatch.midiIn[_curTab]);

    if(msg.type == MIDI_TYPE_NONE)
    {
        _parent->_selPosition = 0;
    }
    else if (msg.type == MIDI_TYPE_PC)
    {
        // _selPosition = constrain(_selPosition, 0, 2);
        _parent->_selPosition = wrap(_parent->_selPosition, 0, 2);
    }
    else if (msg.type == MIDI_TYPE_CC)
    {
        // _selPosition = constrain(_selPosition, 0, 3);
        _parent->_selPosition = wrap(_parent->_selPosition, 0, 3);
    }
    
    _parent->_curPosition = midiCurPos[_parent->_selPosition];
}

void ScreenPsMidi::updateValue(int16_t delta)
{
    Display* lcd = _parent->_lcd;
    Patch_t* curPatch = &_parent->_owner->_curPatch;

    MidiMsg_t msg = (_midiDir == MIDI_DIR_OUT)?
                            (curPatch->midiOut[_curTab]):
                            (curPatch->midiIn[_curTab]);

    switch(_parent->_selPosition)
    {
        case 0:
            {
                int8_t type = msg.type + delta;
                msg.type = constrain(type, MIDI_TYPE_NONE, MIDI_TYPE_CC);
            }
            break;
        case 1:
            {
                int8_t chan = msg.chan + delta;
                // tempMsg.chan = constrain(chan, 0, 15);
                msg.chan = wrap(chan, 0, 15);
            }
            break;
        case 2:
            {
                int8_t num = msg.num + delta;
                // tempMsg.num = constrain(num, 0, 127);
                msg.num = wrap(num, 0, 127);
            }
            break;
        case 3:
            {
                int8_t val = msg.val + delta;
                //tempMsg.val = constrain(val, 0, 127);
                msg.val = wrap(val, 0, 127);
            }
            break;
    }

    // Write back MIDI data to patch
    if(_midiDir == MIDI_DIR_OUT)
    {
        curPatch->midiOut[_curTab] = msg;
    }
    else
    {
        curPatch->midiIn[_curTab] = msg;
    }
    
    printMidiMsg(lcd->line1, LCD_LINE_LEN + 1, msg);
    lcd->setCursor(0, 1);
    lcd->print(lcd->line1);
    lcd->setCursor(_parent->_curPosition, 1);
}


