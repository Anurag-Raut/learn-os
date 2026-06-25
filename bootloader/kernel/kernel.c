#include "interrupts.h"
#include "memory.h"
#include "paging.h"
#include "process.h"
#include "scheduler.h"
#include "screen.h"
#include "timer.h"
#include <stdint.h>

void pic_remap();

void taskA() {
  while (1) {
    print_char('A');
    sleep(100);
  }
}

void taskB() {
  while (1) {
    print_char('B');
    sleep(100);
  }
}
void taskC() {
  while (1) {

    print_char('C');
    sleep(100);
  }
}

int main() {
  init_interrupts();
  pic_remap();
  init_timer(100);
  volatile char *v = (volatile char *)0xB8000;

  v[0] = 'H';
  v[1] = 0x07;

  v[2] = 'Q';
  v[3] = 0x07;
  clear_screen();

  memory_startup();

  __asm__ volatile("sti");

  paging_init();
  paging_enable();

  create_process(taskA);
  create_process(taskB);
  create_process(taskC);

  print_string("DONE   creating process");
  while (1) {
  }

  while (1)
    ;
}
