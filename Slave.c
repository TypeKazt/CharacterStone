#include <avr/iom2560.h>
#include <avr/interrupt.h>

#include "Slave.h"

#ifndef PERIOD_COUNTS
#define PERIOD_COUNTS 9 // default for IR_HZ of 1786Hz (NEC standard)
#endif

char* _data;


void configureTimer()
{
    
}

/*****************************************************
 *  Description:
 *      Interrupt routine for a pin state change.
 *      Used as a hacked up PLL for decoding Manchester
 *      encoding from an infrared remote.
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
			
			break;	
		case PERIOD_COUNTS*3-1: // '1' bit
		case PERIOD_COUNTS*3:
		case PERIOD_COUNTS*3+1:
			break;
		
	}

}
