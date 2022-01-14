#ifndef _KEY_VALUE_TABLE_H
#define _KEY_VALUE_TABLE_H
#include <stdint.h>

#define MAX_NUM_ENTRIES            256

class KeyValueTable 
{
public:
    // Initialize trigger list
    //TriggerList(uint16_t size);
    KeyValueTable();

    //~TriggerList();

    // Current size of key/value table
    uint16_t size(void);

    // true if table is empty (0 elements stored)
    bool isEmpty();

    // true if table is full (MAX_NUM_ENTRIES elements stored)
    bool isFull();

    // true if key is found in table --> value is updated with corresponding value
    // false if key is not found
    bool get(uint16_t key, uint8_t* value);

    // true if key is found in table
    bool contains(uint16_t key);

    // true if item is added to table (if key did not exist) or updated (key actually existed)
    // false if item is not added to table (table is full)
    bool put(uint16_t key, uint8_t table);

    // true if item is deleted (key was found)
    // false if item is not deleted (key was not found)
    bool del(uint16_t key);

    // prints table
    void print(void);

private:

    // Key/value entry 
    typedef struct 
    {
        uint16_t    key;
        uint8_t     value;
    } KVEntry_t;

    KVEntry_t _table[MAX_NUM_ENTRIES];
    //Trigger_t* _list = nullptr;
    uint16_t _count;

    int16_t _search(uint16_t key);
};

#endif // _KEY_VALUE_TABLE_H
