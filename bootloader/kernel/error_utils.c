

#include "screen.h"
void panic(const char *message) {
  print_string("KERNEL PANIC!!!!! : ");
  print_string(message);
  print_string("\n");
  while (1)
    ;
}
