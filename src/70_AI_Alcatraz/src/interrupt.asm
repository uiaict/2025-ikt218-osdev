; filepath: c:\Users\oskar\OneDrive - Universitetet i Agder\Documents\GitHub\2025-ikt218-osdev\src\70_AI_Alcatraz\src\interrupt.asm
[BITS 32]

; Export functions to C code
global idt_flush
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; IRQs
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

; Import C functions
extern isr_handler
extern irq_handler

; IDT loading function
idt_flush:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter
    lidt [eax]        ; Load the IDT pointer
    ret               ; Return to the calling function

; Common ISR stub to save processor state, set up for C code, and restore
%macro ISR_NOERRCODE 1
    isr%1:
        cli                  ; Disable interrupts
        push byte 0          ; Push a dummy error code (if CPU doesn't push one already)
        push byte %1         ; Push the interrupt number
        jmp isr_common_stub  ; Go to common handler
%endmacro

%macro ISR_ERRCODE 1
    isr%1:
        cli                  ; Disable interrupts
        push byte %1         ; Push the interrupt number
        jmp isr_common_stub  ; Go to common handler
%endmacro

; Common IRQ stub for IRQ handlers
%macro IRQ 2
    irq%1:
        cli                  ; Disable interrupts
        push byte 0          ; Push a dummy error code
        push byte %2         ; Push the interrupt number
        jmp irq_common_stub  ; Go to the common IRQ handler
%endmacro

; Define ISRs for CPU exceptions
ISR_NOERRCODE 0  ; Division by zero
ISR_NOERRCODE 1  ; Debug
ISR_NOERRCODE 2  ; Non-maskable interrupt
ISR_NOERRCODE 3  ; Breakpoint
ISR_NOERRCODE 4  ; Overflow
ISR_NOERRCODE 5  ; Bound range exceeded
ISR_NOERRCODE 6  ; Invalid opcode
ISR_NOERRCODE 7  ; Device not available
ISR_ERRCODE   8  ; Double fault
ISR_NOERRCODE 9  ; Coprocessor segment overrun
ISR_ERRCODE   10 ; Invalid TSS
ISR_ERRCODE   11 ; Segment not present
ISR_ERRCODE   12 ; Stack-segment fault
ISR_ERRCODE   13 ; General protection fault
ISR_ERRCODE   14 ; Page fault
ISR_NOERRCODE 15 ; Reserved
ISR_NOERRCODE 16 ; x87 FPU floating-point error
ISR_ERRCODE   17 ; Alignment check
ISR_NOERRCODE 18 ; Machine check
ISR_NOERRCODE 19 ; SIMD floating-point exception
ISR_NOERRCODE 20 ; Virtualization exception
ISR_NOERRCODE 21 ; Reserved
ISR_NOERRCODE 22 ; Reserved
ISR_NOERRCODE 23 ; Reserved
ISR_NOERRCODE 24 ; Reserved
ISR_NOERRCODE 25 ; Reserved
ISR_NOERRCODE 26 ; Reserved
ISR_NOERRCODE 27 ; Reserved
ISR_NOERRCODE 28 ; Reserved
ISR_NOERRCODE 29 ; Reserved
ISR_NOERRCODE 30 ; Reserved
ISR_NOERRCODE 31 ; Reserved

; Map IRQs 0-15 to IDT entries 32-47
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

; Common ISR stub. Saves processor state, sets up for kernel mode segments,
; calls the C-level fault handler, and restores stack frame.
isr_common_stub:
    ; Save all registers
    pusha
    
    ; Save data segment
    mov ax, ds
    push eax
    
    ; Load kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call C handler
    call isr_handler
    
    ; Restore data segment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Restore registers
    popa
    
    ; Clean up error code and interrupt number
    add esp, 8
    
    ; Return from interrupt
    iret

; Common IRQ stub, similar to ISR stub but calls the IRQ handler
irq_common_stub:
    ; Save all registers
    pusha
    
    ; Save data segment
    mov ax, ds
    push eax
    
    ; Load kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call C handler
    call irq_handler
    
    ; Restore data segment
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Restore registers
    popa
    
    ; Clean up error code and interrupt number
    add esp, 8
    
    ; Return from interrupt
    iret
