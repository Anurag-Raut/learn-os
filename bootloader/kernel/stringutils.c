#include <stdbool.h>

bool strcmp(char *a, char *b) {
  while (*a && *b) {
    if (*a != *b) {
      return false;
    }
    a++;
    b++;
  }
  if (!*a && !*b) {
    return true;
  }
  return false;
}
