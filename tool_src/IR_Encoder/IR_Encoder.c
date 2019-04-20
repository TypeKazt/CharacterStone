#include "stdafx.h"
#include "IR_Encoder.h"
#include "IR_Decoder.h"

#include <avr/io.h>



void encodeData(uint8_t* data, uint8_t numBitsToSend)
{
    uint8_t dataIndex = 0;
    while(numBitsToSend > 0)
    {
        uint8_t bitsInByte = (8 > numBitsToSend ? numBitsToSend : 8);
        for(int i = 0; i < bitsInByte; ++i)
        {
            if((1 << i)&data[i])
                TRANSMIT_HIGH(ENCODER_PIN, ENCODER_PORT);
            else
                TRANSMIT_LOW(ENCODER_PIN, ENCODER_PORT);
        }
        dataIndex++;
    }
}