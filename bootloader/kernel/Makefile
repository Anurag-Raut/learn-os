


BUILD =../build
KERNEL_ELF=$(BUILD)/kernel.elf 
KERNEL_BIN=$(BUILD)/kernel.bin 


CFLAGS+=-ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -O0 -fno-stack-protector

C_SOURCES=$(wildcard *.c)
ASM_SOURCES=$(wildcard *.asm)

C_OBJECTS :=  $(patsubst %.c,$(BUILD)/%.o,$(C_SOURCES))
ASM_OBJECTS:= $(patsubst %.asm,$(BUILD)/%.o,$(ASM_SOURCES))

all: $(KERNEL_BIN)

$(BUILD):
	mkdir  -p $(BUILD)


$(BUILD)/%.o: %.c | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.asm | $(BUILD)
	nasm -f elf32 $< -o $@


$(KERNEL_ELF): $(C_OBJECTS) $(ASM_OBJECTS)
	ld -m elf_i386 -nmagic -T kernel.lds $(ASM_OBJECTS) $(C_OBJECTS) -o $@

$(KERNEL_BIN): $(KERNEL_ELF)
	objcopy -O binary $< $@

