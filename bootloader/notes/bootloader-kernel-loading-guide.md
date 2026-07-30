# Bootloader kernel-loading guide

The bootloader changes are fully reverted. `bootloader/bootloader.asm` and `Makefile` now exactly match their original Git versions. Your kernel changes remain untouched.

Before experimenting, remove stale build artifacts:

```bash
make clean
make
```

## What your current bootloader does

Your disk layout is:

```text
Sector 1       Sector 2...
┌───────────┬─────────────────────┐
│ Bootloader│ kernel.elf          │
└───────────┴─────────────────────┘
```

The bootloader:

1. Loads at physical address `0x7C00`.
2. Sets `ES:BX = 0x0050:0`, meaning physical address `0x500`.
3. Asks BIOS to read 17 sectors starting from sector 2.
4. Finds the ELF entry address at file offset `0x18`.
5. Jumps to that address.

This instruction is the central limitation:

```asm
mov al, 17
```

It assumes the useful portion of the kernel always fits within 17 sectors. A tiny code change can move `.data` beyond that boundary, producing seemingly impossible behavior like the direct H/Q write working while `clear_screen()` fails.

## How I recommend approaching it

Start by loading `kernel.bin`, not the complete `kernel.elf`.

`kernel.elf` contains:

- Runtime code and data
- Symbol tables
- Debugging information
- Section metadata

The CPU does not need debugging information. `kernel.bin` contains the runtime image produced by:

```bash
objcopy -O binary build/kernel.elf build/kernel.bin
```

Keep `kernel.elf` separately for GDB.

Your eventual boot sequence should be:

```text
Build kernel.elf
       ↓
Produce kernel.bin
       ↓
Calculate ceil(kernel_size / 512)
       ↓
Pass sector count to bootloader build
       ↓
Write bootloader + kernel.bin to disk
       ↓
Bootloader reads that many sectors
       ↓
Jump to kernel entry
```

## Step 1: calculate the sector count

The formula is:

```text
sectors = (kernel_size + 511) / 512
```

The addition performs round-up division.

For example:

```text
kernel size = 8608 bytes
sectors     = (8608 + 511) / 512
            = 17
```

First calculate this manually and pass it to NASM:

```bash
nasm -D KERNEL_SECTORS=17 ...
```

Then use it in assembly:

```asm
%ifndef KERNEL_SECTORS
    %error "KERNEL_SECTORS was not provided"
%endif
```

Do this manually before automating the Makefile. It makes debugging much easier.

## Step 2: load one sector at a time

Your current loader asks BIOS to read all 17 sectors in one request. That breaks once you cross a track boundary.

For a 1.44 MB floppy:

```text
18 sectors per track
2 heads
80 cylinders
```

Maintain these values:

```asm
cylinder db 0
head     db 0
sector   db 2
remaining dw KERNEL_SECTORS
```

Read one sector:

```asm
mov ah, 0x02
mov al, 1
mov ch, [cylinder]
mov cl, [sector]
mov dh, [head]
mov dl, [boot_drive]
int 0x13
jc disk_error
```

Then advance the destination by 512 bytes. One real-mode segment unit represents 16 bytes, so:

```text
512 / 16 = 0x20
```

Therefore:

```asm
mov ax, es
add ax, 0x20
mov es, ax
```

## Step 3: advance through floppy geometry

After each successful read:

```text
sector++

if sector > 18:
    sector = 1
    head++

if head > 1:
    head = 0
    cylinder++
```

Continue until `remaining == 0`.

This is the important part missing from your current implementation.

## Step 4: preserve the BIOS drive number

Don’t assume the boot drive is always zero:

```asm
mov dl, 0
```

The BIOS passes the actual boot-drive number in `DL`. Save it immediately:

```asm
mov [boot_drive], dl
```

Use that value for every disk operation.

## Step 5: establish a known stack

The BIOS does not give your bootloader a stack arrangement you should depend on:

```asm
cli
xor ax, ax
mov ss, ax
mov sp, 0x7C00
```

Keep interrupts disabled while changing `SS` and `SP`.

## Step 6: avoid overwriting the bootloader

The kernel begins around `0x600`, while the bootloader executes at `0x7C00`.

If the loaded kernel becomes approximately 30 KB, it will reach `0x7C00` and overwrite the code that is still loading it.

Initially, add a size check and reject kernels that reach the bootloader. After your basic loading loop works, implement relocation:

1. Copy the 512-byte boot sector from `0x7C00` to something like `0x90000`.
2. Far-jump to the copied code.
3. Continue loading from the relocated bootloader.

Treat relocation as a separate milestone. Don’t debug dynamic sizing, CHS traversal, and relocation simultaneously.

## Step 7: choose the correct entry point

Your binary begins at address `0x600`, but your actual `start` symbol is currently around `0x640`. That happens because `e820.asm` is linked before `entry.asm`.

You have two choices:

- Extract the ELF entry point during the build and pass it to NASM.
- Arrange the linker/object order so `start` is always at the beginning of `kernel.bin`.

For learning, I recommend passing both values explicitly:

```text
KERNEL_LOAD_ADDRESS = 0x600
KERNEL_ENTRY_ADDRESS = ELF entry point
KERNEL_SECTORS = calculated size
```

Don’t assume the entry equals the load address.

## Step 8: handle `.bss`

`kernel.bin` normally does not contain `.bss`, because `.bss` represents zero-initialized memory rather than file contents.

Your kernel entry code should eventually clear it:

```c
memset(__bss_start, 0, __bss_end - __bss_start);
```

Define `__bss_start` and `__bss_end` in the linker script. This prevents uninitialized globals from randomly breaking your kernel.

## Recommended implementation order

Implement and test one idea at a time:

1. Store `kernel.bin` on disk.
2. Hard-code its current sector count.
3. Load one sector at a time.
4. Handle sector/head/cylinder transitions.
5. Verify the kernel still boots.
6. Pass the sector count from Make.
7. Pass the ELF entry point from Make.
8. Preserve the BIOS boot drive.
9. Add disk-read retries.
10. Relocate the bootloader.
11. Clear `.bss`.
12. Later, write a real ELF loader using program headers.

Your current approach is not foolish—it is a perfectly reasonable first bootstrap. The problem is that the bootloader/kernel contract is implicit and fragile. Your next version should make the load address, entry address, disk location, and sector count explicit.
