#include "process.h"
#include "heap.h"
#include "interrupts.h"
#include "memory.h"
#include "memutils.h"
#include "paging.h"
#include "scheduler.h"
#include "screen.h"

process_t *processes[MAX_PROCESS];
uint32_t process_counter = 0;
process_t *current_process = NULL;

// TODO: Fix process tra
__attribute__((noreturn)) void process_trampoline(void) {
  current_process->entry();
  process_exit(current_process, 0);
  asm volatile("hlt");
}

process_t *create_process(void (*handler)(void)) {
  process_t *p = kmalloc(sizeof(process_t));
  p->entry = handler;
  p->state = READY;
  p->cr3 = create_user_page_dir();
  uint8_t *stack = alloc_user_space(0xFFFFFFFF - PROCESS_STACK_SIZE + 1,
                                    PROCESS_STACK_SIZE, (uint32_t *)p->cr3);
  uint32_t stack_top = (uint32_t)(stack + PROCESS_STACK_SIZE);

  load_cr3(p->cr3);

  interrupt_frame_t *frame =
      (interrupt_frame_t *)(stack_top - sizeof(interrupt_frame_t));
  // building a fake frame
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

  frame->regs.eip = (uint32_t)process_trampoline;
  frame->regs.cs = 0x08;
  frame->regs.eflags = 0x202;

  p->stack = stack;
  p->stack_size = PROCESS_STACK_SIZE;
  p->pid = process_counter;

  p->esp = (uint32_t)frame;
  load_cr3((uint32_t)page_directory);
  // p->regs = frame->regs;

  processes[process_counter++] = p;
  return p;
}

void process_exit(process_t *p, uint32_t exit_code) {
  p->state = TERMINATED;
  p->exit_code = exit_code;

  kfree(p);
  print_string("Exited process with exit code:  ");
  print_int(exit_code);
  print_string("\n");
}
