global idt_flush

idt_flush:
    mov eax, [esp + 4]    ; ta inn peker til IDTPtr
    lidt [eax]            ; last IDT
    ret

