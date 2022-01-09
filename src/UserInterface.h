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

#define MENU_PS_RESET                   0
#define MENU_PS_LOOP_A_ENABLE           1
#define MENU_PS_LOOP_A_MUTE             2
#define MENU_PS_LOOP_B_ENABLE           3
#define MENU_PS_LOOP_B_MUTE             4
#define MENU_PS_MIDI_OUT                5  
#define MENU_PS_MIDI_IN                 6
#define MENU_PS_NAME                    7
#define MENU_PS_WRITE_COPY              8
#define MENU_PS_WRITE_CONFIRM           9

#define LOOP_ID_A                       0
#define LOOP_ID_B                       1

#define MIDI_DIR_OUT                    0
#define MIDI_DIR_IN                     1

class UserInterface 
{
public:
    UserInterface();
    void init(PatchManager* pm);
    void refresh(UiEvents_t events);

    Patch_t _curPatch;
    int8_t _writeTarget;
    PatchManager* _patchMgr;
    Display _lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display

private:
    void _moveToState(uint8_t state);
    
    void _statePatchSelect(UiEvents_t events);
    void _statePatchSettings(UiEvents_t events);
    void _stateGlobalSettings(UiEvents_t events);

    void _printPatchInfo(Patch_t patch, bool isActive);

    bool    _onEnter;
    uint8_t _curState;
    
    // Used to print structured messages with sprintf()
    char _msgString[128];

    uint8_t _menuSubstViewEdit;

    Menu* _globalSettingsMenu;
    Menu* _patchSettingsMenu;
};

#endif // USER_INTERFACE_H