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

void memory_startup() {
  uint64_t total_usable = 0;

  print_string("E820 Memory Map\n");
  print_int(memory_map_entries);
  print_string("\n");

  for (uint16_t i = 0; i < memory_map_entries; i++) {
    E820Entry *entry = &memory_map[i];

    if (entry->length == 0)
      continue;

    print_string("Entry ");
    print_int(i);

    print_string(" Base=");
    print_hex64(entry->base);

    print_string(" Length=");
    print_hex64(entry->length);

    print_string(" Type=");
    print_int(entry->type);

    print_string("\n");

    if (entry->type == 1) {
      total_usable += entry->length;
    }
  }

  print_string("Total usable RAM: ");
  print_int((int)(total_usable / 1024 / 1024));
  print_string(" MB\n");
}
