#include "stdafx.h"
#include "Slave.h"
#include "MessageCodes.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define IR_PIN 0

#define TRANSMIT_PIN 6
#define CLK_PIN 7
#define TRANSMIT(bit, pin) \
	if(bit > 0)\
	{\
		PORTB |= _BV(pin);\
	}\
	else\
	{\
		PORTB &= ~(_BV(pin));\
	}\

void replicateSignal(uint8_t* data)
{
	for(uint8_t byteIndex = 0; byteIndex < (TRANSACTION_LENGTH-1)/8 + 1; byteIndex++)
	{
		for(uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
		{
			_delay_ms(1);
			TRANSMIT(1, CLK_PIN);
			TRANSMIT((data[byteIndex]&(1<<bitIndex)), TRANSMIT_PIN);
			_delay_ms(1);
			TRANSMIT(0, CLK_PIN);
		}
	}
}

inline void pingLed()
{
	for(int i = 0; i < 5; ++i)
	{
		_delay_ms(100);
		TRANSMIT(1, TRANSMIT_PIN);
		_delay_ms(100);
		TRANSMIT(0, TRANSMIT_PIN);
	}
}

inline void decodeMessage()
{
	uint64_t message;
	uint8_t* data = receiveMessage();

#if DEBUG >= 1
	replicateSignal(data);
#endif
	
	for(int i = 0; i < numBytes; ++i)
	{
		message |= (uint64_t)(data[i] << 8*i);
	}

	switch(message)
	{
		case PingLed:
			pingLed();
			break;
		case PingSpeaker:
			break;
		case PingVibrator:
			break;
	}
}

// blink led once per second
int main()
{
	configureSlave();
	int bit = 0;
	
	//switch state every second
	while(1)
	{
		_delay_ms(MAIN_LOOP_TIME_MS);
		if(transactionFinished())
			decodeMessage();
	}
}