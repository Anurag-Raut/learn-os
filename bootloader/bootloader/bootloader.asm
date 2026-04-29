bits 16

start:
    jmp boot
msg db "Welcome to My Operating System!", 0ah, 0dh, 0h
boot:
    cli ;clearing the interrupt flag
    cld ;clearing the direction flag

    mov ax,50h ;this i the position of where we are going to load right ? es:bx
    mov es,ax
    xor bx,bx

    mov al,10 ;number of sectors to read
    mov ch,0 ;cylinder 0
    mov cl,2 ;sector 2
    mov dh,0 ;head 0
    mov dl,0 ;drive 0

    mov ah,0x02 ; instruction to read sector
    int 0x13

    jmp [500h+18h]

    hlt

times 510-($-$$) db 0
dw 0xAA55
