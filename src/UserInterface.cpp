#include "UserInterface.h"
#include "PatchManager.h"
#include <Arduino.h>
#include "screens/ScreenGsMidiIn.h"
#include "screens/ScreenGsFactoryReset.h"
#include "screens/ScreenPsLoopEn.h"
#include "screens/ScreenPsLoopMute.h"
#include "screens/ScreenPsMidi.h"
#include "screens/ScreenPsName.h"
#include "screens/ScreenPsWriteCopy.h"
#include "screens/ScreenPsWriteConfirm.h"
#include "Menu.h"
#include "InterfaceUtils.h"

#define DEBUG_PRINT_INTERFACE       1

/**
 * @brief Construct a new User Interface:: User Interface object
 * 
 */
UserInterface::UserInterface() : 
    _lcd(LCD_ADDRESS, LCD_LINE_LEN, LCD_NUM_LINES)
{
    _globalSettingsMenu = new Menu(this);
    _patchSettingsMenu = new Menu(this);
}

/**
 * @brief 
 * 
 * @param pm 
 */
void UserInterface::init(PatchManager* pm)
{
    _patchMgr = pm;

    // TODO: move out the Wire initialization!!
    Wire.begin();
    Wire.setClock(400000);

    // Initialize LCD & turn on the backlight
    _lcd.init();
    _lcd.clear();
    _lcd.backlight();

    // Initialize global settings menu
    _globalSettingsMenu->registerScreen(MENU_GS_MIDI_IN, new ScreenGsMidiIn());
    _globalSettingsMenu->registerScreen(MENU_GS_FACTORY_RESET, new ScreenGsFactoryReset());

    // Initialize patch settings menu
    _patchSettingsMenu->registerScreen(MENU_PS_LOOP_A_ENABLE, new ScreenPsLoopEn(LOOP_ID_A));
    _patchSettingsMenu->registerScreen(MENU_PS_LOOP_A_MUTE, new ScreenPsLoopMute(LOOP_ID_A));
    _patchSettingsMenu->registerScreen(MENU_PS_LOOP_B_ENABLE, new ScreenPsLoopEn(LOOP_ID_B));
    _patchSettingsMenu->registerScreen(MENU_PS_LOOP_B_MUTE, new ScreenPsLoopMute(LOOP_ID_B));
    _patchSettingsMenu->registerScreen(MENU_PS_MIDI_OUT, new ScreenPsMidi(MIDI_DIR_OUT), 4);
    _patchSettingsMenu->registerScreen(MENU_PS_MIDI_IN, new ScreenPsMidi(MIDI_DIR_IN), 6);
    _patchSettingsMenu->registerScreen(MENU_PS_NAME, new ScreenPsName());
    _patchSettingsMenu->registerScreen(MENU_PS_WRITE_COPY, new ScreenPsWriteCopy());
    _patchSettingsMenu->registerScreen(MENU_PS_WRITE_CONFIRM, new ScreenPsWriteConfirm());

    // Initialize state book-keeping variables
    _onEnter = false;

    // Update screen with currently active patch

    Serial.println("START");
    _moveToState(INT_STATE_PATCH_SEL);
}

/**
 * @brief 
 * 
 * @param events 
 */
void UserInterface::refresh(UiEvents_t events)
{
    switch(_curState)
    {
        case INT_STATE_PATCH_SEL:
            _statePatchSelect(events);
            break;

        case INT_STATE_PATCH_SETTINGS:
            _statePatchSettings(events);
            break;

        case INT_STATE_GLOBAL_SETTINGS:
            _stateGlobalSettings(events);
            break;

        default:
            break;
    }
}

/**
 * @brief 
 * 
 * @param state 
 */
void UserInterface::_moveToState(uint8_t state)
{
    _curState = state;
    _onEnter = true;
}


void UserInterface::_statePatchSelect(UiEvents_t events)
{
    Patch_t patch;

    if(_onEnter)
    {
        _onEnter = false;
        _patchMgr->getSelectedPatch(&patch);
        _printPatchInfo(patch, _patchMgr->isSelActive());
    }

    if(events.EncDelta != 0)
    {           
        // Read new patch
        _patchMgr->updateSelection(events.EncDelta);
        _patchMgr->getSelectedPatch(&patch);
        _printPatchInfo(patch, _patchMgr->isSelActive());
    }

    if(events.ButtonEnc == BTN_CLICK)
    {
        // Activate selected patch (if different than currently active patch)
        if(_patchMgr->isSelActive() == false)
        {
            _patchMgr->activateSelectedPatch();
            _patchMgr->getActivePatch(&patch);
            _printPatchInfo(patch, _patchMgr->isSelActive());
        }
    }

    if(events.ButtonEnc == BTN_LONG)
    {
        if(_patchMgr->isSelActive())
        {   
            // Enable edit mode only if currently shown patch is the active one
            _moveToState(INT_STATE_PATCH_SETTINGS);
        }
        else
        {   
            //... otherwise move to global settings
            _moveToState(INT_STATE_GLOBAL_SETTINGS);
        }
    }
}

void UserInterface::_printPatchInfo(Patch_t patch, bool isActive)
{
    sprintf(_lcd.line0, "%02d| %-12s", patch.num, patch.name);

    char tempA[5];
    char tempB[3];
    loopEnableToStr(patch.loopEnable, 3, 0, tempA);
    loopEnableToStr(patch.loopEnable, 6, 5, tempB);
    snprintf(_lcd.line1, LCD_LINE_LEN + 1, "%s| A%s B%s M0", (isActive)?(" \x7E"):("  "), tempA, tempB);

    _lcd.setCursor(0, 0);
    _lcd.print(_lcd.line0);

    _lcd.setCursor(0, 1);
    _lcd.print(_lcd.line1);    

#if DEBUG_PRINT_INTERFACE    

    Serial.print((isActive)?("->"):("  "));
    Serial.print(patch.name);

    Serial.print(" A");
    Serial.print(tempA);

    Serial.print(" B");
    Serial.print(tempB);

    Serial.print(" M");
//    Serial.print(tempC);

    Serial.println();
#endif
}


void UserInterface::_statePatchSettings(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;
        _patchMgr->getActivePatch(&_curPatch);

        _menuSubstViewEdit = MENU_SUBST_VIEW;

        _patchSettingsMenu->setScreen(MENU_PS_LOOP_A_ENABLE);
        _lcd.clear();
        _patchSettingsMenu->show();
    }

    if(_menuSubstViewEdit == MENU_SUBST_VIEW)
    {
        if(events.EncDelta != 0)
        {
            _patchSettingsMenu->updateScreen(events.EncDelta);
            _patchSettingsMenu->show();
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // Move to EDIT substate
            Serial.println("EDIT");

            _writeTarget = -1;
            _menuSubstViewEdit = MENU_SUBST_EDIT;
            _patchSettingsMenu->_selPosition = 0;

            _patchSettingsMenu->updateCursor(0);
            _lcd.setCursor(_patchSettingsMenu->_curPosition, 1);
            _lcd.blink();
        }

        if(events.ButtonEnc == BTN_LONG)
        {   
            _moveToState(INT_STATE_PATCH_SEL);        
        }
    }
    else
    {
        if(events.ButtonEnc == BTN_CLICK)
        {
            Serial.println("MOVE");
            _patchSettingsMenu->updateCursor(1);
            _lcd.setCursor(_patchSettingsMenu->_curPosition, 1);
        }

        if(events.EncDelta != 0)
        {
            Serial.println("UPDATE");  
            _patchSettingsMenu->updateValue(events.EncDelta);
        }

        if(events.ButtonEnc == BTN_LONG)
        {        
            Serial.println("VIEW");
            _menuSubstViewEdit = MENU_SUBST_VIEW;
            _lcd.noBlink();
        }
    }
}



void UserInterface::_stateGlobalSettings(UiEvents_t events)
{
    if(_onEnter)
    {
        _onEnter = false;
        _menuSubstViewEdit = MENU_SUBST_VIEW;

        _globalSettingsMenu->setScreen(MENU_GS_MIDI_IN);
        _lcd.clear();
        _globalSettingsMenu->show();
    }

    if(_menuSubstViewEdit == MENU_SUBST_VIEW)
    {
        if(events.EncDelta != 0)
        {
            _globalSettingsMenu->updateScreen(events.EncDelta);
            _globalSettingsMenu->show();
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // Move to EDIT substate
            Serial.println("EDIT");
            _menuSubstViewEdit = MENU_SUBST_EDIT;
            _globalSettingsMenu->_selPosition = 0;

            _globalSettingsMenu->updateCursor(0);
            _lcd.setCursor(_globalSettingsMenu->_curPosition, 1);
            _lcd.blink();
        }

        if(events.ButtonEnc == BTN_LONG)
        {   
            _moveToState(INT_STATE_PATCH_SEL);        
        }
    }
    else
    {
        if(events.ButtonEnc == BTN_CLICK)
        {
            Serial.println("MOVE");
            _globalSettingsMenu->updateCursor(1);
            _lcd.setCursor(_globalSettingsMenu->_curPosition, 1);
        }

        if(events.EncDelta != 0)
        {
            Serial.println("UPDATE");  
            _globalSettingsMenu->updateValue(events.EncDelta);
        }

        if(events.ButtonEnc == BTN_LONG)
        {        
            Serial.println("VIEW");
            _menuSubstViewEdit = MENU_SUBST_VIEW;
            _lcd.noBlink();
        }
    }
}

