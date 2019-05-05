#include "stdafx.h"
#include "IR_Decoder.h"
#include "IR_Encoder.h"
#include "MessageCodes.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#define IR_PIN 0
#define LIGHT_ANIN_PIN 0

#define NUM_LIGHT_SAMPLES 100
#define MS_PER_LIGHT_SAMPLE 1

#define LED_PORT PORTB

#define LED_PIN 6
#define CLK_PIN 7

#define MAX_BEACON_DELAY 5000000

// This loses 2 lowest bits of precision, which is ok for our case. 
#define SAMPLE_ADC(RESULT) RESULT = ADCH; RESULT <<= 2;  

void replicateSignal(uint8_t* data)
{
    for(uint8_t byteIndex = 0; byteIndex < (TRANSACTION_LENGTH-1)/8 + 1; byteIndex++)
    {
        for(uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
        {
            _delay_ms(1);
            TRANSMIT_HIGH(CLK_PIN, LED_PORT);
            TRANSMIT((data[byteIndex]&(1<<bitIndex)), LED_PIN, LED_PORT);
            _delay_ms(1);
            TRANSMIT_LOW(CLK_PIN, LED_PORT);
        }
    }
}

inline void pingLed()
{
    for(int i = 0; i < 5; ++i)
    {
        _delay_ms(100);
        TRANSMIT_HIGH(LED_PIN, LED_PORT);
        _delay_ms(100);
        TRANSMIT_LOW(LED_PIN, LED_PORT);
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

void initSlave()
{

    // Setup Analog Input in free running trigger mode (triggers conversion on clk input)
    ADMUX = 16; // set ADC reference voltage as AREF in board, and left adjust ADC
    ADMUX |= LIGHT_ANIN_PIN; // set the analog input pin routed to the ADC
    
    ADCSRA |= _BV(7); // enable the ADC
    ADCSRA |= _BV(5); // auto trigger sample, also prescaler for clk input is 2 by default

    uint16_t light_data_min = 0xFFFF;
random_wait:
    for(uint8_t n = 0; n < NUM_LIGHT_SAMPLES; ++n)
    {
        uint16_t sample = 0;
        SAMPLE_ADC(sample);
        light_data_min = sample < light_data_min ? sample : light_data_min;
    }

    // Use an exponential curve on luminosity to determine "random" delay time
    for(int i = 0; i < (MAX_BEACON_DELAY * exp(light_data_min/1023)); i++)
    {
        if(i % 1000 == 0)
        {
            uint8_t* data = receiveMessage();
            uint64_t message = 0;
            for(int i = 0; i < numBytes; ++i)
            {
                message |= (uint64_t)(data[i] << 8*i);
            } 
            if(message != Empty)
            {
                goto random_wait;
            }
        }
    }

    // Send the Beacon Command
    

    // look at the state diagram layed out by me and...
    // draw the rest of the owl 
    /*
        /`\  ___  /`\
        \d `"\:/"` b/
        /`.--. ` .--.`\
        |/ __ \ / __ \|
        ( ((o) V (o)) )
        |\`""`/^\`""`/|
        \ `--'\ /'--` /
        /`-._  `  _.-`\
       / /.:.:.:.:.:.\ \
      ; |.:.:.:.:.:.:.| ;
      | |:.:.:.:.:.:.:| |
      | |.:.:.:.:.:.:.| |
      | |:.:.:.:.:.:.:| |
      \/\.:.:.:.:.:.:./\/
        _`).-.-:-.-.(`_
    ,=^` |=  =| |=  =| `^=,
   /     \=/\=/ \=/\=/     \
   */
}

// blink led once per second
int main()
{
    configureDecoder();
    int bit = 0;
    
    initSlave();

    while(1)
    {
        _delay_ms(MAIN_LOOP_TIME_MS);
        if(transactionFinished())
            decodeMessage();
    }
}