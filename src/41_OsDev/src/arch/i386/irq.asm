[bits 32]

;///////////////////////////////////////
; External Symbols
;///////////////////////////////////////

extern pit_ticks
extern isr_handler

;///////////////////////////////////////
; Exported IRQ Handlers
;///////////////////////////////////////

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

;///////////////////////////////////////
; PIC Command Ports
;///////////////////////////////////////

%define PIC1_COMMAND 0x20
%define PIC2_COMMAND 0xA0

;///////////////////////////////////////
; IRQ Handler Macro
;///////////////////////////////////////
; %1 = IRQ label, %2 = IRQ number
; Handles saving state, calling C handler,
; sending EOI, and returning from interrupt
;///////////////////////////////////////

%macro IRQ_HANDLER 2
%1:
    cli
    pusha
    push dword %2
    call isr_handler
    add esp, 4

%ifidni %1, irq0
    inc dword [pit_ticks]
%endif

    ; Send EOI to slave PIC if IRQ >= 40
    mov eax, %2
    cmp eax, 40
    jb .skip_slave
    mov al, 0x20
    mov dx, PIC2_COMMAND
    out dx, al
.skip_slave:
    mov al, 0x20
    mov dx, PIC1_COMMAND
    out dx, al

    popa
    iret
%endmacro

;///////////////////////////////////////
; Define IRQ Handlers (IRQ 0–15)
;///////////////////////////////////////

IRQ_HANDLER irq0, 32
IRQ_HANDLER irq1, 33
IRQ_HANDLER irq2, 34
IRQ_HANDLER irq3, 35
IRQ_HANDLER irq4, 36
IRQ_HANDLER irq5, 37
IRQ_HANDLER irq6, 38
IRQ_HANDLER irq7, 39
IRQ_HANDLER irq8, 40
IRQ_HANDLER irq9, 41
IRQ_HANDLER irq10, 42
IRQ_HANDLER irq11, 43
IRQ_HANDLER irq12, 44
IRQ_HANDLER irq13, 45
IRQ_HANDLER irq14, 46
IRQ_HANDLER irq15, 47
