section .text
    extern exception_handler, pit_handler, keyboard_handler

%macro MAKE_EXCEPTION 1
global exception_%1
exception_%1:
    pusha
    mov eax, %1
    push eax
    call exception_handler
    add esp, 4
    popa
    mov al, 0x20
    out 0x20, al
    iret
%endmacro

%assign i 0
%rep 32 ; 31
    MAKE_EXCEPTION i
    %assign i i+1
%endrep

global isr_32, isr_33

isr_32:
    pusha
    call pit_handler
    popa
    mov al, 0x20
    out 0x20, al
    iret

isr_33:
    pusha
    call keyboard_handler
    popa
    mov al, 0x20
    out 0x20, al
    iret