#include "timer.h"
#include "io.h"
#include "process.h"
#include "scheduler.h"
#include "screen.h"
#include <stdint.h>

uint32_t ticks;

uint32_t timer_handler(interrupt_frame_t *frame) {
  ticks++;
  if (ticks % 100 == 0) {

    return schedule(frame);
  }

  return (uint32_t)frame;
}

void init_timer(uint32_t freq) {
  uint32_t divisor = 1193182 / freq;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, (divisor >> 8) & 0xFF);
}

void sleep(uint32_t ticks_count) {
  uint32_t start = ticks;
  while (ticks - start < ticks_count)
    ;
}
