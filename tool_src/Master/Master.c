// placeholder
//

#include "stdafx.h"

#ifndef AVR_PLATFORM_OVERRIDE
#define __AVR_ATmega2560__
#endif 


#include "IR_Decoder.h"
#include "IR_Encoder.h"


int main()
{
    uint8_t testData[4] = {0xaa, 0x55, 0xaa, 0x55};

    while(1)
    {
        encodeData(testData, 32);
        _delay_ms(100);
    }


}
