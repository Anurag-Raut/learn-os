;old
; section .text
; extern interrupt_handler
; extern scheduled_cr3
;
; section .text
; bits 32
; global isr_default
; global isr14
; isr_default:
;   cli
;   hlt
;
; isr14:
;     pusha
;
;     push dword [esp + 32]    ; push CPU error code
;     push dword 14            ; interrupt number
;
;     call interrupt_handler
;
;     add esp, 8
;     popa
;     add esp, 4               ; remove CPU error code
;     iret
;
; irq_common:
;   push esp
;   call interrupt_handler
;   add  esp,4
;
;   ; Keep both switch values in registers. The outgoing stack must remain mapped
;   ; until CR3 and ESP are changed together below.
;   mov edx, eax
;   mov al, 0x20
;
;   out 0x20, al ;end of interrupt command to PIC
;
;   mov ecx, [scheduled_cr3]
;   test ecx, ecx
;   jz .restore_frame
;
;   mov dword [scheduled_cr3], 0
;   mov cr3, ecx
;
; .restore_frame:
;   mov esp, edx
;
;   add esp,8 ;to remove the two 4 bytte values pushed (first pop the 2 values then the popa LIFO)  
;   popa
;   iret
;
; %macro IRQ 2
; global irq%1
;
; irq%1:
;     pusha
;     push 0
;     push %2
;     jmp irq_common
; %endmacro
;
;
; IRQ 0, 32
; IRQ 1, 33
; IRQ 2, 34
; IRQ 3, 35
; IRQ 4, 36
; IRQ 5, 37
; IRQ 6, 38
; IRQ 7, 39
; IRQ 8, 40
; IRQ 9, 41
; IRQ 10, 42
; IRQ 11, 43
; IRQ 12, 44
; IRQ 13, 45
; IRQ 14, 46
; IRQ 15, 47
;
;

 ; new

section .text
extern interrupt_handler
extern scheduled_cr3

section .text
bits 32
global isr_default
global isr14
isr_default:
  cli
  hlt

; Interrupt Frame - 
; ESP <- stack ponter
; Interrupt number 
; Error code 
; Rgisters (pusha)
; CPU Flags (CSP ,EIP)


; Page Fault interrupt 
interrupt_common:
  push esp
  call interrupt_handler
  add esp , 4
  ; jmp near .interrupt_cleanup

.interrupt_cleanup:

  ; add  esp,4
  ; Keep both switch values in registers. The outgoing stack must remain mapped
  ; until CR3 and ESP are changed together below.
  mov edx, eax
  mov al, 0x20

  out 0x20, al ;end of interrupt command to PIC

  mov ecx, [scheduled_cr3]
  test ecx, ecx
  jz .restore_frame

  mov dword [scheduled_cr3], 0
  mov cr3, ecx

.restore_frame:
  mov esp, edx

  add esp,8 ;to remove the two 4 bytte values pushed (first pop the 2 values then the popa LIFO)  
  popa
  iret


%macro IRQ 2
global irq%1

irq%1:
    pusha
    push 0
    push %2
    ;push esp
    jmp interrupt_common
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

%macro ISR 1 
global isr%1

; Check if it uses error code 

isr%1:
 pusha
 push %1
 ; push esp
 jmp interrupt_common
%endmacro

isr_common:
 call interrupt_handler
 ; jmp interrupt_cleanup


ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
