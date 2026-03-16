section .data
number dq 10

section .text
global _start
_start:
    mov rax, 60
    mov rdi, number
    syscall


