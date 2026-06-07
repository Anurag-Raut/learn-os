#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>
typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
} Direction;

void print_char(char c);
void print_int(int value);
void print_hex64(uint64_t val);
void print_string(char *input);
void move_cursor(Direction d);
void clear_screen();
#endif
