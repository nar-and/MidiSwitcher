#include "Interface.h"
#include <Arduino.h>

void Interface::_statePatchSelect(UiEvents_t events)
{
    Patch_t patch;

    if(_onEnter)
    {
        _onEnter = false;
        _patchMgr.getPatch(_selectedPatchIndx, &patch);
        _printPatchInfo(patch, (_selectedPatchIndx == _activePatchIndx));
    }

    if(events.EncDelta != 0)
    {   
        // Read new patch
        _selectedPatchIndx += events.EncDelta;
        _selectedPatchIndx = constrain(_selectedPatchIndx, 0, PATCH_LIBRARY_LEN - 1);

        _patchMgr.getPatch(_selectedPatchIndx, &patch);
        _printPatchInfo(patch, (_selectedPatchIndx == _activePatchIndx));
    }

    if(events.ButtonEnc == BTN_CLICK)
    {
        // Activate selected patch (if different than currently active patch)
        if(_activePatchIndx != _selectedPatchIndx)
        {
            _activePatchIndx = _selectedPatchIndx;
            _patchMgr.getPatch(_activePatchIndx, &patch);
            _printPatchInfo(patch, (_selectedPatchIndx == _activePatchIndx));
        }
    }

    if(events.ButtonEnc == BTN_LONG_PRESS)
    {
        _moveToState(INT_STATE_PATCH_EDIT);
    }
}
