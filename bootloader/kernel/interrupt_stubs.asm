section .text
extern interrupt_handler

section .text
bits 32
global isr_default
global isr14
isr_default:
  cli
  hlt

isr14:
  pusha
  push 0
  push 14
  call interrupt_handler
  add esp, 8 
  popa
  



irq_common:
  call interrupt_handler
  mov al, 0x20
  out 0x20, al ;end of interrupt command to PIC
  
  add esp,8 ;to remove the two 4 bytte values pushed (first pop the 2 values then the popa LIFO)  
  popa
  iret

%macro IRQ 2
global irq%1

irq%1:
    pusha
    push 0
    push %2
    jmp irq_common
%endmacro


IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47



 
