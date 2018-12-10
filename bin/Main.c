#include "stdafx.h"
#include "Slave.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IR_PIN 0

#define TRANSMIT_PIN 7
#define TRANSMIT(bit) \
	if(bit > 0)\
	{\
		PORTB |= _BV(TRANSMIT_PIN);\
	}\
	else\
	{\
		PORTB &= ~(_BV(TRANSMIT_PIN));\
	}\

void replicateSignal()
{
	uint8_t* data;
	data = receiveMessage(data);
	for(uint8_t byteIndex = 0; byteIndex < (TRANSACTION_LENGTH-1)/8 + 1; byteIndex++)
	{
		for(uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
		{
			_delay_ms(1);
			TRANSMIT((data[byteIndex]&(1<<bitIndex)));
			_delay_ms(1);
		}
	}
}

// blink led once per second
int main()
{
	configureSlave();
	
	//switch state every second
	while(1)
	{
		_delay_ms(MAIN_LOOP_TIME_MS);
		if(transactionFinished())
		{
			resetTransactions();
#if DEBUG == 1
			replicateSignal();
#endif
		}
	}
}