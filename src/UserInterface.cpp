#include "UserInterface.h"
#include "PatchManager.h"
#include <Arduino.h>

#define DEBUG_PRINT_INTERFACE       1

/**
 * @brief Construct a new User Interface:: User Interface object
 * 
 */
UserInterface::UserInterface() : _lcd(LCD_ADDRESS, LCD_LINE_LEN, LCD_NUM_LINES)
{
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

    // Initialize LCD
    _lcd.init();
    _lcd.clear();
    _lcd.backlight();

    // Initialize state book-keeping variables
    _onEnter = false;

    // Update screen with currently active patch
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

