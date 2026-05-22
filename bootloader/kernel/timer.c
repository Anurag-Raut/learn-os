#include "timer.h"
#include "io.h"
#include <stdint.h>

uint32_t ticks;

void timer_handler() { ticks++; }

void init_timer(uint32_t freq) {
  uint32_t divisor = 1193182 / freq;
  outb(0x43, 0x36);
  outb(0x40, divisor & 0xFF);
  outb(0x40, (divisor >> 8) & 0xFF);
}
