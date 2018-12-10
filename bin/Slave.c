#include "stdafx.h"

#include <avr/interrupt.h>
#if DEBUG == 1
#include <avr/io.h>
#endif

#include "Slave.h"

#define PAUSE_COUNTER() TCCR0B = 0
#define RESUME_COUNTER() TCCR0B = 5
#define RESET_COUNTER() TCNT0 = 0


uint8_t _data[1+((uint8_t)TRANSACTION_LENGTH-1)/8];
uint8_t _bitCount = 0;

uint8_t _state = 1; // 2 == awaiting init bit, 1 == awaiting first bit, 0 >= transaction


void configureSlave()
{
	// CLK I/0 on the mega is ~= 8MHz
	// prescale clk for counter 0 by 1024 (7812.5 Hz)
	// #TODO '5' should be macrofied
	TCCR0B = 5;

#if DEBUG == 1
	DDRB |= _BV(DDB7) | _BV(DDB6) | _BV(DDB5) | _BV(DDB4);;
#endif

	DDRD &= 0xFE; // set first pin to input
	PORTD &= 0xFE; // enable pull up resistor

	EIMSK |= 1 << INT0; // enable external interrupt 0
	EICRA |= 2 << INT0; // configure external interrupt 0 for falling edge

	sei(); // enable global interrupts

	// TIMSK0 = 1; // set overflow flag for counter 0, use for testing
}

void resetTransactions()
{
	_state = 2;
	_bitCount = 0;
}

uint8_t* receiveMessage()
{
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
 * 	On average takes ~= 1.5us to run
 * 
*****************************************************/
ISR(INT0_vect)
{
#if DEBUG == 1
	PORTB |= _BV(4);
#endif
	if(_state <= 0)
	{
		uint8_t timerCounts = TCNT0; // count of first 8-bit counter
		uint8_t byteIndex;
		switch(timerCounts)
		{
			case ZERO_BIT_COUNTS-1: // '0' bit
			case ZERO_BIT_COUNTS:
			case ZERO_BIT_COUNTS+1:
				_data[getByteIndex(_bitCount)] <<= 1;
				_bitCount++;
#if DEBUG == 1
					PORTB |= _BV(6);
#endif
				break;	
			case ONE_BIT_COUNTS-1: // '1' bit
			case ONE_BIT_COUNTS:
			case ONE_BIT_COUNTS+1:
				byteIndex = getByteIndex(_bitCount);
				_data[byteIndex] <<= 1;
				_data[byteIndex]+= 1;
				_bitCount++;
#if DEBUG == 1
				PORTB &= ~(_BV(6));
#endif
				break;
		}
	}
	else
	{
		--_state;
	}

EXIT:
#if DEBUG == 1
	PORTB ^= _BV(5);
	PORTB &= ~(_BV(4));
#endif
	RESET_COUNTER();
	reti();
}

ISR(TIMER0_OVF_vect)
{
	// PORTB ^= _BV(PORTB7); // test code for measuring Clk I/O
	resetTransactions();
#if DEBUG == 1
	PORTB &= ~(_BV(5));
#endif
}