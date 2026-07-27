#include "memory.h"
#include "screen.h"
#include <stdint.h>

typedef struct __attribute__((packed)) {
  uint64_t base;
  uint64_t length;
  uint32_t type;
  uint32_t attributes;
} E820Entry;

extern E820Entry memory_map[];
extern uint16_t memory_map_entries;
static uint8_t pm_table[MAX_PAGES];
uint32_t pm_table_length;
uint32_t pm_table_base;

void memory_startup() {
  uint64_t total_usable = 0;

  uint16_t max_chunk_ind = 0;
  uint64_t max_chunk_size = 0;
  for (uint16_t i = 0; i < memory_map_entries; i++) {
    E820Entry *entry = &memory_map[i];

    if (entry->length == 0)
      continue;
    if (entry->type == 1) {
      total_usable += entry->length;
      if (entry->length > max_chunk_size) {
        max_chunk_size = entry->length;
        max_chunk_ind = i;
      }
    }
  }

  uint32_t max_chunk_size_kb = max_chunk_size / 1024;

  pm_table_length = max_chunk_size / PAGE_SIZE;
  pm_table_base = memory_map[max_chunk_ind].base;
  for (int i = 0; i < pm_table_length; i++) {
    pm_table[i] = 0;
  }
}

uint32_t pm_allocate() {
  for (uint32_t i = 0; i < pm_table_length; i++) {
    if (pm_table[i] == 0) {
      pm_table[i] = 1;
      uint32_t addr = (PAGE_SIZE * i) + pm_table_base;

      return addr;
    }
  }
  return 0;
}
void pm_free(uint32_t addr) {
  print_string("\n freeing ADDR=");
  print_int(addr);
  print_string("\n");
  uint32_t ind = ((addr - pm_table_base) / PAGE_SIZE);
  if (ind >= pm_table_length) {
    return;
  }
  pm_table[ind] = 0;
}

void pm_print_used() {
  for (int i = 0; i < pm_table_length; i++) {
    if (pm_table[i] == 1) {
      print_string("USED: ");
      print_int(i);
      print_string("\n");
    }
  }
  print_string("\n\n");
}
