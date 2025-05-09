[GLOBAL idt_flush]
idt_flush:
    mov eax, [esp+4]  ; Get the IDT pointer
    lidt [eax]        ; Load IDT into CPU
    ret
