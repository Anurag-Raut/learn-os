
#include "screen.h"
#include "stringutils.h"

volatile char *video = (volatile char *)0xB8000;
int cursor = 0;

void print_char(char c) {
  if (c == '\n') {
    move_cursor(DOWN);
    return;
  }
  if (c == '\b') {
    move_cursor(LEFT);
    return;
  }
  video[2 * cursor] = c;
  video[2 * cursor + 1] = 0x07;
  move_cursor(RIGHT);
}
void print_int(int value) {
  char buffer[32];
  int_to_string(value, buffer);
  print_string(buffer);
}
void print_hex32(uint32_t value) {
  char buffer[11];

  buffer[0] = '0';
  buffer[1] = 'x';

  for (int i = 0; i < 8; i++) {
    uint8_t nibble = (value >> (28 - i * 4)) & 0xF;

    if (nibble < 10)
      buffer[i + 2] = '0' + nibble;
    else
      buffer[i + 2] = 'A' + (nibble - 10);
  }

  buffer[10] = '\0';

  print_string(buffer);
}
void print_hex64(uint64_t value) {
  print_hex32((uint32_t)(value >> 32));
  print_hex32((uint32_t)value);
}
void print_string(char *input) {
  int i = 0;
  while (input[i] != '\0') {
    print_char(input[i]);
    i++;
  }
}

void move_cursor(Direction d) {
  switch (d) {
  case DOWN: {
    int line = cursor / 80;
    cursor = (line + 1) * 80;
    break;
  }

  case UP: {
    int line = cursor / 80;
    if (line > 0)
      cursor = (line - 1) * 80;
    break;
  }

  case LEFT: {
    if (cursor > 0)
      cursor--;
    break;
  }

  case RIGHT: {
    cursor++;
    break;
  }
  }
}

void clear_screen() {
  for (int i = 0; i < 80; i++) {
    for (int j = 0; j < 25; j++) {
      int index = i + j * 80;
      video[2 * index] = ' ';
      video[2 * index + 1] = 0x07;
    }
  }
}
