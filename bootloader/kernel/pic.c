#include "io.h"

void pic_remap() {
  outb(0x20, 0x11); // Master PIC: start initialization + expect ICW4
  outb(0xA0, 0x11); // Slave PIC: start initialization + expect ICW4

  outb(0x21, 0x20); // Master PIC interrupt offset = 32 (IRQ0 -> INT32)
  outb(0xA1, 0x28); // Slave PIC interrupt offset = 40 (IRQ8 -> INT40)

  outb(0x21, 0x04); // Tell master PIC that slave is connected on IRQ2
  outb(0xA1, 0x02); // Tell slave PIC its cascade identity is IRQ2

  outb(0x21, 0x01); // Enable 8086/x86 interrupt mode
  outb(0xA1, 0x01); // Enable 8086/x86 interrupt mode
}
