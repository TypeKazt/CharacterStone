// placeholder
//

#include "stdafx.h"

#ifndef AVR_PLATFORM_OVERRIDE
#define __AVR_ATmega2560__
#endif 


#include "IR_Decoder.h"
#include "IR_Encoder.h"
#include "MessageCodes.h"

#include <avr/delay.h>

uint8_t m_slaveDataBase[256];
uint8_t m_foundSlaves = 0;

void PingSlave(uint8_t _slaveId)
{
    uint8_t transmission[numBytes];
    uint64_t message = _slaveId;
    message <<= 8;
    message |= PingLed;
    int64ToByteBuffer(transmission, message);
    encodeData(transmission, TRANSACTION_LENGTH);
}

void BeaconCommand()
{
    int count = 0;

    uint8_t transmission[numBytes];
    uint64_t message;
    while(1)
    {
        message = m_foundSlaves;
        message <<= 8;
        message |= IdAssignment;
        int64ToByteBuffer(transmission, message);

        encodeData(transmission, TRANSACTION_LENGTH);

        _delay_ms(40);

        uint8_t* data = receiveMessage();
        decodeData(message, data);
        if(message & MESSAGE_MASK == SlaveAck)
        {
            if(message >> 8 == m_foundSlaves)
            {
                m_slaveDataBase[m_foundSlaves] = 1;
                PingSlave(m_foundSlaves);
                m_foundSlaves++;
                break;
            }
            else
            {
                count++;
            }
        }
        else
        {
            if(++count > ACK_RETRIES)
            {
                break;
            }
        }
    }
}


int main()
{
    initEncoder();
    configureDecoder();

    uint64_t message = 0;

    while(1)
    {
        _delay_us(100);

        if(transactionFinished())
        {
            uint8_t* data = receiveMessage();
            decodeData(message, data);

            switch (message & MESSAGE_MASK)
            {
            case Beacon:
                break;
            
            default:
                break;
            }

        }
    }
}
