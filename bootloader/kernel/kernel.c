#include <stdint.h>

extern void isr_default();
extern void irq0();
void pic_remap();
struct idt_entry {
  uint16_t offset_low; // Lower 16 bits of handler function address
  uint16_t selector;   // Code segment selector (from GDT, usually 0x08)
  uint8_t zero;        // Reserved, must be 0
  uint8_t
      type_attr; // Flags: present, privilege level, gate type (usually 0x8E)
  uint16_t offset_high; // Upper 16 bits of handler function address
} __attribute__((packed));

struct idt_entry idt_table[256];

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

void main() {
  // setting up IDT
  build_idt(idt_table, isr_default);
  set_idt_entry(32, irq0);
  load_idt(idt_table);
  pic_remap();
  __asm__ volatile("sti");
  volatile char *v = (volatile char *)0xB8000;

  v[0] = 'H';
  v[1] = 0x07;

  v[2] = 'Q';
  v[3] = 0x07;
  while (1)
    ;
}
