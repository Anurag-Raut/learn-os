
#include "memory.h"
#include "paging.h"
#include "screen.h"
#include <stddef.h>
#include <stdint.h>

static uint32_t heap_next =
    0x02000000; // 16 MB - kernel heap starting at virtual addr 16 mb
void *kmalloc(size_t size) {

  void *ptr = (void *)heap_next;
  while (size > 0) {

    uint32_t phys = (uint32_t)pm_allocate();

    map_page(heap_next, phys, NULL);
    if (size < PAGE_SIZE) {
      size = 0;
    } else {

      size = size - PAGE_SIZE;
    }
    heap_next -= PAGE_SIZE;
  }
  return ptr;
}

void kfree(void *addr) {
  uint32_t iaddr = (uint32_t)addr;
  print_string("\n POINTER: ");
  print_int(iaddr);
  print_string("\n");
  uint32_t *paddr = unmap_page(iaddr, NULL);
  // print_int((uint32_t)paddr);
  if (paddr != NULL) {
    pm_free((uint32_t)paddr);
  }
}

void *alloc_user_space(uint32_t virtual_addr, size_t size, uint32_t *pd) {

  void *ptr = (void *)virtual_addr;
  while (size > 0) {

    uint32_t phys = (uint32_t)pm_allocate();

    map_page(virtual_addr, phys, pd);
    if (size < PAGE_SIZE) {
      size = 0;
    } else {

      size = size - PAGE_SIZE;
    }
    virtual_addr += PAGE_SIZE;
  }
  return ptr;
}
