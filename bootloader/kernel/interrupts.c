#include "interrupts.h"
#include "keyboard.h"
#include "screen.h"
#include "stringutils.h"
#include "timer.h"

extern void isr_default();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void (*irq_table[16])() = {irq0,  irq1,  irq2,  irq3, irq4,  irq5,
                           irq6,  irq7,  irq8,  irq9, irq10, irq11,
                           irq12, irq13, irq14, irq15};
struct idt_entry {
  uint16_t offset_low; // Lower 16 bits of handler function address
  uint16_t selector;   // Code segment selector (from GDT, usually 0x08)
  uint8_t zero;        // Reserved, must be 0
  uint8_t
      type_attr; // Flags: present, privilege level, gate type (usually 0x8E)
  uint16_t offset_high; // Upper 16 bits of handler function address
} __attribute__((packed));

struct idt_entry idt_table[256];

void interrupt_handler(uint32_t interrupt_number, uint32_t error_code) {
  // char interrupt_string[16];
  // int_to_string(interrupt_number, interrupt_string);
  // print_string(interrupt_string);
  if (interrupt_number > 31) {
    // IRQ handlers
    switch (interrupt_number) {
    case 32:
      timer_handler();
      break;
    case 33:
      keyboard_handler();
      break;
    default:
      print_string("INVALID HANDLER");
      break;
    }
  }
}

void build_idt(struct idt_entry *idt_table, void (*handler)()) {
  uint32_t addr = (uint32_t)handler;
  for (int i = 0; i < 256; i++) {
    idt_table[i].offset_low = addr & 0xFFFF;
    idt_table[i].offset_high = (addr >> 16) & 0xFFFF;
    idt_table[i].selector = 0x08;
    idt_table[i].zero = 0;
    idt_table[i].type_attr = 0x8E;
  }
}
void load_idt(struct idt_entry *idt_table) {
  struct idt_pointer {
    uint16_t limit;
    uint32_t base;
  } __attribute__((packed));
  struct idt_pointer idtp;
  idtp.base = (uint32_t)idt_table;
  idtp.limit = sizeof(struct idt_entry) * 256 - 1;
  __asm__ volatile("lidt %0" : : "m"(idtp));
}

void set_idt_entry(uint8_t i, void (*handler)()) {
  uint32_t addr = (uint32_t)handler;
  idt_table[i].offset_low = addr & 0xFFFF;
  idt_table[i].offset_high = (addr >> 16) & 0xFFFF;
  idt_table[i].selector = 0x08;
  idt_table[i].zero = 0;
  idt_table[i].type_attr = 0x8E;
}
void init_interrupts() {

  build_idt(idt_table, isr_default);

  for (int i = 0; i < 16; i++) {
    set_idt_entry(32 + i, irq_table[i]);
  }
  load_idt(idt_table);
}
