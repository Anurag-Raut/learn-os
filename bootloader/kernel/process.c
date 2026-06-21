#include "process.h"
#include "heap.h"
#include "interrupts.h"
#include "screen.h"
process_t *processes[MAX_PROCESS];
int process_counter = 0;
process_t *current_process = NULL;

process_t *create_process(void (*handler)(void)) {
  process_t *p = kmalloc(sizeof(process_t));
  p->entry = handler;
  p->state = READY;

  uint8_t *stack = kmalloc(PROCESS_STACK_SIZE);
  uint32_t stack_top = (uint32_t)(stack + PROCESS_STACK_SIZE);

  interrupt_frame_t *frame =
      (interrupt_frame_t *)(stack_top - sizeof(interrupt_frame_t));

  frame->interrupt_number = 32;
  frame->error_code = 0;

  frame->regs.eax = 0;
  frame->regs.ebx = 0;
  frame->regs.ecx = 0;
  frame->regs.edx = 0;
  frame->regs.esi = 0;
  frame->regs.edi = 0;

  frame->regs.ebp = stack_top;
  frame->regs.esp = stack_top;

  frame->regs.eip = (uint32_t)handler;
  frame->regs.cs = 0x08;
  frame->regs.eflags = 0x202;

  p->stack = stack;
  p->stack_size = PROCESS_STACK_SIZE;

  p->esp = (uint32_t)frame;
  // p->regs = frame->regs;

  processes[process_counter++] = p;
  return p;
}
