global idt_load

section .text
bits 32

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret
