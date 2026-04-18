#include "SwitcherLogic.h"
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
SwitcherLogic::SwitcherLogic() : 
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
void SwitcherLogic::init(PatchManager* pm)
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
    _patchSettingsMenu->registerScreen(MENU_PS_MIDI_OUT, new ScreenPsMidi(MIDI_DIR_OUT), MAX_NUM_MIDI_OUT);
    _patchSettingsMenu->registerScreen(MENU_PS_MIDI_IN, new ScreenPsMidi(MIDI_DIR_IN), MAX_NUM_MIDI_IN);
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
void SwitcherLogic::refresh(UiEvents_t events, MidiMsg_t rxMsg)
{
    switch(_curState)
    {
        case INT_STATE_PATCH_SEL:
            _statePatchSelect(events, rxMsg);
            break;

        case INT_STATE_PATCH_SETTINGS:
            _statePatchSettings(events, rxMsg);
            break;

        case INT_STATE_GLOBAL_SETTINGS:
            _stateGlobalSettings(events, rxMsg);
            break;

        default:
            break;
    }
}

void SwitcherLogic::_moveToState(uint8_t state)
{
    _curState = state;
    _onEnter = true;
}

void SwitcherLogic::_statePatchSelect(UiEvents_t events, MidiMsg_t rxMsg)
{
    Patch_t patch;

    if(_onEnter)
    {
        // ** On entering the state: set currently active patch **
        _onEnter = false;

        // TODO: set patch
        // Move switches
        // Send out related MIDI messages

        // Display info about currently selected patch
        _patchMgr->getSelectedPatch(&patch);
        _printPatchInfo(patch, _patchMgr->isSelActive());
    }

    if(rxMsg.type != MIDI_TYPE_NONE)
    {
        // Check if RX message triggers a patch change
        uint8_t targetPatch;
        if(_patchMgr->checkMidiInTrigger(rxMsg, &targetPatch))
        {
            // If so, activate new patch and update visualization
            _patchMgr->activatePatch(targetPatch);

            // TODO: add activation!!

            // Update info showing that patch is now active
            _patchMgr->getActivePatch(&patch);
            _printPatchInfo(patch, _patchMgr->isSelActive());
        }
    }

    if(events.EncDelta != 0)
    {           
        // ** On knob rotation: change patch selection (without activating it) ** 
        // Update patch selection
        _patchMgr->updateSelection(events.EncDelta);

        // Display info about newly selected patch
        _patchMgr->getSelectedPatch(&patch);
        _printPatchInfo(patch, _patchMgr->isSelActive());
    }

    if(events.ButtonEnc == BTN_CLICK)
    {
        // ** On button click: activate currently selected patch **        
        if(_patchMgr->isSelActive() == false)
        {
            // Activate selected patch only if not already active 
            // (avoids retriggering relay actions)
            _patchMgr->activateSelectedPatch();
            
            // TODO: add activation!!

            // Update info showing that patch is now active
            _patchMgr->getActivePatch(&patch);
            _printPatchInfo(patch, _patchMgr->isSelActive());
        }
    }

    if(events.ButtonEnc == BTN_LONG)
    {
        // ** On button long press: go to settings menu **        
        if(_patchMgr->isSelActive())
        {   
            // Currently selected patch is active --> move to patch settings menu
            _moveToState(INT_STATE_PATCH_SETTINGS);
        }
        else
        {   
            // Currently selected patch is NOT active --> move to global settings menu
            _moveToState(INT_STATE_GLOBAL_SETTINGS);
        }
    }
}

void SwitcherLogic::_printPatchInfo(Patch_t patch, bool isActive)
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


void SwitcherLogic::_statePatchSettings(UiEvents_t events, MidiMsg_t rxMsg)
{
    if(_onEnter)
    {
        // ** On entering the state: initialize settings menu **
        _onEnter = false;

        // Read active patch for further processing
        _patchMgr->getActivePatch(&_curPatch);

        // Start settings menu in view mode (i.e. patch info browsing, not editing)
        _menuMode = MENU_MODE_VIEW;

        // Move to first menu screen
        _patchSettingsMenu->setScreen(MENU_PS_LOOP_A_ENABLE);
        _lcd.clear();
        _patchSettingsMenu->show();
    }

    {
        // TODO: if MIDI in message has been received: IGNORE IT
    }

    if(_menuMode == MENU_MODE_VIEW)
    {
        // ** Menu VIEW mode **
        if(events.EncDelta != 0)
        {
            // ** On knob rotation: change menu screen ** 
            _patchSettingsMenu->updateScreen(events.EncDelta);
            _patchSettingsMenu->show();
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // ** On button click: move to EDIT mode **
            _menuMode = MENU_MODE_EDIT;

            // Reset book-keeping variables
            _writeTarget = -1;
            _patchSettingsMenu->_selPosition = 0;

            // Highlight first field to be edited with blinking cursor
            _patchSettingsMenu->updateCursor(0);            
            _lcd.setCursor(_patchSettingsMenu->_curPosition, 1);
            _lcd.blink();
        }

        if(events.ButtonEnc == BTN_LONG)
        {   
            // ** On button long press: go back to patch selection **
            _moveToState(INT_STATE_PATCH_SEL);        
        }
    }
    else
    {
        // ** Menu EDIT mode **        
        if(events.ButtonEnc == BTN_CLICK)
        {
            // ** On button click: move to next field **
            _patchSettingsMenu->updateCursor(1);
            _lcd.setCursor(_patchSettingsMenu->_curPosition, 1);
        }

        if(events.EncDelta != 0)
        {
            // ** On knob rotation: change field value ** 
            _patchSettingsMenu->updateValue(events.EncDelta);
        }

        if(events.ButtonEnc == BTN_LONG)
        {        
            // ** On button long press: go back to VIEW mode **
            _menuMode = MENU_MODE_VIEW;

            // Turn off blinking cursor
            _lcd.noBlink();
        }
    }
}

void SwitcherLogic::_stateGlobalSettings(UiEvents_t events, MidiMsg_t rxMsg)
{
    if(_onEnter)
    {
        // ** On entering the state: initialize settings menu **        
        _onEnter = false;

        // Read global settings for further processing
        _patchMgr->getGlobalSettings(&_globalSettings);

        // Start settings menu in view mode (i.e. patch info browsing, not editing)
        _menuMode = MENU_MODE_VIEW;

        // Move to first menu screen
        _globalSettingsMenu->setScreen(MENU_GS_MIDI_IN);
        _lcd.clear();
        _globalSettingsMenu->show();
    }

    {
        // TODO: if MIDI in message has been received: IGNORE IT
    }

    if(_menuMode == MENU_MODE_VIEW)
    {
        // ** Menu VIEW mode **
        if(events.EncDelta != 0)
        {
            // ** On knob rotation: change menu screen ** 
            _globalSettingsMenu->updateScreen(events.EncDelta);
            _globalSettingsMenu->show();
        }

        if(events.ButtonEnc == BTN_CLICK)
        {
            // ** On button click: move to EDIT mode **
            _menuMode = MENU_MODE_EDIT;

            // Reset book-keeping variables            
            _globalSettingsMenu->_selPosition = 0;

            // Highlight first field to be edited with blinking cursor
            _globalSettingsMenu->updateCursor(0);
            _lcd.setCursor(_globalSettingsMenu->_curPosition, 1);
            _lcd.blink();
        }

        if(events.ButtonEnc == BTN_LONG)
        {   
            // ** On button long press: go back to patch selection **
            _moveToState(INT_STATE_PATCH_SEL);        
        }
    }
    else
    {
        // ** Menu EDIT mode **
        if(events.ButtonEnc == BTN_CLICK)
        {
            // ** On button click: move to next field **
            _globalSettingsMenu->updateCursor(1);
            _lcd.setCursor(_globalSettingsMenu->_curPosition, 1);
        }

        if(events.EncDelta != 0)
        {
            // ** On knob rotation: change field value ** 
            _globalSettingsMenu->updateValue(events.EncDelta);
        }

        if(events.ButtonEnc == BTN_LONG)
        {        
            // ** On button long press: go back to VIEW mode **
            _menuMode = MENU_MODE_VIEW;

            // Turn off blinking cursor
            _lcd.noBlink();
        }
    }
}

