#include "stdafx.h"
#include "IR_Encoder.h"
#include "IR_Decoder.h"

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>



void initEncoder()
{
    ENCODER_PORT_DDR |= _BV(ENCODER_PIN);
}

/**
 *  This method encodes a byte buffer into an
 *  IR transaction following the NEC standard 
**/

void encodeData(uint8_t* data, uint8_t numBitsToSend)
{
    cli(); // disable global interrupts

    uint8_t dataIndex = 0;
    TRANSMIT_HIGH(ENCODER_PIN, ENCODER_PORT);
    _delay_ms(1);
    while(numBitsToSend > 0)
    {
        uint8_t bitsInByte = (8 > numBitsToSend ? numBitsToSend : 8);
        for(int i = 0; i < bitsInByte; ++i)
        {
            if((1 << i)&data[dataIndex]) // if bit is 1
            {
                TRANSMIT_LOW(ENCODER_PIN, ENCODER_PORT);
                _delay_ms(DEFAULT_WAIT_MS);
                TRANSMIT_HIGH(ENCODER_PIN, ENCODER_PORT);
                _delay_ms(ONE_BIT_MS);
            }
            else
            {
                TRANSMIT_LOW(ENCODER_PIN, ENCODER_PORT);
                _delay_ms(DEFAULT_WAIT_MS);
                TRANSMIT_HIGH(ENCODER_PIN, ENCODER_PORT);
                _delay_ms(ZERO_BIT_MS);
            }
        }
        numBitsToSend -= 8;
        dataIndex++;
    }
    TRANSMIT_LOW(ENCODER_PIN, ENCODER_PORT);
    _delay_ms(DEFAULT_WAIT_MS);
    TRANSMIT_HIGH(ENCODER_PIN, ENCODER_PORT);
    _delay_ms(DEFAULT_WAIT_MS);
    TRANSMIT_LOW(ENCODER_PIN, ENCODER_PORT);

    sei(); // enable global interrupts
}