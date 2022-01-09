#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "UserInput.h"
#include "PatchManager.h"
#include "Display.h"
#include "Menu.h"



#define INT_STATE_PATCH_SEL         0
#define INT_STATE_PATCH_SETTINGS    1
#define INT_STATE_GLOBAL_SETTINGS   2


#define MENU_SUBST_VIEW     0
#define MENU_SUBST_EDIT     1



#define MENU_GS_MIDI_IN             0
#define MENU_GS_FACTORY_RESET       1


class UserInterface 
{
public:
    UserInterface();
    void init(PatchManager* pm);
    void refresh(UiEvents_t events);

private:
    void _moveToState(uint8_t state);
    
    void _statePatchSelect(UiEvents_t events);
    void _statePatchSettings(UiEvents_t events);
    void _stateGlobalSettings(UiEvents_t events);

    void _printPatchInfo(Patch_t patch, bool isActive);
    void _loopEnableToStr(uint8_t loopEnable, int8_t startBit, int8_t stopBit, char* str);

    PatchManager* _patchMgr;
    Display _lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display
    bool    _onEnter;
    uint8_t _curState;
    
    // Used to print structured messages with sprintf()
    char _msgString[128];

    void _MenuInit(void);
    void _MenuUpdate(int16_t delta);
    void _MenuShow(bool clearLcd);
    int8_t _MenuFieldUpdate(int8_t delta);
    void _MenuCursorPos(int8_t delta);
    void _printMidiMsg(char* buf, int maxLen, MidiMsg_t msg);

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


    Menu* _globalSettingsMenu;
};

#endif // USER_INTERFACE_H