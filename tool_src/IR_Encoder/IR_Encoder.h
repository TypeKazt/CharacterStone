#ifndef __IR_ENCODER__
#define __IR_ENCODER__

#define ENCODER_PORT
#define ENCODER_PIN

#define TRANSMIT_HIGH(PIN, PORT) PORT |= _BV(PIN)
#define TRANSMIT_LOW(PIN, PORT) PORT &= ~(_BV(PIN))

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