section .text
extern keyboard_handler
section .data

counter db 0

section .text
bits 32
global isr_default
isr_default:
  cli
  hlt



global irq0

irq0:
    pusha
    mov al, [0xb8000]
    cmp byte [counter], 10
    
    jne .skip
    mov byte [counter] ,0
    
    inc al

    ; call store
    cmp al, 'Z'
    jle normal

    mov al, 'A'

    jmp normal


.skip:
 inc byte [counter]
 jmp normal

global irq1
irq1:
  pusha
  in al, 0x60
  push eax
  call keyboard_handler
  add esp ,4 ; add 4(bytes) to move the stack pointer upwards since we pushed an orgument 
  ;call store
  jmp normal
  

store:
  mov byte [0xb8000], al
  mov byte [0xb8001], 0x0F
  ret

normal:

    mov al, 0x20
    out 0x20, al ;signal the pic that we are done.
    popa
    iret
