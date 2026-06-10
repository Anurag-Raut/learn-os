#include "memory.h"
#include "memutils.h"
#include "screen.h"
#include <stdint.h>

uint32_t *page_directory;
uint32_t *page_table;
void paging_init() {

  page_directory = (uint32_t *)pm_allocate();
  page_table = (uint32_t *)pm_allocate();

  memset(page_directory, 0, 4096);
  memset(page_table, 0, 4096);

  for (uint32_t i = 0; i < 1024; i++) {
    page_table[i] = (i * 0x1000) | 0x3;
  }
  page_directory[0] = ((uint32_t)page_table) | 0x3;
}

void paging_enable(void) {
  asm volatile("mov %0, %%cr3" : : "r"(page_directory) : "memory");

  uint32_t cr0;

  asm volatile("mov %%cr0, %0" : "=r"(cr0));

  cr0 |= 0x80000000;

  asm volatile("mov %0, %%cr0" : : "r"(cr0));
  print_string("Enabled paging...\n");
}
