global idt_flush

idt_flush:
    mov eax, [esp + 4] ; Load pointer to IDT into EAX
    lidt [eax]
    ret