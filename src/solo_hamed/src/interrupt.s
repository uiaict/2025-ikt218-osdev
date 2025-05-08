# interrupt.s - Contains interrupt service routine wrappers.
# Based on code from JamesM's kernel development tutorials.

.section .text

# This macro creates a stub for an ISR which does NOT pass it's own
# error code (adds a dummy error code of 0)
.macro ISR_NOERRCODE num
.global isr\num
.type isr\num, @function
isr\num:
    cli                  # Disable interrupts
    push $0              # Push a dummy error code
    push $\num           # Push the interrupt number
    jmp isr_common_stub  # Go to our common handler.
.endm

# This macro creates a stub for an ISR which passes it's own
# error code.
.macro ISR_ERRCODE num
.global isr\num
.type isr\num, @function
isr\num:
    cli                  # Disable interrupts
    # Error code already pushed by CPU
    push $\num           # Push the interrupt number
    jmp isr_common_stub
.endm

# This macro creates a stub for an IRQ - the first parameter is
# the IRQ number, the second is the ISR number it is remapped to.
.macro IRQ irqnum, isrnum
.global irq\irqnum
.type irq\irqnum, @function
irq\irqnum:
    cli                  # Disable interrupts
    push $0              # Push a dummy error code
    push $\isrnum        # Push the interrupt number
    jmp irq_common_stub  # Go to our common handler
.endm

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.

.extern isr_handler

isr_common_stub:
    # Save all registers
    pusha

    # Save the data segment descriptor
    mov %ds, %ax         # Lower 16-bits of eax = ds.
    push %eax            # save the data segment descriptor

    # Load the kernel data segment descriptor
    mov $0x10, %ax       # 0x10 is the kernel data segment selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Call the C function
    call isr_handler

    # Restore data segment selector
    pop %eax             # Reload the original data segment descriptor
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Restore registers
    popa
    
    # Cleanup error code and ISR number
    add $8, %esp         # Cleans up the pushed error code and pushed ISR number
    sti                  # Re-enable interrupts
    iret                 # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

# This is our common IRQ stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level IRQ handler,
# and finally restores the stack frame.

.extern irq_handler

irq_common_stub:
    # Save all registers
    pusha

    # Save the data segment descriptor
    mov %ds, %ax         # Lower 16-bits of eax = ds.
    push %eax            # save the data segment descriptor

    # Load the kernel data segment descriptor
    mov $0x10, %ax       # 0x10 is the kernel data segment selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Call the C function
    call irq_handler

    # Restore data segment selector
    pop %eax             # Reload the original data segment descriptor
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # Restore registers
    popa
    
    # Cleanup error code and ISR number
    add $8, %esp         # Cleans up the pushed error code and pushed ISR number
    sti                  # Re-enable interrupts
    iret                 # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

# CPU exceptions
ISR_NOERRCODE 0    # Division by zero
ISR_NOERRCODE 1    # Debug
ISR_NOERRCODE 2    # Non-maskable interrupt
ISR_NOERRCODE 3    # Breakpoint
ISR_NOERRCODE 4    # Detected overflow
ISR_NOERRCODE 5    # Out-of-bounds
ISR_NOERRCODE 6    # Invalid opcode
ISR_NOERRCODE 7    # No coprocessor
ISR_ERRCODE   8    # Double fault
ISR_NOERRCODE 9    # Coprocessor segment overrun
ISR_ERRCODE   10   # Bad TSS
ISR_ERRCODE   11   # Segment not present
ISR_ERRCODE   12   # Stack fault
ISR_ERRCODE   13   # General protection fault
ISR_ERRCODE   14   # Page fault
ISR_NOERRCODE 15   # Unknown interrupt
ISR_NOERRCODE 16   # Coprocessor fault
ISR_NOERRCODE 17   # Alignment check
ISR_NOERRCODE 18   # Machine check
ISR_NOERRCODE 19   # Reserved
ISR_NOERRCODE 20   # Reserved
ISR_NOERRCODE 21   # Reserved
ISR_NOERRCODE 22   # Reserved
ISR_NOERRCODE 23   # Reserved
ISR_NOERRCODE 24   # Reserved
ISR_NOERRCODE 25   # Reserved
ISR_NOERRCODE 26   # Reserved
ISR_NOERRCODE 27   # Reserved
ISR_NOERRCODE 28   # Reserved
ISR_NOERRCODE 29   # Reserved
ISR_NOERRCODE 30   # Reserved
ISR_NOERRCODE 31   # Reserved

# IRQ stubs
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

# Add a note indicating we don't need an executable stack
.section .note.GNU-stack,"",%progbits