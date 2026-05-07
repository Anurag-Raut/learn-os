


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


### PIC Mapping (PIC = Programmable Interrupt Controller)
Currently all cpu and hardware interruypts have the same handler we nee to provide different for each

IRQ = Interrupt Requests

| IRQ   | Device   |
| ----- | -------- |
| IRQ0  | Timer    |
| IRQ1  | Keyboard |
| IRQ14 | Disk     |

⚠️ The BIG problem

By default, PIC maps IRQs like this:

IRQ	Interrupt Number
IRQ0	8
IRQ1	9

But CPU exceptions already use:

0–31

CPU cannot distinguish:

"Was this timer?"
or
"Was this double fault?"


Solution: Remap PIC

We move hardware IRQs away from CPU exceptions.

Standard mapping:

IRQ	Interrupt
IRQ0	32
IRQ1	33


TLDR Remapping the Hardware interrupts so they dont overlap with CPU

we need to implement outb for 

Goal of this step

Change PIC mappings from:

IRQ0–IRQ7  → interrupts 8–15   ❌
IRQ8–IRQ15 → interrupts 70–77  ❌

to:

IRQ0–IRQ7  → interrupts 32–39  ✅
IRQ8–IRQ15 → interrupts 40–47  ✅

Why there are TWO PICs

Old x86 systems use:

Master PIC
Slave PIC

Because one PIC only supports 8 IRQs.

So:

Master → IRQ0–IRQ7
Slave  → IRQ8–IRQ15


TODO: change make file so other pic files are also compiled in kernel directory 
