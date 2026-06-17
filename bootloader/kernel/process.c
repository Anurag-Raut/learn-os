

#include "process.h"
#include "heap.h"

process *processes[MAX_PROCESS];
int process_counter = 0;
process *current_process;
process *create_process(void (*handler)(void)) {
  process *p = kmalloc(sizeof(process));
  p->entry = handler;
  p->state = READY;
  processes[process_counter] = p;
  process_counter++;

  return p;
}
