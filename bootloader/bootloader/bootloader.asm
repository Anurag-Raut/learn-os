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

    mov al,39 ;number of sectors to read TODO: make this dynamic ,so i wont need to change everytime
    mov ch,0 ;cylinder 0
    mov cl,2 ;sector 2
    mov dh,0 ;head 0
    mov dl,0 ;drive 0

    mov ah,0x02 ; instruction to read sector
    int 0x13
    jc disk_error
    
    jmp 0x0000:0x0500

    hlt

disk_error:
hlt

times 510-($-$$) db 0
dw 0xAA55
