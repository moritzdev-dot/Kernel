org 0x7c00
bits 16

code_seg equ gdt_code - gdt_start
data_seg equ gdt_data - gdt_start
_start:
    jmp short main
    nop

times 33 db 0

main:
    jmp 0:step2

step2:
    cli
    ;Setup Data and Extra Segment
    mov ax, 0
    mov ds, ax
    mov es, ax

    mov ss, ax
    mov sp, 0x7c00
    sti


.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp code_seg:load32


gdt_start:
gdt_null: 
    dd 0x0
    dd 0x0

gdt_code:
    dw 0x0ffff
    dw 0
    db 0
    db 0x9a
    db 11001111b
    db 0

gdt_data:
    dw 0x0ffff
    dw 0
    db 0
    db 0x92
    db 11001111b
    db 0


gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start -1
    dd gdt_start

[bits 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp code_seg:0x0100000

ata_lba_read:
    mov ebx, eax

    shr eax, 24
    or eax, 0xE0;
    mov dx, 0x1F6
    out dx, al

    mov eax, ecx
    mov dx, 0x1F2
    out dx, al

    mov eax, ebx
    mov dx, 0x1F3
    out dx, al

    mov eax, ebx
    mov dx, 0x1F4
    shr eax, 8
    out dx, al

    mov eax, ebx 
    mov dx, 0x1F
    shr eax, 16
    out dx, al

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al


.next_sector:
    push ecx

.try_again:
    mov dx, 0x1F7
    in al, dx
    test al, 8
    jz .try_again

    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector

    ret



times 510-($ - $$) db 0
dw 0xAA55

