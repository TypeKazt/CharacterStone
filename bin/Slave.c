#include "stdafx.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "Slave.h"

uint8_t _data[1+((uint8_t)TRANSACTION_LENGTH-1)/8];
uint8_t _bitCount = 0;
uint8_t _firstBit = 1;

void setMessageSizeInBytes(uint8_t _size)
{
/*	if(_data != 0)
	{
		free(_data);
	}
	_data = (uint8_t*)malloc(sizeof(uint8_t)*_size);*/
}

void configureTimer()
{
	// CLK I/0 on the mega is ~= 8MHz

	TCCR0B = 1; // prescale clk for counter 0 by 1
	// TIMSK0 = 1; // set overflow flag for counter 0, use for testing
}

uint8_t* receiveMessage()
{
#if DEBUG == 1
	for(int i = 0; i < 1+((uint8_t)TRANSACTION_LENGTH-1)/8; ++i)
	{
		_data[i] = 0xaa;
	}
#endif
	return _data;
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
	//__disable_interrupts();
	if(_firstBit == 0)
	{
		uint8_t timerCounts = 0;
		uint8_t byteIndex;
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
				byteIndex = getByteIndex(_bitCount);
				_data[byteIndex] <<= 1;
				_data[byteIndex]+= 1;
				_bitCount++;
				break;
			default:
				reti();
		}
	}
	else
	{
		_firstBit = 0;
	}
	reti();
}

/* test code for measuring frequency of CLK I/O
ISR(TIMER0_OVF_vect)
{
	PORTB ^= _BV(PORTB7);
}
*/