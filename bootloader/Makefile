BUILD=build
BOOT=bootloader
KERNEL=kernel

BOOT_ELF=$(BUILD)/bootloader.elf
BOOT_BIN=$(BUILD)/bootloader.bin


KERNEL_ELF=$(BUILD)/kernel.elf
KERNEL_BIN=$(BUILD)/kernel.bin

DISK=$(BUILD)/disk.img
CFLAGS+=-ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -O0 -fno-stack-protector

all: $(DISK)

$(BUILD):
	mkdir -p $(BUILD)

$(BOOT_ELF): $(BOOT)/bootloader.asm $(BOOT)/bootloader.lds | $(BUILD)
	nasm -f elf32 $< -o $(BUILD)/bootloader.o
	ld -m elf_i386 -T $(BOOT)/bootloader.lds $(BUILD)/bootloader.o -o $@

$(BOOT_BIN): $(BOOT_ELF)
	objcopy -O binary $< $@

$(KERNEL_ELF): $(KERNEL)/kernel.c $(KERNEL)/kernel.lds | $(BUILD)
	nasm -f elf32 $(KERNEL)/entry.asm -o $(BUILD)/entry.o
	gcc $(CFLAGS) -c $< -o $(BUILD)/kernel.o
	ld -m elf_i386 -nmagic -T $(KERNEL)/kernel.lds $(BUILD)/entry.o $(BUILD)/kernel.o  -o $@

$(KERNEL_BIN): $(KERNEL_ELF)
	objcopy -O binary $< $@

$(DISK): $(BOOT_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$(BOOT_BIN) of=$@ conv=notrunc bs=512 count=1 seek=0
	dd if=$(KERNEL_ELF) of=$@ conv=notrunc bs=512 seek=1

run: $(DISK)
	qemu-system-i386 -machine q35 -fda $(DISK)

debug: $(DISK)
	qemu-system-i386 -machine q35 -fda $(DISK) -gdb tcp::26000 -S

clean:
	rm -rf $(BUILD)
