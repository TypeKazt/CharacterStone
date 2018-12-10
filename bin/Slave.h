#ifndef __CHARACTER_STONE__
#define __CHARACTER_STONE__


#include <inttypes.h>

//== Timer configuration

extern void configureSlave();
 

//== NEC encoding/decoding

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

extern void resetTransactions();

extern uint8_t _bitCount;

#define getClksElapsed(period_counts, counts) (counts/period_counts)
#define getByteIndex(bitCount) bitCount/8
#define getBitIndex(bitCount) bitCount%8

#ifndef ZERO_BIT_COUNTS
#define ZERO_BIT_COUNTS 9 // number of counts for 0 bit
#endif
#ifndef ONE_BIT_COUNTS
#define ONE_BIT_COUNTS 18 // number of counts for 1 bit
#endif 
#ifndef IR_HZ
#define IR_HZ 1786
#endif
#ifndef TRANSACTION_LENGTH
#define TRANSACTION_LENGTH 32
#endif

#define transactionFinished() _bitCount == TRANSACTION_LENGTH

extern uint8_t _data[1+((uint8_t)TRANSACTION_LENGTH-1)/8];

#endif