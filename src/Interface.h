#ifndef INTERFACE_H
#define INTERFACE_H

#include "UserInput.h"
#include "PatchManager.h"
#include "libs/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

#define INT_STATE_INIT              0
#define INT_STATE_PATCH_SEL         1
#define INT_STATE_PATCH_EDIT        2
#define INT_STATE_GLOBAL_SETTINGS   3

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
    void _statePatchEdit(UiEvents_t events);
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

};

#endif // INTERFACE_H