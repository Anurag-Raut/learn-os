
#include <stdint.h>
#ifndef KEYBOARD_HANDLER
#define KEYBOARD_HANDLER

char scancode_to_char(uint8_t scancode);
void keyboard_handler(void);

#endif // KEYBOOARD_HANDLER
