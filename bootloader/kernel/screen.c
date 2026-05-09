
#include "screen.h"
char *video = (char *)0xB8000;
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
    cursor = 80 * line + 1;
  }
  case UP: {
    int line = cursor / 80;
    cursor = 80 * line + 1;
  }
  case LEFT: {
    cursor--;
  }
  case RIGHT: {
    cursor++;
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
