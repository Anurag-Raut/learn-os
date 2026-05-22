section .text
extern keyboard_handler
extern timer_handler

section .text
bits 32
global isr_default
isr_default:
  cli
  hlt



global irq0

irq0:
    pusha
    call timer_handler
    jmp eoi



global irq1
irq1:
  pusha
  in al, 0x60
  push eax
  call keyboard_handler
  add esp ,4 ; add 4(bytes) to move the stack pointer upwards since we pushed an orgument 
  ;call store
  jmp eoi
  

store:
  mov byte [0xb8000], al
  mov byte [0xb8001], 0x0F
  ret

eoi: ; END of interrupt to PIC
    
    mov al, 0x20
    out 0x20, al ;signal the pic that we are done.
    popa
    iret

 
