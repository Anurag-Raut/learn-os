#include "stdint.h"

#ifndef IO_H
#define IO_H

static inline void outb(uint16_t port, uint8_t value) {
  __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
  /* Nd - d is basically to use d Register , N is added so Coimpiler can use
   immediate value if the port number if small enough , (only 8 bit value is
   accepted (0 - 255)
   */
}

#endif
