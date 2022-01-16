#include "LoopSwitch.h"
#include <Arduino.h>

void LoopSwitch::init(void)
{
    Serial.print("PORTB");Serial.println(VPORTB.OUT, HEX);
    Serial.print("DDRB");Serial.println(VPORTB.DIR, HEX);
    Serial.print("PINB");Serial.println(VPORTB.IN, HEX);

//    VPORTA.OUT &= 0xFE;         // A1
    VPORTB.OUT &= 0xFC;         // B0, B1
    VPORTE.OUT &= 0xF4;         // E0, E1, E3
//    VPORTF.OUT &= 0xEF;         // F4
    //VPORTB.OUT |= 02;
//    VPORTA.DIR |= 0x01;
    VPORTB.DIR |= 0x03;
    VPORTE.DIR |= 0x0B;
//    VPORTF.DIR |= 0x10;

    Serial.print("*PORTB");Serial.println(VPORTB.OUT, HEX);
    Serial.print("*DDRB");Serial.println(VPORTB.DIR, HEX);
    Serial.print("*PINB");Serial.println(VPORTB.IN, HEX);
}

void LoopSwitch::execute(uint8_t status)
{
    // TODO: align implementation with real status
    //            VPORTA.OUT ^= 0x01;
    VPORTB.OUT ^= 0x03;
    VPORTE.OUT ^= 0x0B;
    //            VPORTF.OUT ^= 0x10;
    // digitalWrite(10, testStatus);
    Serial.print("*PORTB");Serial.println(VPORTB.OUT, HEX);
}
