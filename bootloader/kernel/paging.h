
#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
void paging_init();
void paging_enable();
void map_page(uint32_t virt, uint32_t phys);
uint32_t *unmap_page(uint32_t virt);
#endif // !PAGING_H
