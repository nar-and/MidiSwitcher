#ifndef INTERFACE_H
#define INTERFACE_H

#include "UserInput.h"
#include "PatchManager.h"
#include "libs/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

#define     LCD_NUM_LINES       2
#define     LCD_LINE_LEN        16


#define INT_STATE_INIT              0
#define INT_STATE_PATCH_SEL         1
#define INT_STATE_PATCH_SETTINGS    2
#define INT_STATE_GLOBAL_SETTINGS   3


#define MENU_SUBST_VIEW     0
#define MENU_SUBST_EDIT     1

class Interface 
{

public:
    Interface();
    void init(void);
    void refresh(UiEvents_t events);

private:
    bool _onEnter;
    uint8_t _curState;

    void _moveToState(uint8_t state);

    void _statePatchSelect(UiEvents_t events);
    void _statePatchSettings(UiEvents_t events);
    void _stateGlobalSettings(UiEvents_t events);

    void _printPatchInfo(Patch_t patch, bool isActive);
    void _loopEnableToStr(uint8_t loopEnable, int8_t startBit, int8_t stopBit, char* str);

    PatchManager _patchMgr;
    LiquidCrystal_I2C _lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display
    
    // Used to print structured messages with sprintf()
    char _msgString[128];

    void _MenuInit(void);
    void _MenuUpdate(int16_t delta);
    void _MenuShow(bool clearLcd);
    int8_t _MenuFieldUpdate(int8_t delta);
    void _MenuCursorPos(int8_t delta);
    void _printMidiMsg(char* buf, int maxLen, MidiMsg_t msg);


    void _MenuGsInit(void);
    void _MenuGsUpdate(int16_t delta);
    void _MenuGsShow(bool clearLcd);
    int8_t _MenuGsFieldUpdate(int8_t delta);
    void _MenuGsCursorPos(int8_t delta);

    Patch_t _curPatch;

    uint8_t _menuSubstViewEdit;
    int8_t _selPosition;
    int8_t _curPosition;
    bool _isSelected;
    int8_t _writeTarget;

    uint8_t _loopAB;                 // 0 = A, 1 = B
    int8_t _midiOutIndx;
    int8_t _midiInIndx;
    uint8_t _menuState;

    char lcdLine0[LCD_LINE_LEN + 1];    // Accounts for null
    char lcdLine1[LCD_LINE_LEN + 1];    // Accounts for null
};

#endif // INTERFACE_H