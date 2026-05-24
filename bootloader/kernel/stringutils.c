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

void int_to_string(int num, char *buffer) {
  int i = 0;

  if (num == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return;
  }

  while (num > 0) {
    int digit = num % 10;
    buffer[i] = '0' + digit;
    num = num / 10;
    i++;
  }

  buffer[i] = '\0';

  int start = 0;
  int end = i - 1;

  while (start < end) {
    char temp = buffer[start];
    buffer[start] = buffer[end];
    buffer[end] = temp;

    start++;
    end--;
  }
}
