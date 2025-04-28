[bits 32]
[section .text]

global idt_load

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

extern isr_handler   ; C function to handle CPU exceptions
extern irq_handler   ; C function to handle hardware IRQs

global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
global isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
global isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
global isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

global irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
global irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

; Macro for ISRs without an error code
%macro ISR_NOERROR 1
isr%1:
    cli
    push byte 0           ; Dummy error code
    push byte %1          ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Macro for ISRs with an error code
%macro ISR_WITHERROR 1
isr%1:
    cli
    push byte %1          ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Macro for IRQs
%macro IRQ 2
irq%1:
    cli
    push byte 0           ; Dummy error code
    push byte %2          ; Push interrupt number (IRQ remapped to IDT entry)
    jmp irq_common_stub
%endmacro

; ISRs 0-31
ISR_NOERROR 0
ISR_NOERROR 1
ISR_NOERROR 2
ISR_NOERROR 3
ISR_NOERROR 4
ISR_NOERROR 5
ISR_NOERROR 6
ISR_NOERROR 7
ISR_WITHERROR 8
ISR_NOERROR 9
ISR_WITHERROR 10
ISR_WITHERROR 11
ISR_WITHERROR 12
ISR_WITHERROR 13
ISR_WITHERROR 14
ISR_NOERROR 15
ISR_NOERROR 16
ISR_WITHERROR 17
ISR_NOERROR 18
ISR_NOERROR 19
ISR_NOERROR 20
ISR_WITHERROR 21
ISR_NOERROR 22
ISR_NOERROR 23
ISR_NOERROR 24
ISR_NOERROR 25
ISR_NOERROR 26
ISR_NOERROR 27
ISR_WITHERROR 28
ISR_WITHERROR 29
ISR_WITHERROR 30
ISR_NOERROR 31

; IRQs 0-15 (mapped from 32 to 47)
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

; Common ISR stub (CPU exceptions)
isr_common_stub:
    pusha                   ; Save all general-purpose registers

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10             ; Load kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                 ; Push pointer to register structure
    call isr_handler         ; Call C ISR handler
    add esp, 4               ; Clean up stack (remove argument)

    pop gs
    pop fs
    pop es
    pop ds

    popa                    ; Restore registers
    add esp, 8               ; Clean up pushed error code + interrupt number

    sti
    iret

; Common IRQ stub (Hardware IRQs)
irq_common_stub:
    pusha

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds

    popa
    add esp, 8

    sti
    iret
