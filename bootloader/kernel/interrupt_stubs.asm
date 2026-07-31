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

.interrupt_cleanup:

  add  esp,4
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

;ISR CPU execptions 

%macro ISR 1 
global isr%1

; Check if it uses error code 

isr%1:
 pusha

; Handling error codes
  %if %1 = 10 || %1 = 11 || %1 = 12 ||  %1 = 13 ||  %1 = 14 || %1 = 17 || %1 = 21 || %1 = 29 ||  %1 = 30  
   ;CPU pushes the error code
    push %1
  %else 
    push 0
    push %1 
  %endif

 jmp interrupt_common
%endmacro

isr_common:
 call interrupt_handler
 ; jmp interrupt_cleanup



; ============================================================================
; x86 CPU Exceptions (ISRs 0-31)
; ============================================================================
;
; ISR  0 - Divide Error                      - No Error Code
; ISR  1 - Debug                             - No Error Code
; ISR  2 - Non-Maskable Interrupt (NMI)      - No Error Code
; ISR  3 - Breakpoint                        - No Error Code
; ISR  4 - Overflow                          - No Error Code
; ISR  5 - BOUND Range Exceeded              - No Error Code
; ISR  6 - Invalid Opcode                    - No Error Code
; ISR  7 - Device Not Available              - No Error Code
; ISR  8 - Double Fault                      - Error Code (always 0)
; ISR  9 - Coprocessor Segment Overrun       - No Error Code (obsolete)
; ISR 10 - Invalid TSS                       - Error Code
; ISR 11 - Segment Not Present               - Error Code
; ISR 12 - Stack-Segment Fault               - Error Code
; ISR 13 - General Protection Fault          - Error Code
; ISR 14 - Page Fault                        - Error Code
; ISR 15 - Reserved                          - No Error Code
; ISR 16 - x87 Floating-Point Exception      - No Error Code
; ISR 17 - Alignment Check                   - Error Code
; ISR 18 - Machine Check                     - No Error Code
; ISR 19 - SIMD Floating-Point Exception     - No Error Code
; ISR 20 - Virtualization Exception          - No Error Code
; ISR 21 - Control Protection Exception      - Error Code
; ISR 22 - Reserved                          - No Error Code
; ISR 23 - Reserved                          - No Error Code
; ISR 24 - Reserved                          - No Error Code
; ISR 25 - Reserved                          - No Error Code
; ISR 26 - Reserved                          - No Error Code
; ISR 27 - Reserved                          - No Error Code
; ISR 28 - Hypervisor Injection Exception    - No Error Code
; ISR 29 - VMM Communication Exception       - Error Code
; ISR 30 - Security Exception                - Error Code
; ISR 31 - Reserved                          - No Error Code
; ============================================================================


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
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31
