bits 16

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


bits 32 
protected_mode:
  mov ax,0x10 ;data segment 
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov fs, ax
  mov gs, ax

  mov esp, 0x90000 ;setup the stack pointer
  extern main 
  call main
