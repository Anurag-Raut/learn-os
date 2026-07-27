#include <stddef.h>
#include <stdint.h>
#ifndef MEMUTILS_H
#define MEMUTILS_H

void memset(uint32_t *addr, int value, size_t count);
void memcpy(uint32_t *dest, uint32_t *src, size_t count);
#endif // !MEMUTILS_H
