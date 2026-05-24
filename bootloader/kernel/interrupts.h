#include <stdint.h>
#ifndef INTERRUPT_HANDLER
#define INTERRUPT_HANDLER

void interrupt_handler(uint32_t interrupt_number, uint32_t error_code);
void init_interrupts();
#endif // INTERRUPT_HANDLER
