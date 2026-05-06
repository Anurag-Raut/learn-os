


### setting up stack
Instead of having stack in the text region we should guarantte it gets free space and does not collide with the kernel code when it gets bigger

So for this we are going to define it in BSS,

### Setup an IDT

0–31   → CPU exceptions
32–255 → hardware + software interrupts

When an interrupt happens, CPU does:

Push state onto stack:
EIP
CS
EFLAGS
Look up IDT entry
Jump to your handler

This step is part of kernel so in kernel.c

Structure of IDT entry- 
struct idt_entry {
  uint16_t offset_low;   // Lower 16 bits of handler function address
  uint16_t selector;     // Code segment selector (from GDT, usually 0x08)
  uint8_t  zero;         // Reserved, must be 0
  uint8_t  type_attr;    // Flags: present, privilege level, gate type (usually 0x8E)
  uint16_t offset_high;  // Upper 16 bits of handler function address
}


TODO
