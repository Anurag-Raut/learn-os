#include "screen.h"
#include "stringutils.h"

char shell_buffer[256];
int shell_index = 0;

/*
 * IN  VGA text mode 80 *25 characteres
 */
void execute_command() {
  if (strcmp(shell_buffer, "help")) {
    print_string("Hello from Void");
  }
}

void handle_input(char c) {
  print_char(c);
  if (c == '\b') {
    shell_index--;
  } else if (c == '\n') {
    shell_buffer[shell_index] = '\0';
    execute_command();
    shell_index = 0;
  } else {
    shell_buffer[shell_index++] = c;
  }
}
