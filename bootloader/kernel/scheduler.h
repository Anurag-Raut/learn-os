#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "interrupts.h"
#include "process.h"
uint32_t schedule(interrupt_frame_t *frame);
#endif // !SCHEDULER_H
