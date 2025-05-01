.section .text
.global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
.global isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
.global isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
.global isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
.global isr128
.global irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
.global irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
.global isr_common_stub, irq_common_stub

/* ISR stubs without error code */
.macro ISR_NOERRCODE num
isr\num:
    pushl $0
    pushl $\num
    jmp isr_common_stub
.endm

/* ISR stubs with error code (hardware pushes it) */
.macro ISR_ERRCODE num
isr\num:
    pushl $\num
    jmp isr_common_stub
.endm

/* IRQ stubs */
.macro IRQ num irqnum
irq\num:
    pushl $0
    pushl $\irqnum
    jmp irq_common_stub
.endm

/* Generate ISRs 0–31 */
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
ISR_NOERRCODE 128

/* Generate IRQs 0–15 (IRQ0–IRQ15 → INT 32–47) */
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

.extern isr_handler
.extern irq_handler

isr_common_stub:
    pusha
    movw %ds, %ax
    pushl %eax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    call isr_handler

    popl %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs

    popa
    addl $8, %esp
    sti
    iret

irq_common_stub:
    pusha
    movw %ds, %ax
    pushl %eax

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl %esp
    call irq_handler
    addl $4, %esp

    popl %ebx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs

    popa
    addl $8, %esp
    iret