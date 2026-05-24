#include "keyboard.h"
#include "io.h"
#include "shell.h"
#include <stdint.h>

char scancode_to_char(uint8_t scancode) {
  static char keyboard_map[128] = {
      0,   27,  '1',  '2',  '3',  '4', '5', '6',  '7', '8', '9', '0',
      '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't', 'y', 'u', 'i',
      'o', 'p', '[',  ']',  '\n', 0,   'a', 's',  'd', 'f', 'g', 'h',
      'j', 'k', 'l',  ';',  '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
      'b', 'n', 'm',  ',',  '.',  '/', 0,   '*',  0,   ' '};
  if (scancode == 0x1C) {
    return '\n';
  }
  if (scancode == 0x0E) {
    return '\b';
  }
  if (scancode >= 128)
    return 0;

  return keyboard_map[scancode];
}
void keyboard_handler() {
  uint8_t scancode = inb(0x60);
  char c = scancode_to_char(scancode);
  if (c == 0) {
    return;
  }
  handle_input(c);
}
