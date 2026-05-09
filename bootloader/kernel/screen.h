#ifndef SCREEN_H
#define SCREEN_H

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
} Direction;

void print_char(char c);
void print_string(char *input);
void move_cursor(Direction d);
void clear_screen();
#endif
