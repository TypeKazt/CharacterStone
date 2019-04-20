#ifndef __QUEUE__
#define __QUEUE__

#include "Slave.h"

#define QUEUE_SIZE 10

uint64_t _queueBuffer[QUEUE_SIZE]; // size arbitrarily chosen
uint8_t writer, reader; // writer is the write index of the circular buffer, reader is the reader (duh)

void init_queue()
{
    writer = 0;
    reader = 0;
}

void push_queue(uint64_t p_item)
{
    writer = writer >= QUEUE_SIZE ? -1 : writer;
    _queueBuffer[writer] = p_item;
    writer++;
}

uint64_t pop_queue()
{
    reader = reader >= QUEUE_SIZE ? 0 : reader;
    uint8_t index = reader;
    reader++;
    _queueBuffer[index] = 0;
    return _queueBuffer[index];
}

#endif