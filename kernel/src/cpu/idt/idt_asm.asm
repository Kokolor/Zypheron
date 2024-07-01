section .text
    extern isr_handler

%macro MAKE_ISR 1
global isr_%1
isr_%1:
    pusha
    mov eax, %1
    push eax
    call isr_handler
    add esp, 4
    popa
    mov al, 0x20
    out 0x20, al
    iret
%endmacro

%assign i 0
%rep 34 ; 33
    MAKE_ISR i
    %assign i i+1
%endrep