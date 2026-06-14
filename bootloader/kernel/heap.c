
#include "memory.h"
#include "paging.h"
#include "screen.h"
#include <stddef.h>
#include <stdint.h>

static uint32_t heap_next = 0x02000000; // 16 MB
void *kmalloc(size_t size) {

  void *ptr = (void *)heap_next;
  while (size > 0) {

    uint32_t phys = (uint32_t)pm_allocate();

    map_page(heap_next, phys);
    if (size < PAGE_SIZE) {
      size = 0;
    } else {

      size = size - PAGE_SIZE;
    }
    heap_next += PAGE_SIZE;
  }
  return ptr;
}

void kfree(void *addr) {
  uint32_t iaddr = (uint32_t)addr;
  print_string("\n POINTER: ");
  print_int(iaddr);
  print_string("\n");
  uint32_t *paddr = unmap_page(iaddr);
  // print_int((uint32_t)paddr);
  if (paddr != NULL) {
    pm_free((uint32_t)paddr);
  }
}
