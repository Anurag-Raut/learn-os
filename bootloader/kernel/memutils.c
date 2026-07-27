
#include <stddef.h>
#include <stdint.h>
void memset(uint32_t *addr, int val, size_t count) {
  uint8_t *paddr = (uint8_t *)addr;
  for (int i = 0; i < count; i++) {
    addr[i] = (uint8_t)val;
  }
}

void memcpy(uint32_t *dest, uint32_t *src, size_t count) {
  for (int i = 0; i < count; i++) {
    dest[i] = src[i];
  }
}
