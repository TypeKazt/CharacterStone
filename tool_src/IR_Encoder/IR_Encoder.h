#ifndef __IR_ENCODER__
#define __IR_ENCODER__

#include "stdafx.h"

#ifndef AVR_PLATFORM_OVERRIDE
#define __AVR_ATmega2560__
#endif 

#define ENCODER_PORT PORTB
#define ENCODER_PIN 5

#define TRANSMIT_HIGH(PIN, PORT)\
 PORT |= _BV(PIN)
#define TRANSMIT_LOW(PIN, PORT)\
 PORT &= ~(_BV(PIN))
#define TRANSMIT(VAL, PIN, PORT)\
if(VAL > 0)\
    TRANSMIT_HIGH(PIN, PORT);\
else TRANSMIT_LOW(PIN, PORT);



/*****************************************************
 * Description:
 *  Encodes a byte buffer into an infrared signal
 * 
 * Args:
 *  data: char* -> buffer to send
 *  numBitsToSend -> number of bits to encode
 * 
*****************************************************/
 extern void encodeData(uint8_t* data, uint8_t numBitsToSend);

#endif