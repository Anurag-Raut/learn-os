#ifndef TIMER_H
#define TIMER_H

#include "interrupts.h"
#include "process.h"
#include <stdint.h>
extern uint32_t ticks;

void init_timer(uint32_t frequency);
uint32_t timer_handler(interrupt_frame_t *frame);
void sleep(uint32_t ticks_count);

#endif
