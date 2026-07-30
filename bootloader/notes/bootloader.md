# Bootloader: how to think about kernel loading

The bootloader and kernel have a contract. Before writing the loading loop, make every part of that contract explicit:

```text
Where is the kernel stored on disk?
How many bytes or sectors must be read?
At which physical address should those bytes be placed?
What is the kernel entry address?
Which memory must the loader avoid overwriting?
```

If one of these values is only an assumption, a small kernel change can silently break the boot process.

## What the current bootloader does

The current disk layout is:

```text
disk sector 1: bootloader
disk sector 2 onward: kernel.elf
```

The bootloader loads 17 sectors beginning at sector 2 into physical address `0x500`:

```asm
mov ax, 0x50
mov es, ax
xor bx, bx
mov al, 17
```

In real mode, the physical destination is calculated as:

```text
physical address = ES * 16 + BX
                 = 0x50 * 16 + 0
                 = 0x500
```

It then reads the ELF entry point from offset `0x18` in the loaded ELF header and jumps to it.

This is a useful first bootloader, but the number 17 is an implicit size limit. If code growth moves initialized data beyond those sectors, executable code may run while some global variables remain unloaded. This is why a direct VGA write using the literal `0xB8000` can work while `clear_screen()`, which uses an initialized global pointer, fails.

This is not a cache problem. It is a partial-file-loading problem.

## Recommended learning path

Implement one change at a time and prove that the kernel still boots after each step.

### 1. Understand the two kernel files

Keep `kernel.elf` for symbols and debugging. Load `kernel.bin` during boot initially.

`kernel.elf` can contain debug information and metadata that the CPU does not need at runtime. `kernel.bin` is the compact runtime image produced by `objcopy`.

### 2. Calculate the required sectors

Use round-up division:

```text
sector count = (kernel byte size + 511) / 512
```

First calculate and pass this value manually. Once the loader works, automate it in the Makefile and pass it to NASM with a definition such as `KERNEL_SECTORS`.

### 3. Load one sector at a time

A 1.44 MB floppy has:

```text
18 sectors per track
2 heads per cylinder
80 cylinders
```

Track three pieces of CHS state:

```text
sector starts at 2
head starts at 0
cylinder starts at 0
```

After every successful read:

```text
sector++
if sector > 18: sector = 1 and head++
if head > 1: head = 0 and cylinder++
```

Advance the destination by 512 bytes. Because a real-mode segment unit is 16 bytes, increasing `ES` by `0x20` advances the physical destination by one sector:

```text
0x20 * 16 = 512
```

### 4. Preserve the boot drive

The BIOS provides the boot-drive number in `DL`. Save it instead of assuming drive zero. Use that saved value for every `INT 0x13` operation.

### 5. Distinguish load address from entry address

The load address says where the image begins in memory. The entry address says where execution begins. They are not necessarily equal.

In this project, `kernel.bin` begins with code linked around `0x600`, while the ELF entry symbol can appear later because other assembly objects are linked before `start`. Either extract the ELF entry during the build or deliberately arrange the linker order so the entry is at a documented fixed address.

### 6. Prevent the kernel from overwriting the loader

The BIOS places the boot sector at `0x7C00`. A kernel loaded below it can eventually grow into that address while the bootloader is still executing.

First add a size/overlap check. Later, relocate the 512-byte bootloader to safe high conventional memory, far-jump to the relocated copy, and continue loading from there.

### 7. Clear `.bss`

The binary normally does not store `.bss` bytes. The kernel entry code must clear the region between linker-defined `__bss_start` and `__bss_end` before using C globals.

### 8. Add reliability after correctness

After the basic loading loop works:

- Retry failed BIOS reads.
- Display a useful disk-error message.
- Reject images that do not fit the disk or safe memory area.
- Test a kernel deliberately padded beyond 17 and 36 sectors.
- Inspect loaded memory with GDB and compare it with `kernel.bin`.

## Debugging method

Do not begin by changing several loader mechanisms together. Use checkpoints:

```text
boot sector executes
-> first kernel sector is correct in memory
-> reads cross sector 18 correctly
-> all expected sectors are present
-> jump reaches the kernel entry
-> .bss is zero
-> C main executes
```

When something fails, identify the first checkpoint that is false. Then inspect only the state responsible for that transition: CHS values, `ES:BX`, remaining-sector count, destination overlap, or entry address.

## Later: a real ELF loader

Once loading `kernel.bin` is reliable, a second-stage ELF loader is a good next project. It should parse ELF program headers, load each `PT_LOAD` segment at its requested address, zero the difference between `p_memsz` and `p_filesz`, and jump to `e_entry`.

The simple binary loader teaches disk and memory mechanics. The ELF loader then teaches executable formats and removes many fixed-layout assumptions.
