[bits 32]

global _start
extern kernel_main

code_seg equ 0x08
data_seg equ 0x10

_start:
    mov ax, data_seg
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    in ax, 0x92
    or al, 2
    out 0x92, al

    call kernel_main

    jmp $
times 512-($ - $$) db 0
