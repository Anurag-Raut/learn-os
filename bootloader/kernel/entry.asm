bits 16
section .text
start:
  cli
  lgdt [gdt_descriptor] ;loading gdt
  mov eax, cr0
  or eax, 1
  mov cr0, eax
  jmp 0x08:protected_mode ; 0x08 is the code segment 
  hlt





gdt_start:
;each gdt table entry id 8 byts 
; Address      Entry
; --------------------------
; X + 0        null (8 bytes)
; X + 8        code (8 bytes)
; X + 16       data (8 bytes)
gdt_null:
    dq 0x0000000000000000

gdt_code:
    dq 0x00CF9A000000FFFF

gdt_data:
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

section .bss 
align 16 
stack_bottom: 
  resb 4096
stack_top:

section .text ;(Important , otherwise the prtected mode would also go in .bss and wont exits in text,  so the bootlasder doesnt load this protect mode and this call never happens)
bits 32 
protected_mode:
  mov ax,0x10 ;data segment 
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov fs, ax
  mov gs, ax

  ;mov esp, 0x90000 ;setup the stack pointer(old way withtout free guaranttee)
  mov esp, stack_top 

  extern main 
  call main

section .text
bits 32
global isr_default
isr_default:
  cli
  hlt

global irq0

irq0:
    mov al, [0xb8000]

    inc al

    cmp al, 'Z'
    jle .normal

    mov al, 'A'
    jmp .normal

.normal:
    call store

    mov al, 0x20
    out 0x20, al

    iret

store:
    mov byte [0xb8000], al
    mov byte [0xb8001], 0x0F
    ret
