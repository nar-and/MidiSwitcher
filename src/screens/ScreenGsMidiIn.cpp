#include "ScreenGsMidiIn.h"
#include "../Display.h"
#include "../Menu.h"
#include "../PatchManager.h"
#include "../InterfaceUtils.h"

#define RX_CHAN_STARTPOS             13

void ScreenGsMidiIn::show(void)
{
    Display* lcd = _parent->_lcd;
    GlobalSettings_t* gs = &_parent->_owner->_globalSettings;

    snprintf(lcd->line0, LCD_LINE_LEN + 1, "MIDI In         ");

    char chanStr[4];
    printMidiInChan(chanStr, 4, gs->midiInChannel);
    snprintf(lcd->line1, LCD_LINE_LEN + 1, "RX Chan.%+8s", chanStr);
}

void ScreenGsMidiIn::updateCursor(int16_t delta)
{
    _parent->_selPosition = 0;
    _parent->_curPosition = RX_CHAN_STARTPOS;
}

void ScreenGsMidiIn::updateValue(int16_t delta)
{
    Display* lcd = _parent->_lcd;
    GlobalSettings_t* gs = &_parent->_owner->_globalSettings;

    int8_t chan = gs->midiInChannel + delta;
    gs->midiInChannel = constrain(chan, 0, MIDI_IN_CHAN_OMNI);

    char chanStr[4];
    printMidiInChan(chanStr, 4, gs->midiInChannel);
    lcd->print(chanStr);
    lcd->setCursor(_parent->_curPosition, 1);
}

