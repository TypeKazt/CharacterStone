#ifndef __IR_DECODER__
#define __IR_DECODER__


#include <inttypes.h>

//== Timer configuration

extern void configureEncoder();
 

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

#ifndef DEFAULT_WAIT_MS
#define DEFAULT_WAIT_MS 0.560
#endif

#ifndef ZERO_BIT_MS
#define ZERO_BIT_TOTAL_MS 1.14
#define ZERO_BIT_MS 0.680
#endif
#ifndef ZERO_BIT_COUNTS
#define ZERO_BIT_COUNTS 18 // number of counts for 0 bit (reflecting 877 Hz at a clk of 7812.5 Hz)
#endif

#ifndef ONE_BIT_MS
#define ONE_BIT_TOTAL_MS 2.2831
#define ONE_BIT_MS 1.7231
#endif
#ifndef ONE_BIT_COUNTS
#define ONE_BIT_COUNTS 36 // number of counts for 1 bit (reflecting 438 Hz at a clk of 7812.5 Hz)
#endif 

#ifndef IR_HZ
#define IR_HZ 1786
#endif

#ifndef TRANSACTION_LENGTH
#define TRANSACTION_LENGTH 32
#endif


extern uint8_t _data[1+((uint8_t)TRANSACTION_LENGTH-1)/8];
extern uint8_t numBytes;

#define transactionFinished() _bitCount == TRANSACTION_LENGTH
#define decodeData(result) for(int i = 0; i < numBytes; ++i){result |= ((uint64_t)_data[i]) << 8*i;}

#endif