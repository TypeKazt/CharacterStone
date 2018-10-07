#include <avr/iom2560.h>
#include <avr/io.h>
#include <avr/util/delay.h>

#include "Slave.h"

extern void configureTimer();
extern void setMessageSizeInBytes(uint8_t _size);
extern void receiveMessage(char* _data);

// blink led once per second
int main()
{
	//set port as OUTPUT
	DDRB |= _BV(DDB7);;
	
	//switch state every second
	while(1)
	{
		PORTB ^= _BV(PORTB7);
		_delay_ms(1000);
	}
}
