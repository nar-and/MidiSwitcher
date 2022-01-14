#include "KeyValueTable.h"
#include <stdio.h>
#include <Arduino.h>


KeyValueTable::KeyValueTable()
{
    _count = 0;
}

/*
KeyValueTable::KeyValueTable(uint16_t length)
{
    _table = new Trigger_t[length];
    _count = length;
}

KeyValueTable::~KeyValueTable()
{
    delete[] _table;
    _table = nullptr; 
    _count = 0;
}
*/

uint16_t KeyValueTable::size(void)
{ 
    return _count;
}

bool KeyValueTable::isEmpty() 
{
    return (size() == 0);
}

bool KeyValueTable::isFull() 
{
    return (size() == MAX_NUM_ENTRIES);
}

int16_t KeyValueTable::_search(uint16_t key)
{
    // Starting range = whole array
    int16_t lo = 0, hi = _count-1;
    
    while (lo <= hi)
    {
        // Compute index of middle element
        int16_t mid = lo + (hi - lo) / 2;

        if (key < _table[mid].key) 
        { 
            // Search into left range
            hi = mid - 1;
        }
        else if (key > _table[mid].key) 
        {
            // Search into right range
            lo = mid + 1;
        }
        else
        {
            // Element found
            return mid;
        }
    }
    return lo;
}

bool KeyValueTable::get(uint16_t key, uint8_t* value)
{
    // Check if table is empty
    if (isEmpty()) return false;

    int16_t i = _search(key);
    if (i < _count && key == _table[i].key) 
    {
        *value = _table[i].value;
        return true;
    }
    else 
        return false;
}

bool KeyValueTable::contains(uint16_t key) 
{
    uint8_t temp;
    return get(key, &temp);
}

bool KeyValueTable::put(uint16_t key, uint8_t value)
{ 
    // Search for key
    int16_t i = _search(key);
    if (i < _count && key == _table[i].key)
    { 
        // Key found --> update value
        _table[i].value = value;
        return true;
    }

    // Key not found --> grow table if there is still space available
    if (isFull()) return false;

    for (int16_t j = _count; j > i; j--)
    { 
        _table[j] = _table[j-1];
    }
    
    _table[i].key = key; 
    _table[i].value = value; 
    _count++;

    return true;
}

bool KeyValueTable::del(uint16_t key)
{ 
    if (isEmpty()) return false;

    // Search for key. Shrink table if found
    int16_t i = _search(key);
    if (i < _count && key == _table[i].key)
    { 
        // Found -> Shrink table
        for (int16_t j = i; j < (_count-1); j++)
        { 
            _table[j] = _table[j+1];
        }
        _count--;
        return true;
    }
    else
        return false;
}

void KeyValueTable::print(void)
{
    char temp[50];
    sprintf(temp, "Key/Value table, size = %d\n", size()); Serial.print(temp);

    for(int i = 0; i < _count; i++)
    {
        sprintf(temp, "  [%04x] -> %d\n", _table[i].key, _table[i].value); Serial.print(temp);
    }
}