#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

extern uint32_t ticks;

void init_timer(uint32_t frequency);
void timer_handler();

#endif
