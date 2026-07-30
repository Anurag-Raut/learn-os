#include "interrupts.h"
#include "error_utils.h"
#include "keyboard.h"
#include "screen.h"
#include "stringutils.h"
#include "timer.h"

// hardware interrupts
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

// CPU execptions
extern void isr_default();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();

void (*irq_table[16])() = {irq0,  irq1,  irq2,  irq3, irq4,  irq5,
                           irq6,  irq7,  irq8,  irq9, irq10, irq11,
                           irq12, irq13, irq14, irq15};
void (*isr_table[16])() = {isr0,  isr1,  isr2,  isr3, isr4,  isr5,
                           isr6,  isr7,  isr8,  isr9, isr10, isr11,
                           isr12, isr13, isr14, isr15};
struct idt_entry {
  uint16_t offset_low; // Lower 16 bits of handler function address
  uint16_t selector;   // Code segment selector (from GDT, usually 0x08)
  uint8_t zero;        // Reserved, must be 0
  uint8_t
      type_attr; // Flags: present, privilege level, gate type (usually 0x8E)
  uint16_t offset_high; // Upper 16 bits of handler function address
} __attribute__((packed));

struct idt_entry idt_table[256];

uint32_t interrupt_handler(interrupt_frame_t *frame) {
  uint32_t interrupt_number = frame->interrupt_number;

  // print_string("INTERRupt no : ");
  // print_int(interrupt_number);
  // print_string("\n  error code: ");
  // print_int(error_code);
  // print_string("\n")

  if (interrupt_number > 31) {
    // IRQ handlers
    switch (interrupt_number) {
    case 32:
      return timer_handler(frame);
      break;
    case 33:
      keyboard_handler();
      break;
    default:
      print_string("int number -  ");
      print_int(interrupt_number);
      print_string("  INVALID HANDLER\n");
      break;
    }
  } else {
    // CPU exception
    print_string("=== EXCEPTION ===\n");
    switch (interrupt_number) {

    case 14: {

      panic("PAGE FAULT");
      break;
    }
    default: {
      print_string("\n CPU EXCEPTION handler not found for  interrupt : ");
      print_int(interrupt_number);
      print_string(" /n");
      panic("EXECPTION HANDLER NOT FOUND");
      break;
    }
    }

    asm volatile("hlt");
  }

  return (uint32_t)frame;
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
    set_idt_entry(i, isr_table[i]);
  }
  for (int i = 0; i < 16; i++) {
    set_idt_entry(32 + i, irq_table[i]);
  }
  load_idt(idt_table);
}
