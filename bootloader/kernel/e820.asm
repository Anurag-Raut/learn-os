bits 16
global memory_map
global memory_map_entries
global load_memory
section .bss

memory_map:
    resb 24*28               ; 128 E820 entries

memory_map_entries:
    resw 1                     ; number of entries


section .text

load_memory:
    pusha

    xor ax, ax
    mov es, ax                 ; ES:DI = destination buffer

    mov di, memory_map

    xor ebx, ebx     ; first E820 call
    
.loop:

    mov eax, 0xE820            ; E820 memory map function
    mov edx, 0x534D4150        ; "SMAP"
    mov ecx, 24                ; request 24-byte entries

    mov dword [es:di+20], 1    ; ACPI 3.0 attributes

    int 0x15

    jc .error                  ; BIOS error

    cmp eax, 0x534D4150
    jne .error                 ; invalid response

    inc word [memory_map_entries]

    add di, 24                 ; next entry slot

    test ebx, ebx              ; more entries?
    jnz .loop

.done:
    popa
    ret

.error:
    popa
    ret
