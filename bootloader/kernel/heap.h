#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

void *kmalloc(size_t size);
void kfree(void *addr);
void *alloc_user_space(uint32_t virtual_addr, size_t size,
                       uint32_t *page_directory);

#endif // !HEAP_H
