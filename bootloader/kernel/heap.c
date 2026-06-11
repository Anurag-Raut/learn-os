
#include "memory.h"
#include "paging.h"
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
