#include <stdint.h>

#ifndef MEMORY_H
#define MEMORY_H
#define MAX_PAGES 131072
#define PAGE_SIZE 4096
void memory_startup();

uint32_t pm_allocate();
void pm_free(uint32_t addr);
void pm_print_used();
#endif // !MEMORY_H
