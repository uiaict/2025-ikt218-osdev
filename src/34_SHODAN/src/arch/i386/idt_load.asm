global idt_load
extern idtp

idt_load:
    mov eax, idtp
    lidt [eax]
    ret
