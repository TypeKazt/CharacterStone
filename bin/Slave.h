#ifndef __CHARACTER_STONE__
#define __CHARACTER_STONE__


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
extern inline void receiveMessage(uint8_t* p_data);

extern uint8_t _bitCount;

#define getClksElapsed(period_counts, counts) (counts/period_counts)
#define getByteIndex(bitCount) bitCount/8
#define getBitIndex(bitCount) bitCount%8

#ifndef PERIOD_COUNTS
#define PERIOD_COUNTS 9 // default for IR_HZ of 1786Hz (NEC standard)
#endif
#ifndef TRANSACTION_LENGTH
#define TRANSACTION_LENGTH 64
#endif

#define transactionFinished() _bitCount == TRANSACTION_LENGTH

#endif