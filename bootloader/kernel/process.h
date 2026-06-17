#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESS 100
typedef enum {
  READY,
  RUNNING,
} process_state;
typedef struct process {
  void (*entry)(void);
  process_state state;
} process;

extern process *processes[MAX_PROCESS];
extern int process_counter;
extern process *current_process;
process *create_process(void (*handler)(void));

#endif // !PROCESS_H
