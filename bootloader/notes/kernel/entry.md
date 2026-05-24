# Setting Up Stack

Instead of having the stack inside the `.text` region, we should guarantee that it has dedicated free space and does not collide with kernel code as it grows.

For this, we define the stack inside the `.bss` section.

Example:

```asm
section .bss
stack_bottom resb 4096
stack_top:
```

The stack typically grows downward:

```text
stack_top
↓
↓
stack_bottom
```

---

# Setting Up the IDT

The IDT (Interrupt Descriptor Table) tells the CPU:

```text
"When interrupt X happens, jump to handler Y"
```

Interrupt layout:

|Interrupt Range|Purpose|
|---|---|
|0–31|CPU Exceptions|
|32–255|Hardware + Software Interrupts|

---

# What Happens During an Interrupt

When an interrupt occurs, the CPU automatically:

1. Pushes CPU state onto the stack:
    
    - EIP
        
    - CS
        
    - EFLAGS
        
2. Looks up the interrupt entry inside the IDT
    
3. Jumps to the corresponding interrupt handler
    

---

# IDT Entry Structure

```c
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
};
```

---

# Explanation of Each Field

|Field|Purpose|
|---|---|
|`offset_low`|Lower 16 bits of handler address|
|`selector`|Code segment selector from GDT (usually `0x08`)|
|`zero`|Reserved field, must be 0|
|`type_attr`|Interrupt gate flags (`0x8E` usually)|
|`offset_high`|Upper 16 bits of handler address|

Since x86 protected mode uses 32-bit addresses, the handler address is split into:

- lower 16 bits
    
- upper 16 bits
    

---

# PIC Remapping

PIC stands for:

```text
Programmable Interrupt Controller
```

The PIC is responsible for forwarding hardware interrupts to the CPU.

---

# IRQ (Interrupt Request)

Hardware devices generate IRQs.

Examples:

|IRQ|Device|
|---|---|
|IRQ0|Timer|
|IRQ1|Keyboard|
|IRQ14|Disk|

---

# The Problem

By default, the PIC maps IRQs like this:

|IRQ|Interrupt Number|
|---|---|
|IRQ0|8|
|IRQ1|9|

But CPU exceptions already use:

```text
0–31
```

This creates conflicts.

Example:

- Interrupt 8 could mean:
    
    - Timer IRQ
        
    - Double Fault Exception
        

The CPU cannot distinguish them correctly.

---

# Solution: Remap the PIC

We move hardware interrupts away from CPU exception space.

Standard remapping:

|IRQ|New Interrupt|
|---|---|
|IRQ0|32|
|IRQ1|33|
|IRQ8|40|
|IRQ15|47|

This avoids overlap with CPU exceptions.

---

# Why There Are Two PICs

Classic x86 systems use:

- Master PIC
    
- Slave PIC
    

Reason:

- One PIC only supports 8 IRQ lines.
    

So the system uses:

|PIC|IRQ Range|
|---|---|
|Master PIC|IRQ0–IRQ7|
|Slave PIC|IRQ8–IRQ15|

The slave PIC is connected through the master PIC.

---

# Goal of PIC Remapping

Change this:

|Old Mapping|Status|
|---|---|
|IRQ0–IRQ7 → 8–15|❌|
|IRQ8–IRQ15 → 70–77|❌|

Into this:

|New Mapping|Status|
|---|---|
|IRQ0–IRQ7 → 32–39|✅|
|IRQ8–IRQ15 → 40–47|✅|

---

# `outb`

To communicate with hardware ports, we implement:

```c
outb(port, value)
```

This sends a byte directly to an I/O port.

Used heavily for:

- PIC
    
- PIT
    
- keyboard controller
    
- hardware devices
    

---

# Keyboard Interrupts

Keyboard input uses:

|Component|Value|
|---|---|
|IRQ|IRQ1|
|Interrupt|33|
|Data Port|`0x60`|

When a key is pressed:

1. Keyboard controller raises IRQ1
    
2. PIC forwards interrupt 33
    
3. CPU jumps to keyboard handler
    
4. Scancode is read from port `0x60`
    

---

# TODO

- [x]  Change Makefile so kernel directory files are compiled correctly
- [ ]  Proper exception handlers
- [x]  PIT initialization
- [ ]  Paging
- [ ]  Kernel heap allocator
- [ ]  Scheduler
- [ ]  Context switching