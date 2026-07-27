

#include "heap.h"
#include "interrupts.h"
#include "paging.h"
#include "process.h"
#include "screen.h"

uint32_t scheduled_cr3 = 0;

static void copy_regs(registers_t *dst, const registers_t *src) {
  dst->edi = src->edi;
  dst->esi = src->esi;
  dst->ebp = src->ebp;
  dst->esp = src->esp;
  dst->ebx = src->ebx;
  dst->edx = src->edx;
  dst->ecx = src->ecx;
  dst->eax = src->eax;
  dst->eip = src->eip;
  dst->cs = src->cs;
  dst->eflags = src->eflags;
}
uint32_t schedule(interrupt_frame_t *frame) {
  if (current_process != NULL) {
    current_process->state = RUNNING;
    current_process->esp = (uint32_t)frame;
  }
  print_string("\n proocss counttter: ");
  // for (int i = 0; i < process_counter; i++) {

  print_int(process_counter);
  // }

  print_string("\n");
  for (int i = 0; i < process_counter; i++) {

    int ind = (i + (current_process != NULL ? current_process->pid : 0)) %
              process_counter;
    process_t *p = processes[ind];

    if (p->state == READY) {
      print_string("CURRENT PROCESS: ");
      print_int(ind);
      print_string("\n");
      p->state = RUNNING;

      if (current_process != NULL) {
        current_process->state = READY;
      }

      current_process = p;
      scheduled_cr3 = p->cr3;
      break;
    }
  }
  return current_process->esp;
}
