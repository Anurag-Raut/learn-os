#ifndef PROCESS_H
#define PROCESS_H

#include "interrupts.h"
#include <stdint.h>
#define MAX_PROCESS 100
#define PROCESS_STACK_SIZE 4096

typedef enum {
  READY,
  RUNNING,
  BLOCKED,
  TERMINATED,
} process_state;

typedef struct process {
  void (*entry)(void);
  process_state state;
  registers_t regs;
  uint8_t *stack;
  uint32_t stack_size;
  uint32_t esp;

  uint16_t pid;
  uint32_t cr3;
  uint32_t exit_code;
} process_t;

extern process_t *processes[MAX_PROCESS];
extern uint32_t process_counter;
extern process_t *current_process;
process_t *create_process(void (*handler)(void));
void process_exit(process_t *process, uint32_t exit_code);
#endif // !PROCESS_H
