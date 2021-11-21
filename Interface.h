#ifndef INTERFACE_H
#define INTERFACE_H

#include "UserInput.h"
#include "PatchManager.h"
#include <LiquidCrystal_I2C.h>

#define INT_STATE_INIT              0
#define INT_STATE_PATCH_SEL         1
#define INT_STATE_PATCH_EDIT        2
#define INT_STATE_GLOBAL_SETTINGS   3


typedef struct
{
    int8_t level;
    char name[16];
} MenuItem_t;

typedef struct
{
    int8_t      curLevel;
    int8_t      selItem;
    int8_t      firstItemShown;
    int8_t      numItems;
    MenuItem_t  items[];
} Menu_t;

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


    void _showEditMenu(Menu_t* menu);

    int16_t _selectedPatchIndx;
    int16_t _activePatchIndx;
    PatchManager _patchMgr;
    LiquidCrystal_I2C _lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display

    // Used to print structured messages with sprintf()
    char _msgString[128];
};

#endif // INTERFACE_H