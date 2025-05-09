[bits 32]

;///////////////////////////////////////
; Exported Symbols
;///////////////////////////////////////

global isr0
global isr1
global isr2
global idt_load

;///////////////////////////////////////
; External Symbols
;///////////////////////////////////////

extern idtp
extern isr_handler

;///////////////////////////////////////
; Load IDT Instruction
;///////////////////////////////////////

idt_load:
    lidt [idtp]
    ret

;///////////////////////////////////////
; ISR Handlers (Exceptions)
;///////////////////////////////////////

isr0:
    cli
    pusha
    push dword 0         ; Interrupt number 1
    call isr_handler
    add esp, 4
    popa
    iret

isr1:
    cli
    pusha
    push dword 1         ; Interrupt number 2
    call isr_handler
    add esp, 4
    popa
    iret

isr2:
    cli
    pusha
    push dword 2         ; Interrupt number 3
    call isr_handler
    add esp, 4
    popa
    iret
