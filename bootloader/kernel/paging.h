
#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define KERNEL_IDENTITY_PDE_COUNT 10

extern uint32_t *page_directory;

void paging_init();
void paging_enable();
void map_page(uint32_t virtual_addr, uint32_t physical_addr,
              uint32_t *user_page_directory);
uint32_t *unmap_page(uint32_t virtual_addr, uint32_t *user_page_directory);
void load_cr3(uint32_t addr);

uint32_t get_user_page_dir();
#endif // !PAGING_H
