#include <avr/iom2560.h>
#include <avr/io.h>
#include <avr/util/delay.h>
#include <avr/interrupt.h>

#include "Slave.h"

#define IR_PIN 0

#define TRANSMIT_PIN 7
#define TRANSMIT(bit) \
	if(bit > 0)\
	{\
		PORTB |= _BV(TRANSMIT_PIN);\
	}\
	else\
	{\
		PORTB &= (0xFF ^ _BV(TRANSMIT_PIN));\
	}\

void replicateSignal()
{
	char* data;
	receiveMessage(data);
	for(uint8_t byteIndex = 0; byteIndex < (TRANSACTION_LENGTH-1)/8 + 1; byteIndex++)
	{
		for(uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
		{
			TRANSMIT(data[byteIndex])
		}
	}
}

// blink led once per second
int main()
{
	//set port as OUTPUT
	DDRB |= _BV(DDB7);;

	DDRD &= 0xFE; // set first pin to input
	PORTD &= 0xFE; // enable pull up resistor

	EIMSK |= 1 << INT0; // enable external interrupt 0
	EICRA |= 2 << INT0; // configure external interrupt 0 for falling edge
	
	//switch state every second
	while(1)
	{
		PORTB ^= _BV(PORTB7);
		_delay_ms(100);
		if(transactionFinished())
		{
			replicateSignal();
		}
	}
}
