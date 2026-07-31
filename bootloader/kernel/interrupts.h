#include <stdint.h>
#ifndef INTERRUPT_HANDLER
#define INTERRUPT_HANDLER
typedef struct registers {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
} registers_t;

typedef struct interrupt_frame {
  uint32_t interrupt_number;
  uint32_t error_code;
  registers_t regs;

} interrupt_frame_t;

uint32_t interrupt_handler(interrupt_frame_t *frame);
void init_interrupts();
#endif // INTERRUPT_HANDLER
