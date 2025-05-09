# gdt.s - Contains global descriptor table and interrupt descriptor table setup code.
# Based on code from JamesM's kernel development tutorials.

.section .text

# The GDT is loaded by using the lgdt instruction, which takes a pointer to a special data structure that describes the GDT.
# This structure is 6 bytes long, and contains:
# - The GDT's size (16 bits)
# - The GDT's address (32 bits)

# Defined in descriptor_tables.c
.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    # Get the pointer to the GDT, passed as a parameter.
    mov 4(%esp), %eax
    # Load the new GDT pointer
    lgdt (%eax)
    
    # 0x10 is the offset in the GDT to our data segment
    mov $0x10, %ax
    # Load all data segment selectors
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    # 0x08 is the offset to our code segment: Far jump!
    ljmp $0x08, $.flush
.flush:
    ret

# Defined in descriptor_tables.c
.global idt_flush
.type idt_flush, @function

idt_flush:
    # Get the pointer to the IDT, passed as a parameter.
    mov 4(%esp), %eax
    # Load the IDT pointer.
    lidt (%eax)
    ret

# Add a note indicating we don't need an executable stack
.section .note.GNU-stack,"",%progbits