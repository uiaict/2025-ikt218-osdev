global idt_flush

section .text
idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    ret
