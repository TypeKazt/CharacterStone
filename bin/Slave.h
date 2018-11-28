#ifndef __CHARACTER_STONE__
#define __CHARACTER_STONE__


#include <inttypes.h>

//== Timer configuration

extern void configureTimer();
 

//== NEC encoding/decoding

/*****************************************************
 * Description:
 *  Number of bytes transferred (including redundancies)
 * 
 * Args:
 *  _size: uint8_t -> size of buffer in bytes
 * 
*****************************************************/
extern void setMessageSizeInBytes(uint8_t _size);

/*****************************************************
 * Description:
 *  Decodes stored data from IR transmitter and populates
 *  a char buffer with the correct bytes. The size of 
 *  transfer is static
 * 
 * Args:
 *  _data: char* -> buffer populated by function
 * 
*****************************************************/
extern uint8_t* receiveMessage();

extern uint8_t _bitCount;

#define getClksElapsed(period_counts, counts) (counts/period_counts)
#define getByteIndex(bitCount) bitCount/8
#define getBitIndex(bitCount) bitCount%8

#ifndef PERIOD_COUNTS
#define PERIOD_COUNTS 9 // default for IR_HZ of 1786Hz (NEC standard)
#endif
#ifndef IR_HZ
#define IR_HZ 1786
#endif
#ifndef TRANSACTION_LENGTH
#define TRANSACTION_LENGTH 64
#endif

#define transactionFinished() _bitCount == TRANSACTION_LENGTH

extern uint8_t _data[1+((uint8_t)TRANSACTION_LENGTH-1)/8];

#endif