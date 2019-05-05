// placeholder
//

#include "stdafx.h"

#ifndef AVR_PLATFORM_OVERRIDE
#define __AVR_ATmega2560__
#endif 


#include "IR_Decoder.h"
#include "IR_Encoder.h"

#include <avr/delay.h>
#include <avr/io.h>

int main()
{
    initEncoder();

    uint8_t testData[4] = {0xaa, 0x55, 0xaa, 0x55};

    DDRB |= _BV(4);

    while(1)
    {
        encodeData(testData, 32);
        PORTB ^= _BV(4);
        _delay_ms(100);
    }
}
