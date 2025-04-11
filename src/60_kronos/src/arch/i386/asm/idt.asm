global idt_load

idt_flush:
    mov eax, [esp + 4]
    lidt [eax]
    ret