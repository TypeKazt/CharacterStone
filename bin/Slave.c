#include <avr/interrupt.h>

#include "Slave.h"

uint8_t* _data;
uint8_t _bitCount = 0;

void setMessageSizeInBytes(uint8_t _size)
{
	if(_data != 0)
	{
		free(_data);
	}
	_data = (uint8_t*)malloc(sizeof(uint8_t)*_size);
}

void configureTimer()
{
    
}

void receiveMessage(uint8_t* p_data)
{
	p_data = _data;	
}

/*****************************************************
 *  Description:
 *      Interrupt routine for a pin state change.
 *      Used as a hacked up PLL for decoding 
 *      NEC from an infrared remote.
 * 
 *  Methodology:
 *      For each Pin state change the timer/counter
 *      value is observed, and the time difference 
 *      between the previous timer/counter observation
 *      is used to determine data encoded in time slice.
 * 		A switch case is used as a "filter" that allows some
 * 		room for jitter in the encoded data
 * 
*****************************************************/
ISR(INT0_vect)
{
	uint8_t timerCounts = 0;
	timerCounts = getClksElapsed(PERIOD_COUNTS, timerCounts);
	switch(timerCounts)
	{
		case PERIOD_COUNTS-1: // '0' bit
		case PERIOD_COUNTS:
		case PERIOD_COUNTS+1:
			_data[getByteIndex(_bitCount)] <<= 1;
			_bitCount++;
			break;	
		case PERIOD_COUNTS*3-1: // '1' bit
		case PERIOD_COUNTS*3:
		case PERIOD_COUNTS*3+1:
			uint8_t byteIndex = getByteIndex(_bitCount);
			_data[byteIndex] <<= 1;
			_data[byteIndex]+= 1;
			_bitCount++;
			break;
	}
}
