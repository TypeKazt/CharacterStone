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
extern void receiveMessage(char* _data);

#define getClksElapsed(period_counts, counts) (counts/period_counts)

#endif