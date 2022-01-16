#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "UserInput.h"
#include "PatchManager.h"
#include "Display.h"
#include "Menu.h"


// Interface states definition
#define INT_STATE_PATCH_SEL         0
#define INT_STATE_PATCH_SETTINGS    1
#define INT_STATE_GLOBAL_SETTINGS   2

// Menu modes (VIEW/EDIT)
#define MENU_MODE_VIEW              0
#define MENU_MODE_EDIT              1

// Global settings menu screen IDs
#define MENU_GS_MIDI_IN                 0
#define MENU_GS_FACTORY_RESET           1

// Patch settings menu screen IDs
#define MENU_PS_LOOP_A_ENABLE           0
#define MENU_PS_LOOP_A_MUTE             1
#define MENU_PS_LOOP_B_ENABLE           2
#define MENU_PS_LOOP_B_MUTE             3
#define MENU_PS_MIDI_OUT                4  
#define MENU_PS_MIDI_IN                 5
#define MENU_PS_NAME                    6
#define MENU_PS_WRITE_COPY              7
#define MENU_PS_WRITE_CONFIRM           8

// Loop A/B identifier
#define LOOP_ID_A                       0
#define LOOP_ID_B                       1

// MIDI Out/In identifier
#define MIDI_DIR_OUT                    0
#define MIDI_DIR_IN                     1

class UserInterface 
{
public:
    UserInterface();
    void init(PatchManager* pm);
    void refresh(UiEvents_t events);

    Patch_t             _curPatch;
    GlobalSettings_t    _globalSettings;
    int8_t          _writeTarget;
    PatchManager*   _patchMgr;
    Display         _lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display

private:
    // State management methods & members
    void _moveToState(uint8_t state);   // Move to destination state
    bool    _onEnter;                   // True on state enter, must be set to false by state
    uint8_t _curState;                  // Identifies current state

    // State handlers
    void _statePatchSelect(UiEvents_t events);
    void _statePatchSettings(UiEvents_t events);
    void _stateGlobalSettings(UiEvents_t events);

    // Helpers
    void _printPatchInfo(Patch_t patch, bool isActive);
   
    // Menu management
    uint8_t _menuMode;
    Menu* _globalSettingsMenu;          // Global settings menu management
    Menu* _patchSettingsMenu;           // Global settings menu management
};

#endif // USER_INTERFACE_H