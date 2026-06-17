

#include "heap.h"
#include "process.h"
void schedule() {
  for (int i = 0; i < process_counter; i++) {
    process *p = processes[i];
    if (p->state == READY) {
      p->state = RUNNING;
      if (current_process != NULL) {
        current_process->state = READY;
      }
      current_process = p;
      current_process->entry();
    }
  }
}
