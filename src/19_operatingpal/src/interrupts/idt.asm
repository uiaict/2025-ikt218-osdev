%macro ISR 1
[GLOBAL isr%1]         ; ISR without error code
isr%1:
    cli
    push byte 0
    push byte %1
    jmp isrCommonStub
%endmacro

%macro ISR_ERRORCODE 1
[GLOBAL isr%1]         ; ISR with error code (e.g., page fault)
isr%1:
    cli
    push byte %1
    jmp isrCommonStub
%endmacro

%macro IRQ 2
[GLOBAL irq%1]         ; IRQ handler
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irqCommonStub
%endmacro

; Define ISRs (0–31)
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR_ERRORCODE 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31

; Define IRQs (32–47)
IRQ  0, 32
IRQ  1, 33
IRQ  2, 34
IRQ  3, 35
IRQ  4, 36
IRQ  5, 37
IRQ  6, 38
IRQ  7, 39
IRQ  8, 40
IRQ  9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

; Common ISR stub (calls isrHandler in C)
[EXTERN isrHandler]
isrCommonStub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isrHandler
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    sti
    iret

; Common IRQ stub (calls irqHandler in C)
[EXTERN irqHandler]
irqCommonStub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irqHandler
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    sti
    iret
