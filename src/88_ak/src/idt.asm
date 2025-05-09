global idt_flush
idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    sti
    ret

%macro isr_noerrcode 1
    global isr%1
isr%1:
    cli
    push long 0
    push long %1
    jmp isr_common_stub
%endmacro

%macro isr_errcode 1
    global isr%1
isr%1:
    cli
    push long %1
    jmp isr_common_stub
%endmacro

%macro irq 1
    global irq%1
irq%1:
    cli
    push long 0
    jmp irq_common_stub
%endmacro

%macro irq 2
    global irq%1
irq%1:
    cli
    push long 0
    push long %2
    jmp irq_common_stub
%endmacro

isr_noerrcode 0
isr_noerrcode 1
isr_noerrcode 2
isr_noerrcode 3
isr_noerrcode 4
isr_noerrcode 5
isr_noerrcode 6
isr_noerrcode 7
isr_errcode   8
isr_noerrcode 9
isr_errcode  10
isr_errcode  11
isr_errcode  12
isr_errcode  13
isr_errcode  14
isr_noerrcode 15
isr_noerrcode 16
isr_errcode  17
isr_noerrcode 18
isr_noerrcode 19
isr_noerrcode 20
isr_noerrcode 21
isr_noerrcode 22
isr_noerrcode 23
isr_noerrcode 24
isr_noerrcode 25
isr_noerrcode 26
isr_noerrcode 27
isr_noerrcode 28
isr_noerrcode 29
isr_errcode  30
isr_noerrcode 31
isr_noerrcode 128
isr_noerrcode 177

irq 0,  32
irq 1,  33
irq 2,  34
irq 3,  35
irq 4,  36
irq 5,  37
irq 6,  38
irq 7,  39
irq 8,  40
irq 9,  41
irq 10, 42
irq 11, 43
irq 12, 44
irq 13, 45
irq 14, 46
irq 15, 47

extern isr_handler
isr_common_stub:
    pusha
    mov eax, ds
    push eax
    mov eax, cr2
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call isr_handler

    add esp, 8
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8
    sti
    iret

extern irq_handler
irq_common_stub:
    pusha
    mov eax, ds
    push eax
    mov eax, cr2
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler

    add esp, 8
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8
    sti
    iret
