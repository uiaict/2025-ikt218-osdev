global idt_flush
idt_flush:
    MOV eax, [esp+4]
    LIDT [eax]
    STI
    RET

%macro ISR_NOERRORCODE 1
    global isr%1
    isr%1:
        CLI
        PUSH LONG 0
        PUSH LONG %1
        JMP isr_common_stub
%endmacro

%macro ISR_ERRORCODE 1
    global isr%1
    isr%1:
        CLI
        PUSH LONG %1
        JMP isr_common_stub
%endmacro

%macro IRQ 2
    global irq%1
    irq%1:
        CLI
        PUSH LONG 0
        PUSH LONG %2
        JMP irq_common_stub
%endmacro

ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7

ISR_ERRORCODE 8
ISR_NOERRORCODE 9
ISR_ERRORCODE 10
ISR_ERRORCODE 11
ISR_ERRORCODE 12
ISR_ERRORCODE 13
ISR_ERRORCODE 14

ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_NOERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31
ISR_NOERRORCODE 128
ISR_NOERRORCODE 177

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


extern isr_handler
isr_common_stub:
    pusha
    mov eax,ds
    PUSH eax
    MOV eax, cr2
    PUSH eax

    MOV ax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax

    PUSH esp
    CALL isr_handler

    ADD esp, 8
    POP ebx
    MOV ds, bx
    MOV es, bx
    MOV fs, bx
    MOV gs, bx

    POPA
    ADD esp, 8
    STI
    IRET


extern irq_handler
irq_common_stub:
    pusha
    mov eax,ds
    PUSH eax
    MOV eax, cr2
    PUSH eax

    MOV ax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax

    PUSH esp
    CALL irq_handler

    ADD esp, 8
    POP ebx
    MOV ds, bx
    MOV es, bx
    MOV fs, bx
    MOV gs, bx

    POPA
    ADD esp, 8
    STI
    IRET