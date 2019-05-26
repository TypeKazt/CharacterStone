#ifndef __MESSAGE_CODES__
#define __MESSAGE_CODES__

#define MESSAGE_MASK 0xFF
#define NUM_MESSAGE_BITS 8

#define ACK_RETRIES 10

enum CharacterStoneMessages
{
      Empty = 0x0000000000000000
    , PingLed
    , PingVibrator
    , PingSpeaker
    , HealthUp
    , HealthDown
    , Beacon
    , MasterAck
    , SlaveAck
    , IdAssignment
};

#endif