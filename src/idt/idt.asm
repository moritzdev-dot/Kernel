section .asm

extern int21_handler
extern no_int_handler

global no_int
global int21h
global idt_load
global enable_int
global disable_int

enable_int:
    sti
    ret

disable_int:
    cli
    ret

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt [ebx]
    
    pop ebp
    ret


int21h:
    cli
    pushad

    call int21_handler

    popad
    sti
    iret

no_int:
    cli
    pushad

    call no_int_handler

    popad
    sti
    iret

